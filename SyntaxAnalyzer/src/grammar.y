%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"

extern int yylex();
extern char *yytext;
extern int yylineno;
extern FILE *yyin;

#ifndef _GNU_SOURCE
char *strdup(const char *s);
#endif

void yyerror(const char *s);
int error_count = 0;

ast_node *parse_tree = NULL;
symbol_table *current_scope = NULL;
extern symbol_table *global_scope;

typedef struct token_entry {
    char *token;
    char *type;
    struct token_entry *next;
} token_entry;

token_entry *token_list = NULL;
token_entry *token_list_tail = NULL;

void add_token_to_list(const char *token, const char *type) {
    token_entry *entry = malloc(sizeof(token_entry));
    if (!entry) return;
    entry->token = malloc(strlen(token) + 1);
    strcpy(entry->token, token);
    entry->type = malloc(strlen(type) + 1);
    strcpy(entry->type, type);
    entry->next = NULL;

    if (!token_list) {
        token_list = entry;
        token_list_tail = entry;
    } else {
        token_list_tail->next = entry;
        token_list_tail = entry;
    }
}

void print_token_table() {
    printf("\n%-25s %-20s\n", "Token", "Token_Type");
    printf("------------------------- --------------------\n");
    token_entry *current = token_list;
    while (current != NULL) {
        printf("%-25s %-20s\n", current->token, current->type);
        current = current->next;
    }
}

void free_token_list() {
    token_entry *current = token_list;
    while (current) {
        token_entry *next = current->next;
        free(current->token);
        free(current->type);
        free(current);
        current = next;
    }
    token_list = NULL;
    token_list_tail = NULL;
}

data_type get_type_from_string(const char* type_str) {
    if (!type_str) return TYPE_VOID;
    if (strcmp(type_str, "int") == 0) return TYPE_INT;
    if (strcmp(type_str, "float") == 0) return TYPE_FLOAT;
    if (strcmp(type_str, "char") == 0) return TYPE_CHAR;
    if (strcmp(type_str, "double") == 0) return TYPE_DOUBLE;
    if (strcmp(type_str, "void") == 0) return TYPE_VOID;
    return TYPE_CUSTOM;
}
%}

%define parse.error detailed
%locations

%union {
    int ival;
    float fval;
    char cval;
    char *sval;
    ast_node *node;
}

%token <ival> INTEGER_LITERAL
%token <fval> FLOAT_LITERAL
%token <cval> CHAR_LITERAL
%token <sval> STRING_LITERAL IDENTIFIER
%token IF ELSE WHILE FOR DO SWITCH CASE DEFAULT BREAK CONTINUE RETURN GOTO
%token STATIC STRUCT TYPEDEF
%token INT CHAR FLOAT DOUBLE VOID
%token CLASS PUBLIC PRIVATE PROTECTED VIRTUAL OVERRIDE NEW DELETE THIS
%token LAMBDA AUTO MALLOC FREE
%token PLUS MINUS MULTIPLY DIVIDE MODULO INCREMENT DECREMENT
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN MULT_ASSIGN DIV_ASSIGN
%token EQUAL NOT_EQUAL LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%token LOGICAL_AND LOGICAL_OR LOGICAL_NOT
%token BITWISE_AND BITWISE_OR BITWISE_XOR BITWISE_NOT LEFT_SHIFT RIGHT_SHIFT
%token SEMICOLON COMMA DOT ARROW SCOPE COLON
%token LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE

%type <node> program translation_unit external_declaration function_declaration function_definition
%type <node> variable_declaration array_declaration class_declaration struct_declaration typedef_declaration
%type <node> declaration statement_list statement compound_statement expression_statement
%type <node> non_if_statement matched_stmt unmatched_stmt
%type <node> selection_statement iteration_statement jump_statement labeled_statement expression
%type <node> assignment_expression logical_or_expression logical_and_expression equality_expression
%type <node> relational_expression additive_expression multiplicative_expression unary_expression
%type <node> postfix_expression primary_expression argument_list expression_list parameter_list
%type <node> parameter type_specifier declarator member_declaration_list member_declaration
%type <node> member_list member access_specifier

