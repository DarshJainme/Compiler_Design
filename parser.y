%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "semantic.h"
#include "symbol_table.h"
#include "tac.h"

ASTNode *root = NULL;
int parse_errors = 0;

extern int yylineno;
extern int yylex();
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);

#define MAX_TYPENAMES 100
static char* type_names[MAX_TYPENAMES];
static int type_name_count = 0;

void add_typename(const char* name) {
    if (!name) return;
    for (int i = 0; i < type_name_count; ++i) {
        if (strcmp(type_names[i], name) == 0) return;
    }
    if (type_name_count < MAX_TYPENAMES) {
        type_names[type_name_count++] = strdup(name);
    }
}

int is_typename(const char* name) {
    if (!name) return 0;
    for (int i = 0; i < type_name_count; ++i) {
        if (strcmp(type_names[i], name) == 0) return 1;
    }
    return 0;
}

%}

%union {
    char *str;
    struct ASTNode *node;
    struct ASTNodeList *list;
    int token;
}

/* --- Token Declarations --- */
%token <str> IDENTIFIER CONSTANT STRING_LITERAL CHAR_LITERAL BOOL_TRUE BOOL_FALSE
%token <str> TYPE_NAME
%token SIZEOF PTR_OP INC_OP DEC_OP LSHIFT_OP RSHIFT_OP LE_OP GE_OP EQ_OP NE_OP
%token LAND_OP LOR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN 
%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token STRING CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID BOOL
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token CLASS DELETE FRIEND NAMESPACE NEW OVERRIDE
%token PRIVATE PROTECTED PUBLIC THROW TRY USING VIRTUAL
%token SCOPE_OP
%token CATCH FINAL UNTIL FILE_TYPE

/* --- Type Declarations for Nonterminals --- */
%type <node> program translation_unit external_declaration
%type <node> primary_expression postfix_expression unary_expression cast_expression
%type <node> multiplicative_expression additive_expression shift_expression
%type <node> relational_expression equality_expression and_expression
%type <node> exclusive_or_expression inclusive_or_expression
%type <node> logical_and_expression logical_or_expression conditional_expression
%type <node> assignment_expression expression constant_expression
%type <list> argument_expression_list
%type <token> assignment_operator
%type <node> new_expression delete_expression lambda_expression

%type <node> statement labeled_statement compound_statement expression_statement
%type <node> selection_statement iteration_statement jump_statement
%type <list> block_item_list
%type <node> block_item

%type <node> qualified_id
%type <list> nested_name_specifier

%type <node> declaration function_definition
%type <list> declaration_specifiers init_declarator_list
%type <node> init_declarator declarator direct_declarator ptr_operator
%type <node> storage_class_specifier type_specifier type_qualifier
%type <node> struct_or_union_specifier enum_specifier
%type <list> struct_declaration_list enumerator_list type_qualifier_list
%type <node> struct_declaration enumerator
%type <list> struct_declarator_list
%type <node> struct_declarator
%type <list> type_qualifier_list_opt
%type <node> declaration_specifier

%type <node> class_specifier class_name
%type <list> base_clause class_member_list
%type <node> base_specifier class_member access_specifier
%type <node> parameter_declaration type_name abstract_declarator direct_abstract_declarator
%type <list> parameter_list parameter_type_list
%type <node> initializer
%type <list> initializer_list

/* --- Precedence and Associativity --- */
%right '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%right '?' ':'
%left LOR_OP
%left LAND_OP
%left '|'
%left '^'
%left '&'
%left EQ_OP NE_OP
%left '<' '>' LE_OP GE_OP
%left LSHIFT_OP RSHIFT_OP
%left '+' '-'
%left '*' '/' '%'
%right NEW DELETE SIZEOF '!' '~' INC_OP DEC_OP // Unary operators
%left '.' PTR_OP '[' ']' '(' ')' SCOPE_OP
%right ELSE

%start program

