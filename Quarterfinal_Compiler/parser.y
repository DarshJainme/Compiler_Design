%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol_table.h"
#define MAX_TYPENAMES 100
static char* type_names[MAX_TYPENAMES];
static int type_name_count = 0;

TokenEntry *token_list = NULL;
int error_flag = 1; /* Assume success until an error is found */

extern int yylineno;

void add_typename(const char* name) {
    for (int i = 0; i < type_name_count; ++i) {
        if (strcmp(type_names[i], name) == 0) return; // already present
    }
    if (type_name_count < MAX_TYPENAMES) {
        type_names[type_name_count++] = strdup(name);
    }
}

int is_typename(const char* name) {
    for (int i = 0; i < type_name_count; ++i) {
        if (strcmp(type_names[i], name) == 0) return 1;
    }
    return 0;
}
extern int yylex();
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);
%}

%union {
    char *str;
}

/* --- Token Declarations --- */
%token <str> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF CHAR_LITERAL
%token <str> PTR_OP INC_OP DEC_OP LSHIFT_OP RSHIFT_OP LE_OP GE_OP EQ_OP NE_OP
%token <str> LAND_OP LOR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <str> SUB_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN
%token <str> XOR_ASSIGN OR_ASSIGN TYPE_NAME FILE_TYPE
%token <str> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <str> STRING CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID BOOL
%token <str> BOOL_TRUE BOOL_FALSE
%token <str> STRUCT UNION ENUM ELLIPSIS
%token <str> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN UNTIL
%token CATCH CLASS DELETE FINAL FRIEND NAMESPACE NEW OVERRIDE
%token PRIVATE PROTECTED PUBLIC THROW TRY USING VIRTUAL
%token SCOPE_OP

/* --- Type Declarations for Nonterminals --- */
%type <str> primary_expression postfix_expression unary_expression
%type <str> multiplicative_expression additive_expression shift_expression
%type <str> relational_expression equality_expression and_expression
%type <str> exclusive_or_expression inclusive_or_expression
%type <str> logical_and_expression logical_or_expression
%type <str> conditional_expression assignment_expression expression
%type <str> lambda_expression lambda_introducer lambda_declarator_opt
%type <str> lambda_capture_opt lambda_specifier_opt trailing_return_type_opt
%type <str> declaration declaration_specifiers type_specifier storage_class_specifier
%type <str> init_declarator init_declarator_list declarator direct_declarator pointer declarator_id
%type <str> struct_or_union_specifier struct_or_union struct_declaration_list struct_declaration
%type <str> specifier_qualifier_list struct_declarator_list struct_declarator type_qualifier
%type <str> class_specifier base_class_list access_specifier class_member_list class_member class_name
%type <str> struct_head

%left ',' 
%right '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
       LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
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
%right INC_OP DEC_OP


/* --- Precedence and Associativity --- */
%start translation_unit
%right ELSE
%error-verbose

%%

/* --- Grammar Rules --- */

primary_expression
    : IDENTIFIER                { $$ = $1; }
    | CONSTANT                  { $$ = $1; }
    | STRING_LITERAL            { $$ = $1; }
    | CHAR_LITERAL              { $$ = $1; }
    | BOOL_TRUE                 { $$ = "true"; }
    | BOOL_FALSE                { $$ = "false"; }
    | '(' expression ')'        { $$ = $2; }
    | lambda_expression          { $$ = $1; }
    ;

postfix_expression
    : primary_expression                                  { $$ = $1; }
    | postfix_expression '[' expression ']'               { $$ = $1; } /* Array access, simple pass-through for now */
    | postfix_expression '(' ')'                          { $$ = $1; } /* Function call, simple pass-through for now */
    | postfix_expression '(' argument_expression_list ')' { $$ = $1; } /* Function call, simple pass-through for now */
    | postfix_expression '.' IDENTIFIER                   { $$ = $1; } /* Struct member, simple pass-through for now */
    | postfix_expression PTR_OP IDENTIFIER                { $$ = $1; } /* Struct pointer member, simple pass-through for now */
    | postfix_expression INC_OP                           { $$ = $1; }
    | postfix_expression DEC_OP                           { $$ = $1; }
    ;