/* assignments */
%right ASSIGN PLUS_ASSIGN MINUS_ASSIGN MULT_ASSIGN DIV_ASSIGN MOD_ASSIGN

/* ternary */
%right '?' ':'

/* logical/bitwise/equality/relational/shift/add/mul*/
%left LOGICAL_OR
%left LOGICAL_AND
%left BITWISE_OR
%left BITWISE_XOR
%left BITWISE_AND
%left EQUAL NOT_EQUAL
%left LESS_THAN GREATER_THAN LESS_EQUAL GREATER_EQUAL
%left LEFT_SHIFT RIGHT_SHIFT
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
/* unary */
%right LOGICAL_NOT BITWISE_NOT UMINUS UPLUS PREFIX_INC PREFIX_DEC

/* postfix */
%left POSTFIX_INC POSTFIX_DEC DOT ARROW LPAREN LBRACKET

%%

program         : translation_unit { parse_tree = $1; };

translation_unit : external_declaration { $$ = $1; }
                | translation_unit external_declaration {
                    $$ = create_ast_node(NODE_PROGRAM, NULL, yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $2);
                };

external_declaration : function_definition { $$ = $1; }
                | function_declaration { $$ = $1; }
                | variable_declaration { $$ = $1; }
                | array_declaration { $$ = $1; }
                | struct_declaration { $$ = $1; }
                | class_declaration { $$ = $1; }
                | typedef_declaration { $$ = $1; };

variable_declaration:
    STRUCT IDENTIFIER IDENTIFIER SEMICOLON {
        $$ = create_ast_node(NODE_DECLARATION, $3, yylineno);
    }
    | type_specifier IDENTIFIER SEMICOLON {
        $$ = create_variable_declaration($1, create_identifier_declarator($2), NULL);
        if (current_scope) {
            const char* type_name = $1->data.type_specifier.type_name;
            data_type dt = get_type_from_string(type_name);
            symbol *sym = create_symbol($2, SYMBOL_VARIABLE, dt);
            insert_symbol(current_scope, sym);
        }
    }
    | type_specifier IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = create_variable_declaration($1, create_identifier_declarator($2), $4);
        if (current_scope) {
            const char* type_name = $1->data.type_specifier.type_name;
            data_type dt = get_type_from_string(type_name);
            symbol *sym = create_symbol($2, SYMBOL_VARIABLE, dt);
            sym->is_initialized = 1;
            insert_symbol(current_scope, sym);
        }
    }
    | STATIC type_specifier IDENTIFIER SEMICOLON {
        $$ = create_static_variable_declaration($2, create_identifier_declarator($3), NULL);
    }
    | STATIC type_specifier IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = create_static_variable_declaration($2, create_identifier_declarator($3), $5);
    }
    | type_specifier MULTIPLY IDENTIFIER SEMICOLON {
        $$ = create_variable_declaration($1, create_pointer_declarator(NULL, create_identifier_declarator($3)), NULL);
    }
    | type_specifier MULTIPLY IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = create_variable_declaration($1, create_pointer_declarator(NULL, create_identifier_declarator($3)), $5);
    }
    | type_specifier error SEMICOLON {
        yyerror("Invalid variable declaration");
        yyerrok;
        $$ = NULL;
    };

