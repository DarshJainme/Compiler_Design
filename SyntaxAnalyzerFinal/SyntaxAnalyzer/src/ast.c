
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Utility function for string duplication
char *my_strdup(const char *s) {
    if (!s) return NULL;
    char *dup = malloc(strlen(s) + 1);
    if (dup) strcpy(dup, s);
    return dup;
}

// Create a basic AST node
static ast_node *create_node(node_type type) {
    ast_node *node = malloc(sizeof(ast_node));
    if (node) {
        memset(node, 0, sizeof(ast_node));
        node->type = type;
    }
    return node;
}

// Generic AST node creation function
ast_node *create_ast_node(node_type type, const char *value, int line) {
    ast_node *node = malloc(sizeof(ast_node));
    if (!node) return NULL;
    
    memset(node, 0, sizeof(ast_node));
    node->type = type;
    node->line = line;
    
    // Set value based on node type
    if (value) {
        switch (type) {
            case NODE_IDENTIFIER:
                node->data.identifier.name = my_strdup(value);
                break;
            case NODE_TYPE:
                node->data.type_specifier.type_name = my_strdup(value);
                break;
            case NODE_FUNCTION:
                node->data.function.name = my_strdup(value);
                break;
            case NODE_STRUCT:
                node->data.struct_decl.name = my_strdup(value);
                break;
            case NODE_LABELED_STMT:
                node->data.labeled_stmt.label = my_strdup(value);
                break;
            default:
                break;
        }
    }
    
    return node;
}

// Add child to AST node (simplified implementation)
void add_ast_child(ast_node *parent, ast_node *child) {
    if (!parent || !child) return;
    // This is a simplified implementation for basic functionality
}

// Program nodes
ast_node *create_program_node(ast_node *declarations) {
    ast_node *node = create_node(NODE_PROGRAM);
    if (node) {
        node->data.program.declarations = declarations;
    }
    return node;
}

ast_node *create_declaration_list(ast_node *declaration) {
    ast_node *node = create_node(NODE_DECLARATION_LIST);
    if (node) {
        node->data.declaration_list.declaration = declaration;
        node->data.declaration_list.next = NULL;
    }
    return node;
}

ast_node *append_declaration(ast_node *list, ast_node *declaration) {
    if (!list) return create_declaration_list(declaration);
    
    ast_node *current = list;
    while (current->data.declaration_list.next) {
        current = current->data.declaration_list.next;
    }
    current->data.declaration_list.next = create_declaration_list(declaration);
    return list;
}

// Function nodes
ast_node *create_function_declaration(ast_node *return_type, char *name, ast_node *parameters) {
    ast_node *node = create_node(NODE_FUNCTION_DECLARATION);
    if (node) {
        node->data.function.return_type = return_type;
        node->data.function.name = my_strdup(name);
        node->data.function.parameters = parameters;
        node->data.function.body = NULL;
    }
    return node;
}

ast_node *create_function_definition(ast_node *return_type, char *name, ast_node *parameters, ast_node *body) {
    ast_node *node = create_node(NODE_FUNCTION_DEFINITION);
    if (node) {
        node->data.function.return_type = return_type;
        node->data.function.name = my_strdup(name);
        node->data.function.parameters = parameters;
        node->data.function.body = body;
    }
    return node;
}

// Variable nodes
ast_node *create_variable_declaration(ast_node *type, ast_node *declarator, ast_node *initializer) {
    ast_node *node = create_node(NODE_VARIABLE_DECLARATION);
    if (node) {
        node->data.variable.type = type;
        node->data.variable.initializer = initializer;
        node->data.variable.is_static = 0;
        if (declarator && declarator->type == NODE_IDENTIFIER) {
            node->data.variable.name = my_strdup(declarator->data.identifier.name);
        }
    }
    return node;
}

