#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tac.h"
#include "parser.tab.h" // For token definitions like EQ_OP, LE_OP etc.
#include "ast.h"        // Need the full AST definition
#include "types.h"      // Need type information
#include "symbol_table.h" // Need symbol table access

TacInstr* tac_list_head = NULL;
TacInstr* tac_list_tail = NULL;
StringLiteral* string_literal_list = NULL; // Initialize list head

static int temp_count = 0;
static int label_count = 0;
static int string_lit_count = 0; // Counter for unique string literal labels
int get_type_size(Type* type);

// --- Context for break/continue ---
#define MAX_LOOP_SWITCH_DEPTH 20
typedef struct ControlFlowContext {
    TacAddr* break_label;
    TacAddr* continue_label; // Also used for switch default/fallthrough
} ControlFlowContext;

static ControlFlowContext control_flow_stack[MAX_LOOP_SWITCH_DEPTH];
static int control_flow_stack_top = -1;

void push_control_flow(TacAddr* break_lbl, TacAddr* continue_lbl) {
    if (control_flow_stack_top < MAX_LOOP_SWITCH_DEPTH - 1) {
        control_flow_stack_top++;
        control_flow_stack[control_flow_stack_top].break_label = break_lbl;
        control_flow_stack[control_flow_stack_top].continue_label = continue_lbl;
    } else {
        fprintf(stderr, "Error: Loop/switch nesting depth exceeded.\n");
        // Exit or handle error
    }
}

void pop_control_flow() {
    if (control_flow_stack_top >= 0) {
        control_flow_stack_top--;
    }
}

ControlFlowContext* current_control_flow() {
    if (control_flow_stack_top >= 0) {
        return &control_flow_stack[control_flow_stack_top];
    }
    return NULL;
}
// --- End Context ---
// --- NEW: GOTO/LABEL Map for Function Scopes ---
typedef struct LabelMap {
    char* name;
    TacAddr* addr;
    struct LabelMap* next;
} LabelMap;

// This map is local to the function being processed
static LabelMap* current_label_map = NULL;

// Creates a new TacAddr for a label and adds it to the map
void map_new_label(const char* name) {
    if (!name) return;
    
    // Check if it already exists
    for (LabelMap* l = current_label_map; l; l = l->next) {
        if (l->name && strcmp(l->name, name) == 0) {
            return; // Already mapped (semantic should have caught duplicates)
        }
    }

    // Add new label
    LabelMap* new_label = (LabelMap*)calloc(1, sizeof(LabelMap));
    new_label->name = strdup(name);
    new_label->addr = new_label_addr();
    new_label->next = current_label_map;
    current_label_map = new_label;
}

// Retrieves the TacAddr for a given label name
TacAddr* get_label_addr(const char* name) {
    if (!name) return NULL;
    for (LabelMap* l = current_label_map; l; l = l->next) {
        if (l->name && strcmp(l->name, name) == 0) {
            return l->addr;
        }
    }
    return NULL; // Should not happen if semantic analysis passed
}

// Frees the label map
void destroy_label_map() {
    LabelMap* l = current_label_map;
    while (l) {
        LabelMap* next = l->next;
        free(l->name);
        // Do not free l->addr, it's owned by the TAC list
        free(l);
        l = next;
    }
    current_label_map = NULL;
}

// Recursive pre-pass to find all goto labels in a function body
void find_all_labels_in_body(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_LABELED_STATEMENT:
            map_new_label(node->data.labeled_statement.label);
            // Recurse on the statement
            find_all_labels_in_body(node->data.labeled_statement.statement);
            break;

        // --- Recurse into statements that contain other statements ---
        case NODE_COMPOUND_STATEMENT:
            for (ASTNodeList* item = node->data.compound_statement.items; item; item = item->next) {
                find_all_labels_in_body(item->node);
            }
            break;
        case NODE_IF_STATEMENT:
            find_all_labels_in_body(node->data.if_statement.if_body);
            if (node->data.if_statement.else_body) {
                find_all_labels_in_body(node->data.if_statement.else_body);
            }
            break;
        case NODE_SWITCH_STATEMENT:
            find_all_labels_in_body(node->data.switch_statement.body);
            break;
        case NODE_WHILE_STATEMENT:
            find_all_labels_in_body(node->data.while_statement.body);
            break;
        case NODE_UNTIL_STATEMENT:
             find_all_labels_in_body(node->data.until_statement.body);
            break;
        case NODE_DO_WHILE_STATEMENT:
            find_all_labels_in_body(node->data.do_while_statement.body);
            break;
        case NODE_FOR_STATEMENT:
            find_all_labels_in_body(node->data.for_statement.body);
            break;

        // These statements don't contain other statements
        case NODE_EXPRESSION_STATEMENT:
        case NODE_DECLARATION:
        case NODE_GOTO_STATEMENT:
        case NODE_CONTINUE_STATEMENT:
        case NODE_BREAK_STATEMENT:
        case NODE_RETURN_STATEMENT:
        default:
            break;
    }
}

// Calculates the *byte offset* for an array element access recursively.
// base_node: The AST node representing the base array/pointer identifier.
// subscript_nodes: A list of AST nodes representing the subscript expressions [idxN-1]...[idx1][idx0].
// num_subscripts: The total number of subscripts applied.
// current_dim_index: The index of the dimension currently being processed (starts at 0 for outermost).
// array_type: The full Type* of the base array (e.g., int[2][3]).
// Returns a TacAddr* (temporary or constant) holding the final byte offset.
TacAddr* calculate_array_offset_recursive_new(ASTNode* base_node, ASTNodeList* subscript_nodes, int num_subscripts, int current_dim_index, Type* array_type) {

    if (current_dim_index >= num_subscripts) {
        // Base case: all dimensions processed, initial offset is 0.
        return new_const_int(0);
    }

    // Get the AST node for the current subscript expression
    ASTNodeList* current_subscript_item = subscript_nodes;
    for (int i = 0; i < current_dim_index; ++i) {
        if (!current_subscript_item) return NULL; // Should not happen
        current_subscript_item = current_subscript_item->next;
    }
    ASTNode* index_node = current_subscript_item ? current_subscript_item->node : NULL;
    if (!index_node) return NULL;

    // Get the size of the subarray/element for the *next* dimension
    int stride = 0;
    Type* sub_array_type = array_type; // Type for the current dimension level

    if (sub_array_type->kind == TYPE_ARRAY) {
         if (current_dim_index < sub_array_type->data.base_info.num_dimensions) {
             // Calculate the size of one element in the current dimension
             // This is the size of the base type multiplied by all subsequent dimension sizes
             stride = get_type_size(sub_array_type->data.base_info.base); // Start with base element size
             for (int d = current_dim_index + 1; d < sub_array_type->data.base_info.num_dimensions; ++d) {
                  int dim_size = sub_array_type->data.base_info.dimensions[d];
                  if (dim_size <= 0) {
                      fprintf(stderr, "Error line %d: Array dimension size missing or invalid for offset calculation (dim %d).\n", index_node->lineno, d);
                      return NULL;
                  }
                  stride *= dim_size;
             }
         } else {
              fprintf(stderr, "Error line %d: Too many dimensions used in array access.\n", index_node->lineno);
              return NULL;
         }
    } else if (sub_array_type->kind == TYPE_POINTER && current_dim_index == 0) {
        // If the base is a pointer (e.g., int *p; p[i]), the stride is the size of the pointed-to type.
        stride = get_type_size(sub_array_type->data.base_info.base);
    }
     else {
         fprintf(stderr, "Error line %d: Subscript applied to non-array/non-pointer type '%s' at dimension %d.\n", index_node->lineno, type_to_string(sub_array_type), current_dim_index);
         return NULL;
     }

    if (stride <= 0) {
        fprintf(stderr, "Error line %d: Cannot determine stride for array offset calculation (element size might be zero or incomplete).\n", index_node->lineno);
        return NULL;
    }


    // Generate TAC for the current index expression
    TacAddr* index_val = gen_tac_for_expr(index_node, false);
    if (!index_val) return NULL;

    // Calculate offset for this dimension: index_val * stride
    TacAddr* current_dim_offset = new_temp(create_type(TYPE_INT)); // Offset is integer
    emit(TAC_MUL, current_dim_offset, index_val, new_const_int(stride));

    // Recursively calculate offset for inner dimensions
    TacAddr* inner_offset = calculate_array_offset_recursive_new(base_node, subscript_nodes, num_subscripts, current_dim_index + 1, array_type);
    if (!inner_offset) return NULL;

    // Total offset = current_dim_offset + inner_offset
    TacAddr* total_offset = new_temp(create_type(TYPE_INT));
    emit(TAC_ADD, total_offset, current_dim_offset, inner_offset);

    return total_offset;
}

/* --- Type Size Helper (Simplified) --- */
int get_type_size(Type* type) {
    if (!type) return 0;

    // --- NEW: Handle potential typedefs ---
    // If the type name comes from a typedef, we need the underlying type size.
    // This requires the symbol table info, which we don't have here.
    // The type passed SHOULD ideally be the resolved base type after semantic analysis.
    // If semantic analysis stored the resolved type correctly, this works.

    switch(type->kind) {
        case TYPE_VOID:   return 0;
        case TYPE_BOOL:
        case TYPE_CHAR:   return 1;
        case TYPE_SHORT:  return 2;
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_ENUM:   return 4; // Assuming 4 bytes for enums (usually int)
        case TYPE_LONG:
        case TYPE_DOUBLE: return 8; // Assuming 8 bytes
        case TYPE_POINTER:
        case TYPE_REFERENCE:
        case TYPE_STRING: // Represents char* essentially
        case TYPE_FUNCTION: // Size of function pointer
                          return 8; // Assuming 64-bit architecture

        case TYPE_ARRAY: {
            // --- FULL ARRAY SIZE CALCULATION ---
            if (type->data.base_info.num_dimensions == 0) {
                 fprintf(stderr, "Warning: Cannot calculate size of array with no dimensions specified.\n");
                 return 0; // Or return base size? Returning 0 is safer.
            }

            int base_element_size = get_type_size(type->data.base_info.base);
            if (base_element_size == 0 && type->data.base_info.base->kind != TYPE_VOID) {
                // If base size is 0 (and not void), likely an incomplete type (e.g., struct declared but not defined)
                fprintf(stderr, "Warning: Cannot calculate size of array of incomplete type '%s'.\n", type_to_string(type->data.base_info.base));
                return 0;
            }

            int total_size = base_element_size;
            for (int i = 0; i < type->data.base_info.num_dimensions; ++i) {
                int dim_size = type->data.base_info.dimensions[i];
                if (dim_size <= 0) {
                     fprintf(stderr, "Warning: Cannot calculate size of array with unspecified or zero dimension (dimension %d).\n", i);
                     // This is valid C for function parameters or extern declarations,
                     // but we can't get a total size.
                     return 0;
                }
                total_size *= dim_size;
            }
            return total_size;
            // --- END ARRAY CALCULATION ---
        }
        case TYPE_STRUCT: {
             // --- STRUCT SIZE CALCULATION (with basic alignment) ---
             int size = 0;
             int max_alignment = 1;
             for (Member* m = type->data.struct_union_info.members; m; m = m->next) {
                 int member_size = get_type_size(m->type);
                 int member_alignment = member_size; // Simplified: align to size
                 if (member_alignment > max_alignment) max_alignment = member_alignment;

                 // Add padding before member if needed
                 if (size % member_alignment != 0) {
                     size += member_alignment - (size % member_alignment);
                 }
                 size += member_size;
             }
             // Add padding at the end of the struct for array alignment
             if (max_alignment > 0 && size % max_alignment != 0) {
                 size += max_alignment - (size % max_alignment);
             }
             return size > 0 ? size : 1; // Structs usually have min size 1
             // --- END STRUCT CALCULATION ---
        }
        case TYPE_UNION: {
             // --- UNION SIZE CALCULATION ---
             int max_member_size = 0;
             for (Member* m = type->data.struct_union_info.members; m; m = m->next) {
                 int member_size = get_type_size(m->type);
                 if (member_size > max_member_size) {
                     max_member_size = member_size;
                 }
             }
             // Union size also needs alignment, typically to the largest member's alignment
             return max_member_size > 0 ? max_member_size : 1; // Unions usually have min size 1
             // --- END UNION CALCULATION ---
        }
        default:
             fprintf(stderr, "Warning: Cannot determine size for type kind %d.\n", type->kind);
             return 0;
    }
}