array_declaration:
    type_specifier IDENTIFIER LBRACKET INTEGER_LITERAL RBRACKET SEMICOLON {
        $$ = create_array_declaration($1, $2, $4, 1);
    }
    | type_specifier IDENTIFIER LBRACKET INTEGER_LITERAL RBRACKET ASSIGN LBRACE expression_list RBRACE SEMICOLON {
        $$ = create_array_declaration($1, $2, $4, 1);
    }
    | type_specifier IDENTIFIER LBRACKET INTEGER_LITERAL RBRACKET ASSIGN STRING_LITERAL SEMICOLON {
        $$ = create_array_declaration($1, $2, $4, 1);
    }
    | type_specifier IDENTIFIER LBRACKET INTEGER_LITERAL RBRACKET LBRACKET INTEGER_LITERAL RBRACKET SEMICOLON {
        $$ = create_multidimensional_array_declaration($1, $2, $4, $7);
    }
    | type_specifier IDENTIFIER LBRACKET RBRACKET SEMICOLON {
        yyerror("Array size not specified");
        $$ = NULL;
    }
    | type_specifier IDENTIFIER LBRACKET error RBRACKET SEMICOLON {
        yyerror("Invalid array size");
        yyerrok;
        $$ = NULL;
    };

function_declaration:
    type_specifier IDENTIFIER LPAREN parameter_list RPAREN SEMICOLON {
        $$ = create_function_declaration($1, $2, $4);
        if (current_scope) {
            symbol *sym = create_symbol($2, SYMBOL_FUNCTION, TYPE_FUNCTION);
            insert_symbol(current_scope, sym);
        }
    }
    | type_specifier IDENTIFIER LPAREN RPAREN SEMICOLON {
        $$ = create_function_declaration($1, $2, NULL);
        if (current_scope) {
            symbol *sym = create_symbol($2, SYMBOL_FUNCTION, TYPE_FUNCTION);
            insert_symbol(current_scope, sym);
        }
    }
    | type_specifier IDENTIFIER LPAREN error RPAREN SEMICOLON {
        yyerror("Invalid function parameter list");
        yyerrok;
        $$ = NULL;
    };

function_definition : type_specifier IDENTIFIER LPAREN parameter_list RPAREN
    {
        current_scope = create_child_scope(current_scope, $2);
    }
    compound_statement
    {
        current_scope = current_scope->parent;
        $$ = create_ast_node(NODE_FUNCTION, $2, yylineno);
        add_ast_child($$, $1);
        add_ast_child($$, $4);
        add_ast_child($$, $7);
        if (current_scope) {
            symbol *sym = create_symbol($2, SYMBOL_FUNCTION, TYPE_FUNCTION);
            sym->is_function_defined = 1;
            insert_symbol(current_scope, sym);
        }
    }
    | type_specifier IDENTIFIER LPAREN RPAREN
    {
        current_scope = create_child_scope(current_scope, $2);
    }
    compound_statement
    {
        current_scope = current_scope->parent;

        $$ = create_ast_node(NODE_FUNCTION, $2, yylineno);
        add_ast_child($$, $1);
        add_ast_child($$, $6);
        if (current_scope) {
            symbol *sym = create_symbol($2, SYMBOL_FUNCTION, TYPE_FUNCTION);
            sym->is_function_defined = 1;
            insert_symbol(current_scope, sym);
        }
    };
    | type_specifier IDENTIFIER SCOPE IDENTIFIER LPAREN parameter_list RPAREN
    compound_statement
    {
        // Rule for: type ClassName::FunctionName(params) { ... }
        // Create the node in the final action block.
        // All components are available here.
        // $1=type_specifier, $2=ClassName, $4=FunctionName, $6=param_list, $8=compound_statement
        ast_node* func_def_node = create_ast_node(NODE_SCOPED_FUNCTION_DEFINITION, NULL, yylineno);

        add_ast_child(func_def_node, $1);
        ast_node* scoped_name = create_ast_node(NODE_SCOPED_ACCESS, NULL, yylineno);
        add_ast_child(scoped_name, create_ast_node(NODE_IDENTIFIER, $2, yylineno));
        add_ast_child(scoped_name, create_ast_node(NODE_IDENTIFIER, $4, yylineno));
        add_ast_child(func_def_node, scoped_name);
        add_ast_child(func_def_node, $6);
        add_ast_child(func_def_node, $8);
        $$ = func_def_node;
    };