argument_expression_list
    : assignment_expression
    | argument_expression_list ',' assignment_expression
    ;

unary_expression
    : postfix_expression          { $$ = $1; }
    | INC_OP unary_expression     { $$ = $2; }
    | DEC_OP unary_expression     { $$ = $2; }
    | unary_operator cast_expression
    | SIZEOF unary_expression
    | SIZEOF '(' type_name ')'
    ;

unary_operator : '&' | '*' | '+' | '-' | '~' | '!' ;

cast_expression
    : unary_expression
    | '(' type_name ')' cast_expression
    ;

multiplicative_expression
    : cast_expression
    | multiplicative_expression '*' cast_expression
    | multiplicative_expression '/' cast_expression
    | multiplicative_expression '%' cast_expression
    ;

additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;

shift_expression
    : additive_expression
    | shift_expression LSHIFT_OP additive_expression
    | shift_expression RSHIFT_OP additive_expression
    ;

relational_expression
    : shift_expression
    | relational_expression '<' shift_expression
    | relational_expression '>' shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;

equality_expression
    : relational_expression
    | equality_expression EQ_OP relational_expression
    | equality_expression NE_OP relational_expression
    ;

and_expression
    : equality_expression
    | and_expression '&' equality_expression
    ;

exclusive_or_expression
    : and_expression
    | exclusive_or_expression '^' and_expression
    ;

inclusive_or_expression
    : exclusive_or_expression
    | inclusive_or_expression '|' exclusive_or_expression
    ;

logical_and_expression
    : inclusive_or_expression
    | logical_and_expression LAND_OP inclusive_or_expression
    ;

logical_or_expression
    : logical_and_expression
    | logical_or_expression LOR_OP logical_and_expression
    ;

conditional_expression
    : logical_or_expression
    | logical_or_expression '?' expression ':' conditional_expression
    ;

assignment_expression
    : conditional_expression
    | unary_expression assignment_operator assignment_expression
    ;

assignment_operator
    : '=' | MUL_ASSIGN | DIV_ASSIGN | MOD_ASSIGN | ADD_ASSIGN
    | SUB_ASSIGN | LSHIFT_ASSIGN | RSHIFT_ASSIGN | AND_ASSIGN
    | XOR_ASSIGN | OR_ASSIGN
    ;

expression
    : assignment_expression
    | expression ',' assignment_expression
    ;

lambda_expression
    : lambda_introducer lambda_declarator_opt compound_statement
      { $$ = strdup("[LAMBDA]"); }
    ;

/* The introducer: [ capture-list (optional) ] */
lambda_introducer
    : '[' lambda_capture_opt ']'
    ;

/* Simplified rule for what can be inside the capture clause */
lambda_capture_opt
    : /* empty */
    | assignment_expression
    ;

/* The declarator: (params) static -> T. The whole thing is optional. */
lambda_declarator_opt
    : /* empty */
    | '(' parameter_type_list ')' lambda_specifier_opt trailing_return_type_opt
    | '(' ')' lambda_specifier_opt trailing_return_type_opt
    ;

/* Optional specifiers (we're only including 'static' as requested) */
lambda_specifier_opt
    : /* empty */
    | STATIC
    ;

/* Optional trailing return type: -> type */
trailing_return_type_opt
    : /* empty */
    | PTR_OP type_name   /* Your lexer already defines PTR_OP as "->" */
    ;

constant_expression : conditional_expression ;

/* --- Declarations --- */

declaration
    : declaration_specifiers ';'
    | declaration_specifiers init_declarator_list ';'
    ;

declarator_id
    : class_name SCOPE_OP IDENTIFIER {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s::%s", $1, $3);
        $$ = strdup(buf);
      }
    | IDENTIFIER { $$ = $1; }
    ;