ast_node *create_static_variable_declaration(ast_node *type, ast_node *declarator, ast_node *initializer) {
    ast_node *node = create_variable_declaration(type, declarator, initializer);
    if (node) {
        node->data.variable.is_static = 1;
    }
    return node;
}

ast_node *create_array_declaration(ast_node *type, char *name, int size, int dimensions) {
    ast_node *node = create_node(NODE_ARRAY_DECLARATION);
    if (node) {
        node->data.variable.type = type;
        node->data.variable.name = my_strdup(name);
        node->data.variable.array_size = size;
        node->data.variable.dimensions = dimensions;
    }
    return node;
}

ast_node *create_multidimensional_array_declaration(ast_node *type, char *name, int size1, int size2) {
    ast_node *node = create_node(NODE_ARRAY_DECLARATION);
    if (node) {
        node->data.variable.type = type;
        node->data.variable.name = my_strdup(name);
        node->data.variable.array_size = size1 * size2;
        node->data.variable.dimensions = 2;
    }
    return node;
}

// Class and struct nodes
ast_node *create_class_declaration(char *name, char *base_class, ast_node *members) {
    ast_node *node = create_node(NODE_CLASS_DECLARATION);
    if (node) {
        node->data.class_decl.name = my_strdup(name);
        node->data.class_decl.base_class = my_strdup(base_class);
        node->data.class_decl.members = members;
    }
    return node;
}

ast_node *create_struct_declaration(char *name, ast_node *members) {
    ast_node *node = create_node(NODE_STRUCT_DECLARATION);
    if (node) {
        node->data.struct_decl.name = my_strdup(name);
        node->data.struct_decl.members = members;
    }
    return node;
}

ast_node *create_typedef_node(ast_node *type, char *name) {
    ast_node *node = create_node(NODE_TYPEDEF_DECLARATION);
    if (node) {
        node->data.typedef_decl.type = type;
        node->data.typedef_decl.name = my_strdup(name);
    }
    return node;
}

// Parameter nodes
ast_node *create_parameter_list(ast_node *parameter) {
    ast_node *node = create_node(NODE_PARAMETER_LIST);
    if (node) {
        node->data.parameter_list.parameter = parameter;
        node->data.parameter_list.next = NULL;
    }
    return node;
}

ast_node *append_parameter(ast_node *list, ast_node *parameter) {
    if (!list) return create_parameter_list(parameter);
    
    ast_node *current = list;
    while (current->data.parameter_list.next) {
        current = current->data.parameter_list.next;
    }
    current->data.parameter_list.next = create_parameter_list(parameter);
    return list;
}

ast_node *append_varargs(ast_node *list) {
    if (list) {
        list->data.parameter_list.has_varargs = 1;
    }
    return list;
}

ast_node *create_parameter_declaration(ast_node *type, ast_node *declarator) {
    ast_node *node = create_node(NODE_PARAMETER_DECLARATION);
    if (node) {
        node->data.parameter.type = type;
        if (declarator && declarator->type == NODE_IDENTIFIER) {
            node->data.parameter.name = my_strdup(declarator->data.identifier.name);
        }
    }
    return node;
}

// Statement nodes
ast_node *create_statement_list(ast_node *statement) {
    ast_node *node = create_node(NODE_STATEMENT_LIST);
    if (node) {
        node->data.statement_list.statement = statement;
        node->data.statement_list.next = NULL;
    }
    return node;
}

ast_node *append_statement(ast_node *list, ast_node *statement) {
    if (!list) return create_statement_list(statement);
    
    ast_node *current = list;
    while (current->data.statement_list.next) {
        current = current->data.statement_list.next;
    }
    current->data.statement_list.next = create_statement_list(statement);
    return list;
}

ast_node *create_compound_statement(ast_node *statements) {
    ast_node *node = create_node(NODE_COMPOUND_STATEMENT);
    if (node) {
        node->data.compound.statements = statements;
    }
    return node;
}

