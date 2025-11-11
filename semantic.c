#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "semantic.h"
#include "symbol_table.h"
#include "parser.tab.h"
#include "st.h"

int semantic_errors=0;
// static Type *current_function_return_type = NULL;

// Forward Declarations for Traversal
void analyze_node(ASTNode *node, FunctionAnalysisContext* context);
Type *analyze_expression(ASTNode *node);
void analyze_declaration(ASTNode *node);
void analyze_struct_or_union_specifier(ASTNode *node);
void analyze_class_specifier(ASTNode *node);
void analyze_function_definition(ASTNode *node);
void analyze_statement_with_context(ASTNode* node, FunctionAnalysisContext* context);
char* get_name_from_declarator(ASTNode* declarator);

ASTNode* find_identifier_in_declarator(ASTNode* declarator) {
    ASTNode* current = declarator;
    while (current) {
        switch (current->type) {
            case NODE_IDENTIFIER:
                return current; // Found it

            // Handle potential nesting/wrapping
            case NODE_POINTER_DECLARATOR:
                current = current->data.pointer_declarator.base_declarator;
                break;
            case NODE_ARRAY_DECLARATOR:
                current = current->data.array_declarator.base_declarator;
                break;
            case NODE_FUNCTION_DECLARATOR:
                current = current->data.function_declarator.base_declarator;
                break;
             case NODE_REFERENCE_DECLARATOR:
                 current = current->data.reference_declarator.base_declarator;
                 break;
            case NODE_QUALIFIED_ID: // If used directly in declarator (less common)
                // The actual identifier is deeper
                current = current->data.qualified_id.identifier;
                // We restart the loop logic essentially for the inner node
                continue;
            default:
                // If it's none of the above, we can't traverse further down this path.
                // This might happen with abstract declarators or complex nested structures.
                // For finding the *name*, this path failed.
                return NULL;
        }
    }
    return NULL; // Return NULL if no identifier was ultimately found
}
static int declarator_is_pointer(ASTNode *decl) {
    if (!decl) return 0;
    switch (decl->type) {
        case NODE_POINTER_DECLARATOR: return 1;
        case NODE_ARRAY_DECLARATOR:
            return declarator_is_pointer(decl->data.array_declarator.base_declarator);
        case NODE_FUNCTION_DECLARATOR:
            return declarator_is_pointer(decl->data.function_declarator.base_declarator);
        case NODE_REFERENCE_DECLARATOR:
            return declarator_is_pointer(decl->data.reference_declarator.base_declarator);
        default: return 0;
    }
}

void analyze_enum_specifier(ASTNode *node) {
    if (!node || node->type != NODE_ENUM_SPECIFIER) return;

    const char *tag_name = node->data.enumerator.name;
    Type *enum_type = create_enum_type(tag_name);
    
    Symbol* existing_symbol = tag_name ? find_symbol_in_current_scope(tag_name) : NULL;
    if(tag_name && existing_symbol) {
        // This is a use of the enum, not a definition.
        // Or it's a redefinition, which add_symbol will catch IF it has members.
        if (node->data.enum_specifier.members && existing_symbol->type->data.struct_union_info.members) {
             fprintf(stderr, "Semantic Error (Line %d): Redefinition of 'enum %s'.\n", node->lineno, tag_name);
             semantic_errors++;
             return;
        }
        if (!node->data.enum_specifier.members) {
            // It's just `enum Color c;`. The existing type is fine.
            return;
        }
    }
    
    if(tag_name) {
        add_symbol(tag_name, enum_type, SYM_TYPEDEF, node->lineno);
        add_typename(tag_name); // Also add to parser's typename list
    }

    ASTNodeList *enumerators = node->data.enum_specifier.members;
    int val = 0;
    for(ASTNodeList *e = enumerators; e; e = e->next) {
        ASTNode *enum_node = e->node;
        const char *ename = enum_node->data.enumerator.name;
        int assigned_val = val;
        if (enum_node->data.enumerator.value) {
            assigned_val = atoi(enum_node->data.enumerator.value->data.stringValue);
        }
        Type *int_type = create_type(TYPE_INT);
        Symbol *sym = add_symbol(ename, int_type, SYM_ENUM_CONST, enum_node->lineno);
        if (sym) sym->type = int_type;
        val = assigned_val + 1;
    }
}
char* get_name_from_declarator(ASTNode* declarator) {
    ASTNode* current = declarator;
    while (current) {
        switch (current->type) {
            case NODE_IDENTIFIER:
                return current->data.stringValue;
            case NODE_QUALIFIED_ID:
                return get_name_from_declarator(current->data.qualified_id.identifier);
            case NODE_POINTER_DECLARATOR:
                current = current->data.pointer_declarator.base_declarator;
                break;
            case NODE_ARRAY_DECLARATOR:
                current = current->data.array_declarator.base_declarator;
                break;
            case NODE_FUNCTION_DECLARATOR:
                current = current->data.function_declarator.base_declarator;
                break;
            case NODE_REFERENCE_DECLARATOR:
                current = current->data.reference_declarator.base_declarator;
                break;
            default:
                return NULL;
        }
    }
    return NULL;
}
// --- First Pass: Label Collection ---
void collect_labels(ASTNode* node, FunctionAnalysisContext* context) {
    if (!node) return;

    switch(node->type) {
        case NODE_LABELED_STATEMENT: {
            char* label_name = node->data.labeled_statement.label;
            // Check for duplicate labels
            for (Label* l = context->labels; l; l = l->next) {
                if (!l->is_case_label && strcmp(l->name, label_name) == 0) {
                    fprintf(stderr, "Semantic Error (Line %d): Duplicate label '%s'.\n", node->lineno, label_name);
                    semantic_errors++;
                    break;
                }
            }
            Label* new_label = calloc(1, sizeof(Label));
            new_label->name = strdup(label_name);
            new_label->is_case_label = false;
            new_label->next = context->labels;
            context->labels = new_label;
            collect_labels(node->data.labeled_statement.statement, context);
            break;
        }
        case NODE_CASE_STATEMENT:
             // We don't need to collect case labels by value in this pass,
             // just note that we are inside a switch.
             collect_labels(node->data.case_statement.body, context);
             break;
        case NODE_DEFAULT_STATEMENT:
             collect_labels(node->data.default_statement.body, context);
             break;
        case NODE_COMPOUND_STATEMENT:
            for (ASTNodeList* item = node->data.compound_statement.items; item; item = item->next) {
                collect_labels(item->node, context);
            }
            break;
        case NODE_IF_STATEMENT:
            collect_labels(node->data.if_statement.if_body, context);
            if (node->data.if_statement.else_body) {
                collect_labels(node->data.if_statement.else_body, context);
            }
            break;
        case NODE_SWITCH_STATEMENT:
            collect_labels(node->data.switch_statement.body, context);
            break;
        case NODE_WHILE_STATEMENT:
        case NODE_UNTIL_STATEMENT: 
        case NODE_DO_WHILE_STATEMENT:
        case NODE_FOR_STATEMENT:
            collect_labels(node->data.for_statement.body, context); // Assuming union has same member name
            break;
        default:
            // Other statements (expressions, jumps) don't contain labels.
            break;
    }
}


