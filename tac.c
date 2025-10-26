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

TacAddr* calculate_array_offset(ASTNode* node, Type* current_type, TacAddr** out_element_type_addr) {
    if (!node || node->type != NODE_ARRAY_SUBSCRIPT) {
        // Base case: We've reached the innermost identifier or pointer
        *out_element_type_addr = gen_tac_for_expr(node, false); // Get base address
        return new_const_int(0); // Initial offset is 0
    }

    // Recursive step: Calculate offset for outer dimensions first
    TacAddr* base_or_parent_addr = NULL; // Address from the level above
    TacAddr* outer_offset = calculate_array_offset(node->data.array_subscript.array, current_type, &base_or_parent_addr);
    if (!outer_offset || !base_or_parent_addr || !base_or_parent_addr->type) return NULL; // Error in recursion

    Type* array_or_ptr_type = base_or_parent_addr->type;
    Type* element_type = NULL;

    if (array_or_ptr_type->kind == TYPE_ARRAY) {
        element_type = array_or_ptr_type->data.base_info.base;
         // --- NEED DIMENSION INFO HERE ---
         // int num_cols = array_or_ptr_type->data.array_info.dimension; // Get next dimension size
         // This requires storing dimensions in your Type struct during semantic analysis!
         fprintf(stderr,"Warning line %d: Multidimensional array offset calculation requires dimension info in Type struct (assuming flat layout).\n", node->lineno);
         // Without dimension info, we can only correctly handle pointers or 1D arrays
         if (array_or_ptr_type->data.base_info.base->kind == TYPE_ARRAY) {
              fprintf(stderr, "Error line %d: Cannot calculate offset for multi-D array without dimension sizes.\n", node->lineno);
              return NULL;
         }
         // --- END DIMENSION REQUIREMENT ---

    } else if (array_or_ptr_type->kind == TYPE_POINTER) {
        element_type = array_or_ptr_type->data.base_info.base;
    } else {
        fprintf(stderr, "Error line %d: Subscript applied to non-array/non-pointer type '%s'.\n", node->lineno, type_to_string(array_or_ptr_type));
        return NULL;
    }

    if (!element_type) return NULL;
    int element_size = get_type_size(element_type);
    if (element_size <= 0) { /* Error or Warning */ element_size = 1; }

    // Calculate offset for the current index
    TacAddr* index_val = gen_tac_for_expr(node->data.array_subscript.index, false);
    if (!index_val) return NULL;

    TacAddr* current_offset = new_temp(create_type(TYPE_INT));
    emit(TAC_MUL, current_offset, index_val, new_const_int(element_size));

    // Add outer offset to current offset
    TacAddr* total_offset = new_temp(create_type(TYPE_INT));
    emit(TAC_ADD, total_offset, outer_offset, current_offset);

    *out_element_type_addr = base_or_parent_addr; // Pass base address up
    return total_offset;
}