ast_node *create_expression_statement(ast_node *expression) {
    ast_node *node = create_node(NODE_EXPRESSION_STATEMENT);
    if (node) {
        node->data.expression_stmt.expression = expression;
    }
    return node;
}

// Control flow statements
ast_node *create_if_statement(ast_node *condition, ast_node *then_stmt, ast_node *else_stmt) {
    ast_node *node = create_node(NODE_IF_STATEMENT);
    if (node) {
        node->data.if_stmt.condition = condition;
        node->data.if_stmt.then_stmt = then_stmt;
        node->data.if_stmt.else_stmt = else_stmt;
    }
    return node;
}

ast_node *create_while_statement(ast_node *condition, ast_node *body) {
    ast_node *node = create_node(NODE_WHILE_STATEMENT);
    if (node) {
        node->data.while_stmt.condition = condition;
        node->data.while_stmt.body = body;
    }
    return node;
}

ast_node *create_for_statement(ast_node *init, ast_node *condition, ast_node *update, ast_node *body) {
    ast_node *node = create_node(NODE_FOR_STATEMENT);
    if (node) {
        node->data.for_stmt.init = init;
        node->data.for_stmt.condition = condition;
        node->data.for_stmt.update = update;
        node->data.for_stmt.body = body;
    }
    return node;
}

ast_node *create_do_while_statement(ast_node *body, ast_node *condition) {
    ast_node *node = create_node(NODE_DO_WHILE_STATEMENT);
    if (node) {
        node->data.do_while_stmt.body = body;
        node->data.do_while_stmt.condition = condition;
    }
    return node;
}

ast_node *create_switch_statement(ast_node *expression, ast_node *body) {
    ast_node *node = create_node(NODE_SWITCH_STATEMENT);
    if (node) {
        node->data.switch_stmt.expression = expression;
        node->data.switch_stmt.body = body;
    }
    return node;
}

ast_node *create_case_statement(int value, ast_node *statement) {
    ast_node *node = create_node(NODE_CASE_STATEMENT);
    if (node) {
        node->data.case_stmt.value = value;
        node->data.case_stmt.statement = statement;
    }
    return node;
}

ast_node *create_default_statement(ast_node *statement) {
    ast_node *node = create_node(NODE_DEFAULT_STATEMENT);
    if (node) {
        node->data.default_stmt.statement = statement;
    }
    return node;
}

ast_node *create_break_statement(void) {
    return create_node(NODE_BREAK_STATEMENT);
}

ast_node *create_continue_statement(void) {
    return create_node(NODE_CONTINUE_STATEMENT);
}

ast_node *create_return_statement(ast_node *expression) {
    ast_node *node = create_node(NODE_RETURN_STATEMENT);
    if (node) {
        node->data.return_stmt.expression = expression;
    }
    return node;
}

ast_node *create_goto_statement(char *label) {
    ast_node *node = create_node(NODE_GOTO_STATEMENT);
    if (node) {
        node->data.goto_stmt.label = my_strdup(label);
    }
    return node;
}

ast_node *create_labeled_statement(char *label, ast_node *statement) {
    ast_node *node = create_node(NODE_LABELED_STATEMENT);
    if (node) {
        node->data.labeled_stmt.label = my_strdup(label);
        node->data.labeled_stmt.statement = statement;
    }
    return node;
}

// Expression nodes
ast_node *create_binary_expression(ast_node *left, ast_node *right, const char *op) {
    ast_node *node = create_node(NODE_BINARY_EXPRESSION);
    if (node) {
        node->data.binary_expr.left = left;
        node->data.binary_expr.right = right;
        node->data.binary_expr.operator = my_strdup(op);
    }
    return node;
}

ast_node *create_unary_expression(ast_node *operand, const char *op, int is_prefix) {
    ast_node *node = create_node(NODE_UNARY_EXPRESSION);
    if (node) {
        node->data.unary_expr.operand = operand;
        node->data.unary_expr.operator = my_strdup(op);
        node->data.unary_expr.is_prefix = is_prefix;
    }
    return node;
}