%%

/* Top Level */
program
    : translation_unit { root = $1; }
    ;

translation_unit
    : external_declaration { $$ = create_node(NODE_TRANSLATION_UNIT); $$->data.items_list = create_list_node($1); }
    | translation_unit external_declaration { if ($2) { $1->data.items_list = append_to_list($1->data.items_list, $2); } $$ = $1; }
    ;

external_declaration
    : function_definition  { $$ = $1; }
    | declaration          { $$ = $1; }
    | expression_statement { $$ = $1; } // Allow top-level statements
    ;

function_definition
    : declaration_specifiers declarator compound_statement { $$ = create_function_definition_node($1, $2, $3); }
    ;

/* --- Expressions --- */
primary_expression
    : IDENTIFIER        { $$ = create_identifier_node($1); }
    | qualified_id      { $$ = $1; }
    | CONSTANT          { $$ = create_constant_node($1); }
    | STRING_LITERAL    { $$ = create_string_literal_node($1); }
    | CHAR_LITERAL      { $$ = create_constant_node($1); }
    | BOOL_TRUE         { $$ = create_constant_node($1); }
    | BOOL_FALSE        { $$ = create_constant_node($1); }
    | '(' expression ')'{ $$ = $2; }
    | lambda_expression { $$ = $1; }
    ;

postfix_expression
    : primary_expression
    | postfix_expression '[' expression ']'                { $$ = create_array_subscript_node($1, $3); }
    | postfix_expression '(' ')'                           { $$ = create_func_call_node($1, NULL); }
    | postfix_expression '(' argument_expression_list ')'  { $$ = create_func_call_node($1, $3); }
    | postfix_expression '.' IDENTIFIER                    { $$ = create_member_access_node($1, $3, 0); }
    | postfix_expression PTR_OP IDENTIFIER                 { $$ = create_member_access_node($1, $3, 1); }
    // for differentiating postfix and prefix part
    | postfix_expression INC_OP                            { $$ = create_unary_expr_node(INC_OP, $1); $$->type = NODE_POSTFIX_UNARY_EXPR; }
    | postfix_expression DEC_OP                            { $$ = create_unary_expr_node(DEC_OP, $1); $$->type = NODE_POSTFIX_UNARY_EXPR; }
    ;

argument_expression_list
    : assignment_expression
        { $$ = create_list_node($1); }
    | argument_expression_list ',' assignment_expression
        { $$ = append_to_list($1, $3); }
    ;
    
unary_expression
    : postfix_expression
    | INC_OP unary_expression      { $$ = create_unary_expr_node(INC_OP, $2); $$->type = NODE_PREFIX_UNARY_EXPR; }
    | DEC_OP unary_expression      { $$ = create_unary_expr_node(DEC_OP, $2); $$->type = NODE_PREFIX_UNARY_EXPR; }
    | '&' cast_expression { $$ = create_unary_expr_node('&', $2); }
    | '*' cast_expression { $$ = create_unary_expr_node('*', $2); }
    | '+' cast_expression { $$ = create_unary_expr_node('+', $2); }
    | '-' cast_expression { $$ = create_unary_expr_node('-', $2); }
    | '~' cast_expression { $$ = create_unary_expr_node('~', $2); }
    | '!' cast_expression { $$ = create_unary_expr_node('!', $2); }
    | SIZEOF unary_expression      { $$ = create_unary_expr_node(SIZEOF, $2); }
    | SIZEOF '(' type_name ')'     { $$ = create_unary_expr_node(SIZEOF, $3); }
    | new_expression               { $$ = $1; }
    | delete_expression            { $$ = $1; }
    ;

// unary_operator
//     : '&' { $$ = '&'; }
//     | '*' { $$ = '*'; }
//     | '+' { $$ = '+'; }
//     | '-' { $$ = '-'; }
//     | '~' { $$ = '~'; }
//     | '!' { $$ = '!'; }
//     ;

cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression { $$ = create_cast_expr_node($2, $4); }
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression { $$ = create_binary_expr_node($1, '*', $3); }
    | multiplicative_expression '/' cast_expression { $$ = create_binary_expr_node($1, '/', $3); }
    | multiplicative_expression '%' cast_expression { $$ = create_binary_expr_node($1, '%', $3); }
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression { $$ = create_binary_expr_node($1, '+', $3); }
    | additive_expression '-' multiplicative_expression { $$ = create_binary_expr_node($1, '-', $3); }
    ;

shift_expression
    : additive_expression
    | shift_expression LSHIFT_OP additive_expression { $$ = create_binary_expr_node($1, LSHIFT_OP, $3); }
    | shift_expression RSHIFT_OP additive_expression { $$ = create_binary_expr_node($1, RSHIFT_OP, $3); }
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression   { $$ = create_binary_expr_node($1, '<', $3); }
    | relational_expression '>' shift_expression   { $$ = create_binary_expr_node($1, '>', $3); }
    | relational_expression LE_OP shift_expression { $$ = create_binary_expr_node($1, LE_OP, $3); }
    | relational_expression GE_OP shift_expression { $$ = create_binary_expr_node($1, GE_OP, $3); }
    ;

equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression { $$ = create_binary_expr_node($1, EQ_OP, $3); }
    | equality_expression NE_OP relational_expression { $$ = create_binary_expr_node($1, NE_OP, $3); }
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression { $$ = create_binary_expr_node($1, '&', $3); }
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression { $$ = create_binary_expr_node($1, '^', $3); }
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression { $$ = create_binary_expr_node($1, '|', $3); }
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression LAND_OP inclusive_or_expression { $$ = create_binary_expr_node($1, LAND_OP, $3); }
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression LOR_OP logical_and_expression { $$ = create_binary_expr_node($1, LOR_OP, $3); }
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression { $$ = create_conditional_expr_node($1, $3, $5); }
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression { $$ = create_assignment_node($1, $2, $3); }
    ;

assignment_operator
    : '='           { $$ = '='; }
    | MUL_ASSIGN    { $$ = MUL_ASSIGN; }
    | DIV_ASSIGN    { $$ = DIV_ASSIGN; }
    | MOD_ASSIGN    { $$ = MOD_ASSIGN; }
    | ADD_ASSIGN    { $$ = ADD_ASSIGN; }
    | SUB_ASSIGN    { $$ = SUB_ASSIGN; }
    | LSHIFT_ASSIGN { $$ = LSHIFT_ASSIGN; }
    | RSHIFT_ASSIGN { $$ = RSHIFT_ASSIGN; }
    | AND_ASSIGN    { $$ = AND_ASSIGN; }
    | XOR_ASSIGN    { $$ = XOR_ASSIGN; }
    | OR_ASSIGN     { $$ = OR_ASSIGN; }
    ;

expression
    : assignment_expression
    | expression ',' assignment_expression { $$ = create_binary_expr_node($1, ',', $3); }
    ;

constant_expression
    : conditional_expression
    ;

// Add scope operator
new_expression
    : NEW type_name '(' ')' { $$ = create_new_expr_node($2, NULL); }
    | NEW type_name         { $$ = create_new_expr_node($2, NULL); }
    | NEW type_name initializer { $$ = create_new_expr_node($2, $3); }
    | SCOPE_OP NEW type_name '(' ')' { $$ = create_new_expr_node($3, NULL); /* Mark as global */ }
    | SCOPE_OP NEW type_name         { $$ = create_new_expr_node($3, NULL); /* Mark as global */ }
    | SCOPE_OP NEW type_name initializer { $$ = create_new_expr_node($3, $4); /* Mark as global */ }
    ;

delete_expression
    : DELETE cast_expression { $$ = create_delete_expr_node($2); }
    | SCOPE_OP DELETE cast_expression { $$ = create_delete_expr_node($3); /* Mark as global */ }
    ;

