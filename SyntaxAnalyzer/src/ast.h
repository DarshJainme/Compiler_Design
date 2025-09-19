
#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

/* Forward declaration */
typedef struct ast_node ast_node;
typedef struct token_info token_info;

/* Enumeration for different types of AST nodes */
typedef enum {
    /* Program and declarations */
    NODE_PROGRAM,
    NODE_DECLARATION_LIST,
    NODE_FUNCTION_DECLARATION,
    NODE_FUNCTION_DEFINITION,
    NODE_SCOPED_FUNCTION_DEFINITION,
    NODE_VARIABLE_DECLARATION,
    NODE_ARRAY_DECLARATION,
    NODE_CLASS_DECLARATION,
    NODE_STRUCT_DECLARATION,
    NODE_TYPEDEF_DECLARATION,
    NODE_PARAMETER_LIST,
    NODE_PARAMETER_DECLARATION,
    NODE_DECLARATION,
    NODE_FUNCTION,
    NODE_LIST,
    NODE_PARAMETER,
    NODE_ARRAY,
    NODE_STRUCT,
    NODE_TYPE,
    NODE_COMPOUND,
    NODE_EXPRESSION_STMT,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_RETURN,
    NODE_BREAK,
    NODE_CONTINUE,
    NODE_LABELED_STMT,
    NODE_CASE,
    NODE_DEFAULT,
    NODE_ASSIGNMENT,
    NODE_BINOP,
    NODE_UNARY,
    NODE_ARRAY_ACCESS,
    
    /* Statements */
    NODE_STATEMENT_LIST,
    NODE_COMPOUND_STATEMENT,
    NODE_EXPRESSION_STATEMENT,
    NODE_IF_STATEMENT,
    NODE_WHILE_STATEMENT,
    NODE_FOR_STATEMENT,
    NODE_DO_WHILE_STATEMENT,
    NODE_SWITCH_STATEMENT,
    NODE_CASE_STATEMENT,
    NODE_DEFAULT_STATEMENT,
    NODE_BREAK_STATEMENT,
    NODE_CONTINUE_STATEMENT,
    NODE_RETURN_STATEMENT,
    NODE_GOTO_STATEMENT,
    NODE_LABELED_STATEMENT,
    
    /* Expressions */
    NODE_BINARY_EXPRESSION,
    NODE_UNARY_EXPRESSION,
    NODE_ASSIGNMENT_EXPRESSION,
    NODE_CONDITIONAL_EXPRESSION,
    NODE_FUNCTION_CALL,
    NODE_MEMBER_ACCESS,
    NODE_SCOPED_ACCESS,
    
    /* Literals and identifiers */
    NODE_IDENTIFIER,
    NODE_INTEGER_LITERAL,
    NODE_FLOAT_LITERAL,
    NODE_CHAR_LITERAL,
    NODE_STRING_LITERAL,
    
    /* Type system */
    NODE_TYPE_SPECIFIER,
    NODE_DECLARATOR,
    NODE_POINTER,
    
    /* Initializers and arguments */
    NODE_INITIALIZER_LIST,
    NODE_ARGUMENT_LIST,
    
    /* Member declarations */
    NODE_MEMBER_DECLARATION,
    NODE_ACCESS_SPECIFIER,
    
    /* Advanced features */
    NODE_LAMBDA_EXPRESSION,
    NODE_NEW_EXPRESSION,
    NODE_DELETE_EXPRESSION,
    NODE_THIS_EXPRESSION,
    NODE_MALLOC_CALL,
    NODE_FREE_CALL
} node_type;

/* Token information structure */
struct token_info {
    char *text;
    char *type;
    int line;
    int column;
};