parameter_list: parameter { $$ = $1; }
    | parameter_list COMMA parameter {
        $$ = create_ast_node(NODE_LIST, NULL, yylineno);
        add_ast_child($$, $1);
        add_ast_child($$, $3);
    };

parameter : type_specifier declarator {
        $$ = create_ast_node(NODE_PARAMETER, NULL, yylineno);
        add_ast_child($$, $1);
        add_ast_child($$, $2);
    }
    | type_specifier {
        $$ = create_ast_node(NODE_PARAMETER, NULL, yylineno);
        add_ast_child($$, $1);
    };

declarator : IDENTIFIER {
        $$ = create_ast_node(NODE_IDENTIFIER, $1, yylineno);
    }
    | MULTIPLY declarator {
        $$ = create_ast_node(NODE_POINTER, NULL, yylineno);
        add_ast_child($$, $2);
    }
    | declarator LBRACKET INTEGER_LITERAL RBRACKET {
        $$ = create_ast_node(NODE_ARRAY, NULL, yylineno);
        add_ast_child($$, $1);
        add_ast_child($$, create_integer_literal($3));
    }
    | declarator LBRACKET RBRACKET {
        $$ = create_ast_node(NODE_ARRAY, NULL, yylineno);
        add_ast_child($$, $1);
    }
    | declarator LPAREN parameter_list RPAREN {
        $$ = create_ast_node(NODE_FUNCTION, NULL, yylineno);
        add_ast_child($$, $1);
        add_ast_child($$, $3);
    }
    | declarator LPAREN RPAREN {
        $$ = create_ast_node(NODE_FUNCTION, NULL, yylineno);
        add_ast_child($$, $1);
    }
    | LPAREN declarator RPAREN { $$ = $2; };

struct_declaration : STRUCT IDENTIFIER
    {
        current_scope = create_child_scope(current_scope, $2);
    }
    LBRACE member_list RBRACE SEMICOLON
    {
        current_scope = current_scope->parent;
        $$ = create_ast_node(NODE_STRUCT, $2, yylineno);
        add_ast_child($$, $5);
        if (current_scope) {
            symbol *sym = create_symbol($2, SYMBOL_STRUCT, TYPE_STRUCT);
            insert_symbol(current_scope, sym);
        }
    };

// === FIX 1: Added the missing member_list rule ===
member_list : member { $$ = $1; }
            | member_list member {
                $$ = create_ast_node(NODE_LIST, NULL, yylineno);
                add_ast_child($$, $1);
                add_ast_child($$, $2);
            };

member : type_specifier declarator SEMICOLON {
        $$ = create_ast_node(NODE_DECLARATION, NULL, yylineno);
        add_ast_child($$, $1);
        add_ast_child($$, $2);
    };

class_declaration:
    CLASS IDENTIFIER
    {
        current_scope = create_child_scope(current_scope, $2);
    }
    LBRACE member_declaration_list RBRACE SEMICOLON
    {
        current_scope = current_scope->parent;
        $$ = create_class_declaration($2, NULL, $5);
    }
    | CLASS IDENTIFIER COLON access_specifier IDENTIFIER LBRACE member_declaration_list RBRACE SEMICOLON {
        $$ = create_class_declaration($2, $5, $7);
    }
    | CLASS IDENTIFIER LBRACE member_declaration_list RBRACE {
        yyerror("Missing semicolon after class declaration");
        $$ = NULL;
    };

member_declaration_list:
    member_declaration { $$ = create_member_declaration_list($1); }
    | member_declaration_list member_declaration { $$ = append_member_declaration($1, $2); };

member_declaration:
    type_specifier IDENTIFIER SEMICOLON {
        $$ = create_member_declaration($1, create_identifier_declarator($2));
    }
    | access_specifier COLON { $$ = $1; }
    | function_definition { $$ = $1; };

access_specifier:
    PUBLIC { $$ = create_access_specifier("public"); }
    | PRIVATE { $$ = create_access_specifier("private"); }
    | PROTECTED { $$ = create_access_specifier("protected"); };