/* --- Struct Member Offset Helper (Simplified) --- */
int get_member_offset(Type* struct_type, const char* member_name) {
    if (!struct_type || (struct_type->kind != TYPE_STRUCT && struct_type->kind != TYPE_UNION)) return -1;
    if (struct_type->kind == TYPE_UNION) return 0; // All union members start at offset 0

    int offset = 0;
    for (Member* m = struct_type->data.struct_union_info.members; m; m = m->next) {
        if (strcmp(m->name, member_name) == 0) {
            return offset;
        }
        offset += get_type_size(m->type); // Simplified: no padding/alignment
    }
    return -1; // Member not found
}


// A simple stack to keep track of loop labels for break/continue
#define MAX_LOOP_DEPTH 50
static TacAddr* continue_stack[MAX_LOOP_DEPTH];
static TacAddr* break_stack[MAX_LOOP_DEPTH];
static int loop_stack_ptr = -1;

void push_loop_labels(TacAddr* continue_label, TacAddr* break_label) {
    if (loop_stack_ptr < MAX_LOOP_DEPTH - 1) {
        loop_stack_ptr++;
        continue_stack[loop_stack_ptr] = continue_label;
        break_stack[loop_stack_ptr] = break_label;
    } else {
        fprintf(stderr, "Fatal: Exceeded maximum loop nesting depth.\n");
        exit(1);
    }
}

void pop_loop_labels() {
    if (loop_stack_ptr >= 0) {
        loop_stack_ptr--;
    }
}

TacAddr* get_loop_continue_label() {
    if (loop_stack_ptr >= 0) {
        return continue_stack[loop_stack_ptr];
    }
    return NULL;
}

TacAddr* get_loop_break_label() {
    if (loop_stack_ptr >= 0) {
        return break_stack[loop_stack_ptr];
    }
    return NULL;
}

/* --- Address Creation Helpers --- */

TacAddr* new_temp(Type* type) { // Store type
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_TEMP;
    addr->val.temp_id = temp_count++;
    addr->type = type ? copy_type(type) : create_type(TYPE_UNKNOWN); // Store a copy of the type
    return addr;
}

TacAddr* new_label_addr() {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_LABEL;
    addr->val.label_id = label_count++;
    addr->type = NULL;
    return addr;
}

// Renamed and New Constant Creators
TacAddr* new_const_int(int val) {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_CONSTANT_INT;
    addr->val.const_int = val;
    addr->type = create_type(TYPE_INT);
    return addr;
}

TacAddr* new_const_float(double val) { // Store as double
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_CONSTANT_FLOAT;
    addr->val.const_float = val;
    // Base type is double, adjust if float vs double matters significantly in TAC
    addr->type = create_type(TYPE_DOUBLE);
    return addr;
}

// Helper specifically for char constants -> store as int
TacAddr* new_const_char(char val) {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_CONSTANT_INT; // Store char const as its integer value
    addr->val.const_int = (int)val;
    addr->type = create_type(TYPE_CHAR);
    return addr;
}

TacAddr* new_var_addr(Symbol* sym) {
    if (!sym) {
         fprintf(stderr, "Error: Attempting to create variable address from NULL symbol.\n");
         return NULL;
    }
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_VARIABLE;
    addr->val.var = sym;
    addr->type = sym->type ? copy_type(sym->type) : create_type(TYPE_UNKNOWN); // Copy type from symbol
    return addr;
}

// Manage string literals
TacAddr* new_string_addr(char* string_value) {
    // Check if this string already exists
    for (StringLiteral* sl = string_literal_list; sl; sl = sl->next) {
        if (strcmp(sl->value, string_value) == 0) {
            TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
            addr->kind = ADDR_STRING;
            addr->val.string_label = strdup(sl->label);
            addr->type = create_type(TYPE_STRING);
            return addr;
        }
    }

    // Create a new entry
    char label_buf[32];
    sprintf(label_buf, "S%d", string_lit_count++);

    StringLiteral* new_sl = (StringLiteral*)malloc(sizeof(StringLiteral));
    new_sl->label = strdup(label_buf);
    new_sl->value = strdup(string_value); // Store the actual string content
    new_sl->next = string_literal_list;
    string_literal_list = new_sl;

    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_STRING;
    addr->val.string_label = strdup(label_buf);
    addr->type = create_type(TYPE_STRING); // Type is conceptually char* or string
    return addr;
}

/* --- Core TAC Emitter --- */

void emit(TacOp op, TacAddr* res, TacAddr* arg1, TacAddr* arg2) {
    TacInstr* instr = (TacInstr*)calloc(1, sizeof(TacInstr));
    instr->op = op;
    instr->res = res;
    instr->arg1 = arg1;
    instr->arg2 = arg2;
    instr->next = NULL;
    instr->prev = tac_list_tail;

    if (!instr->res && (op == TAC_LABEL || op == TAC_GOTO || op == TAC_IFZ || op == TAC_IFNZ || op == TAC_STORE)) {
         // These ops use 'res' for label or store address, allow NULL if not needed by op
    } else if (!instr->res && op != TAC_BEGIN_FUNC && op != TAC_END_FUNC && op != TAC_PARAM && op != TAC_RETURN && op != TAC_CALL) {
        // Most ops require a result address, except those listed above
        // fprintf(stderr, "Warning: Emitting instruction (Op: %d) with NULL result address.\n", op);
        // This can happen validly if expression result is discarded
    }


    if (tac_list_head == NULL) {
        tac_list_head = instr;
        tac_list_tail = instr;
    } else {
        tac_list_tail->next = instr;
        tac_list_tail = instr;
    }
}

/* --- TAC Printing Utility --- */

void print_addr(TacAddr* addr) {
    if (!addr) {
        printf("----");
        return;
    }
    switch (addr->kind) {
        case ADDR_VARIABLE: printf("%s", addr->val.var ? addr->val.var->name : "NULLSYM"); break;
        case ADDR_TEMP:     printf("t%d", addr->val.temp_id); break;
        case ADDR_CONSTANT_INT: printf("%d", addr->val.const_int); break; // <-- UPDATED
        case ADDR_CONSTANT_FLOAT: printf("%.*g", addr->type && addr->type->kind == TYPE_FLOAT ? 7 : 15, addr->val.const_float); break; // <-- NEW (Print float/double)
        case ADDR_LABEL:    printf("L%d", addr->val.label_id); break;
        case ADDR_STRING:   printf("%s", addr->val.string_label); break;
        default:            printf("BAD_ADDR"); break;
    }
}

// Helper to map binary AST ops to TAC ops
TacOp get_tac_op_for_binary(int ast_op, Type* type1, Type* type2) {
    // Determine if float operation is needed
    bool is_float = (type1 && (type1->kind == TYPE_FLOAT || type1->kind == TYPE_DOUBLE)) ||
                    (type2 && (type2->kind == TYPE_FLOAT || type2->kind == TYPE_DOUBLE));

    switch (ast_op) {
        case '+': return is_float ? TAC_FADD : TAC_ADD;
        case '-': return is_float ? TAC_FSUB : TAC_SUB;
        case '*': return is_float ? TAC_FMUL : TAC_MUL;
        case '/': return is_float ? TAC_FDIV : TAC_DIV;
        case '%': return TAC_MOD; // Modulo usually integer only

        // Bitwise/Shift are integer only
        case '&': return TAC_BAND;
        case '|': return TAC_BOR;
        case '^': return TAC_BXOR;
        case LSHIFT_OP: return TAC_LSHIFT;
        case RSHIFT_OP: return TAC_RSHIFT;

        // Relational
        case EQ_OP: return is_float ? TAC_FEQ : TAC_EQ;
        case NE_OP: return is_float ? TAC_FNE : TAC_NE;
        case '<':   return is_float ? TAC_FLT : TAC_LT;
        case LE_OP: return is_float ? TAC_FLE : TAC_LE;
        case '>':   return is_float ? TAC_FGT : TAC_GT;
        case GE_OP: return is_float ? TAC_FGE : TAC_GE;

        default: return TAC_UNDEF;
    }
}

TacOp get_base_op_for_assignment(int assign_op) {
    switch(assign_op) {
        case ADD_ASSIGN: return TAC_ADD;
        case SUB_ASSIGN: return TAC_SUB;
        case MUL_ASSIGN: return TAC_MUL;
        case DIV_ASSIGN: return TAC_DIV;
        case MOD_ASSIGN: return TAC_MOD;
        case LSHIFT_ASSIGN: return TAC_LSHIFT;
        case RSHIFT_ASSIGN: return TAC_RSHIFT;
        case AND_ASSIGN: return TAC_BAND;
        case XOR_ASSIGN: return TAC_BXOR;
        case OR_ASSIGN: return TAC_BOR;
        default: return TAC_UNDEF; // Includes '=' which is handled separately
    }
}
TacOp get_base_op_for_assignment_float(int assign_op) {
     switch(assign_op) {
        case ADD_ASSIGN: return TAC_FADD;
        case SUB_ASSIGN: return TAC_FSUB;
        case MUL_ASSIGN: return TAC_FMUL;
        case DIV_ASSIGN: return TAC_FDIV;
        default: return TAC_UNDEF; // Others usually integer only
    }
}

