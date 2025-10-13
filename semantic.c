#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "symbol_table.h"
#include "parser.tab.h"
#include "st.h"

static int semantic_errors = 0;
static Type* current_function_return_type = NULL;

// Forward Declarations for Traversal
void analyze_node(ASTNode* node);
Type* analyze_expression(ASTNode* node);

// Helper function to extract the name from a declarator
const char* get_name_from_declarator(ASTNode* declarator) {
    ASTNode* current = declarator;
    while (current) {
        if (current->type == NODE_IDENTIFIER) {
            return current->data.stringValue;
        } else if (current->type == NODE_POINTER_DECLARATOR) {
            current = current->data.pointer_declarator.base_declarator;
        } else if (current->type == NODE_ARRAY_DECLARATOR) {
            current = current->data.array_declarator.base_declarator;
        } else if (current->type == NODE_FUNCTION_DECLARATOR) {
            current = current->data.function_declarator.base_declarator;
        } else {
            return NULL; // Should not happen for valid declarators
        }
    }
    return NULL;
}

void analyze_declaration(ASTNode* node) {
    Type* base_type = get_type_from_specifiers(node->data.declaration.specifiers);
    
    // Check if this is a typedef
    int is_typedef = 0;
    for (ASTNodeList* spec = node->data.declaration.specifiers; spec; spec = spec->next) {
        if (spec->node->type == NODE_SPECIFIER && spec->node->data.specifier == TYPEDEF) {
            is_typedef = 1;
            break;
        }
    }

    if (node->data.declaration.declarators) {
        for (ASTNodeList* d = node->data.declaration.declarators; d; d = d->next) {
            ASTNode* init_decl = d->node;
            ASTNode* declarator = init_decl->data.init_declarator.declarator;
            const char* name = get_name_from_declarator(declarator);

            if (!name) {
                fprintf(stderr, "Semantic Error (Line %d): Declarator is missing a name.\n", node->lineno);
                semantic_errors++;
                continue;
            }

            Type* final_type = build_type_from_declarator(copy_type(base_type), declarator);

            if (is_typedef) {
                add_symbol(name, final_type, SYM_TYPEDEF);
                add_typename(name);
            } else {
                add_symbol(name, final_type, SYM_VARIABLE);
            }

            if (init_decl->data.init_declarator.initializer) {
                Type* initializer_type = analyze_expression(init_decl->data.init_declarator.initializer);
                if (!are_types_compatible(final_type, initializer_type)) {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible types in initialization of '%s'. Cannot assign '%s' to '%s'.\n",
                        init_decl->lineno, name, type_to_string(initializer_type), type_to_string(final_type));
                    semantic_errors++;
                }
            }
        }
    }
    free(base_type);
}

void analyze_function_definition(ASTNode* node) {
    Type* return_type = get_type_from_specifiers(node->data.function_definition.specifiers);
    const char* name = get_name_from_declarator(node->data.function_definition.declarator);
    
    if (!name) {
        fprintf(stderr, "Semantic Error (Line %d): Function definition is missing a name.\n", node->lineno);
        semantic_errors++;
        return;
    }
    
    Type* func_type = create_type(TYPE_FUNCTION);
    func_type->data.function_sig.return_type = return_type;
    
    // Add function to parent scope before processing body to allow recursion
    add_symbol(name, func_type, SYM_FUNCTION);
    
    // Now process body
    enter_scope();
    current_function_return_type = return_type;
    
    // TODO: Add function parameters to the new scope
    
    analyze_node(node->data.function_definition.body);
    
    current_function_return_type = NULL;
    leave_scope();
}

void analyze_statement(ASTNode* node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_COMPOUND_STATEMENT:
            enter_scope();
            for (ASTNodeList* item = node->data.compound_statement.items; item; item = item->next) {
                analyze_node(item->node);
            }
            leave_scope();
            break;
        case NODE_EXPRESSION_STATEMENT:
            if (node->data.expression_statement.expression) {
                analyze_expression(node->data.expression_statement.expression);
            }
            break;
        case NODE_IF_STATEMENT:
        case NODE_WHILE_STATEMENT:
        case NODE_FOR_STATEMENT:
        case NODE_DO_WHILE_STATEMENT: {
            Type* cond_type = NULL;
            if (node->type == NODE_IF_STATEMENT) {
                cond_type = analyze_expression(node->data.if_statement.condition);
                analyze_statement(node->data.if_statement.if_body);
                if (node->data.if_statement.else_body) analyze_statement(node->data.if_statement.else_body);
            } else if (node->type == NODE_WHILE_STATEMENT) {
                cond_type = analyze_expression(node->data.while_statement.condition);
                analyze_statement(node->data.while_statement.body);
            } else if (node->type == NODE_FOR_STATEMENT) {
                if (node->data.for_statement.init) analyze_expression(node->data.for_statement.init);
                if (node->data.for_statement.condition) cond_type = analyze_expression(node->data.for_statement.condition);
                if (node->data.for_statement.increment) analyze_expression(node->data.for_statement.increment);
                analyze_statement(node->data.for_statement.body);
            } else if (node->type == NODE_DO_WHILE_STATEMENT) {
                analyze_statement(node->data.do_while_statement.body);
                cond_type = analyze_expression(node->data.do_while_statement.condition);
            } 
            
            if (cond_type && !is_scalar_type(cond_type)) {
                fprintf(stderr, "Semantic Error (Line %d): Condition of statement must be a scalar type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
                semantic_errors++;
            }
            break;
        }
        case NODE_RETURN_STATEMENT:
            if (!current_function_return_type) {
                fprintf(stderr, "Semantic Error (Line %d): return statement not in a function.\n", node->lineno);
                semantic_errors++;
            } else {
                Type* return_expr_type = node->data.return_statement.expression ?
                    analyze_expression(node->data.return_statement.expression) :
                    create_type(TYPE_VOID);
                if (!are_types_compatible(current_function_return_type, return_expr_type)) {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible return type. Expected '%s' but got '%s'.\n", node->lineno, type_to_string(current_function_return_type), type_to_string(return_expr_type));
                    semantic_errors++;
                }
            }
            break;
        default:
            // For simple statements like break, continue, etc., there's no analysis needed.
            break;
    }
}