typedef_declaration:
    TYPEDEF type_specifier IDENTIFIER SEMICOLON {
        $$ = create_typedef_node($2, $3);
    }
    | TYPEDEF type_specifier error SEMICOLON {
        yyerror("Invalid typedef declaration");
        yyerrok;
        $$ = NULL;
    };

type_specifier  : INT { $$ = create_ast_node(NODE_TYPE, "int", yylineno); }
                | CHAR { $$ = create_ast_node(NODE_TYPE, "char", yylineno); }
                | FLOAT { $$ = create_ast_node(NODE_TYPE, "float", yylineno); }
                | DOUBLE { $$ = create_ast_node(NODE_TYPE, "double", yylineno); }
                | VOID { $$ = create_ast_node(NODE_TYPE, "void", yylineno); }
                | STRUCT IDENTIFIER { $$ = create_ast_node(NODE_TYPE, $2, yylineno); }
                | CLASS IDENTIFIER { $$ = create_ast_node(NODE_TYPE, $2, yylineno); }
                | struct_declaration { $$ = $1; }
                | class_declaration { $$ = $1; };

declaration     : variable_declaration { $$ = $1; }
                | array_declaration { $$ = $1; }
                | function_declaration { $$ = $1; }
                | struct_declaration { $$ = $1; }
                | class_declaration { $$ = $1; }
                | typedef_declaration { $$ = $1; };

statement       : expression_statement { $$ = $1; }
                | compound_statement { $$ = $1; }
                | selection_statement { $$ = $1; }
                | iteration_statement { $$ = $1; }
                | jump_statement { $$ = $1; }
                | declaration { $$ = $1; }
                | labeled_statement { $$ = $1; };

compound_statement : LBRACE
    {
        // Action 1: Enter a new scope. This is correct.
        current_scope = create_child_scope(current_scope, "block");
    }
    statement_list RBRACE
    {
        // Action 2: This is now the FINAL action for the rule.
        // It runs after the LBRACE, statement_list, and RBRACE are all parsed.

        // First, exit the scope.
        current_scope = current_scope->parent;

        // Now, it's safe to assign to $$ and build the AST node.
        // Note: statement_list is now '$3' because the first action counts as '$2'.
        $$ = create_ast_node(NODE_COMPOUND, NULL, yylineno);
        add_ast_child($$, $3);
    }
    | LBRACE RBRACE {
        // This rule for an empty block is correct as it is.
        current_scope = create_child_scope(current_scope, "empty_block");
        current_scope = current_scope->parent;
        $$ = create_ast_node(NODE_COMPOUND, NULL, yylineno);
    };

statement_list  : statement { $$ = $1; }
                | statement_list statement {
                    $$ = create_ast_node(NODE_LIST, NULL, yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $2);
                };

expression_statement : expression SEMICOLON {
                    $$ = create_ast_node(NODE_EXPRESSION_STMT, NULL, yylineno);
                    add_ast_child($$, $1);
                }
                | SEMICOLON {
                    $$ = create_ast_node(NODE_EXPRESSION_STMT, NULL, yylineno);
                };

/* Control structures */
/* non_if_statement = "others" (all statements that are NOT an if) */
non_if_statement
    : expression_statement           { $$ = $1; }
    | compound_statement             { $$ = $1; }
    | iteration_statement            { $$ = $1; }
    | jump_statement                 { $$ = $1; }
    | declaration                    { $$ = $1; }
    | labeled_statement              { $$ = $1; }
    ;

/* selection_statement delegates to matched/unmatched forms */
selection_statement
    : matched_stmt
    | unmatched_stmt
    ;

/* matched-stmt: if with an else already matched, or any non-if statement */
matched_stmt
    : IF LPAREN expression RPAREN matched_stmt ELSE matched_stmt
        {
            $$ = create_ast_node(NODE_IF, NULL, yylineno);
            /* $3 = condition, $5 = then, $7 = else */
            add_ast_child($$, $3);
            add_ast_child($$, $5);
            add_ast_child($$, $7);
        }
    | non_if_statement
        {
            $$ = $1;
        }
    ;