void print_tac() {
    // Print string literals first (Data Segment)
    printf("\n--- Data Segment ---\n");
    StringLiteral* current_sl = string_literal_list;
    StringLiteral* reversed_list = NULL;
    while (current_sl) {
        StringLiteral* next = current_sl->next;
        current_sl->next = reversed_list;
        reversed_list = current_sl;
        current_sl = next;
    }
    for (StringLiteral* sl = reversed_list; sl; sl = sl->next) {
         // Escape special characters in the string for .asciiz
         printf("%s: .asciiz \"", sl->label);
         for(char *p = sl->value; *p; p++) {
             switch(*p) {
                 case '\\': printf("\\\\"); break;
                 case '"': printf("\\\""); break;
                 case '\n': printf("\\n"); break;
                 case '\t': printf("\\t"); break;
                 // Add other escapes if needed
                 default: putchar(*p); break;
             }
         }
         printf("\"\n");
    }
    string_literal_list = NULL; // Clear/reset for potential future use


    printf("\n--- Three-Address Code (Text Segment) ---\n");
    for (TacInstr* instr = tac_list_head; instr; instr = instr->next) {
        if (instr->op == TAC_LABEL) {
            print_addr(instr->res);
            printf(":");
        } else if (instr->op == TAC_BEGIN_FUNC) {
             printf("BEGIN_FUNC: ");
             print_addr(instr->res);
             printf("\n");
             printf("\t(PROLOGUE: push ebp, mov ebp, esp, sub esp, <locals_size>)\n");
        } else if (instr->op == TAC_END_FUNC) {
             printf("\t(EPILOGUE: mov esp, ebp, pop ebp)\n");
             printf("END_FUNC: ");
             print_addr(instr->res);
             printf("\n");
        } else {
            printf("\t");
            // Do not print a generic "res = " for ops that handle their own formatting
            if (instr->res && instr->op != TAC_GOTO && instr->op != TAC_IFZ &&
                instr->op != TAC_IFNZ && instr->op != TAC_STORE &&
                instr->op != TAC_PARAM && instr->op != TAC_LABEL &&
                instr->op != TAC_RETURN && instr->op != TAC_CALL &&
                instr->op != TAC_BEGIN_FUNC && instr->op != TAC_END_FUNC)
             {
                 print_addr(instr->res);
                 printf(" = ");
             }

            switch (instr->op) {
                // Assignment & Arithmetic
                case TAC_ASSIGN: print_addr(instr->arg1); break;
                case TAC_ADD: print_addr(instr->arg1); printf(" + "); print_addr(instr->arg2); break;
                case TAC_SUB: print_addr(instr->arg1); printf(" - "); print_addr(instr->arg2); break;
                case TAC_MUL: print_addr(instr->arg1); printf(" * "); print_addr(instr->arg2); break;
                case TAC_DIV: print_addr(instr->arg1); printf(" / "); print_addr(instr->arg2); break;
                case TAC_MOD: print_addr(instr->arg1); printf(" %% "); print_addr(instr->arg2); break;
                case TAC_NEG: printf("-"); print_addr(instr->arg1); break;

                // Floating Point
                case TAC_FADD: print_addr(instr->arg1); printf(" F+ "); print_addr(instr->arg2); break;
                case TAC_FSUB: print_addr(instr->arg1); printf(" F- "); print_addr(instr->arg2); break;
                case TAC_FMUL: print_addr(instr->arg1); printf(" F* "); print_addr(instr->arg2); break;
                case TAC_FDIV: print_addr(instr->arg1); printf(" F/ "); print_addr(instr->arg2); break;
                case TAC_FNEG: printf("F-"); print_addr(instr->arg1); break;

                // Conversions
                case TAC_CVT_I2F: printf("(float)"); print_addr(instr->arg1); break;
                case TAC_CVT_F2I: printf("(int)"); print_addr(instr->arg1); break;
                case TAC_CVT_D2F: printf("(float)"); print_addr(instr->arg1); break;  // <-- NEW
                case TAC_CVT_F2D: printf("(double)"); print_addr(instr->arg1); break; // <-- NEW

                // Bitwise
                case TAC_BAND: print_addr(instr->arg1); printf(" & "); print_addr(instr->arg2); break;
                case TAC_BOR: print_addr(instr->arg1); printf(" | "); print_addr(instr->arg2); break;
                case TAC_BXOR: print_addr(instr->arg1); printf(" ^ "); print_addr(instr->arg2); break;
                case TAC_BNOT: printf("~"); print_addr(instr->arg1); break;
                case TAC_LSHIFT: print_addr(instr->arg1); printf(" << "); print_addr(instr->arg2); break;
                case TAC_RSHIFT: print_addr(instr->arg1); printf(" >> "); print_addr(instr->arg2); break;

                // Logical & Relational
                case TAC_EQ: print_addr(instr->arg1); printf(" == "); print_addr(instr->arg2); break;
                case TAC_NE: print_addr(instr->arg1); printf(" != "); print_addr(instr->arg2); break;
                case TAC_LT: print_addr(instr->arg1); printf(" < "); print_addr(instr->arg2); break;
                case TAC_LE: print_addr(instr->arg1); printf(" <= "); print_addr(instr->arg2); break;
                case TAC_GT: print_addr(instr->arg1); printf(" > "); print_addr(instr->arg2); break;
                case TAC_GE: print_addr(instr->arg1); printf(" >= "); print_addr(instr->arg2); break;
                case TAC_NOT: printf("!"); print_addr(instr->arg1); break;

                // Float Relational
                case TAC_FEQ: print_addr(instr->arg1); printf(" F== "); print_addr(instr->arg2); break;
                case TAC_FNE: print_addr(instr->arg1); printf(" F!= "); print_addr(instr->arg2); break;
                case TAC_FLT: print_addr(instr->arg1); printf(" F< "); print_addr(instr->arg2); break;
                case TAC_FLE: print_addr(instr->arg1); printf(" F<= "); print_addr(instr->arg2); break;
                case TAC_FGT: print_addr(instr->arg1); printf(" F> "); print_addr(instr->arg2); break;
                case TAC_FGE: print_addr(instr->arg1); printf(" F>= "); print_addr(instr->arg2); break;


                // Jumps & Labels
                // Correction in printing jump instructions
                case TAC_GOTO: printf("goto "); print_addr(instr->res); break;
                case TAC_IFZ: printf("ifz "); print_addr(instr->arg1); printf(" goto "); print_addr(instr->res); break;
                case TAC_IFNZ: printf("ifnz "); print_addr(instr->arg1); printf(" goto "); print_addr(instr->res); break;

                // Function Calls
                case TAC_STORE:
                   printf("*"); // Indicate pointer dereference for store
                   print_addr(instr->res); // The address to store into
                   printf(" = ");
                   print_addr(instr->arg1); // The value to store
                   break;
                case TAC_RETURN:
                    printf("return");
                    if (instr->arg1) { // --- FIX: Check arg1 for return value ---
                        printf(" ");
                        print_addr(instr->arg1);
                    }
                    break;
                case TAC_PARAM: printf("param "); print_addr(instr->arg1); break;
                case TAC_CALL:
                    if (instr->res) { print_addr(instr->res); printf(" = "); }
                    printf("call "); print_addr(instr->arg1);
                    if (instr->arg2) { printf(", "); print_addr(instr->arg2); } // Arg count
                    break;

                 // Memory, Pointers, Addresses
                 case TAC_ADDR: printf("&"); print_addr(instr->arg1); break;
                 case TAC_DEREF: printf("*"); print_addr(instr->arg1); break;
                 case TAC_LEA: print_addr(instr->arg1); printf(" + "); print_addr(instr->arg2); break; // Or specific LEA format

                 // Array/Member Access (Simplified)
                 case TAC_INDEX_READ: print_addr(instr->arg1); printf("["); print_addr(instr->arg2); printf("]"); break;
                 case TAC_INDEX_WRITE: print_addr(instr->arg1); printf("["); print_addr(instr->arg2); printf("] = "); print_addr(instr->res); break;

                default: printf("UNKNOWN_OP(%d)", instr->op); break;
            }
        }
        printf("\n");
    }
    printf("--------------------------------------------\n");
}

/* --- Forward Declarations --- */
void gen_tac_for_node(ASTNode* node);
TacAddr* gen_tac_for_expr(ASTNode* node, bool is_lvalue);
void gen_tac_for_condition(ASTNode* node, TacAddr* true_label, TacAddr* false_label);
TacAddr* emit_conversion(TacAddr* source_addr, Type* target_type);

void gen_tac_for_initializer_list(TacAddr* base_addr, Type* aggregate_type, ASTNode* initializer) {
    if (!initializer || initializer->type != NODE_INITIALIZER_LIST) {
        // Allow single expression to initialize first element of struct/array? C++ allows this. C99 does for first member.
        // Let's stick to requiring braces for aggregates for now.
        fprintf(stderr, "Error line %d: Expected initializer list '{...}' for aggregate type '%s'.\n", initializer ? initializer->lineno : 0, type_to_string(aggregate_type));
        return;
    }
    if (!aggregate_type || (aggregate_type->kind != TYPE_ARRAY && aggregate_type->kind != TYPE_STRUCT && aggregate_type->kind != TYPE_UNION)) {
         fprintf(stderr, "Error line %d: Initializer list target is not array, struct, or union.\n", initializer->lineno);
         return;
    }
    // Cannot initialize unions with initializer lists directly in this manner easily
    if (aggregate_type->kind == TYPE_UNION) {
         fprintf(stderr, "Error line %d: Cannot use initializer list for unions directly (initialize first member instead).\n", initializer->lineno);
         return;
    }

    int index = 0; // Tracks current element index for arrays
    Member* current_member = (aggregate_type->kind == TYPE_STRUCT) ? aggregate_type->data.struct_union_info.members : NULL;
    int current_struct_offset = 0; // Tracks current byte offset within struct (for padding)

    for (ASTNodeList* item = initializer->data.items_list; item; item = item->next) {
        ASTNode* current_init_node = item->node;
        Type* elem_type = NULL;
        int elem_offset = 0; // Byte offset from base_addr for this element
        int elem_alignment = 1; // Required alignment for this element

        if (aggregate_type->kind == TYPE_ARRAY) {
            elem_type = aggregate_type->data.base_info.base;
            if (!elem_type) { /* Error */ continue; }

            // --- CORRECTED OFFSET/STRIDE CALCULATION ---
            int element_size_with_padding = get_type_size(elem_type); // Get full size including potential padding
            if (element_size_with_padding <= 0 && elem_type->kind != TYPE_VOID) {
                fprintf(stderr, "Error line %d: Cannot determine size of array element type '%s'.\n", current_init_node->lineno, type_to_string(elem_type));
                return; // Cannot proceed
            }
            elem_offset = index * element_size_with_padding; // Offset = index * size_of_element_including_padding
            // --- END CORRECTION ---

            index++; // Increment index only for arrays

        } else { // Struct
            if (!current_member) {
                fprintf(stderr, "Warning line %d: Too many initializers for struct '%s'.\n", current_init_node->lineno, aggregate_type->data.struct_union_info.name ? aggregate_type->data.struct_union_info.name : "");
                break;
            }
            elem_type = current_member->type;
            if (!elem_type) { /* Error */ current_member = current_member->next; continue; }

            // --- Calculate offset considering alignment ---
            int member_size = get_type_size(elem_type);
            elem_alignment = member_size; // Simple alignment
            // Add padding BEFORE member
            if (current_struct_offset % elem_alignment != 0) {
                 current_struct_offset += elem_alignment - (current_struct_offset % elem_alignment);
            }
            elem_offset = current_struct_offset;
            current_struct_offset += member_size; // Advance offset for next member
            // --- End alignment ---

            current_member = current_member->next;
        }

        // Calculate address for the current element: base_addr + elem_offset
        TacAddr* current_elem_addr = base_addr;
        if (elem_offset != 0) { // Optimize: no ADD if offset is 0
             current_elem_addr = new_temp(create_pointer_type(elem_type)); // Address is pointer to element
             emit(TAC_ADD, current_elem_addr, base_addr, new_const_int(elem_offset));
        }


        if (current_init_node->type == NODE_INITIALIZER_LIST) {
            // Recursive call for nested aggregates (arrays or structs)
            gen_tac_for_initializer_list(current_elem_addr, elem_type, current_init_node);
        } else {
            // Generate value for the current element
            TacAddr* rvalue = gen_tac_for_expr(current_init_node, false);
            if (!rvalue) continue;
            rvalue = emit_conversion(rvalue, elem_type); // Convert initializer value if needed

            // Store value: *(base_addr + elem_offset) = rvalue
            emit(TAC_STORE, current_elem_addr, rvalue, NULL); // Use calculated address
        }
    }
}