declaration_specifiers
    : storage_class_specifier                   { $$ = $1; }
    | storage_class_specifier declaration_specifiers {
          char buf[128];
          snprintf(buf, sizeof(buf), "%s %s", $1, $2);
          $$ = strdup(buf);
      }
    | type_specifier                            { $$ = $1; }
    | type_specifier declaration_specifiers     {
          char buf[128];
          snprintf(buf, sizeof(buf), "%s %s", $1, $2);
          $$ = strdup(buf);
      }
    | type_qualifier                            { $$ = $1; }
    | type_qualifier declaration_specifiers     {
          char buf[128];
          snprintf(buf, sizeof(buf), "%s %s", $1, $2);
          $$ = strdup(buf);
      }
    ;

init_declarator_list
    : init_declarator                         { add_token($1, $<str>0); }
    | init_declarator_list ',' init_declarator
    ;

init_declarator
    : declarator                  { $$ = $1; }
    | declarator '=' initializer  { $$ = $1; }
    ;

storage_class_specifier
    : TYPEDEF      { $$ = "typedef"; }
    | EXTERN       { $$ = "extern"; }
    | STATIC       { $$ = "static"; }
    | AUTO         { $$ = "auto"; }
    | REGISTER     { $$ = "register"; }
    ;

type_specifier
    : VOID         { $$ = "void"; }
    | STRING      { $$ = "string"; }
    | CHAR         { $$ = "char"; }
    | SHORT        { $$ = "short"; }
    | INT          { $$ = "int"; }
    | LONG         { $$ = "long"; }
    | FLOAT        { $$ = "float"; }
    | DOUBLE       { $$ = "double"; }
    | SIGNED       { $$ = "signed"; }
    | UNSIGNED     { $$ = "unsigned"; }
    | BOOL         { $$ = "bool"; }
    | FILE_TYPE    { $$ = "file"; }
    | struct_or_union_specifier { $$ = $1; }
    | class_specifier          { $$ = $1; }
    | enum_specifier
    | TYPE_NAME    { $$ = $1; }
    ;

/* New rule for the common prefix: "struct Student" */
struct_head
    : struct_or_union IDENTIFIER { 
          char buf[128];
          snprintf(buf, sizeof(buf), "%s %s", $1, $2);
          $$ = strdup(buf);
      }
    ;

/* Refactored specifier now uses the new "struct_head" rule */
struct_or_union_specifier
    /* Rule for a full DEFINITION: struct_head { ... } */
    : struct_head '{' struct_declaration_list '}' 
        { 
            add_typename($1); 
            add_token($1, "struct"); 
            $$ = $1; 
        }

    /* Rule for USAGE: struct_head */
    | struct_head { $$ = $1; }

    /* Rule for anonymous DEFINITION: struct { ... } (This rule has no head) */
    | struct_or_union '{' struct_declaration_list '}'
    ;

struct_or_union 
    : STRUCT       { $$ = "struct"; }
    | UNION        { $$ = "union"; }
    ;

struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;

struct_declaration
    : specifier_qualifier_list struct_declarator_list ';'
    ;

specifier_qualifier_list
    : type_specifier specifier_qualifier_list  { /* Handle complex types if needed */ }
    | type_specifier
    | type_qualifier specifier_qualifier_list
    | type_qualifier
    ;

struct_declarator_list
    : struct_declarator
    | struct_declarator_list ',' struct_declarator
    ;

struct_declarator
    : declarator
    | declarator ':' constant_expression
    | ':' constant_expression
    ;

class_specifier
    : CLASS class_name '{' class_member_list '}' { add_typename($2); add_token($2, "class"); $$ = $2; }
    | CLASS class_name ':' base_class_list '{' class_member_list '}' { add_typename($2); add_token($2, "class"); $$ = $2; }
    ;

base_class_list
    : access_specifier class_name
    | base_class_list ',' access_specifier class_name
    ;

access_specifier
    : PUBLIC
    | PRIVATE
    | PROTECTED
    ;