/* unmatched-stmt: an if that does not (yet) have an else attached */
unmatched_stmt
    : IF LPAREN expression RPAREN statement
        {
            /* if (cond) stmt   -- no else branch */
            $$ = create_ast_node(NODE_IF, NULL, yylineno);
            add_ast_child($$, $3);   /* cond */
            add_ast_child($$, $5);   /* then (stmt) */
            /* no else child added */
        }
    | IF LPAREN expression RPAREN matched_stmt ELSE unmatched_stmt
        {
            /* handles nested cases where else attaches to nearest if */
            $$ = create_ast_node(NODE_IF, NULL, yylineno);
            add_ast_child($$, $3);   /* cond */
            add_ast_child($$, $5);   /* then (matched_stmt) */
            add_ast_child($$, $7);   /* else (unmatched_stmt) */
        }
    ;

iteration_statement : WHILE LPAREN expression RPAREN statement {
                    $$ = create_ast_node(NODE_WHILE, NULL, yylineno);
                    add_ast_child($$, $3);
                    add_ast_child($$, $5);
                }
                | FOR LPAREN expression_statement expression_statement expression RPAREN statement {
                    $$ = create_ast_node(NODE_FOR, NULL, yylineno);
                    add_ast_child($$, $3);
                    add_ast_child($$, $4);
                    add_ast_child($$, $5);
                    add_ast_child($$, $7);
                }
                | FOR LPAREN expression_statement expression_statement RPAREN statement {
                    $$ = create_ast_node(NODE_FOR, NULL, yylineno);
                    add_ast_child($$, $3);
                    add_ast_child($$, $4);
                    add_ast_child($$, $6);
                }
                | DO statement WHILE LPAREN expression RPAREN SEMICOLON {
                    $$ = create_ast_node(NODE_WHILE, NULL, yylineno);
                    add_ast_child($$, $2);
                    add_ast_child($$, $5);
                }
                ;

jump_statement  : RETURN expression SEMICOLON {
                    $$ = create_ast_node(NODE_RETURN, NULL, yylineno);
                    add_ast_child($$, $2);
                }
                | RETURN SEMICOLON {
                    $$ = create_ast_node(NODE_RETURN, NULL, yylineno);
                }
                | BREAK SEMICOLON {
                    $$ = create_ast_node(NODE_BREAK, NULL, yylineno);
                }
                | CONTINUE SEMICOLON {
                    $$ = create_ast_node(NODE_CONTINUE, NULL, yylineno);
                }
                ;

labeled_statement : IDENTIFIER COLON statement {
                    $$ = create_ast_node(NODE_LABELED_STMT, $1, yylineno);
                    add_ast_child($$, $3);
                }
                | CASE INTEGER_LITERAL COLON statement {
                    $$ = create_ast_node(NODE_CASE, NULL, yylineno);
                    add_ast_child($$, create_integer_literal($2));
                    add_ast_child($$, $4);
                }
                | DEFAULT COLON statement {
                    $$ = create_ast_node(NODE_DEFAULT, NULL, yylineno);
                    add_ast_child($$, $3);
                }
                ;

/* Expressions */
expression      : assignment_expression { $$ = $1; }
                ;