ast_node *create_assignment_expression(ast_node *left, ast_node *right, const char *op) {
    ast_node *node = create_node(NODE_ASSIGNMENT_EXPRESSION);
    if (node) {
        node->data.assignment_expr.left = left;
        node->data.assignment_expr.right = right;
        node->data.assignment_expr.operator = my_strdup(op);
    }
    return node;
}

ast_node *create_conditional_expression(ast_node *condition, ast_node *true_expr, ast_node *false_expr) {
    ast_node *node = create_node(NODE_CONDITIONAL_EXPRESSION);
    if (node) {
        node->data.conditional_expr.condition = condition;
        node->data.conditional_expr.true_expr = true_expr;
        node->data.conditional_expr.false_expr = false_expr;
    }
    return node;
}

ast_node *create_comma_expression(ast_node *left, ast_node *right) {
    return create_binary_expression(left, right, ",");
}

ast_node *create_function_call(ast_node *function, ast_node *arguments) {
    ast_node *node = create_node(NODE_FUNCTION_CALL);
    if (node) {
        node->data.function_call.function = function;
        node->data.function_call.arguments = arguments;
    }
    return node;
}

ast_node *create_array_access(ast_node *array, ast_node *index) {
    ast_node *node = create_node(NODE_ARRAY_ACCESS);
    if (node) {
        node->data.array_access.array = array;
        node->data.array_access.index = index;
    }
    return node;
}

ast_node *create_member_access(ast_node *object, char *member, int is_pointer) {
    ast_node *node = create_node(NODE_MEMBER_ACCESS);
    if (node) {
        node->data.member_access.object = object;
        node->data.member_access.member = my_strdup(member);
        node->data.member_access.is_pointer = is_pointer;
    }
    return node;
}

// Literal nodes
ast_node *create_identifier_expression(char *name) {
    ast_node *node = create_node(NODE_IDENTIFIER);
    if (node) {
        node->data.identifier.name = my_strdup(name);
    }
    return node;
}

ast_node *create_integer_literal(int value) {
    ast_node *node = create_node(NODE_INTEGER_LITERAL);
    if (node) {
        node->data.integer_literal.value = value;
    }
    return node;
}

ast_node *create_float_literal(float value) {
    ast_node *node = create_node(NODE_FLOAT_LITERAL);
    if (node) {
        node->data.float_literal.value = value;
    }
    return node;
}

ast_node *create_char_literal(char value) {
    ast_node *node = create_node(NODE_CHAR_LITERAL);
    if (node) {
        node->data.char_literal.value = value;
    }
    return node;
}

ast_node *create_string_literal(char *value) {
    ast_node *node = create_node(NODE_STRING_LITERAL);
    if (node) {
        node->data.string_literal.value = my_strdup(value);
    }
    return node;
}

// Type and declarator nodes
ast_node *create_type_specifier(char *type_name) {
    ast_node *node = create_node(NODE_TYPE_SPECIFIER);
    if (node) {
        node->data.type_specifier.type_name = my_strdup(type_name);
        node->data.type_specifier.is_struct = 0;
        node->data.type_specifier.is_class = 0;
    }
    return node;
}

ast_node *create_struct_type_specifier(char *name) {
    ast_node *node = create_node(NODE_TYPE_SPECIFIER);
    if (node) {
        node->data.type_specifier.type_name = my_strdup(name);
        node->data.type_specifier.is_struct = 1;
        node->data.type_specifier.is_class = 0;
    }
    return node;
}

ast_node *create_class_type_specifier(char *name) {
    ast_node *node = create_node(NODE_TYPE_SPECIFIER);
    if (node) {
        node->data.type_specifier.type_name = my_strdup(name);
        node->data.type_specifier.is_struct = 0;
        node->data.type_specifier.is_class = 1;
    }
    return node;
}