class_member_list
    : /* empty */
    | class_member_list class_member
    ;

class_member
    : access_specifier ':'
    | declaration
    | function_definition
    ;

class_name
    : IDENTIFIER { $$ = $1; }
    ;

enum_specifier
    : ENUM '{' enumerator_list '}'
    | ENUM IDENTIFIER '{' enumerator_list '}' { add_token($2, "enum"); }
    ;

enumerator_list : enumerator | enumerator_list ',' enumerator ;
enumerator : IDENTIFIER | IDENTIFIER '=' constant_expression ;
type_qualifier : CONST | VOLATILE ;

declarator
    : pointer direct_declarator {
          char buf[128];
          snprintf(buf, sizeof(buf), "%s%s", $1, $2);
          $$ = strdup(buf);
      }
    | direct_declarator { $$ = $1; }
    ;

direct_declarator
    : declarator_id                                       /* <<< CHANGED FROM IDENTIFIER */
    | '(' declarator ')'                                  { $$ = $2; }
    | direct_declarator '[' constant_expression ']'       
    | direct_declarator '[' ']'                           
    | direct_declarator '(' parameter_type_list ')'       { add_token($1, "PROCEDURE"); }
    | direct_declarator '(' identifier_list ')'           { add_token($1, "PROCEDURE"); }
    | direct_declarator '(' ')'                           { add_token($1, "PROCEDURE"); }
    ;

pointer
    : '*'                { $$ = "*"; }
    | '*' pointer        {
          char buf[32];
          snprintf(buf, sizeof(buf), "*%s", $2);
          $$ = strdup(buf);
      }
    ;

/* --- Other Rules (simplified for clarity) --- */

parameter_type_list: parameter_list | parameter_list ',' ELLIPSIS;
parameter_list: parameter_declaration | parameter_list ',' parameter_declaration;
parameter_declaration
    : declaration_specifiers declarator { add_token($2, $1); }
    | declaration_specifiers abstract_declarator
    | declaration_specifiers
    ;
identifier_list: IDENTIFIER | identifier_list ',' IDENTIFIER;
type_name: specifier_qualifier_list | specifier_qualifier_list abstract_declarator;
abstract_declarator: pointer | direct_abstract_declarator | pointer direct_abstract_declarator;
direct_abstract_declarator: '(' abstract_declarator ')' | '[' ']' | '[' constant_expression ']' | direct_abstract_declarator '[' ']' | direct_abstract_declarator '[' constant_expression ']';
initializer: assignment_expression | '{' initializer_list '}' | '{' initializer_list ',' '}';
initializer_list: initializer | initializer_list ',' initializer;

statement
    : labeled_statement
    | compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

labeled_statement
    : IDENTIFIER ':' statement
    | CASE constant_expression ':' statement
    | DEFAULT ':' statement
    ;

compound_statement
    : '{' '}'
    | '{' block_items '}'
    ;

block_items: block_item | block_items block_item | error { yyerrok; yyclearin; };
block_item: declaration | statement;
expression_statement: ';' | expression ';';

selection_statement
    : IF '(' expression ')' statement %prec ELSE
    | IF '(' expression ')' statement ELSE statement
    | SWITCH '(' expression ')' statement
    ;

iteration_statement
    : WHILE '(' expression ')' statement
    | UNTIL '(' expression ')' statement
    | DO statement WHILE '(' expression ')' ';'
    | FOR '(' expression_statement expression_statement ')' statement
    | FOR '(' expression_statement expression_statement expression ')' statement
    | FOR '(' declaration expression_statement ')' statement
    | FOR '(' declaration expression_statement expression ')' statement
    ;

jump_statement
    : GOTO IDENTIFIER ';'
    | CONTINUE ';'
    | BREAK ';'
    | RETURN ';'
    | RETURN expression ';'
    ;

translation_unit
    : external_declaration
    | translation_unit external_declaration
    ;

