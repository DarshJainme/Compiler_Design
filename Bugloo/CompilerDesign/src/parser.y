%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylex();
    extern int yylineno;
    void yyerror(const char *s);
%}

/* Declare tokens from lexer */
%token IDENTIFIER NUMBER
%token OP_LESS_THAN OP_GREATER_THAN COMMA
%token KEYWORD_TYPEDEF KEYWORD_CLASS KEYWORD_ENUM KEYWORD_NAMESPACE

/* You can also alias tokens if lexer and grammar names differ */
%token OP_LESS_THAN OP_GREATER_THAN
%left OP_LESS_THAN OP_GREATER_THAN

%%

/* Start rule */
translation_unit
    : declaration_list
    ;

declaration_list
    : declaration_list declaration
    | declaration
    ;

/* Very simplified example declarations */
declaration
    : typedef_declaration
    | class_declaration
    | enum_declaration
    ;

typedef_declaration
    : KEYWORD_TYPEDEF typedef_name ';'
    ;

class_declaration
    : KEYWORD_CLASS class_name '{' '}'
    ;

enum_declaration
    : KEYWORD_ENUM enum_name '{' '}'
    ;

/* === NAMES === */
typedef_name
    : IDENTIFIER
    | template_id
    ;

class_name
    : IDENTIFIER
    | template_id
    ;

enum_name
    : IDENTIFIER
    ;

/* === TEMPLATE === */
template_id
    : IDENTIFIER OP_LESS_THAN template_argument_list OP_GREATER_THAN
    ;

template_argument_list
    : template_argument
    | template_argument_list COMMA template_argument
    ;

template_argument
    : IDENTIFIER
    | NUMBER
    | template_id
    ;

%%

/* Error handler */
void yyerror(const char *s) {
    fprintf(stderr, "Parse Error at line %d: %s\n", yylineno, s);
}