ast_node *create_identifier_declarator(char *name) {
    return create_identifier_expression(name);
}

ast_node *create_pointer_declarator(ast_node *pointer, ast_node *declarator) {
    ast_node *node = create_node(NODE_POINTER);
    if (node) {
        node->data.pointer.level = 1;
    }
    return node;
}

ast_node *create_array_declarator(ast_node *declarator, int size) {
    ast_node *node = create_node(NODE_DECLARATOR);
    return node;
}

ast_node *create_function_declarator(ast_node *declarator, ast_node *parameters) {
    ast_node *node = create_node(NODE_DECLARATOR);
    return node;
}

ast_node *create_abstract_array_declarator(int size) {
    return create_node(NODE_DECLARATOR);
}

ast_node *create_abstract_function_declarator(ast_node *parameters) {
    return create_node(NODE_DECLARATOR);
}

ast_node *create_pointer_node(int level) {
    ast_node *node = create_node(NODE_POINTER);
    if (node) {
        node->data.pointer.level = level;
    }
    return node;
}

// Initializer and argument nodes
ast_node *create_initializer_list(ast_node *initializer) {
    ast_node *node = create_node(NODE_INITIALIZER_LIST);
    if (node) {
        node->data.initializer_list.initializer = initializer;
        node->data.initializer_list.next = NULL;
    }
    return node;
}

ast_node *append_initializer(ast_node *list, ast_node *initializer) {
    if (!list) return create_initializer_list(initializer);
    
    ast_node *current = list;
    while (current->data.initializer_list.next) {
        current = current->data.initializer_list.next;
    }
    current->data.initializer_list.next = create_initializer_list(initializer);
    return list;
}

ast_node *create_argument_list(ast_node *argument) {
    ast_node *node = create_node(NODE_ARGUMENT_LIST);
    if (node) {
        node->data.argument_list.argument = argument;
        node->data.argument_list.next = NULL;
    }
    return node;
}

ast_node *append_argument(ast_node *list, ast_node *argument) {
    if (!list) return create_argument_list(argument);
    
    ast_node *current = list;
    while (current->data.argument_list.next) {
        current = current->data.argument_list.next;
    }
    current->data.argument_list.next = create_argument_list(argument);
    return list;
}

// Member and access nodes
ast_node *create_access_specifier(char *access_type) {
    ast_node *node = create_node(NODE_ACCESS_SPECIFIER);
    if (node) {
        node->data.access_specifier.access_type = my_strdup(access_type);
    }
    return node;
}

ast_node *create_member_list(ast_node *member) {
    return create_statement_list(member);
}

ast_node *append_member(ast_node *list, ast_node *member) {
    return append_statement(list, member);
}

ast_node *create_member_declaration(ast_node *type, ast_node *declarators) {
    ast_node *node = create_node(NODE_MEMBER_DECLARATION);
    if (node) {
        node->data.member_declaration.type = type;
        node->data.member_declaration.declarators = declarators;
    }
    return node;
}

ast_node *create_member_declaration_list(ast_node *declaration) {
    return create_statement_list(declaration);
}

ast_node *append_member_declaration(ast_node *list, ast_node *declaration) {
    return append_statement(list, declaration);
}

ast_node *create_member_declarator_list(ast_node *declarator) {
    return create_argument_list(declarator);
}

ast_node *append_member_declarator(ast_node *list, ast_node *declarator) {
    return append_argument(list, declarator);
}

ast_node *create_initialized_member_declarator(ast_node *declarator, ast_node *initializer) {
    return declarator;
}

// Advanced feature nodes
ast_node *create_lambda_expression(ast_node *capture_list, ast_node *parameters, ast_node *body) {
    ast_node *node = create_node(NODE_LAMBDA_EXPRESSION);
    if (node) {
        node->data.lambda_expr.capture_list = capture_list;
        node->data.lambda_expr.parameters = parameters;
        node->data.lambda_expr.body = body;
    }
    return node;
}