assignment_expression : logical_or_expression { $$ = $1; }
                | IDENTIFIER ASSIGN assignment_expression {
                    $$ = create_ast_node(NODE_ASSIGNMENT, NULL, yylineno);
                    ast_node *id = create_ast_node(NODE_IDENTIFIER, $1, yylineno);
                    add_ast_child($$, id);
                    add_ast_child($$, $3);
                }
                | IDENTIFIER DOT IDENTIFIER ASSIGN assignment_expression {
                    $$ = create_ast_node(NODE_ASSIGNMENT, NULL, yylineno);
                    ast_node *member = create_ast_node(NODE_MEMBER_ACCESS, NULL, yylineno);
                    add_ast_child(member, create_ast_node(NODE_IDENTIFIER, $1, yylineno));
                    add_ast_child(member, create_ast_node(NODE_IDENTIFIER, $3, yylineno));
                    add_ast_child($$, member);
                    add_ast_child($$, $5);
                }
                | IDENTIFIER PLUS_ASSIGN assignment_expression {
                    $$ = create_ast_node(NODE_ASSIGNMENT, NULL, yylineno);
                    ast_node *id = create_ast_node(NODE_IDENTIFIER, $1, yylineno);
                    add_ast_child($$, id);
                    add_ast_child($$, $3);
                }
                | IDENTIFIER MINUS_ASSIGN assignment_expression {
                    $$ = create_ast_node(NODE_ASSIGNMENT, NULL, yylineno);
                    ast_node *id = create_ast_node(NODE_IDENTIFIER, $1, yylineno);
                    add_ast_child($$, id);
                    add_ast_child($$, $3);
                }
                | IDENTIFIER MULT_ASSIGN assignment_expression {
                    $$ = create_ast_node(NODE_ASSIGNMENT, NULL, yylineno);
                    ast_node *id = create_ast_node(NODE_IDENTIFIER, $1, yylineno);
                    add_ast_child($$, id);
                    add_ast_child($$, $3);
                }
                | IDENTIFIER DIV_ASSIGN assignment_expression {
                    $$ = create_ast_node(NODE_ASSIGNMENT, NULL, yylineno);
                    ast_node *id = create_ast_node(NODE_IDENTIFIER, $1, yylineno);
                    add_ast_child($$, id);
                    add_ast_child($$, $3);
                }
                ;