lambda_expression
    : '[' ']' '(' parameter_type_list ')' compound_statement { $$ = create_lambda_node(NULL, $4, $6); }
    | '[' ']' '(' ')' compound_statement                      { $$ = create_lambda_node(NULL, NULL, $5); }
    ;

/* --- Statements --- */
statement
    : labeled_statement    { $$ = $1; }
    | compound_statement   { $$ = $1; }
    | expression_statement { $$ = $1; }
    | selection_statement  { $$ = $1; }
    | iteration_statement  { $$ = $1; }
    | jump_statement       { $$ = $1; }
    ;

labeled_statement
    : IDENTIFIER ':' statement            { $$ = create_labeled_statement_node($1, $3); }
    | CASE constant_expression ':' statement { $$ = create_case_statement_node($2, $4); }
    | DEFAULT ':' statement                { $$ = create_default_statement_node($3); }
    ;

compound_statement
    : '{' '}'               { $$ = create_compound_statement_node(NULL); }
    | '{' block_item_list '}' { $$ = create_compound_statement_node($2); }
    ;

block_item_list
    : block_item
        { $$ = create_list_node($1); }
    | block_item_list block_item
        { $$ = append_to_list($1, $2); }
    ;

block_item
    : declaration { $$ = $1; }
    | statement   { $$ = $1; }
    ;

expression_statement
    : ';'              { $$ = create_expression_statement_node(NULL); }
    | expression ';'   { $$ = create_expression_statement_node($1); }
    ;

selection_statement
    : IF '(' expression ')' statement %prec ELSE   { $$ = create_if_statement_node($3, $5, NULL); }
    | IF '(' expression ')' statement ELSE statement { $$ = create_if_statement_node($3, $5, $7); }
    | SWITCH '(' expression ')' statement          { $$ = create_switch_statement_node($3, $5); }
    ;

iteration_statement
    : WHILE '(' expression ')' statement { $$ = create_while_statement_node($3, $5); }
    // add until keyword
    | UNTIL '(' expression ')' statement { $$ = create_until_statement_node($3, $5); }
    | DO statement WHILE '(' expression ')' ';' { $$ = create_do_while_statement_node($2, $5); }
    | FOR '(' expression_statement expression_statement ')' statement { $$ = create_for_statement_node($3, $4, NULL, $6); }
    | FOR '(' expression_statement expression_statement expression ')' statement { $$ = create_for_statement_node($3, $4, $5, $7); }
    | FOR '(' declaration expression_statement ')' statement { $$ = create_for_statement_node($3, $4, NULL, $6); }
    | FOR '(' declaration expression_statement expression ')' statement { $$ = create_for_statement_node($3, $4, $5, $7); }
    ;

jump_statement
    : GOTO IDENTIFIER ';'     { $$ = create_goto_statement_node($2); }
    | CONTINUE ';'            { $$ = create_node(NODE_CONTINUE_STATEMENT); }
    | BREAK ';'               { $$ = create_node(NODE_BREAK_STATEMENT); }
    | RETURN ';'              { $$ = create_return_statement_node(NULL); }
    | RETURN expression ';'   { $$ = create_return_statement_node($2); }
    ;

/* --- Declarations --- */
declaration
    : declaration_specifiers ';' { $$ = create_declaration_node($1, NULL); }
    | declaration_specifiers init_declarator_list ';' { $$ = create_declaration_node($1, $2); }
    | CLASS IDENTIFIER ';' { add_typename($2); $$ = create_class_node($2, NULL, NULL); }
    // we added this as we wanted to add forward declaration
    ;

declaration_specifiers
    : declaration_specifier                { $$ = create_list_node($1); }
    | declaration_specifiers declaration_specifier { $$ = append_to_list($1, $2); }
    ;