// --- Second Pass: Full Analysis with Context ---
void analyze_statement_with_context(ASTNode* node, FunctionAnalysisContext* context) {
    if (!node) return;

    switch (node->type) {
        case NODE_COMPOUND_STATEMENT:
            enter_scope();
            for (ASTNodeList *item = node->data.compound_statement.items; item; item = item->next) {
                // Use analyze_node which will delegate back to this function for statements
                analyze_node(item->node, context); 
            }
            leave_scope();
            break;

        case NODE_EXPRESSION_STATEMENT:
            if (node->data.expression_statement.expression) {
                analyze_expression(node->data.expression_statement.expression);
            }
            break;

        case NODE_IF_STATEMENT: {
            Type *cond_type = analyze_expression(node->data.if_statement.condition);
            if (cond_type && !is_scalar_type(cond_type)) { /* error */ }
            analyze_statement_with_context(node->data.if_statement.if_body, context);
            if (node->data.if_statement.else_body) {
                analyze_statement_with_context(node->data.if_statement.else_body, context);
            }
            break;
        }
        case NODE_WHILE_STATEMENT:
        case NODE_UNTIL_STATEMENT:
        case NODE_DO_WHILE_STATEMENT: {
            bool was_in_loop = context->in_loop; // Save previous state
            context->in_loop = true;

            Type *cond_type = NULL;
            if (node->type == NODE_WHILE_STATEMENT)
            {
                cond_type = analyze_expression(node->data.while_statement.condition);
                analyze_statement_with_context(node->data.while_statement.body, context);
            }
            else if (node->type == NODE_UNTIL_STATEMENT)
            {
                cond_type = analyze_expression(node->data.until_statement.condition);
                analyze_statement_with_context(node->data.until_statement.body, context);
            }
            else // NODE_DO_WHILE_STATEMENT
            {
                analyze_statement_with_context(node->data.do_while_statement.body, context);
                cond_type = analyze_expression(node->data.do_while_statement.condition);
            }

            if (cond_type && !is_scalar_type(cond_type))
            {
                fprintf(stderr, "Semantic Error (Line %d): Condition of statement must be a scalar type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
                semantic_errors++;
            }

            context->in_loop = was_in_loop; // Restore previous state
            break;
        }
        case NODE_FOR_STATEMENT: {
            bool was_in_loop = context->in_loop;
            context->in_loop = true;
            enter_scope(); // Scope for the loop variable and body
            // 1. Analyze the INIT part
            if (node->data.for_statement.init)
            {
                if (node->data.for_statement.init->type == NODE_DECLARATION)
                {
                    // If it's a declaration, analyze it as such
                    analyze_declaration(node->data.for_statement.init);
                }
                else
                {
                    // If it's an expression statement, unwrap and analyze the expression
                    ASTNode *init_expr = node->data.for_statement.init->data.expression_statement.expression;
                    if (init_expr)
                    {
                        analyze_expression(init_expr);
                    }
                }
            }

            // 2. Analyze the CONDITION part
            if (node->data.for_statement.condition)
            {
                // Unwrap the expression from the expression statement
                ASTNode *cond_expr = node->data.for_statement.condition->data.expression_statement.expression;
                if (cond_expr)
                {
                    Type *cond_type = analyze_expression(cond_expr);
                    if (cond_type && !is_scalar_type(cond_type))
                    {
                        fprintf(stderr, "Semantic Error (Line %d): Condition of for-loop must be a scalar type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
                        semantic_errors++;
                    }
                }
            }

            // 3. Analyze the INCREMENT part
            if (node->data.for_statement.increment)
            {
                // The increment part is a raw expression, so no unwrapping is needed here
                analyze_expression(node->data.for_statement.increment);
            }

            // 4. Analyze the BODY
            analyze_statement_with_context(node->data.for_statement.body, context); // Use the context-aware version
            leave_scope();
            context->in_loop = was_in_loop;
            break;
        }

        case NODE_SWITCH_STATEMENT: {
            Type *cond_type = analyze_expression(node->data.switch_statement.expression);
            if (!is_integer_type(cond_type)) { 
                fprintf(stderr, "Semantic Error (Line %d): Condition of switch statement must be an integer type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
                semantic_errors++;
            }
            context->in_switch = true;
            analyze_statement_with_context(node->data.switch_statement.body, context);
            context->in_switch = false;
            break;
        }

        case NODE_LABELED_STATEMENT:
            // The label was already validated in the first pass. Just analyze the statement.
            analyze_statement_with_context(node->data.labeled_statement.statement, context);
            break;

        case NODE_CASE_STATEMENT:
            if (!context->in_switch) {
                fprintf(stderr, "Semantic Error (Line %d): 'case' statement not in a switch.\n", node->lineno);
                semantic_errors++;
            }
            // Analyze the constant expression and the body
            analyze_expression(node->data.case_statement.expression);
            analyze_statement_with_context(node->data.case_statement.body, context);
            break;
        case NODE_DEFAULT_STATEMENT:
            if (!context->in_switch) {
                fprintf(stderr, "Semantic Error (Line %d): 'default' statement not in a switch.\n", node->lineno);
                semantic_errors++;
            }
            analyze_statement_with_context(node->data.default_statement.body, context);
            break;

        case NODE_GOTO_STATEMENT: {
            bool found = false;
            for (Label* l = context->labels; l; l = l->next) {
                if (!l->is_case_label && strcmp(l->name, node->data.goto_statement.label) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                fprintf(stderr, "Semantic Error (Line %d): Use of undeclared label '%s'.\n", node->lineno, node->data.goto_statement.label);
                semantic_errors++;
            }
            break;
        }
        case NODE_CONTINUE_STATEMENT:
            if (!context->in_loop) {
                fprintf(stderr, "Semantic Error (Line %d): 'continue' statement not in a loop.\n", node->lineno);
                semantic_errors++;
            }
            break;
        case NODE_BREAK_STATEMENT:
            if (!context->in_loop && !context->in_switch) {
                fprintf(stderr, "Semantic Error (Line %d): 'break' statement not in a loop or switch.\n", node->lineno);
                semantic_errors++;
            }
            break;
        case NODE_RETURN_STATEMENT: {
            if (!context || !context->return_type)
            {
                fprintf(stderr, "Semantic Error (Line %d): return statement not in a function.\n", node->lineno);
                semantic_errors++;
            }
            else
            {
                Type *return_expr_type = node->data.return_statement.expression ? analyze_expression(node->data.return_statement.expression) : create_type(TYPE_VOID);
                if (!are_types_compatible(context->return_type, return_expr_type))
                {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible return type. Expected '%s' but got '%s'.\n", node->lineno, type_to_string(context->return_type), type_to_string(return_expr_type));
                    semantic_errors++;
                }
                else if (context->return_type->kind != return_expr_type->kind)
                {
                    // Insert cast node for implicit conversion
                    node->data.return_statement.expression = create_cast_expr_node(
                        create_typename_node(type_to_string(context->return_type)),
                        node->data.return_statement.expression);
                }
            }
            break;
        }
        
        default:
             break;
    }
}


// Helper function to extract the name from a declarator
// const char *get_name_from_declarator(ASTNode *declarator)
// {
//     ASTNode *current = declarator;
//     while (current)
//     {
//         if (current->type == NODE_IDENTIFIER)
//         {
//             return current->data.stringValue;
//         } else if (current->type == NODE_QUALIFIED_ID) 
//         {
//             // For a qualified ID, the "name" is the final identifier in the chain.
//             return get_name_from_declarator(current->data.qualified_id.identifier);
//         }
//         else if (current->type == NODE_POINTER_DECLARATOR)
//         {
//             current = current->data.pointer_declarator.base_declarator;
//         } else if (current->type == NODE_REFERENCE_DECLARATOR) {
//             current = current->data.reference_declarator.base_declarator;
//         }
//         else if (current->type == NODE_ARRAY_DECLARATOR)
//         {
//             current = current->data.array_declarator.base_declarator;
//         }
//         else if (current->type == NODE_FUNCTION_DECLARATOR)
//         {
//             current = current->data.function_declarator.base_declarator;
//         }
//         else
//         {
//             return NULL; // Should not happen for valid declarators
//         }
//     }
//     return NULL;
// }

// Helper to infer type from constant string
Type* infer_constant_type(const char* val) {
    if (!val || val[0] == '\0') return create_type(TYPE_INT); // Default or error

    int len = strlen(val);

    // Check for character literal ('c')
    if (len >= 3 && val[0] == '\'' && val[len - 1] == '\'') {
        return create_type(TYPE_CHAR);
    }

    // Check for floating point: contains '.', 'e', 'E', or ends with 'f'/'F'
    bool has_decimal = strchr(val, '.') != NULL;
    bool has_exponent = strchr(val, 'e') != NULL || strchr(val, 'E') != NULL;
    bool has_f_suffix = (len > 1 && (val[len - 1] == 'f' || val[len - 1] == 'F'));

    if (has_decimal || has_exponent || has_f_suffix) {
        if (has_f_suffix) {
            return create_type(TYPE_FLOAT);
        } else {
            // Check for 'l'/'L' suffix for long double (treat as double for now)
             if (len > 1 && (val[len - 1] == 'l' || val[len - 1] == 'L')) {
                 return create_type(TYPE_DOUBLE); // Representing long double as double
             }
            return create_type(TYPE_DOUBLE); // Default float is double
        }
    }

    // Integer types (handle suffixes: U, L, LL, UL, ULL, LU, LLU)
    bool is_unsigned = false;
    bool is_long = false;
    // bool is_long_long = false; // Add if TYPE_LONG_LONG exists

    int suffix_start = len;
    while (suffix_start > 0 && (val[suffix_start - 1] == 'u' || val[suffix_start - 1] == 'U' || val[suffix_start - 1] == 'l' || val[suffix_start - 1] == 'L')) {
        suffix_start--;
    }

    for (int i = suffix_start; i < len; ++i) {
        if (val[i] == 'u' || val[i] == 'U') is_unsigned = true;
        if (val[i] == 'l' || val[i] == 'L') {
            // Check for LL
            // if (is_long) is_long_long = true;
            is_long = true;
        }
    }

    Type* int_type;
    // if (is_long_long) int_type = create_type(TYPE_LONG_LONG);
    if (is_long) int_type = create_type(TYPE_LONG);
    else int_type = create_type(TYPE_INT);

    int_type->is_unsigned = is_unsigned;
    return int_type;
}

// Recursively check array initializer list against array type
// Returns 1 if compatible, 0 otherwise.
int check_array_initializer(Type *array_type, ASTNode *initializer) {
    if (!array_type || array_type->kind != TYPE_ARRAY) {
        fprintf(stderr, "Internal Compiler Error: check_array_initializer called with non-array type.\n");
        return 0; // Should not happen if called correctly
    }
    if (!initializer) {
        return 1; // Empty initializer is okay (results in zero-initialization)
    }

    Type* element_type = array_type->data.base_info.base;

    if (initializer->type == NODE_INITIALIZER_LIST) {
        ASTNodeList *elems = initializer->data.items_list;
        int element_count = 0;
        for (ASTNodeList *e = elems; e; e = e->next) {
            element_count++;
            // Check current element e->node against element_type

            if (element_type->kind == TYPE_ARRAY) {
                // Expecting a nested list or a compatible element for the inner array
                if (e->node->type == NODE_INITIALIZER_LIST) {
                    if (!check_array_initializer(element_type, e->node)) {
                        return 0; // Recursive check failed
                    }
                } else {
                     // Allow initializing array with flat list? C allows this sometimes.
                     // For simplicity, let's require nested lists for nested arrays for now.
                     // A more complex check could flatten the list.
                     // OR, maybe this element is being assigned to the first element of the sub-array?
                     // Let's analyze the type of the single element provided.
                     Type *single_elem_type = analyze_expression(e->node);
                     if (!are_types_compatible(element_type->data.base_info.base, single_elem_type)) {
                           fprintf(stderr, "Semantic Error (Line %d): Initializer element type '%s' incompatible with array element type '%s'.\n",
                                   e->node->lineno, type_to_string(single_elem_type), type_to_string(element_type->data.base_info.base));
                           // semantic_errors++; // Let the caller increment
                           return 0;
                     }
                     // This is tricky - how many elements does this single value initialize? Only the first?
                     // Standard C might require braces for sub-aggregates. Let's enforce that.
                      fprintf(stderr, "Semantic Error (Line %d): Missing braces for sub-array initializer.\n", e->node->lineno);
                      // semantic_errors++;
                      return 0;
                }
            } else {
                // Expecting a single element compatible with element_type
                if (e->node->type == NODE_INITIALIZER_LIST) {
                     fprintf(stderr, "Semantic Error (Line %d): Braces around scalar initializer.\n", e->node->lineno);
                    // semantic_errors++;
                    return 0;
                }
                Type *elem_init_type = analyze_expression(e->node);
                if (!are_types_compatible(element_type, elem_init_type)) {
                    fprintf(stderr, "Semantic Error (Line %d): Initializer element type '%s' incompatible with array element type '%s'.\n",
                           e->node->lineno, type_to_string(elem_init_type), type_to_string(element_type));
                   // semantic_errors++;
                   return 0;
                }
                 // If compatible but different, insert cast
                 else if (element_type->kind != elem_init_type->kind && elem_init_type->kind != TYPE_UNKNOWN) {
                      e->node = create_cast_expr_node(create_typename_node(type_to_string(element_type)), e->node);
                 }
            }
        }
        // Optional: Check if element_count exceeds array_type->data.base_info.dimensions[0] if known
        // if (array_type->data.base_info.num_dimensions > 0 && array_type->data.base_info.dimensions[0] > 0 && element_count > array_type->data.base_info.dimensions[0]) {
        //      fprintf(stderr, "Semantic Error (Line %d): Excess elements in array initializer.\n", initializer->lineno);
        //      // semantic_errors++; // Let the caller increment
        //      return 0;
        // }
        return 1; // All elements checked okay
    }
     else if (array_type->data.base_info.base->kind == TYPE_CHAR && initializer->type == NODE_STRING_LITERAL) {
         // Special case: char arr[] = "string";
         // Optional: Check if string length exceeds array size if known.
         return 1;
     }
    else {
        // Trying to initialize an array with a single non-list value (invalid)
        fprintf(stderr, "Semantic Error (Line %d): Array initializer must be an initializer list or string literal.\n", initializer->lineno);
        // semantic_errors++;
        return 0;
    }
}

void analyze_struct_or_union_specifier(ASTNode *node) {
    if (!node || node->type != NODE_STRUCT_OR_UNION_SPECIFIER) return;

    const char* tag_name = node->data.struct_or_union_specifier.name;
    ASTNodeList* members = node->data.struct_or_union_specifier.members;
    Type* type = NULL;
    
    // Find symbol in *any* scope. Type names live in a flat namespace for this compiler.
    Symbol* existing_symbol = tag_name ? find_symbol(tag_name) : NULL; 

    // Case 1: Full definition (e.g., struct T { ... })
    if (members) {
        if (existing_symbol && existing_symbol->type->data.struct_union_info.members) {
            fprintf(stderr, "Semantic Error (Line %d): Redefinition of '%s %s'.\n", node->lineno, token_to_string(node->data.struct_or_union_specifier.kind), tag_name);
            semantic_errors++;
            return;
        }
        
        type = create_type_struct_union(node->data.struct_or_union_specifier.kind == STRUCT ? TYPE_STRUCT : TYPE_UNION, tag_name);
        if(tag_name) {
             // If it existed as a forward-declaration, update it. Otherwise, add new.
            if(existing_symbol) {
                existing_symbol->type = type; // Update the type
            } else {
                add_symbol(tag_name, type, SYM_TYPEDEF, node->lineno);
                add_typename(tag_name); // Also add to parser's typename list
            }
        }

        enter_scope();
        for (ASTNodeList* decl_item = members; decl_item; decl_item = decl_item->next) {
            ASTNode* member_decl = decl_item->node;
            analyze_declaration(decl_item->node);
            if (tag_name && member_decl->data.declaration.declarators) {
                for (ASTNodeList* d = member_decl->data.declaration.declarators; d; d = d->next) {
                    ASTNode* init_decl = d->node;
                    ASTNodeList* specs = member_decl->data.declaration.specifiers;
                    ASTNode* spec0 = specs ? specs->node : NULL;
                    if (spec0 && spec0->type == NODE_STRUCT_OR_UNION_SPECIFIER &&
                        spec0->data.struct_or_union_specifier.name &&
                        strcmp(spec0->data.struct_or_union_specifier.name, tag_name) == 0 &&
                        !spec0->data.struct_or_union_specifier.members &&
                        !declarator_is_pointer(init_decl->data.init_declarator.declarator))
                    {
                        fprintf(stderr, "Semantic Error (Line %d): struct member '%s' must be a pointer to '%s'.\n",
                                init_decl->lineno,
                                get_name_from_declarator(init_decl->data.init_declarator.declarator),
                                tag_name);
                        semantic_errors++;
                    }
                }
            }
        }

        Scope* member_scope = get_current_scope();
        Member* head = NULL, *tail = NULL;
        for (int i = 0; i < member_scope->symbol_count; i++) {
            Symbol* member_symbol = member_scope->symbols[i];
            Member* new_member = (Member*)calloc(1, sizeof(Member));
            new_member->name = strdup(member_symbol->name);
            new_member->type = copy_type(member_symbol->type);
            if (!head) { head = tail = new_member; } else { tail->next = new_member; tail = new_member; }
        }
        leave_scope();
        type->data.struct_union_info.members = head;
    }
    // Case 2: Forward declaration (e.g., struct T;) or Use (e.g. struct T my_var;)
    else if (tag_name && !existing_symbol) {
        // This is the first time we see this tag, and it has no body.
        // Treat it as a forward declaration.
        type = create_type_struct_union(node->data.struct_or_union_specifier.kind == STRUCT ? TYPE_STRUCT : TYPE_UNION, tag_name);
        add_symbol(tag_name, type, SYM_TYPEDEF, node->lineno);
        add_typename(tag_name); // Also add to parser's typename list
    }
    // Case 3: Use of existing type (e.g. struct T my_var;).
    // `existing_symbol` is found, `members` is NULL. We don't need to do anything here.
    // get_type_from_specifiers() will handle finding the type.
}

void analyze_class_specifier(ASTNode *node) {
    if (!node || node->type != NODE_CLASS_SPECIFIER) return;

    const char* class_name = node->data.class_specifier.name;
    if (!class_name) return;

    Type* class_type = NULL;
    Symbol* existing_symbol = find_symbol(class_name);

    if (existing_symbol && existing_symbol->kind == SYM_TYPEDEF && (existing_symbol->type->kind == TYPE_STRUCT || existing_symbol->type->kind == TYPE_UNION)) {
        if (existing_symbol->type->data.struct_union_info.members == NULL && node->data.class_specifier.members) {
            class_type = existing_symbol->type;
        } else if (node->data.class_specifier.members) { // Check if we are trying to redefine
            fprintf(stderr, "Semantic Error (Line %d): Redefinition of class '%s'.\n", node->lineno, class_name);
            semantic_errors++;
            return;
        } else {
            // This is just a use, like `class MyClass var;`, so we do nothing.
            return;
        }
    }

    if (!class_type) {
        class_type = create_type(TYPE_STRUCT);
        class_type->data.struct_union_info.name = strdup(class_name);
        add_symbol(class_name, class_type, SYM_TYPEDEF, node->lineno);
        add_typename(class_name);
    }
    
    // Only process members if this is a definition
    if (!node->data.class_specifier.members) {
        return;
    }

    Member* head = NULL, *tail = NULL;

     ASTNodeList* base_clauses = node->data.class_specifier.base_classes;
    for (ASTNodeList* base_item = base_clauses; base_item; base_item = base_item->next) {
        ASTNode* base_specifier_node = base_item->node;
        if (base_specifier_node->type != NODE_BASE_CLASS) continue; // Use NODE_BASE_CLASS

        // Access data via .base_class and .stringValue
        const char* base_class_name = base_specifier_node->data.base_class.class_name->data.stringValue;
        Symbol* base_symbol = find_symbol(base_class_name);

        if (!base_symbol || base_symbol->type->kind != TYPE_STRUCT) {
            fprintf(stderr, "Semantic Error (Line %d): Base class '%s' is not a defined class type.\n", base_specifier_node->lineno, base_class_name);
            semantic_errors++;
            continue;
        }

        Type* base_type = base_symbol->type;
        if (!base_type->data.struct_union_info.members) {
            fprintf(stderr, "Semantic Error (Line %d): Base class '%s' is an incomplete type.\n", base_specifier_node->lineno, base_class_name);
            semantic_errors++;
            continue;
        }

        // Copy members from base class
        for (Member* base_member = base_type->data.struct_union_info.members; base_member; base_member = base_member->next) {
            Member* new_member = (Member*)calloc(1, sizeof(Member));
            new_member->name = strdup(base_member->name);
            new_member->type = copy_type(base_member->type);
            
            if (!head) {
                head = tail = new_member;
            } else {
                tail->next = new_member;
                tail = new_member;
            }
        }
    }
    //

    enter_scope();
    ASTNodeList* members = node->data.class_specifier.members;
    for (ASTNodeList* member_item = members; member_item; member_item = member_item->next) {
        analyze_node(member_item->node, NULL);
    }

    Scope* member_scope = get_current_scope();
    for (int i = 0; i < member_scope->symbol_count; i++) {
        Symbol* member_symbol = member_scope->symbols[i];
        
        Member* new_member = (Member*)calloc(1, sizeof(Member));
        new_member->name = strdup(member_symbol->name);
        new_member->type = copy_type(member_symbol->type);
        
        if (member_symbol->type->storage_class == STATIC) {
            new_member->is_static = 1;
        } else {
            new_member->is_static = 0;
        }

        if (!head) {
            head = tail = new_member;
        } else {
            tail->next = new_member;
            tail = new_member;
        }
    }
    class_type->data.struct_union_info.member_scope = leave_scope();
    class_type->data.struct_union_info.members = head;
}

// Add all function parameters to the current scope
void add_function_parameters(ASTNode *declarator)
{
    if (!declarator) return;
    ASTNode *current = declarator;
    // Find the function declarator node
    while (current) {
        if (current->type == NODE_FUNCTION_DECLARATOR) {
            ASTNodeList *params = current->data.function_declarator.parameters;
            if (!params) return; // no parameters to add
            for (ASTNodeList *p = params; p; p = p->next) {
                ASTNode *param_decl_node = p->node; // parameter_declaration node
                if (!param_decl_node || param_decl_node->type != NODE_PARAMETER_DECLARATION) continue; // Safety

                // Determine the declared base type from specifiers
                Type *param_base_type = get_type_from_specifiers(param_decl_node->data.parameter_declaration.specifiers);
                ASTNode* param_declarator = param_decl_node->data.parameter_declaration.declarator;

                // If there's a declarator, build the full type from it; otherwise the base type is the param type
                Type *final_type = NULL;
                if (param_declarator) {
                    final_type = build_type_from_declarator(param_base_type, param_declarator);
                } else {
                    final_type = param_base_type;
                }

                // Get the parameter name (if any). Some parameters may be unnamed (e.g., in prototypes).
                const char *param_name = get_name_from_declarator(param_declarator);
                if (!param_name) {
                    // No name -> nothing to add to symbol table for this parameter
                    // Freeing final_type is intentionally left to caller/overall memory management
                    continue;
                }

                // Add symbol to the current scope as a variable (parameter)
                Symbol *sym = add_symbol(param_name, final_type, SYM_VARIABLE, param_decl_node->lineno);
                if (!sym) {
                    // add_symbol reports semantic error (redefinition); still try to annotate AST if possible
                    // But don't crash: continue to next param
                    continue;
                }

                // Annotate the AST identifier node within the declarator so later passes can use node->symbol
                ASTNode *ident_node = find_identifier_in_declarator(param_declarator);
                if (ident_node && ident_node->type == NODE_IDENTIFIER) {
                    ident_node->symbol = sym;
                }
            }
            break; // processed parameters
        }
        // Traverse down if nested (e.g., pointer to function)
        else if (current->type == NODE_POINTER_DECLARATOR) {
            current = current->data.pointer_declarator.base_declarator;
        } else {
            break; // Stop if not a function or pointer declarator
        }
    }
}

void analyze_declaration(ASTNode *node)
{
    if (!node || node->type != NODE_DECLARATION) return;
    
    // Make returns conditional on having declarators
    bool has_declarators = node->data.declaration.declarators != NULL;
    
    ASTNode* first_spec = node->data.declaration.specifiers->node;
    if (first_spec->type == NODE_ENUM_SPECIFIER ){
        analyze_enum_specifier(first_spec);
        if (!has_declarators) return; // Only return if no variables are declared
    }
    if (first_spec->type == NODE_STRUCT_OR_UNION_SPECIFIER) {
        analyze_struct_or_union_specifier(first_spec);
        if (!has_declarators) return; // Only return if no variables are declared
    }
    if (first_spec->type == NODE_CLASS_SPECIFIER && !has_declarators) {
        analyze_class_specifier(first_spec);
        return; // This logic was already correct
    }
    
    Type *base_type = get_type_from_specifiers(node->data.declaration.specifiers);
    int is_auto = (base_type->storage_class == AUTO);
    if (is_auto) {
         // C-style 'auto' is a storage class, but C++ 'auto' is a type.
         // We'll treat it as C++ 'auto' if it's the main type.
         // If the base type is INT (default) and storage is AUTO, it's auto.
         if (base_type->kind != TYPE_INT) is_auto = 0; // Not 'auto' if user specified 'auto int'
    }
    // Check if this is a typedef
    int is_typedef = (base_type->storage_class == TYPEDEF);
    // for (ASTNodeList *spec = node->data.declaration.specifiers; spec; spec = spec->next)
    // {
    //     if (spec->node->type == NODE_SPECIFIER && spec->node->data.specifier == TYPEDEF)
    //     {
    //         is_typedef = 1;
    //         break;
    //     }
    // }
    
    // This handles `struct T;` which has no declarators
    if (!node->data.declaration.declarators) {
        free(base_type); // The type is processed, we can free the temporary one.
        return;
    }
    
    // can maybe replace with else.
    if (node->data.declaration.declarators)
    {
        for (ASTNodeList *d = node->data.declaration.declarators; d; d = d->next)
        {
            ASTNode *init_decl = d->node;
            ASTNode *declarator = init_decl->data.init_declarator.declarator;
            ASTNode *initializer = init_decl->data.init_declarator.initializer;
            const char *name = get_name_from_declarator(declarator);

            // Handle out-of-class static member definitions (e.g., int Animal::count = 0;)
            if (declarator->type == NODE_QUALIFIED_ID) {
                ASTNode* qualifier_node = declarator->data.qualified_id.qualifiers->node;
                const char* class_name = qualifier_node->data.stringValue;
                const char* member_name = declarator->data.qualified_id.identifier->data.stringValue;

                // Create a unique "qualified" name like "Animal::count"
                char qualified_name[256];
                snprintf(qualified_name, sizeof(qualified_name), "%s::%s", class_name, member_name);

                // Build the type and add the symbol with the qualified name
                Type* final_type = build_type_from_declarator(copy_type(base_type), declarator);
                Symbol* new_sym = add_symbol(qualified_name, final_type, SYM_VARIABLE, init_decl->lineno);
                
                if (new_sym) {
                    declarator->symbol = new_sym; // Annotate the qualified_id node for TAC
                }

                if (initializer) {
                    // Analyze the initializer expression and check compatibility
                    Type* initializer_type = analyze_expression(initializer);
                    if (!are_types_compatible(final_type, initializer_type)) {
                         fprintf(stderr, "Semantic Error (Line %d): Incompatible types in initialization of '%s'. Cannot assign '%s' to '%s'.\n",
                            init_decl->lineno, qualified_name, type_to_string(initializer_type), type_to_string(final_type));
                         semantic_errors++;
                    }
                }
                continue; // Skip to the next declarator in the list
            }

            if (!name)
            {
                if (declarator->type == NODE_QUALIFIED_ID) {
                    // This is an out-of-class definition, e.g., int Animal::count = 0;
                    // We're just analyzing the initializer, not adding a new symbol.
                    // A full implementation would find the class scope and update the member.
                    // For now, let's just analyze the initializer.
                    if (initializer) {
                        analyze_expression(initializer);
                    }
                    continue; // Skip to next declarator
                 }
                // This could happen for abstract declarators in typedefs, but we need a name
                // for non-typedefs. Let's assume for now a name is required.
                // In a fuller implementation, this might be legal for `typedef int_arr[10];`
                 if (!is_typedef) {
                    fprintf(stderr, "Semantic Error (Line %d): Declarator is missing a name.\n", init_decl->lineno);
                    semantic_errors++;
                 }
                 // We can't free base_type here, as other declarators might use it.
                 continue; // Skip this declarator
            }

            Type *final_type;
            if (is_auto) {
                if (!initializer) {
                    fprintf(stderr, "Semantic Error (Line %d): Declaration of 'auto' variable '%s' requires an initializer.\n", init_decl->lineno, name);
                    semantic_errors++;
                    continue;
                }
                // The type of the variable is the type of the initializer
                Type *initializer_type = analyze_expression(initializer);
                if (initializer_type->kind == TYPE_UNKNOWN) {
                    final_type = create_type(TYPE_UNKNOWN); // Propagate error
                } else {
                    final_type = build_type_from_declarator(copy_type(initializer_type), declarator);
                }
            } else {
                final_type = build_type_from_declarator(copy_type(base_type), declarator);
            }
            if (is_typedef)
            {
                add_symbol(name, final_type, SYM_TYPEDEF, init_decl->lineno);
                add_typename(name);
            }
            else
            {
                SymbolKind kind = (final_type->kind == TYPE_FUNCTION) ? SYM_FUNCTION : SYM_VARIABLE;
                Symbol* new_sym = add_symbol(name, final_type, kind, init_decl->lineno);

                // ANNOTATE THE DECLARATOR'S IDENTIFIER NODE
                // After adding the symbol, we need to find the actual identifier node
                // in the declarator and attach the symbol to it for the TAC generator.
                if (new_sym) {
                    ASTNode* id_node = declarator;
                    // Traverse down the declarator to find the base identifier node
                    while (id_node && id_node->type != NODE_IDENTIFIER) {
                        if (id_node->type == NODE_POINTER_DECLARATOR) {
                            id_node = id_node->data.pointer_declarator.base_declarator;
                        } else if (id_node->type == NODE_ARRAY_DECLARATOR) {
                            id_node = id_node->data.array_declarator.base_declarator;
                        } else if (id_node->type == NODE_FUNCTION_DECLARATOR) {
                            id_node = id_node->data.function_declarator.base_declarator;
                        } else {
                            id_node = NULL; // Stop if we can't go further
                        }
                    }
                    if (id_node && id_node->type == NODE_IDENTIFIER) {
                        id_node->symbol = new_sym; // This is the missing annotation
                    }
                }
            }
            if (final_type->kind == TYPE_ARRAY && init_decl->data.init_declarator.initializer &&
                init_decl->data.init_declarator.initializer->type == NODE_INITIALIZER_LIST)
            {
                if (!check_array_initializer(final_type, init_decl->data.init_declarator.initializer))
                {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible types in array initialization of '%s'.\n", init_decl->lineno, name);
                    semantic_errors++;
                    continue;
                }
                // If you want, you can also check the number of elements matches the array size
                continue; // Skip the normal type check for arrays
            }
            if (initializer && !is_auto)
            {
                Type *initializer_type = analyze_expression(initializer);
                if (!are_types_compatible(final_type, initializer_type))
                {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible types in initialization of '%s'. Cannot assign '%s' to '%s'.\n",
                            init_decl->lineno, name, type_to_string(initializer_type), type_to_string(final_type));
                    semantic_errors++;
                }
                else if (final_type->kind != initializer_type->kind)
                {
                    // Insert cast node for implicit conversion
                    ASTNode *cast_node = create_cast_expr_node(create_typename_node(type_to_string(final_type)), init_decl->data.init_declarator.initializer);
                    init_decl->data.init_declarator.initializer = cast_node;
                }
            }
        }
    }
    free(base_type);
}

// In semantic.c, replace the entire function
void analyze_function_definition(ASTNode *node)
{
    Type *return_type = get_type_from_specifiers(node->data.function_definition.specifiers);
    const char *name = get_name_from_declarator(node->data.function_definition.declarator);
    if (!name) { 
        if (node->data.function_definition.declarator->type == NODE_FUNCTION_DECLARATOR &&
             node->data.function_definition.declarator->data.function_declarator.base_declarator->type == NODE_QUALIFIED_ID)
        {
             // This is an out-of-class method definition.
             // A full implementation would find the class, find the method, and analyze the body
             // in the class's scope.
             // For now, we'll just analyze the body in a new scope.
             name = get_name_from_declarator(node->data.function_definition.declarator); // This will get "speak" or "bark"
        } else {
            fprintf(stderr, "Semantic Error (Line %d): Function definition is missing a name.\n", node->lineno);
            semantic_errors++;
            return; 
        }
    }

    Type *func_type = build_type_from_declarator(return_type, node->data.function_definition.declarator);
    
    // Check for existing declaration
    Symbol* existing_sym = find_symbol(name);
    if (existing_sym) {
        if (existing_sym->kind != SYM_FUNCTION) {
            fprintf(stderr, "Semantic Error (Line %d): '%s' redefined as a function, but was previously declared as something else.\n", node->lineno, name);
            semantic_errors++;
        }
        // Here you would also compare signatures (return type, params)
        // For now, we just use the new definition.
        existing_sym->type = func_type;
    } else {
        add_symbol(name, func_type, SYM_FUNCTION, node->lineno);
    }

    // --- NEW TWO-PASS ANALYSIS ---
    FunctionAnalysisContext context = {0};
    context.return_type = func_type->data.function_sig.return_type;

    // 1. First Pass: Collect all labels (goto, case, default)
    collect_labels(node->data.function_definition.body, &context);
    
    // 2. Second Pass: Full statement analysis
    enter_scope();
    add_function_parameters(node->data.function_definition.declarator);
    analyze_statement_with_context(node->data.function_definition.body, &context);
    leave_scope();

    // Free the collected labels
    Label* current = context.labels;
    while(current) {
        Label* next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

Type *analyze_expression(ASTNode *node)
{
    if (!node)
        return create_type(TYPE_VOID);

    switch (node->type)
    {
        case NODE_CONSTANT:
            return infer_constant_type(node->data.stringValue);
        case NODE_STRING_LITERAL:
            return create_type(TYPE_STRING);

        case NODE_IDENTIFIER:
        {
            Symbol *sym = find_symbol(node->data.stringValue);
            if (!sym)
            {
                fprintf(stderr, "Semantic Error (Line %d): Use of undeclared identifier '%s'.\n", node->lineno, node->data.stringValue);
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }
            if (sym->kind == SYM_TYPEDEF)
            {
                fprintf(stderr, "Semantic Error (Line %d): Cannot use a typename '%s' as an expression.\n", node->lineno, node->data.stringValue);
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }
            node->symbol = sym;
            return sym->type;
        }

        case NODE_QUALIFIED_ID: { // <--- NEW IMPLEMENTATION
            // This is a simplified implementation for one level of qualification (e.g., Class::member)
            if (!node->data.qualified_id.qualifiers || !node->data.qualified_id.qualifiers->node) {
                return analyze_expression(node->data.qualified_id.identifier); // Fallback to simple identifier
            }
            
            // 1. Get Qualifier Type (e.g., "Animal")
            ASTNode* qualifier_node = node->data.qualified_id.qualifiers->node;
            const char* qualifier_name = NULL;
            
            // The qualifier in the AST is either a TYPENAME or IDENTIFIER
            if (qualifier_node->type == NODE_TYPENAME || qualifier_node->type == NODE_IDENTIFIER) {
                qualifier_name = qualifier_node->data.stringValue;
            } else {
                fprintf(stderr, "Semantic Error (Line %d): Invalid scope qualifier.\n", node->lineno);
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }

            Symbol* qualifier_sym = find_symbol(qualifier_name);
            if (!qualifier_sym || qualifier_sym->kind != SYM_TYPEDEF) {
                fprintf(stderr, "Semantic Error (Line %d): '%s' is not a class, struct, or namespace name.\n", node->lineno, qualifier_name);
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }
            Type* qualifier_type = qualifier_sym->type;
            if (qualifier_type->kind != TYPE_STRUCT && qualifier_type->kind != TYPE_UNION) {
                 fprintf(stderr, "Semantic Error (Line %d): '%s' is not a class or struct.\n", node->lineno, qualifier_name);
                 semantic_errors++;
                 return create_type(TYPE_UNKNOWN);
            }

            // 2. Get Member Name (e.g., "count")
            const char* member_name = node->data.qualified_id.identifier->data.stringValue;

            // 3. Find member in the class/struct's member list
            for (Member *m = qualifier_type->data.struct_union_info.members; m; m = m->next) {
                if (strcmp(m->name, member_name) == 0) {
                    if (m->is_static) {
                        
                        // It's a static member. Find the global symbol using its unique qualified name.
                        char qualified_name[256];
                        snprintf(qualified_name, sizeof(qualified_name), "%s::%s", qualifier_name, member_name);
                        
                        Symbol* static_sym = find_symbol(qualified_name);
                        if (static_sym) {
                            node->symbol = static_sym; // Attach the symbol to the AST node
                            return static_sym->type;
                        }
                    } else {
                        fprintf(stderr, "Semantic Error (Line %d): Cannot access non-static member '%s' via '::'.\n", node->lineno, member_name);
                        semantic_errors++;
                        return create_type(TYPE_UNKNOWN);
                    }
                }
            }
            
            fprintf(stderr, "Semantic Error (Line %d): No member named '%s' in '%s'.\n", node->lineno, member_name, qualifier_name);
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }

        case NODE_BINARY_EXPR:
        {
            Type *left_type = analyze_expression(node->data.binary_expr.left);
            Type *right_type = analyze_expression(node->data.binary_expr.right);

            // String concatenation
            if (node->data.binary_expr.op == '+')
            {
                if ((left_type->kind == TYPE_STRING || (left_type->kind == TYPE_POINTER && left_type->data.base_info.base->kind == TYPE_CHAR)) ||
                    (right_type->kind == TYPE_STRING || (right_type->kind == TYPE_POINTER && right_type->data.base_info.base->kind == TYPE_CHAR)))
                {
                    return create_type(TYPE_STRING);
                }
            }

            // Insert cast nodes for arithmetic type promotion
            if (is_arithmetic_type(left_type) && is_arithmetic_type(right_type))
            {
                Type *result_type = get_common_arithmetic_type(left_type, right_type);
                if (left_type->kind != result_type->kind)
                {
                    node->data.binary_expr.left = create_cast_expr_node(
                        create_typename_node(type_to_string(result_type)),
                        node->data.binary_expr.left);
                }
                if (right_type->kind != result_type->kind)
                {
                    node->data.binary_expr.right = create_cast_expr_node(
                        create_typename_node(type_to_string(result_type)),
                        node->data.binary_expr.right);
                }
                return result_type;
            }

            // Basic pointer arithmetic: ptr + int
            if (left_type->kind == TYPE_POINTER && is_integer_type(right_type))
                return left_type;
            if (is_integer_type(left_type) && right_type->kind == TYPE_POINTER)
                return right_type;

            fprintf(stderr, "Semantic Error (Line %d): Invalid operands for binary operator '%s'. Types are '%s' and '%s'.\n",
                    node->lineno, token_to_string(node->data.binary_expr.op), type_to_string(left_type), type_to_string(right_type));
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }

        case NODE_ASSIGNMENT:
        {
            Type *lvalue_type = analyze_expression(node->data.assignment.lvalue);
            Type *rvalue_type = analyze_expression(node->data.assignment.rvalue);
            if (!are_types_compatible(lvalue_type, rvalue_type))
            {
                fprintf(stderr, "Semantic Error (Line %d): Incompatible types in assignment. Cannot assign '%s' to '%s'.\n",
                        node->lineno, type_to_string(rvalue_type), type_to_string(lvalue_type));
                semantic_errors++;
            }
            else if (lvalue_type->kind != rvalue_type->kind && (lvalue_type->kind != TYPE_UNKNOWN && rvalue_type->kind != TYPE_UNKNOWN)) // Don't cast if one is unknown
            {
                // Insert cast node for implicit conversion
                node->data.assignment.rvalue = create_cast_expr_node(
                    create_typename_node(type_to_string(lvalue_type)),
                    node->data.assignment.rvalue);
            }
            return lvalue_type;
        }
        case NODE_PREFIX_UNARY_EXPR:
        case NODE_POSTFIX_UNARY_EXPR:
        case NODE_UNARY_EXPR: {
            Type* operand_type = analyze_expression(node->data.unary_expr.operand);
            switch (node->data.unary_expr.op) {
                case '&': return create_pointer_type(operand_type);
                case '*':
                    if (operand_type->kind != TYPE_POINTER) {
                        fprintf(stderr, "Semantic Error (Line %d): Cannot dereference a non-pointer type ('%s').\n", node->lineno, type_to_string(operand_type));
                        semantic_errors++;
                        return create_type(TYPE_UNKNOWN);
                    }
                    return operand_type->data.base_info.base;
                case '!': return create_type(TYPE_INT); // Logical NOT always returns an int (0 or 1)
                default: return operand_type; // For +/-/~/++/--
            }
        }
        
        case NODE_FUNC_CALL: {
            // This is a simplified version. A full version would match argument types.
            Type* func_expr_type = analyze_expression(node->data.func_call.function);
            if (func_expr_type->kind == TYPE_POINTER && func_expr_type->data.base_info.base->kind == TYPE_FUNCTION) {
                // Calling a function pointer
                return func_expr_type->data.base_info.base->data.function_sig.return_type;
            }
            if (func_expr_type->kind != TYPE_FUNCTION) {
                 fprintf(stderr, "Semantic Error (Line %d): Called object is not a function or function pointer.\n", node->lineno);
                 semantic_errors++;
                 return create_type(TYPE_UNKNOWN);
            }

            // Argument count and type checking
            ASTNodeList* expected_params = func_expr_type->data.function_sig.params;
            ASTNodeList* actual_args = node->data.func_call.arguments;
            int expected_count = 0;
            int actual_count = 0;

            // Count expected fixed parameters
            for (ASTNodeList* p = expected_params; p; p = p->next) {
                // The NULL node marker for variadics should not be counted.
                if (p->node) expected_count++;
            }

            // Count actual arguments
            for (ASTNodeList* a = actual_args; a; a = a->next) {
                actual_count++;
            }

            // Check argument count
            if (func_expr_type->data.function_sig.is_variadic) {
                if (actual_count < expected_count) {
                    fprintf(stderr, "Semantic Error (Line %d): Too few arguments to variadic function. Expected at least %d, got %d.\n", node->lineno, expected_count, actual_count);
                    semantic_errors++;
                }
            } else { // Not variadic
                if (actual_count != expected_count) {
                    fprintf(stderr, "Semantic Error (Line %d): Wrong number of arguments to function. Expected %d, got %d.\n", node->lineno, expected_count, actual_count);
                    semantic_errors++;
                }
            }
            
            // Analyze the types of all provided arguments
            for (ASTNodeList* a = actual_args; a; a = a->next) {
                analyze_expression(a->node);
            }
            return func_expr_type->data.function_sig.return_type;
        }

        case NODE_CONDITIONAL_EXPR:
        {
            Type *cond_type = analyze_expression(node->data.conditional_expr.condition);
            Type *true_type = analyze_expression(node->data.conditional_expr.true_expr);
            Type *false_type = analyze_expression(node->data.conditional_expr.false_expr);
            if (!is_scalar_type(cond_type))
            {
                fprintf(stderr, "Semantic Error (Line %d): Condition in ternary operator must be scalar, got '%s'.\n", node->lineno, type_to_string(cond_type));
                semantic_errors++;
            }
            if (are_types_compatible(true_type, false_type))
            {
                Type *result_type = get_common_arithmetic_type(true_type, false_type);
                // Insert cast nodes if needed
                if (true_type->kind != result_type->kind)
                {
                    node->data.conditional_expr.true_expr = create_cast_expr_node(
                        create_typename_node(type_to_string(result_type)),
                        node->data.conditional_expr.true_expr);
                }
                if (false_type->kind != result_type->kind)
                {
                    node->data.conditional_expr.false_expr = create_cast_expr_node(
                        create_typename_node(type_to_string(result_type)),
                        node->data.conditional_expr.false_expr);
                }
                return result_type;
            }
            else
            {
                fprintf(stderr, "Semantic Error (Line %d): Types in ternary operator branches are incompatible: '%s' and '%s'.\n", node->lineno, type_to_string(true_type), type_to_string(false_type));
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }
        }

        case NODE_CAST_EXPRESSION:
        {
            // Already a cast node, just return the type
            ASTNode *type_name_node = node->data.cast_expr.type_name;
            if (type_name_node && type_name_node->type == NODE_DECLARATION)
            {
                return get_type_from_specifiers(type_name_node->data.declaration.specifiers);
            }
            // Fallback: treat as unknown
            return create_type(TYPE_UNKNOWN);
        }

        case NODE_ARRAY_SUBSCRIPT:
        {
            Type *array_type = analyze_expression(node->data.array_subscript.array);
            Type *index_type = analyze_expression(node->data.array_subscript.index);

            if (array_type->kind != TYPE_ARRAY && array_type->kind != TYPE_POINTER) {
                fprintf(stderr, "Semantic Error (Line %d): Subscripted value is not an array or pointer.\n", node->lineno);
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }

            if (!is_integer_type(index_type)) {
                fprintf(stderr, "Semantic Error (Line %d): Array subscript is not an integer.\n", node->lineno);
                semantic_errors++;
            }

            return array_type->data.base_info.base; // The type of the result is the base type of the array
        }
        case NODE_MEMBER_ACCESS:
        {
            Type *object_type = analyze_expression(node->data.member_access.object);
            const char *member_name = node->data.member_access.member_name;

            if (node->data.member_access.is_pointer) {
                if (object_type->kind != TYPE_POINTER) {
                    fprintf(stderr, "Semantic Error (Line %d): Request for member '%s' in something not a pointer to a structure or union.\n", node->lineno, member_name);
                    semantic_errors++;
                    return create_type(TYPE_UNKNOWN);
                }
                object_type = object_type->data.base_info.base; // Dereference the pointer to get the struct/union type
            }
            
            if (object_type->kind == TYPE_UNKNOWN) {
                // Error already reported for undeclared identifier, just stop.
                return create_type(TYPE_UNKNOWN);
            }

            if (object_type->kind != TYPE_STRUCT && object_type->kind != TYPE_UNION) {
                fprintf(stderr, "Semantic Error (Line %d): Request for member '%s' in something not a structure or union (got %s).\n", 
                    node->lineno, member_name, type_to_string(object_type));
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }

            // Find the member in the type's member list
            for (Member *m = object_type->data.struct_union_info.members; m; m = m->next) {
                if (strcmp(m->name, member_name) == 0) {
                    // If the member is a function, find its global symbol and annotate the node.
                    if (m->type->kind == TYPE_FUNCTION) {
                        Symbol* func_sym = find_symbol(member_name); // Find the global function symbol
                        if (func_sym) {
                            node->symbol = func_sym; // Attach the symbol to the AST node
                        }
                    }
                    return m->type; // Found it, return its type
                }
            }

            fprintf(stderr, "Semantic Error (Line %d): No member named '%s' in struct or union.\n", node->lineno, member_name);
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }
        case NODE_LAMBDA_EXPR: {
            Type* func_type = create_type(TYPE_FUNCTION);
            func_type->data.function_sig.return_type = create_type(TYPE_INT);
            func_type->data.function_sig.params = node->data.lambda_expr.params;
            return create_pointer_type(func_type);
        }
        default:
            return create_type(TYPE_UNKNOWN);
    }
}

void analyze_node(ASTNode *node, FunctionAnalysisContext* context) // Pass context down
{
    if (!node)
        return;
    switch (node->type)
    {
    case NODE_TRANSLATION_UNIT:
        for (ASTNodeList *item = node->data.items_list; item; item = item->next)
        {
            analyze_node(item->node, NULL); // Pass NULL context for global scope
        }
        break;
    case NODE_DECLARATION:
        // Remove the entire "pre-pass" block
        /*
        // Pre-pass for definitions that create types (struct/union/class)
        if (node->data.declaration.specifiers) {
            ASTNode* spec_node = node->data.declaration.specifiers->node;
            if (spec_node->type == NODE_STRUCT_OR_UNION_SPECIFIER && spec_node->data.struct_or_union_specifier.members) {
                analyze_struct_or_union_specifier(spec_node);
            } else if (spec_node->type == NODE_CLASS_SPECIFIER && spec_node->data.class_specifier.members) {
                analyze_class_specifier(spec_node);
            }
        }
        */        
        analyze_declaration(node);
        break;
    case NODE_FUNCTION_DEFINITION:
        analyze_function_definition(node);
        break;
    // Remove the NODE_CLASS_SPECIFIER case here, it's handled by NODE_DECLARATION
    default:
        if (context) {
            analyze_statement_with_context(node, context);
        }
        break;
    }
}

int analyze_ast(ASTNode *root)
{
    if (!root)
        return 1; // Nothing to analyze
    semantic_errors = 0;
    init_symbol_table();
    analyze_node(root, NULL);
    return semantic_errors == 0;
}