/* Union for different node data types */
typedef union node_data {
    /* Program */
    struct {
        ast_node *declarations;
    } program;
    
    /* Declaration list */
    struct {
        ast_node *declaration;
        ast_node *next;
    } declaration_list;
    
    /* Function */
    struct {
        ast_node *return_type;
        char *name;
        ast_node *parameters;
        ast_node *body;
    } function;
    
    /* Variable */
    struct {
        ast_node *type;
        char *name;
        ast_node *initializer;
        int is_static;
        int array_size;
        int dimensions;
    } variable;
    
    /* Class declaration */
    struct {
        char *name;
        char *base_class;
        ast_node *members;
    } class_decl;
    
    /* Struct declaration */
    struct {
        char *name;
        ast_node *members;
    } struct_decl;
    
    /* Typedef declaration */
    struct {
        ast_node *type;
        char *name;
    } typedef_decl;
    
    /* Parameter list */
    struct {
        ast_node *parameter;
        ast_node *next;
        int has_varargs;
    } parameter_list;
    
    /* Parameter */
    struct {
        ast_node *type;
        char *name;
    } parameter;
    
    /* Statement list */
    struct {
        ast_node *statement;
        ast_node *next;
    } statement_list;
    
    /* Compound statement */
    struct {
        ast_node *statements;
    } compound;
    
    /* Expression statement */
    struct {
        ast_node *expression;
    } expression_stmt;
    
    /* If statement */
    struct {
        ast_node *condition;
        ast_node *then_stmt;
        ast_node *else_stmt;
    } if_stmt;
    
    /* While statement */
    struct {
        ast_node *condition;
        ast_node *body;
    } while_stmt;
    
    /* For statement */
    struct {
        ast_node *init;
        ast_node *condition;
        ast_node *update;
        ast_node *body;
    } for_stmt;
    
    /* Do-while statement */
    struct {
        ast_node *body;
        ast_node *condition;
    } do_while_stmt;
    
    /* Switch statement */
    struct {
        ast_node *expression;
        ast_node *body;
    } switch_stmt;
    
    /* Case statement */
    struct {
        int value;
        ast_node *statement;
    } case_stmt;
    
    /* Default statement */
    struct {
        ast_node *statement;
    } default_stmt;
    
    /* Return statement */
    struct {
        ast_node *expression;
    } return_stmt;
    
    /* Goto statement */
    struct {
        char *label;
    } goto_stmt;
    
    /* Labeled statement */
    struct {
        char *label;
        ast_node *statement;
    } labeled_stmt;
    
    /* Binary expression */
    struct {
        ast_node *left;
        ast_node *right;
        char *operator;
    } binary_expr;
    
    /* Unary expression */
    struct {
        ast_node *operand;
        char *operator;
        int is_prefix;
    } unary_expr;
    
    /* Assignment expression */
    struct {
        ast_node *left;
        ast_node *right;
        char *operator;
    } assignment_expr;
    
    /* Conditional expression */
    struct {
        ast_node *condition;
        ast_node *true_expr;
        ast_node *false_expr;
    } conditional_expr;
    
    /* Function call */
    struct {
        ast_node *function;
        ast_node *arguments;
    } function_call;
    
    /* Array access */
    struct {
        ast_node *array;
        ast_node *index;
    } array_access;
    
    /* Member access */
    struct {
        ast_node *object;
        char *member;
        int is_pointer;
    } member_access;
    
    /* Identifier */
    struct {
        char *name;
    } identifier;
    
    /* Integer literal */
    struct {
        int value;
    } integer_literal;
    
    /* Float literal */
    struct {
        float value;
    } float_literal;
    
    /* Character literal */
    struct {
        char value;
    } char_literal;
    
    /* String literal */
    struct {
        char *value;
    } string_literal;
    
    /* Type specifier */
    struct {
        char *type_name;
        int is_struct;
        int is_class;
    } type_specifier;
    
    /* Pointer */
    struct {
        int level;
    } pointer;
    
    /* Initializer list */
    struct {
        ast_node *initializer;
        ast_node *next;
    } initializer_list;
    
    /* Argument list */
    struct {
        ast_node *argument;
        ast_node *next;
    } argument_list;
    
    /* Member declaration */
    struct {
        ast_node *type;
        ast_node *declarators;
    } member_declaration;
    
    /* Access specifier */
    struct {
        char *access_type;
    } access_specifier;
    
    /* Lambda expression */
    struct {
        ast_node *capture_list;
        ast_node *parameters;
        ast_node *body;
    } lambda_expr;
    
    /* New expression */
    struct {
        ast_node *type;
        ast_node *size;
    } new_expr;
    
    /* Delete expression */
    struct {
        ast_node *expression;
    } delete_expr;
    /* Malloc call */
    struct {
        ast_node *size;
    } malloc_call;
    
    /* Free call */
    struct {
        ast_node *pointer;
    } free_call;
} node_data;

/* AST node structure */
struct ast_node {
    node_type type;
    node_data data;
    int line;
    int column;
};

/* Function prototypes for AST creation */
ast_node *create_ast_node(node_type type, const char *value, int line);
void add_ast_child(ast_node *parent, ast_node *child);
void save_ast_to_file(ast_node *node, const char *filename);
void save_symbol_table_to_file(void *table, const char *filename);
ast_node *create_program_node(ast_node *declarations);
ast_node *create_declaration_list(ast_node *declaration);
ast_node *append_declaration(ast_node *list, ast_node *declaration);

/* Function nodes */
ast_node *create_function_declaration(ast_node *return_type, char *name, ast_node *parameters);
ast_node *create_function_definition(ast_node *return_type, char *name, ast_node *parameters, ast_node *body);

/* Variable nodes */
ast_node *create_variable_declaration(ast_node *type, ast_node *declarator, ast_node *initializer);
ast_node *create_static_variable_declaration(ast_node *type, ast_node *declarator, ast_node *initializer);
ast_node *create_array_declaration(ast_node *type, char *name, int size, int dimensions);
ast_node *create_multidimensional_array_declaration(ast_node *type, char *name, int size1, int size2);