/* --- TAC Generation from AST --- */

void generate_tac(ASTNode* root) {
    if (!root) return;
    // Reset counters and lists
    temp_count = 0;
    label_count = 0;
    string_lit_count = 0;
    control_flow_stack_top = -1;
    tac_list_head = tac_list_tail = NULL;
    string_literal_list = NULL; // Clear string literal list

    gen_tac_for_node(root);

    // Clean up string literal list (optional, if needed elsewhere)
    // while (string_literal_list) { ... free ... }
}

TacAddr* emit_conversion(TacAddr* source_addr, Type* target_type) {
    if (!source_addr || !target_type || !source_addr->type) return source_addr; // Cannot convert
    if (are_types_compatible(source_addr->type, target_type) && source_addr->type->kind == target_type->kind) {
         return source_addr; // No conversion needed
    }

    bool src_is_float_family = (source_addr->type->kind == TYPE_FLOAT || source_addr->type->kind == TYPE_DOUBLE);
    bool tgt_is_float_family = (target_type->kind == TYPE_FLOAT || target_type->kind == TYPE_DOUBLE);
    bool src_is_int = is_integer_type(source_addr->type);
    bool tgt_is_int = is_integer_type(target_type);
    
    bool src_is_double = (source_addr->type->kind == TYPE_DOUBLE);
    bool tgt_is_float = (target_type->kind == TYPE_FLOAT);
    bool src_is_float = (source_addr->type->kind == TYPE_FLOAT);
    bool tgt_is_double = (target_type->kind == TYPE_DOUBLE);


    if (src_is_int && tgt_is_float_family) {
        TacAddr* result = new_temp(target_type);
        emit(TAC_CVT_I2F, result, source_addr, NULL); // Use I2F, code gen will handle float/double
        return result;
    } else if (src_is_float_family && tgt_is_int) {
        TacAddr* result = new_temp(target_type);
        emit(TAC_CVT_F2I, result, source_addr, NULL); // Use F2I, code gen will handle float/double
        return result;
    } else if (src_is_double && tgt_is_float) {
        TacAddr* result = new_temp(target_type);
        emit(TAC_CVT_D2F, result, source_addr, NULL);
        return result;
    } else if (src_is_float && tgt_is_double) {
        TacAddr* result = new_temp(target_type);
        emit(TAC_CVT_F2D, result, source_addr, NULL);
        return result;
    }
    
    // Add other conversions if needed (e.g., int sizes)
    // For now, assume other compatible types don't need explicit TAC conversion
    source_addr->type = copy_type(target_type); // Update type if implicitly converted
    return source_addr;
}