Type* analyze_expression(ASTNode* node) {
    if (!node) return create_type(TYPE_VOID);

    switch(node->type) {
        case NODE_CONSTANT:
            // A more complete version would check for suffixes (f, l, ul) and format (.)
            return create_type(TYPE_INT);
        case NODE_STRING_LITERAL:
            return create_pointer_type(create_type(TYPE_CHAR));
        
        case NODE_IDENTIFIER: {
            Symbol* sym = find_symbol(node->data.stringValue);
            if (!sym) {
                fprintf(stderr, "Semantic Error (Line %d): Use of undeclared identifier '%s'.\n", node->lineno, node->data.stringValue);
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }
            if (sym->kind == SYM_TYPEDEF) {
                 fprintf(stderr, "Semantic Error (Line %d): Cannot use a typename '%s' as an expression.\n", node->lineno, node->data.stringValue);
                 semantic_errors++;
                 return create_type(TYPE_UNKNOWN);
            }
            return sym->type;
        }

        case NODE_BINARY_EXPR: {
            Type* left_type = analyze_expression(node->data.binary_expr.left);
            Type* right_type = analyze_expression(node->data.binary_expr.right);
            if (left_type->kind == TYPE_UNKNOWN || right_type->kind == TYPE_UNKNOWN) {
                return create_type(TYPE_UNKNOWN);
            }

            if (is_arithmetic_type(left_type) && is_arithmetic_type(right_type)) {
                return get_common_arithmetic_type(left_type, right_type);
            }
            // Basic pointer arithmetic: ptr + int
            if (left_type->kind == TYPE_POINTER && is_integer_type(right_type)) return left_type;
            if (is_integer_type(left_type) && right_type->kind == TYPE_POINTER) return right_type;
            
            fprintf(stderr, "Semantic Error (Line %d): Invalid operands for binary operator '%s'. Types are '%s' and '%s'.\n",
                node->lineno, token_to_string(node->data.binary_expr.op), type_to_string(left_type), type_to_string(right_type));
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }

        case NODE_ASSIGNMENT: {
            Type* lvalue_type = analyze_expression(node->data.assignment.lvalue);
            Type* rvalue_type = analyze_expression(node->data.assignment.rvalue);
            if (!are_types_compatible(lvalue_type, rvalue_type)) {
                 fprintf(stderr, "Semantic Error (Line %d): Incompatible types in assignment. Cannot assign '%s' to '%s'.\n",
                    node->lineno, type_to_string(rvalue_type), type_to_string(lvalue_type));
                semantic_errors++;
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
                    return operand_type->data.base;
                case '!': return create_type(TYPE_INT); // Logical NOT always returns an int (0 or 1)
                default: return operand_type; // For +/-/~/++/--
            }
        }
        
        case NODE_FUNC_CALL: {
            // This is a simplified version. A full version would match argument types.
            Type* func_expr_type = analyze_expression(node->data.func_call.function);
            if (func_expr_type->kind == TYPE_POINTER && func_expr_type->data.base->kind == TYPE_FUNCTION) {
                // Calling a function pointer
                return func_expr_type->data.base->data.function_sig.return_type;
            }
            if (func_expr_type->kind != TYPE_FUNCTION) {
                 fprintf(stderr, "Semantic Error (Line %d): Called object is not a function or function pointer.\n", node->lineno);
                 semantic_errors++;
                 return create_type(TYPE_UNKNOWN);
            }
            return func_expr_type->data.function_sig.return_type;
        }

        default: return create_type(TYPE_UNKNOWN);
    }
}

void analyze_node(ASTNode* node) {
    if (!node) return;
    switch(node->type) {
        case NODE_TRANSLATION_UNIT:
            for (ASTNodeList* item = node->data.items_list; item; item = item->next) {
                analyze_node(item->node);
            }
            break;
        case NODE_DECLARATION:
            analyze_declaration(node);
            break;
        case NODE_FUNCTION_DEFINITION:
            analyze_function_definition(node);
            break;
        default:
            // If it's a statement, analyze it as such.
            // Expressions are handled recursively by statement analyzers.
            analyze_statement(node);
            break;
    }
}

int analyze_ast(ASTNode* root) {
    if (!root) return 1; // Nothing to analyze
    semantic_errors = 0;
    init_symbol_table();
    analyze_node(root);
    return semantic_errors == 0;
}