/* Class and struct nodes */
ast_node *create_class_declaration(char *name, char *base_class, ast_node *members);
ast_node *create_struct_declaration(char *name, ast_node *members);
ast_node *create_typedef_node(ast_node *type, char *name);

/* Parameter nodes */
ast_node *create_parameter_list(ast_node *parameter);
ast_node *append_parameter(ast_node *list, ast_node *parameter);
ast_node *append_varargs(ast_node *list);
ast_node *create_parameter_declaration(ast_node *type, ast_node *declarator);

/* Statement nodes */
ast_node *create_statement_list(ast_node *statement);
ast_node *append_statement(ast_node *list, ast_node *statement);
ast_node *create_compound_statement(ast_node *statements);
ast_node *create_expression_statement(ast_node *expression);
ast_node *create_if_statement(ast_node *condition, ast_node *then_stmt, ast_node *else_stmt);
ast_node *create_while_statement(ast_node *condition, ast_node *body);
ast_node *create_for_statement(ast_node *init, ast_node *condition, ast_node *update, ast_node *body);
ast_node *create_do_while_statement(ast_node *body, ast_node *condition);
ast_node *create_switch_statement(ast_node *expression, ast_node *body);
ast_node *create_case_statement(int value, ast_node *statement);
ast_node *create_default_statement(ast_node *statement);
ast_node *create_break_statement(void);
ast_node *create_continue_statement(void);
ast_node *create_return_statement(ast_node *expression);
ast_node *create_goto_statement(char *label);
ast_node *create_labeled_statement(char *label, ast_node *statement);

/* Expression nodes */
ast_node *create_binary_expression(ast_node *left, ast_node *right, const char *op);
ast_node *create_unary_expression(ast_node *operand, const char *op, int is_prefix);
ast_node *create_assignment_expression(ast_node *left, ast_node *right, const char *op);
ast_node *create_conditional_expression(ast_node *condition, ast_node *true_expr, ast_node *false_expr);
ast_node *create_comma_expression(ast_node *left, ast_node *right);
ast_node *create_function_call(ast_node *function, ast_node *arguments);
ast_node *create_array_access(ast_node *array, ast_node *index);
ast_node *create_member_access(ast_node *object, char *member, int is_pointer);

/* Literal nodes */
ast_node *create_identifier_expression(char *name);
ast_node *create_integer_literal(int value);
ast_node *create_float_literal(float value);
ast_node *create_char_literal(char value);
ast_node *create_string_literal(char *value);

/* Type and declarator nodes */
ast_node *create_type_specifier(char *type_name);
ast_node *create_struct_type_specifier(char *name);
ast_node *create_class_type_specifier(char *name);
ast_node *create_identifier_declarator(char *name);
ast_node *create_pointer_declarator(ast_node *pointer, ast_node *declarator);
ast_node *create_array_declarator(ast_node *declarator, int size);
ast_node *create_function_declarator(ast_node *declarator, ast_node *parameters);
ast_node *create_abstract_array_declarator(int size);
ast_node *create_abstract_function_declarator(ast_node *parameters);
ast_node *create_pointer_node(int level);

/* Initializer and argument nodes */
ast_node *create_initializer_list(ast_node *initializer);
ast_node *append_initializer(ast_node *list, ast_node *initializer);
ast_node *create_argument_list(ast_node *argument);
ast_node *append_argument(ast_node *list, ast_node *argument);

/* Member and access nodes */
ast_node *create_access_specifier(char *access_type);
ast_node *create_member_list(ast_node *member);
ast_node *append_member(ast_node *list, ast_node *member);
ast_node *create_member_declaration(ast_node *type, ast_node *declarators);
ast_node *create_member_declaration_list(ast_node *declaration);
ast_node *append_member_declaration(ast_node *list, ast_node *declaration);
ast_node *create_member_declarator_list(ast_node *declarator);
ast_node *append_member_declarator(ast_node *list, ast_node *declarator);
ast_node *create_initialized_member_declarator(ast_node *declarator, ast_node *initializer);

/* Advanced feature nodes */
ast_node *create_lambda_expression(ast_node *capture_list, ast_node *parameters, ast_node *body);
ast_node *create_new_expression(ast_node *type, ast_node *size);
ast_node *create_delete_expression(ast_node *expression);
ast_node *create_this_expression(void);
ast_node *create_malloc_call(ast_node *size);
ast_node *create_free_call(ast_node *pointer);

/* Utility functions */
void free_ast(ast_node *node);
void print_ast(ast_node *node, int depth);
void print_ast_to_file(ast_node *node, int depth, FILE *file);

#endif /* AST_H */