// Generates TAC for an expression.
// If is_lvalue is true, returns the *address* of the result (e.g., address of arr[i] or obj.member).
// If is_lvalue is false, returns a temporary/variable containing the *value* of the expression.
TacAddr* gen_tac_for_expr(ASTNode* node, bool is_lvalue) {
    if (!node) return NULL;

    switch (node->type) {
        case NODE_CONSTANT: {
            Type* const_type = infer_constant_type(node->data.stringValue);
            if (const_type->kind == TYPE_CHAR) {
                 // Handle 'c' -> get ASCII value
                 if (strlen(node->data.stringValue) >= 3) {
                     // Basic handling, doesn't support escapes like '\n' well here
                     return new_const_char(node->data.stringValue[1]);
                 } else {
                     return new_const_char(0); // Error or default
                 }
            } else if (const_type->kind == TYPE_FLOAT || const_type->kind == TYPE_DOUBLE) {
                // Return float or double constant
                TacAddr* const_addr = new_const_float(atof(node->data.stringValue));
                // Infer_constant_type correctly identifies float vs double
                if (const_type->kind == TYPE_FLOAT) {
                    const_addr->type = create_type(TYPE_FLOAT);
                }
                return const_addr;
            } else { // Integer types
                // Handle hex (0x...), octal (0...), binary (0b...) if needed
                // For simplicity, using strtol for auto-detection
                return new_const_int((int)strtol(node->data.stringValue, NULL, 0));
            }
        }

        case NODE_STRING_LITERAL:
            return new_string_addr(node->data.stringValue);

        case NODE_IDENTIFIER: {
            Symbol* sym = node->symbol; // <<<--- Get symbol directly from AST annotation

            if (!sym) {
                // If the symbol is NULL here, it means semantic analysis failed to annotate it.
                // This indicates a deeper issue, possibly in scope handling during semantic analysis
                // or in the annotation logic itself (e.g., for parameters).
                fprintf(stderr, "Compiler Internal Error (Line %d): AST node for identifier '%s' was not annotated with symbol information. Semantic analysis phase likely incomplete for this node.\n",
                        node->lineno, node->data.stringValue);
                return NULL; // Cannot proceed without symbol info
            }

            TacAddr* var_addr = new_var_addr(sym); // Use the symbol found via annotation

            if (is_lvalue) {
                // We need the address of the variable.
                TacAddr* result_addr = new_temp(create_pointer_type(var_addr->type));
                emit(TAC_ADDR, result_addr, var_addr, NULL); // result_addr = &var_addr
                return result_addr;
            }
            else {
                // If we need the value, handle array/function decay to pointer.
                if (var_addr->type && (var_addr->type->kind == TYPE_ARRAY || var_addr->type->kind == TYPE_FUNCTION)) {
                    TacAddr* result_addr = new_temp(create_pointer_type(var_addr->type));
                    emit(TAC_ADDR, result_addr, var_addr, NULL); // Emit address for arrays/functions
                    return result_addr;
                }
                 // For simple variables (int, float, ptrs), return the var_addr itself.
                 // TAC operations like ADD will treat it as a value.
                return var_addr;
            }
        }
        case NODE_QUALIFIED_ID: {
            // Semantic analysis has attached the symbol for the static member.
            Symbol* sym = node->symbol;
            if (!sym) {
                fprintf(stderr, "Compiler Internal Error (Line %d): AST node for qualified ID was not annotated with symbol information.\n", node->lineno);
                return new_temp(create_type(TYPE_UNKNOWN));
            }
            // Treat it just like a global variable access.
            TacAddr* var_addr = new_var_addr(sym);
            if (is_lvalue) {
                TacAddr* result = new_temp(create_pointer_type(var_addr->type));
                emit(TAC_ADDR, result, var_addr, NULL);
                return result;
            } else {
                return var_addr;
            }
        }

        case NODE_BINARY_EXPR: {
            // Special handling for short-circuiting logical operators (always return value)
            if (node->data.binary_expr.op == LAND_OP || node->data.binary_expr.op == LOR_OP) {
                TacAddr* result = new_temp(create_type(TYPE_INT)); // Logical result is int (0 or 1)
                TacAddr* true_lbl = new_label_addr();
                TacAddr* false_lbl = new_label_addr();
                TacAddr* end_lbl = new_label_addr();

                gen_tac_for_condition(node, true_lbl, false_lbl); // Use helper

                emit(TAC_LABEL, true_lbl, NULL, NULL);      // True case
                emit(TAC_ASSIGN, result, new_const_int(1), NULL);
                emit(TAC_GOTO, end_lbl, NULL, NULL);        // Jump to end

                emit(TAC_LABEL, false_lbl, NULL, NULL);     // False case
                emit(TAC_ASSIGN, result, new_const_int(0), NULL);

                emit(TAC_LABEL, end_lbl, NULL, NULL);       // End label
                return result;

            } else {
                TacAddr* left = gen_tac_for_expr(node->data.binary_expr.left, false);
                TacAddr* right = gen_tac_for_expr(node->data.binary_expr.right, false);
                if (!left || !right) return NULL;

                // --- Determine result type and handle conversions ---
                Type* left_type = left->type;
                Type* right_type = right->type;
                
                // Handle string concatenation
                if (node->data.binary_expr.op == '+') {
                    // Check if either operand is string or char*
                    bool left_is_str = (left_type && (left_type->kind == TYPE_STRING || (left_type->kind == TYPE_POINTER && left_type->data.base_info.base->kind == TYPE_CHAR)));
                    bool right_is_str = (right_type && (right_type->kind == TYPE_STRING || (right_type->kind == TYPE_POINTER && right_type->data.base_info.base->kind == TYPE_CHAR)));

                    if (left_is_str || right_is_str) {
                        // We need runtime support for this. Emit calls to a helper.
                        // Example: result = _string_concat(left, right);
                        // Ensure left and right are converted to string pointers if needed.

                        fprintf(stderr, "Warning line %d: String concatenation TAC requires runtime support (emitting placeholder).\n", node->lineno);

                        // Placeholder: Assume a function _string_concat exists
                        Symbol* concat_sym = find_symbol("_string_concat"); // Need to predefine this or handle lookup failure
                        if (!concat_sym) {
                            // Create a dummy symbol if needed for TAC
                            concat_sym = calloc(1, sizeof(Symbol));
                            concat_sym->name = "_string_concat"; // Runtime function name
                            // Define its type if possible (e.g., function returning string ptr)
                        }

                        TacAddr* result = new_temp(create_type(TYPE_STRING)); // Assuming result is string
                        // Emit params (reverse order)
                        emit(TAC_PARAM, NULL, right, NULL); // TODO: Convert right to string if needed
                        emit(TAC_PARAM, NULL, left, NULL);  // TODO: Convert left to string if needed
                        emit(TAC_CALL, result, new_var_addr(concat_sym), new_const_int(2));
                        return result;
                    }
                    // If not string concat, fall through to arithmetic...
                }

                Type* result_type = get_common_arithmetic_type(left_type, right_type); // Gives float/double if either is float/double

                left = emit_conversion(left, result_type);   // Convert left if necessary
                right = emit_conversion(right, result_type); // Convert right if necessary

                TacAddr* result = new_temp(result_type);
                TacOp op = get_tac_op_for_binary(node->data.binary_expr.op, result_type, result_type); // Use final type

                if (op == TAC_UNDEF) { /* Warning */ return NULL; }
                emit(op, result, left, right);
                return result;
            }
        }

        case NODE_UNARY_EXPR:
        case NODE_PREFIX_UNARY_EXPR: {
            bool operand_must_be_lvalue = (node->data.unary_expr.op == INC_OP || node->data.unary_expr.op == DEC_OP || node->data.unary_expr.op == '&');

            // Special case: `*ptr` used as an lvalue
            if (is_lvalue && node->data.unary_expr.op == '*') {
                 TacAddr* ptr_val = gen_tac_for_expr(node->data.unary_expr.operand, false); // Get pointer value
                 if (!ptr_val || !ptr_val->type || ptr_val->type->kind != TYPE_POINTER) { /* Error */ return NULL;}
                 // The address *is* the value held by the pointer
                 return ptr_val;
            }

            // Get operand value or address
            TacAddr* operand_addr_or_val = gen_tac_for_expr(node->data.unary_expr.operand, operand_must_be_lvalue);
            if (!operand_addr_or_val) return NULL;


            switch(node->data.unary_expr.op) {
                case INC_OP: // ++b
                case DEC_OP: // --b
                    {
                        TacAddr* one_int = new_const_int(1);
                        TacAddr* one_float = new_const_float(1.0); // For potential float increment

                        // operand_addr_or_val holds the ADDRESS (&b) from is_lvalue=true
                        Type* var_type = (operand_addr_or_val->type && operand_addr_or_val->type->kind == TYPE_POINTER) ? operand_addr_or_val->type->data.base_info.base : create_type(TYPE_UNKNOWN);
                        
                        TacAddr* current_val = new_temp(var_type);
                        emit(TAC_DEREF, current_val, operand_addr_or_val, NULL); // current_val = *(&b)

                        bool is_float = var_type && (var_type->kind == TYPE_FLOAT || var_type->kind == TYPE_DOUBLE);
                        TacAddr* result_val = new_temp(var_type);
                        TacAddr* one_val = is_float ? one_float : one_int;
                        TacOp op = (node->data.unary_expr.op == INC_OP) ? (is_float ? TAC_FADD : TAC_ADD) : (is_float ? TAC_FSUB : TAC_SUB);

                        emit(op, result_val, current_val, one_val);     // result = current +/- 1
                        emit(TAC_STORE, operand_addr_or_val, result_val, NULL); // *(&b) = result

                        return result_val; // Prefix returns the *new* value
                    }
                case '&': { // Address-of: operand_addr_or_val already holds the address
                    return operand_addr_or_val;
                }
                case '*': { // Dereference (R-value)
                     Type* deref_type = (operand_addr_or_val->type && operand_addr_or_val->type->kind == TYPE_POINTER) ? operand_addr_or_val->type->data.base_info.base : create_type(TYPE_UNKNOWN);
                     TacAddr* result = new_temp(deref_type);
                     emit(TAC_DEREF, result, operand_addr_or_val, NULL); // result = *operand_addr_or_val (value of pointer)
                     return result;
                }
                case '-': { // Negation
                    bool is_float = operand_addr_or_val->type && (operand_addr_or_val->type->kind == TYPE_FLOAT || operand_addr_or_val->type->kind == TYPE_DOUBLE);
                    TacAddr* result = new_temp(operand_addr_or_val->type);
                    emit(is_float ? TAC_FNEG : TAC_NEG, result, operand_addr_or_val, NULL);
                    return result;
                }
                 case '!': { // Logical Not
                    TacAddr* result = new_temp(create_type(TYPE_INT));
                    emit(TAC_NOT, result, operand_addr_or_val, NULL);
                    return result;
                 }
                 case '~': { // Bitwise Not
                     TacAddr* result = new_temp(operand_addr_or_val->type);
                     emit(TAC_BNOT, result, operand_addr_or_val, NULL);
                     return result;
                 }
                default: /* Warning */ return NULL;
            }
        }
        case NODE_POSTFIX_UNARY_EXPR: { // b++, b--
            TacAddr* one_int = new_const_int(1);
            TacAddr* one_float = new_const_float(1.0);

            // Need both address and current value
            TacAddr* operand_lvalue = gen_tac_for_expr(node->data.unary_expr.operand, true); // Get &b
            if (!operand_lvalue || !operand_lvalue->type || operand_lvalue->type->kind != TYPE_POINTER) return NULL;
            
            Type* var_type = operand_lvalue->type->data.base_info.base;
            TacAddr* current_val = new_temp(var_type); // Create temp for current value
            emit(TAC_DEREF, current_val, operand_lvalue, NULL); // current_val = *(&b)


            bool is_float = var_type && (var_type->kind == TYPE_FLOAT || var_type->kind == TYPE_DOUBLE);
            TacAddr* next_val = new_temp(var_type);
            TacAddr* one_val = is_float ? one_float : one_int;
            TacOp op = (node->data.unary_expr.op == INC_OP) ? (is_float ? TAC_FADD : TAC_ADD) : (is_float ? TAC_FSUB : TAC_SUB);

            emit(op, next_val, current_val, one_val);           // next_val = current +/- 1
            emit(TAC_STORE, operand_lvalue, next_val, NULL);    // *(&b) = next_val

            return current_val; // Postfix returns the *original* value
        }

        case NODE_ASSIGNMENT: {
            TacAddr* lvalue_addr = gen_tac_for_expr(node->data.assignment.lvalue, true); // Get ADDRESS of lvalue
            TacAddr* rvalue = gen_tac_for_expr(node->data.assignment.rvalue, false);   // Get VALUE of rvalue
            if (!lvalue_addr || !rvalue) return NULL;

            // Get target type from the lvalue address (type it points to)
            Type* target_type = (lvalue_addr->type && lvalue_addr->type->kind == TYPE_POINTER) ? lvalue_addr->type->data.base_info.base : NULL;
            if (!target_type) { /* Error */ return NULL; }

            // Convert rvalue if necessary
            rvalue = emit_conversion(rvalue, target_type);

            TacOp base_op = TAC_UNDEF;
            bool is_compound = false;
            bool op_is_float = (target_type && (target_type->kind == TYPE_FLOAT || target_type->kind == TYPE_DOUBLE));

            if (node->data.assignment.op != '=') {
                is_compound = true;
                base_op = op_is_float ? get_base_op_for_assignment_float(node->data.assignment.op)
                                      : get_base_op_for_assignment(node->data.assignment.op);
                if (base_op == TAC_UNDEF) { /* Error or warning */ }
            }

            TacAddr* final_rvalue; // --- Store the final value to return ---

            if (is_compound && base_op != TAC_UNDEF) {
                // Expand: L = L op R
                // 1. Load current value of L
                TacAddr* current_l_val = new_temp(target_type);
                emit(TAC_DEREF, current_l_val, lvalue_addr, NULL); // current = *(addr_L)

                // 2. Compute result
                TacAddr* result_val = new_temp(target_type);
                emit(base_op, result_val, current_l_val, rvalue); // result = current op rvalue

                // 3. Store result back
                emit(TAC_STORE, lvalue_addr, result_val, NULL); // *(addr_L) = result

                final_rvalue = result_val; // --- The new value is the result ---
            } else {
                 // Simple assignment: L = R
                 emit(TAC_STORE, lvalue_addr, rvalue, NULL); // *(addr_L) = rvalue
                 final_rvalue = rvalue; // --- The assigned value is the result ---
            }
            // --- MODIFICATION: Return the final R-value ---
            // The value of an assignment expression is the value that was assigned.
            return final_rvalue;
            // --- END MODIFICATION ---
        }

        case NODE_FUNC_CALL: { // Cannot be lvalue
            ASTNode* func_expr_node = node->data.func_call.function;
            TacAddr* func_addr = NULL; // Will hold the 'this' pointer for member calls

            // 1. Evaluate arguments and emit PARAM instructions (reverse order)
            int arg_count = 0;
            ASTNodeList* arg_list[100]; // Assume max 100 args
            for (ASTNodeList* arg_item = node->data.func_call.arguments; arg_item; arg_item = arg_item->next) {
                 if (arg_count < 100) arg_list[arg_count++] = arg_item;
                 else { /* Error: too many arguments */ break; }
            }
            for (int i = arg_count - 1; i >= 0; --i) {
                TacAddr* arg_addr = gen_tac_for_expr(arg_list[i]->node, false);
                if (!arg_addr) return NULL; // Propagate error
                emit(TAC_PARAM, NULL, arg_addr, NULL);
            }

            // 2. Check for member function call to handle 'this' pointer
            if (func_expr_node->type == NODE_MEMBER_ACCESS) {
                // It's a member call (obj.func() or obj->func()).
                // Get the address of the object to pass as the 'this' pointer.
                func_addr = gen_tac_for_expr(func_expr_node->data.member_access.object, true);
                if (!func_addr) return NULL;

                // Pass the 'this' pointer. As we are pushing in reverse, this will be
                // emitted AFTER the other arguments to become the FIRST argument.
                emit(TAC_PARAM, NULL, func_addr, NULL);
                arg_count++; // Increment arg count for the 'this' pointer
                // Get the address of the MEMBER FUNCTION itself.
                // The semantic analyzer should have attached the function's symbol to the node.
                if (!func_expr_node->symbol) {
                     fprintf(stderr, "Compiler Error line %d: Member function access node not annotated with symbol.\n", func_expr_node->lineno);
                     return NULL;
                }
                func_addr = new_var_addr(func_expr_node->symbol);
            } else {
                // It is a normal C-style call (e.g. factorial(5))
                
                // If it's a direct call to a function (like 'factorial'), the node is an identifier
                // with a symbol attached by the semantic analyzer.
                if (func_expr_node->type == NODE_IDENTIFIER && func_expr_node->symbol) {
                    // Directly use the symbol for the function. This avoids the extra "t = &func" step.
                    func_addr = new_var_addr(func_expr_node->symbol);
                } else {
                    // For more complex cases (like calling a function pointer),
                    // we still need to evaluate the expression to get the address.
                    func_addr = gen_tac_for_expr(func_expr_node, false);
                }
            }

            if (!func_addr) {
                fprintf(stderr, "Compiler Error line %d: Could not resolve function address for call.\n", node->lineno);
                return NULL;
            }

            // 3. Emit CALL instruction
            TacAddr* result = NULL;
            Type* return_type = NULL;

            // Get return type from the function's type, not the object's
            Type* func_type = func_addr->type;
            if (func_type && func_type->kind == TYPE_POINTER && func_type->data.base_info.base->kind == TYPE_FUNCTION) {
                 func_type = func_type->data.base_info.base; // Handle function pointers
            }
            if (func_type && func_type->kind == TYPE_FUNCTION) {
                 return_type = func_type->data.function_sig.return_type;
            } else {
                 fprintf(stderr, "Warning line %d: Cannot determine return type of called function.\n", node->lineno);
                 return_type = create_type(TYPE_INT); // Assume int if unknown
            }

            if (return_type && return_type->kind != TYPE_VOID) {
               result = new_temp(return_type);
            }

            emit(TAC_CALL, result, func_addr, new_const_int(arg_count));
            return result;
        }

        case NODE_CONDITIONAL_EXPR: {
             TacAddr* result;
             TacAddr* true_label = new_label_addr();
             TacAddr* false_label = new_label_addr();
             TacAddr* end_label = new_label_addr();

             // We need the result temp *before* the branches to assign to it.
             // The semantic analyzer has already inserted cast nodes,
             // so both branches will evaluate to the same type.
             // We can generate the code for the true_expr, check its type,
             // and create the result temporary based on that.
             // This is still not ideal (side effects), but it's the
             // simplest fix without annotating the AST.

             // Let's create the result temp *after* the true branch
             // This is the cleanest logic.
             gen_tac_for_condition(node->data.conditional_expr.condition, true_label, false_label);

             // True part
             emit(TAC_LABEL, true_label, NULL, NULL);
             TacAddr* true_val = gen_tac_for_expr(node->data.conditional_expr.true_expr, false);
             if (!true_val) return NULL;

             // Create result temp *after* getting true_val
             result = new_temp(true_val->type);
             emit(TAC_ASSIGN, result, true_val, NULL);
             emit(TAC_GOTO, end_label, NULL, NULL);

             // False part
             emit(TAC_LABEL, false_label, NULL, NULL);
             TacAddr* false_val = gen_tac_for_expr(node->data.conditional_expr.false_expr, false);
             if (!false_val) return NULL;
             
             // Convert false_val to result type if needed
             false_val = emit_conversion(false_val, result->type);
             emit(TAC_ASSIGN, result, false_val, NULL);

             emit(TAC_LABEL, end_label, NULL, NULL);
             return result;
         }

        case NODE_ARRAY_SUBSCRIPT: {
            // --- REVISED LOGIC FOR MULTIDIMENSIONAL ACCESS ---
            // 1. Traverse inwards to find the base identifier/expression
            ASTNode* base_expr_node = node;
            ASTNodeList* subscript_list = NULL;
            int num_subscripts = 0;
            while (base_expr_node && base_expr_node->type == NODE_ARRAY_SUBSCRIPT) {
                // Prepend index node to list (builds list in reverse order of access)
                ASTNodeList* new_item = create_list_node(base_expr_node->data.array_subscript.index);
                new_item->next = subscript_list;
                subscript_list = new_item;
                num_subscripts++;
                base_expr_node = base_expr_node->data.array_subscript.array;
            }

            if (!base_expr_node) return NULL; // Should not happen

            // 2. Get the base address and the full type of the base array/pointer
            TacAddr* base_addr = gen_tac_for_expr(base_expr_node, false); // Get base addr (e.g., &arr or ptr_value)
            if (!base_addr || !base_addr->type) return NULL;

            Type* base_type = base_addr->type;
            Type* final_element_type = base_type; // Will hold type after all subscripts

            // Adjust base_addr and base_type if the base itself is an array (array decay)
             TacAddr* effective_base_addr = base_addr;
             if (base_type->kind == TYPE_ARRAY) {
                 // The address of the array is its base address.
                 effective_base_addr = new_temp(create_pointer_type(base_type->data.base_info.base));
                 emit(TAC_ADDR, effective_base_addr, base_addr, NULL); // Get address if base_addr was just the symbol
                 final_element_type = base_type; // Start with full array type for offset calc
             } else if (base_type->kind == TYPE_POINTER) {
                 // Base address is already the pointer value.
                 effective_base_addr = base_addr;
                 final_element_type = base_type; // Start with pointer type for offset calc
             } else {
                 fprintf(stderr, "Error line %d: Base of array subscript is not an array or pointer.\n", node->lineno);
                 // Free subscript_list before returning
                 while (subscript_list) { ASTNodeList* next = subscript_list->next; free(subscript_list); subscript_list = next; }
                 return NULL;
             }


            // 3. Calculate the total byte offset using the new recursive function
            TacAddr* total_offset = calculate_array_offset_recursive_new(base_expr_node, subscript_list, num_subscripts, 0, final_element_type);

             // Update final_element_type based on dimensions applied
             for(int i=0; i < num_subscripts; ++i) {
                  if (final_element_type && (final_element_type->kind == TYPE_ARRAY || final_element_type->kind == TYPE_POINTER)) {
                     final_element_type = final_element_type->data.base_info.base;
                  } else {
                      final_element_type = create_type(TYPE_UNKNOWN); // Error occurred
                      break;
                  }
             }


            // Free the temporary list used for subscripts
            while (subscript_list) { ASTNodeList* next = subscript_list->next; free(subscript_list); subscript_list = next; }

            if (!total_offset || !final_element_type || final_element_type->kind == TYPE_UNKNOWN) {
                 fprintf(stderr, "Error line %d: Failed to calculate array offset or determine element type.\n", node->lineno);
                 return NULL;
            }


            // 4. Calculate the final element address: effective_base_addr + total_offset
            TacAddr* element_addr = new_temp(create_pointer_type(final_element_type));
            emit(TAC_ADD, element_addr, effective_base_addr, total_offset); // element_addr = base + offset

            if (is_lvalue) {
                return element_addr; // Return the calculated address
            } else {
                // Dereference the final address to get the value
                TacAddr* result = new_temp(final_element_type);
                emit(TAC_DEREF, result, element_addr, NULL); // result = *element_addr
                return result;
            }
            // --- END REVISED LOGIC ---
        }

        case NODE_MEMBER_ACCESS: {
             bool is_ptr = node->data.member_access.is_pointer;
             TacAddr* obj_addr_or_val = gen_tac_for_expr(node->data.member_access.object, is_ptr ? false : true); 
             if (!obj_addr_or_val) return NULL;

             Type* base_type;
             TacAddr* obj_base_addr; // This will hold the *actual* address of the struct

             if (is_ptr) {
                 // obj_addr_or_val is the *value* of the pointer
                 base_type = (obj_addr_or_val->type && obj_addr_or_val->type->kind == TYPE_POINTER) ? obj_addr_or_val->type->data.base_info.base : NULL;
                 obj_base_addr = obj_addr_or_val; // Use the pointer value directly
             } else {
                 // obj_addr_or_val is the *address* of the struct (from is_lvalue=true)
                 base_type = (obj_addr_or_val->type && obj_addr_or_val->type->kind == TYPE_POINTER) ? obj_addr_or_val->type->data.base_info.base : NULL;
                 obj_base_addr = obj_addr_or_val; // Use the address
             }

             if (!base_type || (base_type->kind != TYPE_STRUCT && base_type->kind != TYPE_UNION)) { /* Error */ return NULL; }

             const char* member_name = node->data.member_access.member_name;
             int offset_val = get_member_offset(base_type, member_name);
             if (offset_val < 0) { /* Error: member not found */ return NULL; }
             TacAddr* offset_addr = new_const_int(offset_val);

             // Find member type
             Type* member_type = NULL;
             for (Member* m = base_type->data.struct_union_info.members; m; m=m->next) {
                 if (strcmp(m->name, member_name) == 0) {
                     member_type = m->type;
                     break;
                 }
             }
             if (!member_type) return NULL;

             // If the member is a function, we don't calculate an offset.
             // We just return a direct address to the function's symbol.
             if (member_type->kind == TYPE_FUNCTION) {
                 // The semantic analyzer should have attached the function's symbol to this node.
                 if (node->symbol) {
                     return new_var_addr(node->symbol);
                 } else {
                     // Fallback: This is less robust, but can work if the member name is unique.
                     // It's better to ensure semantic analysis annotates the node.
                     fprintf(stderr, "Warning line %d: Member function access node was not annotated with a direct symbol. Falling back to name lookup for '%s'.\n", node->lineno, member_name);
                     Symbol* func_sym = find_symbol(member_name);
                     if (func_sym) return new_var_addr(func_sym);
                 }
                 // If we reach here, we couldn't find the function symbol.
                 fprintf(stderr, "Compiler Error line %d: Could not resolve symbol for member function '%s'.\n", node->lineno, member_name);
                 return NULL;
             }
             TacAddr* member_abs_addr = new_temp(create_pointer_type(member_type));
             emit(TAC_ADD, member_abs_addr, obj_base_addr, offset_addr); // member_addr = base_addr + offset

             if (is_lvalue) {
                 return member_abs_addr;
             } else {
                 TacAddr* result = new_temp(member_type);
                 emit(TAC_DEREF, result, member_abs_addr, NULL);
                 return result;
             }
         }

        // --- NEW: NODE_CAST_EXPRESSION ---
        case NODE_CAST_EXPRESSION: {
            TacAddr* expr_val = gen_tac_for_expr(node->data.cast_expr.expr, false);
            if (!expr_val) return NULL;

            Type* target_type = NULL;
            if (node->data.cast_expr.type_name && node->data.cast_expr.type_name->type == NODE_DECLARATION) {
                 target_type = get_type_from_specifiers(node->data.cast_expr.type_name->data.declaration.specifiers);
            }
            else if (node->data.cast_expr.type_name && node->data.cast_expr.type_name->type == NODE_TYPENAME) {
                char* type_str = node->data.cast_expr.type_name->data.stringValue;
                // Basic type matching
                if (strcmp(type_str, "int") == 0) target_type = create_type(TYPE_INT);
                else if (strcmp(type_str, "float") == 0) target_type = create_type(TYPE_FLOAT);
                else if (strcmp(type_str, "double") == 0) target_type = create_type(TYPE_DOUBLE);
                else if (strcmp(type_str, "char") == 0) target_type = create_type(TYPE_CHAR);
                else if (strcmp(type_str, "bool") == 0) target_type = create_type(TYPE_BOOL);
                // --- FIX: Handle char[] as char* ---
                else if (strcmp(type_str, "char[]") == 0) {
                     target_type = create_pointer_type(create_type(TYPE_CHAR));
                }
                // --- END FIX ---
                // Add more complex pointer/array types if needed
            }
            else {
                 fprintf(stderr, "Error line %d: Invalid type specified in cast.\n", node->lineno);
                 return new_temp(create_type(TYPE_UNKNOWN));
            }

            if (!target_type) { // Fallback if string didn't match
                fprintf(stderr, "Error line %d: Unhandled cast type '%s'.\n", node->lineno,
                    node->data.cast_expr.type_name->type == NODE_TYPENAME ? node->data.cast_expr.type_name->data.stringValue : "complex_type");
                return expr_val; // Return unconverted
            }

            TacAddr* result = emit_conversion(expr_val, target_type);
            // Don't free target_type here, emit_conversion might use it or copy it.
            // Memory management of types needs careful review.
            // free(target_type); // Potential double free if copy_type used internally
            return result;
        }

        default:
            fprintf(stderr, "Warning line %d: TAC generation not fully implemented for expression node type %d\n", node->lineno, node->type);
            return NULL;
    }
}