external_declaration
    : function_definition
    | declaration
    | statement
    | error { yyerrok; yyclearin; }
    ;

function_definition
    : declaration_specifiers declarator compound_statement { add_token($2, "PROCEDURE"); }
    ;

%%
/* --- User Code Section --- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s) {
    fprintf(stderr, "Syntax Error on line %d: %s near '%s'\n", yylineno, s, yytext);
    error_flag = 0; /* Set flag to indicate failure */
}

/* --- Symbol Table Implementation --- */
void add_token(char *token, char *type){
    if (!token || !type) return;

    /* strtok is not re-entrant and dangerous to use here.
       This custom loop is safer. */
    char* declarator = token;
    
    int pointer_level = 0;
    while (*declarator == '*') {
        pointer_level++;
        declarator++;
    }

    char base_name[100];
    int i = 0;
    while (*declarator && *declarator != '[') {
        base_name[i++] = *declarator;
        declarator++;
    }
    base_name[i] = '\0';

    int array_dim = 0;
    while (strstr(declarator, "[]")) {
        array_dim++;
        declarator += 2;
    }

    char formatted_type[200];
    if (pointer_level == 0 && array_dim == 0) {
        snprintf(formatted_type, sizeof(formatted_type), "%s", type);
    } else if (pointer_level > 0 && array_dim > 0) {
        snprintf(formatted_type, sizeof(formatted_type), "%s %d-dimensional array %d-level pointer", type, array_dim, pointer_level);
    } else if (pointer_level > 0) {
        snprintf(formatted_type, sizeof(formatted_type), "%s %d-level pointer", type, pointer_level);
    } else { // array_dim > 0
        snprintf(formatted_type, sizeof(formatted_type), "%s %d-dimensional array", type, array_dim);
    }

    if(strcmp(base_name, "PROCEDURE") != 0 && strcmp(base_name, "EMPTY") != 0){
        TokenEntry *entry = (TokenEntry *)malloc(sizeof(TokenEntry));
        entry->token = strdup(base_name);
        entry->token_type = strdup(formatted_type);
        entry->next = token_list;
        token_list = entry;
    }
}

struct TokenEntry* reverse_list(struct TokenEntry* head) {
    struct TokenEntry *prev = NULL, *current = head, *next = NULL;
    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

void print_tokens() {
    token_list = reverse_list(token_list);
    printf("\n--- Symbol Table ---\n");
    printf("------------------------------------------------------\n");
    printf("%-25s %-25s\n", "Token", "Type");
    printf("------------------------------------------------------\n");
    TokenEntry *entry = token_list;
    while (entry) {
        printf("%-25s %-25s\n", entry->token, entry->token_type);
        entry = entry->next;
    }
    printf("------------------------------------------------------\n");
}

void prepass_typenames(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char type[32], name[64];
        // Match class/struct/enum definitions
        if (sscanf(line, "class %63s", name) == 1) {
            char *p = strpbrk(name, "{:;\n");
            if (p) *p = '\0';
            add_typename(name);
        } else if (sscanf(line, "struct %63s", name) == 1) {
            char *p = strpbrk(name, "{:;\n");
            if (p) *p = '\0';
            add_typename(name);
        } else if (sscanf(line, "enum %63s", name) == 1) {
            char *p = strpbrk(name, "{:;\n");
            if (p) *p = '\0';
            add_typename(name);
        } else if (sscanf(line, "typedef %31s %63s", type, name) == 2) {
            char *p = strpbrk(name, ";\n");
            if (p) *p = '\0';
            add_typename(name);
        }
    }
    fclose(fp);
}


int main(int argc, char **argv) {
    if (argc > 1) {
        // // Pre-pass to collect type names
        // prepass_typenames(argv[1]);
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror(argv[1]);
            return 1;
        }
        yyin = file;
    }
    yyparse();
    if (error_flag) {
        printf("\nParsing successful.\n");
        print_tokens();
    } else {
        printf("\nParsing failed due to syntax errors.\n");
    }
    return 0;
}