declaration_specifier
    : storage_class_specifier              { $$ = $1; }
    | type_specifier                       { $$ = $1; }
    | type_qualifier                       { $$ = $1; }
    | FRIEND                               { $$ = create_specifier_node(FRIEND); }
    ;

init_declarator_list
    : init_declarator
        { $$ = create_list_node($1); }
    | init_declarator_list ',' init_declarator
        { $$ = append_to_list($1, $3); }
    ;

init_declarator
    : declarator { $$ = create_init_declarator_node($1, NULL); }
    | declarator '=' initializer { $$ = create_init_declarator_node($1, $3); }
    ;

storage_class_specifier
    : TYPEDEF  { $$ = create_specifier_node(TYPEDEF); }
    | EXTERN   { $$ = create_specifier_node(EXTERN); }
    | STATIC   { $$ = create_specifier_node(STATIC); }
    | AUTO     { $$ = create_specifier_node(AUTO); }
    | REGISTER { $$ = create_specifier_node(REGISTER); }
    ;

type_specifier
    : VOID       { $$ = create_specifier_node(VOID); }
    | CHAR       { $$ = create_specifier_node(CHAR); }
    | SHORT      { $$ = create_specifier_node(SHORT); }
    | INT        { $$ = create_specifier_node(INT); }
    | LONG       { $$ = create_specifier_node(LONG); }
    | FLOAT      { $$ = create_specifier_node(FLOAT); }
    | DOUBLE     { $$ = create_specifier_node(DOUBLE); }
    | SIGNED     { $$ = create_specifier_node(SIGNED); }
    | UNSIGNED   { $$ = create_specifier_node(UNSIGNED); }
    | BOOL       { $$ = create_specifier_node(BOOL); }
    | STRING     { $$ = create_specifier_node(STRING); }
    | FILE_TYPE  { $$ = create_specifier_node(FILE_TYPE); }
    | struct_or_union_specifier { $$ = $1; }
    | enum_specifier            { $$ = $1; }
    | class_specifier           { $$ = $1; }
    | TYPE_NAME  { $$ = create_typename_node($1); }
    ;

type_qualifier
    : CONST    { $$ = create_specifier_node(CONST); }
    | VOLATILE { $$ = create_specifier_node(VOLATILE); }
    ;

struct_or_union_specifier
    : STRUCT IDENTIFIER '{' struct_declaration_list '}' { add_typename($2); $$ = create_struct_union_node(STRUCT, $2, $4); }
    | UNION IDENTIFIER '{' struct_declaration_list '}'  { add_typename($2); $$ = create_struct_union_node(UNION, $2, $4); }
    | STRUCT '{' struct_declaration_list '}'            { $$ = create_struct_union_node(STRUCT, NULL, $3); }
    | UNION '{' struct_declaration_list '}'             { $$ = create_struct_union_node(UNION, NULL, $3); }
    | STRUCT TYPE_NAME                                 { $$ = create_struct_union_node(STRUCT, $2, NULL); }
    | UNION TYPE_NAME                                 { $$ = create_struct_union_node(UNION, $2, NULL); }
    ;

struct_declaration_list
    : struct_declaration
        { $$ = create_list_node($1); }
    | struct_declaration_list struct_declaration
        { $$ = append_to_list($1, $2); }
    ;

struct_declaration
    : declaration_specifiers struct_declarator_list ';' { $$ = create_declaration_node($1, $2); }
    ;

struct_declarator_list
    : struct_declarator
        { $$ = create_list_node($1); }
    | struct_declarator_list ',' struct_declarator
        { $$ = append_to_list($1, $3); }
    ;

struct_declarator
    : declarator
    | declarator ':' constant_expression { $$ = $1; }
    ;

enum_specifier
    : ENUM '{' enumerator_list '}'              { $$ = create_enum_specifier_node(NULL, $3); }
    | ENUM IDENTIFIER '{' enumerator_list '}'   { add_typename($2); $$ = create_enum_specifier_node($2, $4); }
    | ENUM TYPE_NAME                           { $$ = create_enum_specifier_node($2, NULL); }
    ;