// Generates jumps based on condition evaluation (handles short-circuiting)
void gen_tac_for_condition(ASTNode* node, TacAddr* true_label, TacAddr* false_label) {
    if (!node || !true_label || !false_label) return;

    if (node->type == NODE_BINARY_EXPR) {
        if (node->data.binary_expr.op == LAND_OP) { // &&
            TacAddr* mid_label = new_label_addr(); // Label after evaluating left side
            gen_tac_for_condition(node->data.binary_expr.left, mid_label, false_label); // If left is false, short-circuit to false_label
            emit(TAC_LABEL, mid_label, NULL, NULL);
            gen_tac_for_condition(node->data.binary_expr.right, true_label, false_label); // Result depends on right side
            return;
        } else if (node->data.binary_expr.op == LOR_OP) { // ||
            TacAddr* mid_label = new_label_addr(); // Label after evaluating left side
            gen_tac_for_condition(node->data.binary_expr.left, true_label, mid_label); // If left is true, short-circuit to true_label
            emit(TAC_LABEL, mid_label, NULL, NULL);
            gen_tac_for_condition(node->data.binary_expr.right, true_label, false_label); // Result depends on right side
            return;
        }
        // Handle relational operators directly
        switch(node->data.binary_expr.op) {
            case EQ_OP: case NE_OP: case '<': case LE_OP: case '>': case GE_OP:
            {
                TacAddr* left = gen_tac_for_expr(node->data.binary_expr.left, false);
                 TacAddr* right = gen_tac_for_expr(node->data.binary_expr.right, false);
                 if (!left || !right) return;

                 // --- Handle type conversions for comparison ---
                 Type* common_type = get_common_arithmetic_type(left->type, right->type);
                 left = emit_conversion(left, common_type);
                 right = emit_conversion(right, common_type);

                 TacOp cmp_op = get_tac_op_for_binary(node->data.binary_expr.op, common_type, common_type);
                 free(common_type); // Free temporary type

                 if (cmp_op >= TAC_EQ && cmp_op <= TAC_GE) { // Integer comparison
                     TacAddr* cmp_res = new_temp(create_type(TYPE_INT));
                     emit(cmp_op, cmp_res, left, right);
                     emit(TAC_IFNZ, true_label, cmp_res, NULL);
                 } else if (cmp_op >= TAC_FEQ && cmp_op <= TAC_FGE) { // Float comparison
                      TacAddr* cmp_res = new_temp(create_type(TYPE_INT)); // Result of comparison is int (0 or 1)
                      emit(cmp_op, cmp_res, left, right);
                      emit(TAC_IFNZ, true_label, cmp_res, NULL);
                 } else {
                     fprintf(stderr, "Error line %d: Invalid comparison operator in condition.\n", node->lineno);
                 }
                 emit(TAC_GOTO, false_label, NULL, NULL);
                 return;
            }
        }
    } else if (node->type == NODE_UNARY_EXPR && node->data.unary_expr.op == '!') {
         // Handle logical NOT: !condition jumps if condition is TRUE
         gen_tac_for_condition(node->data.unary_expr.operand, false_label, true_label); // Swap true/false labels
         return;
    }

    // Default case: Evaluate expression, jump if zero/non-zero
    TacAddr* cond_val = gen_tac_for_expr(node, false);
    if (!cond_val) return;
    emit(TAC_IFNZ, true_label, cond_val, NULL); // If non-zero, jump to true
    emit(TAC_GOTO, false_label, NULL, NULL);    // Otherwise, jump to false
}