logical_or_expression : logical_and_expression { $$ = $1; }
                | logical_or_expression LOGICAL_OR logical_and_expression {
                    $$ = create_ast_node(NODE_BINOP, "||", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                ;

logical_and_expression : equality_expression { $$ = $1; }
                | logical_and_expression LOGICAL_AND equality_expression {
                    $$ = create_ast_node(NODE_BINOP, "&&", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                ;

equality_expression : relational_expression { $$ = $1; }
                | equality_expression EQUAL relational_expression {
                    $$ = create_ast_node(NODE_BINOP, "==", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | equality_expression NOT_EQUAL relational_expression {
                    $$ = create_ast_node(NODE_BINOP, "!=", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                ;

relational_expression : additive_expression { $$ = $1; }
                | relational_expression LESS_THAN additive_expression {
                    $$ = create_ast_node(NODE_BINOP, "<", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | relational_expression GREATER_THAN additive_expression {
                    $$ = create_ast_node(NODE_BINOP, ">", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | relational_expression LESS_EQUAL additive_expression {
                    $$ = create_ast_node(NODE_BINOP, "<=", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | relational_expression GREATER_EQUAL additive_expression {
                    $$ = create_ast_node(NODE_BINOP, ">=", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                ;

additive_expression : multiplicative_expression { $$ = $1; }
                | additive_expression PLUS multiplicative_expression {
                    $$ = create_ast_node(NODE_BINOP, "+", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | additive_expression MINUS multiplicative_expression {
                    $$ = create_ast_node(NODE_BINOP, "-", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                ;

multiplicative_expression : unary_expression { $$ = $1; }
                | multiplicative_expression MULTIPLY unary_expression {
                    $$ = create_ast_node(NODE_BINOP, "*", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | multiplicative_expression DIVIDE unary_expression {
                    $$ = create_ast_node(NODE_BINOP, "/", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | multiplicative_expression MODULO unary_expression {
                    $$ = create_ast_node(NODE_BINOP, "%", yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                ;

unary_expression : postfix_expression { $$ = $1; }
                | INCREMENT unary_expression %prec PREFIX_INC {
                    $$ = create_ast_node(NODE_UNARY, "++", yylineno);
                    add_ast_child($$, $2);
                }
                | DECREMENT unary_expression %prec PREFIX_DEC {
                    $$ = create_ast_node(NODE_UNARY, "--", yylineno);
                    add_ast_child($$, $2);
                }
                | MULTIPLY unary_expression {
                    $$ = create_ast_node(NODE_UNARY, "*", yylineno);
                    add_ast_child($$, $2);
                }
                | BITWISE_AND unary_expression {
                    $$ = create_ast_node(NODE_UNARY, "&", yylineno);
                    add_ast_child($$, $2);
                }
                | NEW type_specifier {
                    $$ = create_new_expression($2, NULL);
                }
                | NEW type_specifier LBRACKET expression RBRACKET {
                    $$ = create_new_expression($2, $4);
                }
                | DELETE unary_expression {
                    $$ = create_delete_expression($2);
                }
                | MALLOC LPAREN expression RPAREN {
                    $$ = create_malloc_call($3);
                }
                | FREE LPAREN expression RPAREN {
                    $$ = create_free_call($3);
                }
                | MULTIPLY error {
                    yyerror("Invalid use of dereference operator");
                    yyerrok;
                    $$ = NULL;
                }
                ;

postfix_expression : primary_expression { $$ = $1; }
                | postfix_expression LBRACKET expression RBRACKET {
                    $$ = create_ast_node(NODE_ARRAY_ACCESS, NULL, yylineno);
                    add_ast_child($$, $1);
                    add_ast_child($$, $3);
                }
                | postfix_expression LPAREN argument_list RPAREN {
                    $$ = create_function_call($1, $3);
                }
                | postfix_expression LPAREN RPAREN {
                    $$ = create_function_call($1, NULL);
                }
                | postfix_expression DOT IDENTIFIER {
                    $$ = create_member_access($1, $3, 0);
                }
                | postfix_expression ARROW IDENTIFIER {
                    $$ = create_member_access($1, $3, 1);
                }
                | postfix_expression INCREMENT %prec POSTFIX_INC {
                    $$ = create_unary_expression($1, "++", 0);
                }
                | postfix_expression DECREMENT %prec POSTFIX_DEC {
                    $$ = create_unary_expression($1, "--", 0);
                }
                | postfix_expression LPAREN error RPAREN {
                    yyerror("Invalid function call arguments");
                    yyerrok;
                    $$ = NULL;
                }
                | postfix_expression LBRACKET error RBRACKET {
                    yyerror("Invalid array index");
                    yyerrok;
                    $$ = NULL;
                }
                ;

primary_expression : IDENTIFIER {
                    $$ = create_ast_node(NODE_IDENTIFIER, $1, yylineno);
                }
                | IDENTIFIER SCOPE IDENTIFIER {
                    $$ = create_ast_node(NODE_SCOPED_ACCESS, NULL, yylineno);
                }
                | INTEGER_LITERAL {
                    $$ = create_integer_literal($1);
                }
                | FLOAT_LITERAL {
                    $$ = create_float_literal($1);
                }
                | CHAR_LITERAL {
                    $$ = create_char_literal($1);
                }
                | STRING_LITERAL {
                    $$ = create_string_literal($1);
                }
                | LPAREN expression RPAREN {
                    $$ = $2;
                }
                | THIS {
                    $$ = create_this_expression();
                }
                | LAMBDA LPAREN parameter_list RPAREN LBRACE statement_list RBRACE {
                    $$ = create_lambda_expression(NULL, $3, $6);
                }
                | LPAREN error RPAREN {
                    yyerror("Invalid expression in parentheses");
                    yyerrok;
                    $$ = NULL;
                }

argument_list: assignment_expression {
                $$ = create_argument_list($1);
            }
            | argument_list COMMA assignment_expression {
                $$ = append_argument($1, $3);
            }
            ;

expression_list: assignment_expression {
                $$ = create_argument_list($1);
            }
            | expression_list COMMA assignment_expression {
                $$ = append_argument($1, $3);
            }
            ;


%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s (near token: %s)\n", 
            yylineno, s, yytext);
    error_count++;
}