/* --- Type Size Helper (Simplified) --- */
int get_type_size(Type* type) {
    if (!type) return 0;
    switch(type->kind) {
        case TYPE_VOID:   return 0;
        case TYPE_BOOL:
        case TYPE_CHAR:   return 1;
        case TYPE_SHORT:  return 2;
        case TYPE_INT:
        case TYPE_FLOAT:
        case TYPE_ENUM:   return 4; // Assuming 4 bytes
        case TYPE_LONG:
        case TYPE_DOUBLE: return 8; // Assuming 8 bytes
        case TYPE_POINTER:
        case TYPE_REFERENCE:
        case TYPE_STRING: // Store pointer to string literal
                          return 8; // Assuming 64-bit pointers
        case TYPE_ARRAY: {
            // Need size information stored in Type struct for accurate calculation
            // int size = type->data.array_info.size; // Assuming size is stored
            // if (size <= 0) size = 1; // Default size if unknown? Problematic.
            // return size * get_type_size(type->data.base_info.base);
            fprintf(stderr, "Warning: Size calculation for arrays not fully implemented.\n");
            return get_type_size(type->data.base_info.base); // Return element size only for now
        }
        case TYPE_STRUCT:
        case TYPE_UNION: {
             // Calculate size based on members (simplified, no padding/alignment)
             int size = 0;
             int max_member_size = 0;
             for (Member* m = type->data.struct_union_info.members; m; m = m->next) {
                 int member_size = get_type_size(m->type);
                 if (type->kind == TYPE_UNION) {
                     if (member_size > max_member_size) max_member_size = member_size;
                 } else { // Struct
                     size += member_size;
                 }
             }
             return (type->kind == TYPE_UNION) ? max_member_size : size;
        }
        default: return 0;
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
            // Only print "res = " for ops that actually produce a result in 'res'
            // Exclude jumps, labels, store, param, return (unless it has a value)
            if (instr->res && instr->op != TAC_GOTO && instr->op != TAC_IFZ &&
                instr->op != TAC_IFNZ && instr->op != TAC_STORE &&
                instr->op != TAC_PARAM && instr->op != TAC_LABEL &&
                !(instr->op == TAC_RETURN && !instr->res) && /* Don't print for return w/o value */
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
                    printf("return ");
                    if (instr->res) {
                        // return can have an optional value
                        printf(" ");
                        print_addr(instr->res);
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

void gen_tac_for_initializer_list(TacAddr* base_addr, Type* base_type, ASTNode* initializer) {
    if (!initializer || initializer->type != NODE_INITIALIZER_LIST) {
        fprintf(stderr, "Error: Expected initializer list.\n"); // Should ideally have lineno
        return;
    }
    if (!base_type || (base_type->kind != TYPE_ARRAY && base_type->kind != TYPE_STRUCT)) {
         fprintf(stderr, "Error: Initializer list target is not array or struct.\n");
         return;
    }

    int index = 0;
    Type* elem_type = NULL;
    int elem_size = 0;

    if (base_type->kind == TYPE_ARRAY) {
        elem_type = base_type->data.base_info.base;
        elem_size = get_type_size(elem_type);
         if (elem_size <= 0) {
             fprintf(stderr, "Warning: Cannot determine element size for array initializer.\n");
             elem_size = 1; // Failsafe, likely wrong
         }
    } else { // Struct
        // We'd need to iterate through struct members simultaneously
        fprintf(stderr, "Warning: TAC generation for struct initializers not fully implemented.\n");
        return; // Simplification: Skip struct initializers for now
    }


    for (ASTNodeList* item = initializer->data.items_list; item; item = item->next, index++) {
        TacAddr* current_elem_addr = base_addr; // Start with base for offset calculation

        if (base_type->kind == TYPE_ARRAY) {
            // Calculate address: base_addr + (index * elem_size)
            TacAddr* offset = new_temp(create_type(TYPE_INT));
            emit(TAC_MUL, offset, new_const_int(index), new_const_int(elem_size));

            current_elem_addr = new_temp(create_pointer_type(elem_type));
            emit(TAC_ADD, current_elem_addr, base_addr, offset);
        }
        // else { // Struct - calculate offset based on member 'index' }

        if (item->node->type == NODE_INITIALIZER_LIST) {
            // Recursive call for nested lists (e.g., multidimensional arrays)
            gen_tac_for_initializer_list(current_elem_addr, elem_type, item->node);
        } else {
            // Generate value for the current element
            TacAddr* rvalue = gen_tac_for_expr(item->node, false);
            if (!rvalue) continue;
            rvalue = emit_conversion(rvalue, elem_type);

            // Store value
            emit(TAC_STORE, current_elem_addr, rvalue, NULL); // *current_elem_addr = rvalue
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
            Symbol* sym = node->symbol;                      // <<<--- ADD THIS LINE (Get symbol from AST)
            if (!sym) {
                // This should ideally not happen if semantic analysis ran correctly and annotated.
                fprintf(stderr, "Compiler Internal Error (Line %d): AST node for '%s' was not annotated with symbol information.\n", node->lineno, node->data.stringValue);
                // Attempt a global lookup as a fallback (might work for globals, not locals)
                sym = find_symbol(node->data.stringValue);
                 if (!sym) {
                     fprintf(stderr, "Error line %d: Symbol '%s' not found during TAC generation (Fallback lookup failed).\n", node->lineno, node->data.stringValue);
                     return NULL;
                 }
                 // If fallback worked, maybe log a warning?
            }

            TacAddr* var_addr = new_var_addr(sym); // This now uses the correct symbol

            if (is_lvalue) {
                TacAddr* result_addr = new_temp(create_pointer_type(var_addr->type));
                emit(TAC_ADDR, result_addr, var_addr, NULL); // result_addr = &var_addr
                return result_addr;
            }
            else {
                if (var_addr->type->kind == TYPE_ARRAY || var_addr->type->kind == TYPE_FUNCTION) {
                    TacAddr* result_addr = new_temp(create_pointer_type(var_addr->type));
                    emit(TAC_ADDR, result_addr, var_addr, NULL);
                    return result_addr;
                }
                return var_addr;
            }
        }
        case NODE_QUALIFIED_ID: {
             fprintf(stderr, "Warning line %d: TAC generation for qualified IDs (::) is not fully supported yet.\n", node->lineno);
             // Return a dummy value or NULL to avoid crashing, but it won't be correct.
             return new_temp(create_type(TYPE_UNKNOWN)); // Dummy temp to prevent cascade failures
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

            // 2. Get function address/label
            TacAddr* func_addr = gen_tac_for_expr(node->data.func_call.function, false);
            if (!func_addr) return NULL;

            // 3. Emit CALL instruction
            TacAddr* result = NULL;
            Type* return_type = NULL;
            if (func_addr->type && func_addr->type->kind == TYPE_FUNCTION) {
                 return_type = func_addr->type->data.function_sig.return_type;
            } else if (func_addr->type && func_addr->type->kind == TYPE_POINTER && func_addr->type->data.base_info.base->kind == TYPE_FUNCTION) {
                 return_type = func_addr->type->data.base_info.base->data.function_sig.return_type;
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
             // --- THIS IS THE FIX ---
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
            TacAddr* base_addr = NULL;
            // Recursively calculate the total byte offset and get the original base address
            TacAddr* total_offset = calculate_array_offset(node, NULL /* Initial type unused */, &base_addr);

            if (!total_offset || !base_addr || !base_addr->type) return NULL; // Error occurred

            Type* final_element_type = base_addr->type;
             // calculate_array_offset effectively peels off layers, so the type of base_addr
             // needs to be adjusted based on the number of subscripts peeled.
             // For now, let's assume calculate_array_offset correctly gives us the base
             // and the effective final element type needs deducing.
             ASTNode* temp_node = node;
             while(temp_node && temp_node->type == NODE_ARRAY_SUBSCRIPT) {
                 if (final_element_type && (final_element_type->kind == TYPE_ARRAY || final_element_type->kind == TYPE_POINTER)) {
                     final_element_type = final_element_type->data.base_info.base;
                 } else {
                     fprintf(stderr, "Error line %d: Too many subscripts applied.\n", node->lineno);
                     return NULL;
                 }
                 temp_node = temp_node->data.array_subscript.array;
             }
             if (!final_element_type) return NULL; // Should have element type


            // Calculate the final address: base_addr + total_offset
            TacAddr* element_addr = new_temp(create_pointer_type(final_element_type));
            emit(TAC_ADD, element_addr, base_addr, total_offset); // element_addr = base_addr + total_offset

            if (is_lvalue) {
                return element_addr; // Return the calculated address
            } else {
                // Dereference the final address to get the value
                TacAddr* result = new_temp(final_element_type);
                emit(TAC_DEREF, result, element_addr, NULL); // result = *element_addr
                return result;
            }
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
            // --- THIS IS THE FIX for semantic-generated casts ---
            else if (node->data.cast_expr.type_name && node->data.cast_expr.type_name->type == NODE_TYPENAME) {
                // HACK: Handle casts inserted by semantic analyzer
                // This is brittle. `type_to_string` must be simple.
                char* type_str = node->data.cast_expr.type_name->data.stringValue;
                if (strcmp(type_str, "int") == 0) target_type = create_type(TYPE_INT);
                else if (strcmp(type_str, "float") == 0) target_type = create_type(TYPE_FLOAT);
                else if (strcmp(type_str, "double") == 0) target_type = create_type(TYPE_DOUBLE);
                else if (strcmp(type_str, "char") == 0) target_type = create_type(TYPE_CHAR);
                else if (strcmp(type_str, "bool") == 0) target_type = create_type(TYPE_BOOL);
                // This won't handle "int*" but it will fix implicit arithmetic casts
            } 
            // --- END FIX ---
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
            if (target_type) free(target_type);
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

            for (ASTNodeList* d_item = node->data.declaration.declarators; d_item; d_item = d_item->next) {
                ASTNode* init_decl = d_item->node;
                ASTNode* declarator = init_decl->data.init_declarator.declarator;
                ASTNode* initializer = init_decl->data.init_declarator.initializer;

                // 1. Find the base Identifier node for this declarator
                ASTNode* id_node = declarator;
                while (id_node && id_node->type != NODE_IDENTIFIER) {
                    if (id_node->type == NODE_POINTER_DECLARATOR) id_node = id_node->data.pointer_declarator.base_declarator;
                    else if (id_node->type == NODE_ARRAY_DECLARATOR) id_node = id_node->data.array_declarator.base_declarator;
                    else if (id_node->type == NODE_FUNCTION_DECLARATOR) id_node = id_node->data.function_declarator.base_declarator;
                    else if (id_node->type == NODE_REFERENCE_DECLARATOR) id_node = id_node->data.reference_declarator.base_declarator;
                    else id_node = NULL; // Should not happen
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
            TacAddr* start_label = new_label_addr(); // Loop body start
            TacAddr* cond_label = new_label_addr();  // Condition check
            TacAddr* end_label = new_label_addr();   // After loop

            push_control_flow(end_label, cond_label);

            emit(TAC_LABEL, start_label, NULL, NULL);
            gen_tac_for_node(node->data.until_statement.body);

            emit(TAC_LABEL, cond_label, NULL, NULL);
            // Inverted logic for UNTIL: if(condition) goto end; else goto start;
            gen_tac_for_condition(node->data.until_statement.condition, end_label, start_label); // True->End, False->Start

            emit(TAC_LABEL, end_label, NULL, NULL);
            pop_control_flow();
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
                // --- FIX: Ensure expression is generated ---
                retval = gen_tac_for_expr(node->data.return_statement.expression, false);
                // The semantic analyzer should ensure type compatibility/insert casts
                 if (!retval) {
                     fprintf(stderr, "Error line %d: Failed to generate TAC for return expression.\n", node->lineno);
                     // Optionally emit return without value or handle error
                 }
                // --- END FIX ---
            }
            // Store return value (or NULL) in `res` field
            emit(TAC_RETURN, retval, NULL, NULL);
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
             TacAddr* default_label = end_label; // Default jumps to end if not specified
             TacAddr* next_case_label = new_label_addr(); // Start checking first case
             
             // We need to store all case labels to generate the jump table
             typedef struct CaseLabel {
                 TacAddr* value_addr; // The constant value
                 TacAddr* body_label; // The label for the case body
                 struct CaseLabel* next;
             } CaseLabel;
             CaseLabel* case_list = NULL;
             ASTNodeList* item;
             
             // --- Pass 1: Collect case labels and find default ---
             for (item = node->data.switch_statement.body->data.compound_statement.items; item; item = item->next) {
                 if (item->node->type == NODE_CASE_STATEMENT) {
                    CaseLabel* new_case = calloc(1, sizeof(CaseLabel));
                    new_case->value_addr = gen_tac_for_expr(item->node->data.case_statement.expression, false);
                    new_case->body_label = new_label_addr();
                    new_case->next = case_list;
                    case_list = new_case; // Build list in reverse
                 } else if (item->node->type == NODE_DEFAULT_STATEMENT) {
                    default_label = new_label_addr();
                 }
             }

             push_control_flow(end_label, NULL); // break goes to end_label

             // --- Pass 2: Emit jump table ---
             emit(TAC_LABEL, next_case_label, NULL, NULL); // Start of switch logic
             for (CaseLabel* c = case_list; c; c = c->next) {
                 TacAddr* cmp_res = new_temp(create_type(TYPE_INT));
                 emit(TAC_EQ, cmp_res, switch_expr, c->value_addr);
                 emit(TAC_IFNZ, c->body_label, cmp_res, NULL); // If equal, jump to case body
             }
             emit(TAC_GOTO, default_label, NULL, NULL); // If no case matched, go to default

             // --- Pass 3: Emit case bodies ---
             for (item = node->data.switch_statement.body->data.compound_statement.items; item; item = item->next) {
                 if (item->node->type == NODE_CASE_STATEMENT) {
                    // Find the corresponding label from our list
                    TacAddr* case_val = gen_tac_for_expr(item->node->data.case_statement.expression, false); // Re-gen to compare
                    for(CaseLabel* c = case_list; c; c = c->next) {
                        // This comparison is weak (e.g. constant folding) but okay for this.
                        if (c->value_addr->kind == case_val->kind && c->value_addr->val.const_int == case_val->val.const_int) {
                            emit(TAC_LABEL, c->body_label, NULL, NULL);
                            break;
                        }
                    }
                    gen_tac_for_node(item->node->data.case_statement.body);
                 } else if (item->node->type == NODE_DEFAULT_STATEMENT) {
                    emit(TAC_LABEL, default_label, NULL, NULL);
                    gen_tac_for_node(item->node->data.default_statement.body);
                 } else {
                    gen_tac_for_node(item->node); // Statements before first case
                 }
             }
             
             // Free case list
             while(case_list) {
                CaseLabel* next = case_list->next;
                // Don't free addrs, they are in the TAC list
                free(case_list);
                case_list = next;
             }

             emit(TAC_LABEL, end_label, NULL, NULL); // Final exit point
             pop_control_flow();
             break;
        }
        case NODE_CASE_STATEMENT: // Handled within SWITCH
        case NODE_DEFAULT_STATEMENT: // Handled within SWITCH
             if (control_flow_stack_top < 0 || !control_flow_stack[control_flow_stack_top].break_label) {
                fprintf(stderr, "Warning line %d: case/default statement outside switch.\n", node->lineno);
             }
             // The logic in NODE_SWITCH_STATEMENT handles the body
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