// Generates TAC for statements and declarations.
void gen_tac_for_node(ASTNode* node) {
    // This function is now only called for simple cases that aren't
    // control-flow statements. The complex logic is in semantic.c.
    if (!node) return;

    switch (node->type) {
        case NODE_TRANSLATION_UNIT:
            // Could emit global variable initializations here if tracked separately
            for (ASTNodeList* item = node->data.items_list; item; item = item->next) {
                gen_tac_for_node(item->node);
            }
            break;

        // --- THIS IS THE MAIN FIX ---
        case NODE_DECLARATION: {
            if (!node->data.declaration.declarators) break; // e.g., `struct Student;`

            // Check if this is a typedef; if so, it produces no executable code.
            if (node->data.declaration.specifiers && node->data.declaration.specifiers->node) {
                ASTNode* first_spec = node->data.declaration.specifiers->node;
                if (first_spec->type == NODE_SPECIFIER && first_spec->data.specifier == TYPEDEF) {
                    break; // Skip TAC generation for typedefs
                }
            }
            for (ASTNodeList* d_item = node->data.declaration.declarators; d_item; d_item = d_item->next) {
                ASTNode* init_decl = d_item->node;
                ASTNode* declarator = init_decl->data.init_declarator.declarator;
                ASTNode* initializer = init_decl->data.init_declarator.initializer;

                // 1. Find the base Identifier node for this declarator
                ASTNode* id_node = declarator;
                // Handle qualified IDs like `Animal::count`
                if (id_node && id_node->type == NODE_QUALIFIED_ID) {
                    // For a qualified ID, the symbol is on the qualified_id node itself.
                    // We don't need to traverse further.
                } 
                else{

                    while (id_node && id_node->type != NODE_IDENTIFIER) {
                        if (id_node->type == NODE_POINTER_DECLARATOR) id_node = id_node->data.pointer_declarator.base_declarator;
                        else if (id_node->type == NODE_ARRAY_DECLARATOR) id_node = id_node->data.array_declarator.base_declarator;
                        else if (id_node->type == NODE_FUNCTION_DECLARATOR) id_node = id_node->data.function_declarator.base_declarator;
                        else if (id_node->type == NODE_REFERENCE_DECLARATOR) id_node = id_node->data.reference_declarator.base_declarator;
                        else id_node = NULL; // Should not happen
                    }
                }
                
                if (!id_node || id_node->type != NODE_IDENTIFIER) continue; // No identifier (e.g., abstract declarator)
                if (!id_node->symbol) { // Safety check
                     fprintf(stderr, "Compiler Error line %d: No symbol for identifier '%s' in declarator.\n", id_node->lineno, id_node->data.stringValue);
                     continue;
                }

                // 2. Handle the initializer
                if (initializer) {
                    // Get the address of the variable being declared (e.g., `&arr`, `&ptr`, `&a`)
                    TacAddr* lvalue_addr = gen_tac_for_expr(id_node, true);
                    if (!lvalue_addr) continue;

                    Type* target_type = id_node->symbol->type;
                    if (!target_type) continue;

                    if (initializer->type == NODE_INITIALIZER_LIST) {
                         // --- FIX FOR NESTED INITIALIZERS ---
                        gen_tac_for_initializer_list(lvalue_addr, target_type, initializer);
                         // --- END FIX ---
                    } else {
                        TacAddr* rvalue = gen_tac_for_expr(initializer, false);
                        if (!rvalue) continue;
                        rvalue = emit_conversion(rvalue, target_type);
                        // --- Use res=address, arg1=value for STORE ---
                        emit(TAC_STORE, lvalue_addr, rvalue, NULL);
                        // --- END FIX ---
                    }
                }
            }
            break;
        }
        case NODE_FUNCTION_DEFINITION: {
            char* func_name = get_name_from_declarator(node->data.function_definition.declarator);
            if (!func_name) { /* Error */ return; }
            Symbol* func_sym = find_symbol(func_name);
            TacAddr* func_label = func_sym ? new_var_addr(func_sym) : NULL;
            if (!func_label) { // Should not happen if semantic analysis passed
                func_label = new_temp(create_type(TYPE_UNKNOWN)); // Error fallback
                func_label->kind = ADDR_VARIABLE;
                func_label->val.var = calloc(1, sizeof(Symbol));
                func_label->val.var->name = strdup(func_name);
            }


            // --- NEW: GOTO/LABEL Pre-pass ---
            destroy_label_map(); // Clear any previous map
            find_all_labels_in_body(node->data.function_definition.body); // Pre-pass
            // --- END NEW ---

            emit(TAC_BEGIN_FUNC, func_label, NULL, NULL);
            // TODO: Emit TAC for parameter retrieval from stack/registers
            // TODO: Emit TAC for local variable stack allocation (requires pre-pass)

            gen_tac_for_node(node->data.function_definition.body);

            // Ensure there's a return for void functions or if last statement isn't return
            if (!tac_list_tail || (tac_list_tail->op != TAC_RETURN && tac_list_tail->op != TAC_GOTO)) {
                if (func_sym && func_sym->type && func_sym->type->kind == TYPE_FUNCTION &&
                    func_sym->type->data.function_sig.return_type->kind == TYPE_VOID) {
                    emit(TAC_RETURN, NULL, NULL, NULL); // FIX: Was emit(TAC_RETURN, NULL, NULL, NULL) which is correct for void
                } else {
                    // Non-void function ending without return - semantic error
                    // We'll emit a return without a value, which is fine
                     emit(TAC_RETURN, NULL, NULL, NULL);
                }
            }

            emit(TAC_END_FUNC, func_label, NULL, NULL); // Marker for end
            break;
        }
        case NODE_COMPOUND_STATEMENT:
            // Enter scope conceptually for symbol table if needed, not usually for TAC
            for (ASTNodeList* item = node->data.compound_statement.items; item; item = item->next) {
                gen_tac_for_node(item->node);
            }
            // Leave scope conceptually
            break;

        case NODE_EXPRESSION_STATEMENT:
            if (node->data.expression_statement.expression) {
                gen_tac_for_expr(node->data.expression_statement.expression, false); // Result is discarded
            }
            break;

        case NODE_IF_STATEMENT: {
            TacAddr* true_label = new_label_addr(); // Label for the 'if' body
            TacAddr* false_label = new_label_addr();
            TacAddr* end_label = node->data.if_statement.else_body ? new_label_addr() : false_label;

            // Generate condition code, jump to true_label if true, false_label if false
            gen_tac_for_condition(node->data.if_statement.condition, true_label, false_label);

            // If body
            emit(TAC_LABEL, true_label, NULL, NULL);
            gen_tac_for_node(node->data.if_statement.if_body);
            if (node->data.if_statement.else_body) {
                 emit(TAC_GOTO, end_label, NULL, NULL); // Skip else part
            }

            // Else part
            emit(TAC_LABEL, false_label, NULL, NULL);
            if (node->data.if_statement.else_body) {
                gen_tac_for_node(node->data.if_statement.else_body);
                emit(TAC_LABEL, end_label, NULL, NULL); // End label after else
            }
            break;
         }
        case NODE_WHILE_STATEMENT: {
            TacAddr* start_label = new_label_addr(); // Condition check
            TacAddr* body_label = new_label_addr();  // Body start
            TacAddr* end_label = new_label_addr();   // After loop

            push_control_flow(end_label, start_label);

            emit(TAC_LABEL, start_label, NULL, NULL);
            gen_tac_for_condition(node->data.while_statement.condition, body_label, end_label);

            emit(TAC_LABEL, body_label, NULL, NULL);
            gen_tac_for_node(node->data.while_statement.body);
            emit(TAC_GOTO, start_label, NULL, NULL);

            emit(TAC_LABEL, end_label, NULL, NULL);
            pop_control_flow();
            break;
         }
        case NODE_UNTIL_STATEMENT: {
            TacAddr* start_label = new_label_addr();
            TacAddr* end_label = new_label_addr();

            // An 'until' loop is equivalent to 'while(!condition)'.
            // We need to check the condition at the top.
            
            push_loop_labels(start_label, end_label); // continue jumps to condition, break jumps to end

            // Label for the start of the loop (condition check)
            emit(TAC_LABEL, start_label, NULL, NULL);

            // Generate TAC for the condition.
            // If the condition is TRUE, we exit the loop (jump to end_label).
            // If the condition is FALSE, we fall through to the loop body.
            TacAddr* cond_val = gen_tac_for_expr(node->data.until_statement.condition, false);
            if (cond_val) {
                emit(TAC_IFNZ, end_label, cond_val, NULL); // if (condition != 0) goto end_label;
            }

            // Generate TAC for the loop body.
            gen_tac_for_node(node->data.until_statement.body);

            // After the body, jump back to the condition check.
            emit(TAC_GOTO, start_label, NULL, NULL);

            // Label for the end of the loop.
            emit(TAC_LABEL, end_label, NULL, NULL);

            pop_loop_labels();
            break;
        }
        case NODE_DO_WHILE_STATEMENT: {
            TacAddr* start_label = new_label_addr(); // Body start
            TacAddr* cond_label = new_label_addr(); // Condition check
            TacAddr* end_label = new_label_addr();

            push_control_flow(end_label, cond_label);

            emit(TAC_LABEL, start_label, NULL, NULL);
            gen_tac_for_node(node->data.do_while_statement.body);

            emit(TAC_LABEL, cond_label, NULL, NULL);
            gen_tac_for_condition(node->data.do_while_statement.condition, start_label, end_label); // True->Start, False->End

            emit(TAC_LABEL, end_label, NULL, NULL);
            pop_control_flow();
            break;
         }
        case NODE_FOR_STATEMENT: {
             TacAddr* cond_label = new_label_addr();
             TacAddr* inc_label = new_label_addr();
             TacAddr* body_label = new_label_addr();
             TacAddr* end_label = new_label_addr();

             push_control_flow(end_label, inc_label);

             if (node->data.for_statement.init) gen_tac_for_node(node->data.for_statement.init);
             emit(TAC_LABEL, cond_label, NULL, NULL);

             if (node->data.for_statement.condition) {
                 ASTNode* cond_expr = node->data.for_statement.condition->data.expression_statement.expression;
                 if (cond_expr) {
                    gen_tac_for_condition(cond_expr, body_label, end_label);
                 } else {
                    emit(TAC_GOTO, end_label, NULL, NULL); // Empty condition is false? No, C for-loop empty condition is true.
                 }
             } else {
                  emit(TAC_GOTO, body_label, NULL, NULL); // No condition == true
             }

             emit(TAC_LABEL, body_label, NULL, NULL);
             gen_tac_for_node(node->data.for_statement.body);
             emit(TAC_LABEL, inc_label, NULL, NULL);
             if (node->data.for_statement.increment) gen_tac_for_expr(node->data.for_statement.increment, false);
             emit(TAC_GOTO, cond_label, NULL, NULL);

             emit(TAC_LABEL, end_label, NULL, NULL);
             pop_control_flow();
             break;
         }

        case NODE_RETURN_STATEMENT: {
            TacAddr* retval = NULL;
            if (node->data.return_statement.expression) {
                retval = gen_tac_for_expr(node->data.return_statement.expression, false);
                 if (!retval) {
                     fprintf(stderr, "Error line %d: Failed to generate TAC for return expression.\n", node->lineno);
                     // Emit return without value as fallback?
                     emit(TAC_RETURN, NULL /* No result reg needed */, NULL /* No value */, NULL);
                 } else {
                     // --- FIX: Pass return value as ARG1 ---
                     emit(TAC_RETURN, NULL /* No result reg needed */, retval /* Value to return */, NULL);
                 }
            } else {
                 emit(TAC_RETURN, NULL, NULL, NULL); // Void return
            }
            break;
        }

        case NODE_BREAK_STATEMENT: {
            ControlFlowContext* ctx = current_control_flow();
            if (ctx && ctx->break_label) {
                emit(TAC_GOTO, ctx->break_label, NULL, NULL);
            } else {
                 fprintf(stderr, "Error line %d: 'break' statement not within loop or switch.\n", node->lineno);
            }
            break;
        }

        case NODE_CONTINUE_STATEMENT: {
            ControlFlowContext* ctx = current_control_flow();
            if (ctx && ctx->continue_label) {
                emit(TAC_GOTO, ctx->continue_label, NULL, NULL);
            } else {
                 fprintf(stderr, "Error line %d: 'continue' statement not within loop.\n", node->lineno);
            }
            break;
        }

        case NODE_SWITCH_STATEMENT: {
             TacAddr* switch_expr = gen_tac_for_expr(node->data.switch_statement.expression, false);
             if (!switch_expr) return;

             TacAddr* end_label = new_label_addr();
             TacAddr* default_label = NULL; // Will be assigned if a default case exists

             // --- START OF FIX: Revised Switch Logic ---

             // A structure to link case statements in the AST to their TAC labels
             typedef struct CaseInfo {
                 ASTNode* node;       // The NODE_CASE_STATEMENT or NODE_DEFAULT_STATEMENT
                 TacAddr* body_label; // The TAC label for the start of its code
                 struct CaseInfo* next;
             } CaseInfo;

             CaseInfo* case_info_list = NULL;
             CaseInfo* case_info_tail = NULL;

             ASTNodeList* body_items = node->data.switch_statement.body->data.compound_statement.items;

             // --- Pass 1: Collect all case/default nodes and create labels IN ORDER ---
             for (ASTNodeList* item = body_items; item; item = item->next) {
                 if (item->node->type == NODE_CASE_STATEMENT || item->node->type == NODE_DEFAULT_STATEMENT) {
                     CaseInfo* new_info = calloc(1, sizeof(CaseInfo));
                     new_info->node = item->node;
                     new_info->body_label = new_label_addr();
                     new_info->next = NULL;

                     if (item->node->type == NODE_DEFAULT_STATEMENT) {
                         default_label = new_info->body_label;
                     }

                     // Append to list to maintain source order
                     if (!case_info_list) {
                         case_info_list = case_info_tail = new_info;
                     } else {
                         case_info_tail->next = new_info;
                         case_info_tail = new_info;
                     }
                 }
             }

             // If no explicit default, the default action is to jump to the end
             if (!default_label) {
                 default_label = end_label;
             }

             push_control_flow(end_label, NULL); // Set break label

             // --- Pass 2: Emit the jump table (the series of comparisons) ---
             for (CaseInfo* ci = case_info_list; ci; ci = ci->next) {
                 if (ci->node->type == NODE_CASE_STATEMENT) {
                     TacAddr* case_val = gen_tac_for_expr(ci->node->data.case_statement.expression, false);
                     TacAddr* cmp_res = new_temp(create_type(TYPE_INT));
                     emit(TAC_EQ, cmp_res, switch_expr, case_val);
                     emit(TAC_IFNZ, ci->body_label, cmp_res, NULL); // If equal, jump to the case body
                 }
             }
             emit(TAC_GOTO, default_label, NULL, NULL); // If no cases match, go to default

             // --- Pass 3: Emit the actual code for the switch body ---
             for (ASTNodeList* item = body_items; item; item = item->next) {
                 // Check if the current statement is a case/default and emit its label
                 for (CaseInfo* ci = case_info_list; ci; ci = ci->next) {
                     if (ci->node == item->node) {
                         emit(TAC_LABEL, ci->body_label, NULL, NULL);
                         break;
                     }
                 }
                 // Generate TAC for the statement itself
                 gen_tac_for_node(item->node);
             }

             // Free the temporary info list
             while (case_info_list) {
                 CaseInfo* next = case_info_list->next;
                 free(case_info_list);
                 case_info_list = next;
             }

             emit(TAC_LABEL, end_label, NULL, NULL); // Final exit point
             pop_control_flow();
             break;
        }
        case NODE_CASE_STATEMENT:
            // The logic in NODE_SWITCH_STATEMENT places the label.
            // Here, we just need to generate the code for the statement that follows the label.
            gen_tac_for_node(node->data.case_statement.body);
            break;
        case NODE_DEFAULT_STATEMENT:
            // The logic in NODE_SWITCH_STATEMENT places the label.
            // Here, we just need to generate the code for the statement that follows the label.
            gen_tac_for_node(node->data.default_statement.body);
            break;


        // Add cases for GOTO, LABELED_STATEMENT, etc.
        case NODE_LABELED_STATEMENT: {
            TacAddr* label_addr = get_label_addr(node->data.labeled_statement.label);
            if (label_addr) {
                emit(TAC_LABEL, label_addr, NULL, NULL);
            } else {
                // This should not happen if the pre-pass worked
                fprintf(stderr, "Compiler Error: Label '%s' not found in map.\n", node->data.labeled_statement.label);
            }
            gen_tac_for_node(node->data.labeled_statement.statement);
            break;
        }
        case NODE_GOTO_STATEMENT: {
            TacAddr* label_addr = get_label_addr(node->data.goto_statement.label);
            if (label_addr) {
                emit(TAC_GOTO, label_addr, NULL, NULL);
            } else {
                 fprintf(stderr, "Error line %d: Label '%s' not found during TAC gen (should've been caught by semantic).\n", node->lineno, node->data.goto_statement.label);
            }
            break;
        }
        default:
             if (node->type >= NODE_IDENTIFIER && node->type <= NODE_LAMBDA_EXPR) {
                 gen_tac_for_expr(node, false);
             } else {
                 fprintf(stderr, "Warning line %d: TAC generation not implemented for statement node type %d\n", node->lineno, node->type);
             }
            break;
    }
}