enumerator_list
    : enumerator
        { $$ = create_list_node($1); }
    | enumerator_list ',' enumerator
        { $$ = append_to_list($1, $3); }
    ;

enumerator
    : IDENTIFIER { $$ = create_identifier_node($1); }
    | IDENTIFIER '=' constant_expression { $$ = create_assignment_node(create_identifier_node($1), '=', $3); }
    ;

class_specifier
    : CLASS class_name base_clause '{' class_member_list '}' { add_typename($2->data.stringValue); $$ = create_class_node($2->data.stringValue, $3, $5); }
    ;

class_name
    : IDENTIFIER { $$ = create_identifier_node($1); }
    | TYPE_NAME { $$ = create_typename_node($1); }
    ;

base_clause
    : /* empty */ { $$ = NULL; }
    | ':' base_specifier { $$ = create_list_node($2); }
    | base_clause ',' base_specifier { $$ = append_to_list($1, $3); }
    ;
    
base_specifier
    : access_specifier class_name { $$ = create_base_class_node($1->data.specifier, $2); }
    | class_name                  { $$ = create_base_class_node(PRIVATE, $1); } // Default to private inheritance
    ;

access_specifier
    : PUBLIC    { $$ = create_access_specifier_node(PUBLIC); }
    | PRIVATE   { $$ = create_access_specifier_node(PRIVATE); }
    | PROTECTED { $$ = create_access_specifier_node(PROTECTED); }
    ;

class_member_list
    : /* empty */ { $$ = NULL; }
    | class_member_list class_member { $$ = $1 ? append_to_list($1, $2) : create_list_node($2); }
    ;

class_member
    : declaration
    | function_definition
    | access_specifier ':' { $$ = $1; }
    ;

// adding scope resolution
nested_name_specifier
    : IDENTIFIER SCOPE_OP                       { $$ = create_list_node(create_identifier_node($1)); }
    | TYPE_NAME SCOPE_OP                        { $$ = create_list_node(create_typename_node($1)); }
    | nested_name_specifier IDENTIFIER SCOPE_OP { $$ = append_to_list($1, create_identifier_node($2)); }
    | nested_name_specifier TYPE_NAME SCOPE_OP  { $$ = append_to_list($1, create_typename_node($2)); }
    ;

qualified_id
    : nested_name_specifier IDENTIFIER          { $$ = create_qualified_id_node($1, create_identifier_node($2)); }
    | nested_name_specifier TYPE_NAME           { $$ = create_qualified_id_node($1, create_typename_node($2)); }
    ;
/* --- Declarators --- */
declarator
    : ptr_operator declarator                   { 
                                                    // Check what kind of node the ptr_operator created
                                                    if ($1->type == NODE_POINTER) {
                                                        $$ = create_pointer_declarator_node($1, $2);
                                                    } else { // It must be a reference
                                                        $$ = create_reference_declarator_node($2);
                                                        free($1); // Free the temporary specifier node
                                                    }
                                                }
    // | '&' direct_declarator     { $$ = create_pointer_declarator_node(create_specifier_node('&'), $2); } /* C++ Reference handled separately */
    | direct_declarator
    ;

direct_declarator
    : IDENTIFIER                                { $$ = create_identifier_node($1); }
    | qualified_id                              { $$ = $1; }
    | '(' declarator ')'                        { $$ = $2; }
    | direct_declarator '[' constant_expression ']' { $$ = create_array_declarator_node($1, $3); }
    | direct_declarator '[' ']'                 { $$ = create_array_declarator_node($1, NULL); }
    | direct_declarator '(' parameter_type_list ')' { $$ = create_function_declarator_node($1, $3); }
    | direct_declarator '(' ')'                 { $$ = create_function_declarator_node($1, NULL); }
    ;