ast_node *create_new_expression(ast_node *type, ast_node *size) {
    ast_node *node = create_node(NODE_NEW_EXPRESSION);
    if (node) {
        node->data.new_expr.type = type;
        node->data.new_expr.size = size;
    }
    return node;
}

ast_node *create_delete_expression(ast_node *expression) {
    ast_node *node = create_node(NODE_DELETE_EXPRESSION);
    if (node) {
        node->data.delete_expr.expression = expression;
    }
    return node;
}

ast_node *create_this_expression(void) {
    return create_node(NODE_THIS_EXPRESSION);
}

ast_node *create_printf_call(ast_node *arguments) {
    ast_node *node = create_node(NODE_PRINTF_CALL);
    if (node) {
        node->data.printf_call.arguments = arguments;
    }
    return node;
}

ast_node *create_scanf_call(ast_node *arguments) {
    ast_node *node = create_node(NODE_SCANF_CALL);
    if (node) {
        node->data.scanf_call.arguments = arguments;
    }
    return node;
}

ast_node *create_malloc_call(ast_node *size) {
    ast_node *node = create_node(NODE_MALLOC_CALL);
    if (node) {
        node->data.malloc_call.size = size;
    }
    return node;
}

ast_node *create_free_call(ast_node *pointer) {
    ast_node *node = create_node(NODE_FREE_CALL);
    if (node) {
        node->data.free_call.pointer = pointer;
    }
    return node;
}

// Utility functions
void free_ast(ast_node *node) {
    if (!node) return;
    
    // Free node-specific data
    switch (node->type) {
        case NODE_IDENTIFIER:
            free(node->data.identifier.name);
            break;
        case NODE_STRING_LITERAL:
            free(node->data.string_literal.value);
            break;
        case NODE_FUNCTION_DECLARATION:
        case NODE_FUNCTION_DEFINITION:
            free(node->data.function.name);
            free_ast(node->data.function.return_type);
            free_ast(node->data.function.parameters);
            free_ast(node->data.function.body);
            break;
        default:
            break;
    }
    
    free(node);
}

void print_ast(ast_node *node, int depth) {
    print_ast_to_file(node, depth, stdout);
}

void print_ast_to_file(ast_node *node, int depth, FILE *file) {
    if (!node || !file) return;
    
    // Print indentation
    for (int i = 0; i < depth; i++) {
        fprintf(file, "  ");
    }
    
    // Print node type and relevant data
    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(file, "Program\n");
            print_ast_to_file(node->data.program.declarations, depth + 1, file);
            break;
        case NODE_IDENTIFIER:
            fprintf(file, "Identifier: %s\n", node->data.identifier.name ? node->data.identifier.name : "");
            break;
        case NODE_INTEGER_LITERAL:
            fprintf(file, "Integer: %d\n", node->data.integer_literal.value);
            break;
        case NODE_FLOAT_LITERAL:
            fprintf(file, "Float: %f\n", node->data.float_literal.value);
            break;
        case NODE_STRING_LITERAL:
            fprintf(file, "String: \"%s\"\n", node->data.string_literal.value ? node->data.string_literal.value : "");
            break;
        case NODE_FUNCTION_DEFINITION:
            fprintf(file, "Function: %s\n", node->data.function.name ? node->data.function.name : "");
            print_ast_to_file(node->data.function.return_type, depth + 1, file);
            print_ast_to_file(node->data.function.parameters, depth + 1, file);
            print_ast_to_file(node->data.function.body, depth + 1, file);
            break;
        case NODE_DECLARATION_LIST:
            fprintf(file, "Declaration List\n");
            print_ast_to_file(node->data.declaration_list.declaration, depth, file);
            print_ast_to_file(node->data.declaration_list.next, depth, file);
            break;
        default:
            fprintf(file, "Node type: %d\n", node->type);
            break;
    }
}