// adding the & too
ptr_operator
    : '*' type_qualifier_list_opt        { $$ = create_pointer_node($2, NULL); }
    | '&' type_qualifier_list_opt        { $$ = create_specifier_node('&'); }
    ;

type_qualifier_list_opt
    : /* empty */           { $$ = NULL; }
    | type_qualifier_list   { $$ = $1; }
    ;

type_qualifier_list
    : type_qualifier
        { $$ = create_list_node($1); }
    | type_qualifier_list type_qualifier
        { $$ = append_to_list($1, $2); }
    ;

parameter_type_list
    : parameter_list
    | parameter_list ',' ELLIPSIS { $$ = $1; }
    ;

parameter_list
    : parameter_declaration
        { $$ = create_list_node($1); }
    | parameter_list ',' parameter_declaration
        { $$ = append_to_list($1, $3); }
    ;

parameter_declaration
    : declaration_specifiers declarator
        { $$ = create_parameter_declaration_node($1, $2); }
    | declaration_specifiers abstract_declarator
        { $$ = create_parameter_declaration_node($1, $2); }
    | declaration_specifiers
        { $$ = create_parameter_declaration_node($1, NULL); }
    ;

type_name
    : declaration_specifiers abstract_declarator { $$ = create_declaration_node($1, create_list_node($2)); }
    | declaration_specifiers                    { $$ = create_declaration_node($1, NULL); }
    ;

abstract_declarator
    : ptr_operator
    | ptr_operator direct_abstract_declarator { $$ = create_pointer_declarator_node($1, $2); }
    | direct_abstract_declarator
    ;

direct_abstract_declarator
    : '(' abstract_declarator ')'   { $$ = $2; }
    | '[' ']'                       { $$ = create_array_declarator_node(NULL, NULL); }
    | '[' constant_expression ']'   { $$ = create_array_declarator_node(NULL, $2); }
    | direct_abstract_declarator '[' ']' { $$ = create_array_declarator_node($1, NULL); }
    | direct_abstract_declarator '[' constant_expression ']' { $$ = create_array_declarator_node($1, $3); }
    | '(' ')'                       { $$ = create_function_declarator_node(NULL, NULL); }
    | '(' parameter_type_list ')'   { $$ = create_function_declarator_node(NULL, $2); }
    | direct_abstract_declarator '(' ')' { $$ = create_function_declarator_node($1, NULL); }
    | direct_abstract_declarator '(' parameter_type_list ')' { $$ = create_function_declarator_node($1, $3); }
    ;

initializer
    : assignment_expression
    | '{' initializer_list '}'       { $$ = create_node(NODE_INITIALIZER_LIST); $$->data.items_list = $2; }
    | '{' initializer_list ',' '}'   { $$ = create_node(NODE_INITIALIZER_LIST); $$->data.items_list = $2; }
    ;

initializer_list
    : initializer
        { $$ = create_list_node($1); }
    | initializer_list ',' initializer
        { $$ = append_to_list($1, $3); }
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error on line %d near '%s': %s\n", yylineno, yytext, s);
    parse_errors++;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    yyparse();

    if (parse_errors == 0) {
        printf("\n--- Parsing Successful ---\n");
            printf("\n--- Abstract Syntax Tree ---\n\n");
            if (root) print_ast(root, 0);
        printf("\n--- Performing Semantic Analysis ---\n");
        if (analyze_ast(root)) {
            printf("--- Semantic Analysis Successful ---\n");
            
            printf("\n--- Generating Three-Address Code ---\n");
            generate_tac(root);
            print_tac();
            // print_symbol_table(current_scope, 0);

        } else {
            printf("\n--- Semantic Analysis Failed ---\n");
        }
    } else {
        printf("\n--- Parsing Failed ---\n");
    }
    
    if (yyin != stdin) {
        fclose(yyin);
    }
    
    return (parse_errors > 0);
}
