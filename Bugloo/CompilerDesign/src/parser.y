%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylex();
    extern int yylineno;
    extern FILE *yyin;
    void yyerror(const char *s);
%}

/* Tell bison to generate the header (use -d flag when running bison). */

%token KEYWORD_INT KEYWORD_CHAR KEYWORD_FLOAT KEYWORD_DOUBLE KEYWORD_VOID
%token KEYWORD_IF KEYWORD_ELSE KEYWORD_FOR KEYWORD_WHILE KEYWORD_RETURN
%token KEYWORD_BREAK KEYWORD_CONTINUE KEYWORD_DO KEYWORD_SWITCH KEYWORD_CASE
%token KEYWORD_DEFAULT KEYWORD_GOTO KEYWORD_ENUM KEYWORD_STRUCT KEYWORD_UNION
%token KEYWORD_TYPEDEF KEYWORD_STATIC KEYWORD_AUTO KEYWORD_CONST KEYWORD_SIZEOF
%token KEYWORD_CLASS KEYWORD_PUBLIC KEYWORD_PRIVATE KEYWORD_PROTECTED
%token KEYWORD_NEW KEYWORD_DELETE KEYWORD_USING KEYWORD_NAMESPACE KEYWORD_BOOL
%token KEYWORD_LONG KEYWORD_TRUE KEYWORD_FALSE

%token IDENTIFIER INTEGER_LITERAL FLOAT_LITERAL CHAR_LITERAL STRING_LITERAL

%token OP_PLUS OP_MINUS OP_MULTIPLY OP_DIVIDE OP_MODULO
%token OP_INCREMENT OP_DECREMENT OP_ASSIGN OP_ADD_ASSIGN OP_SUB_ASSIGN
%token OP_MUL_ASSIGN OP_DIV_ASSIGN OP_MOD_ASSIGN OP_LSHIFT OP_RSHIFT
%token OP_EQUAL OP_NOT_EQUAL OP_LESS_THAN OP_GREATER_THAN
%token OP_LESS_THAN_EQUAL OP_GREATER_THAN_EQUAL OP_LOGICAL_AND OP_LOGICAL_OR
%token OP_LOGICAL_NOT OP_BITWISE_AND OP_BITWISE_OR OP_BITWISE_XOR OP_BITWISE_NOT
%token OP_AND OP_OR OP_XOR OP_NOT  /* in case lexer uses these synonyms */

%token SEMICOLON COMMA OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token OPEN_BRACKET CLOSE_BRACKET OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET
%token COLON POINTER_ARROW DOT REFERENCE_AMPERSAND POINTER_STAR QUESTION_MARK

%token PREPROCESSOR_DIRECTIVE SINGLE_LINE_COMMENT MULTI_LINE_COMMENT
%token NEWLINE WHITESPACE ERROR_TOKEN UNKNOWN END_OF_FILE

%%

/* Start rule */
translation_unit
    : declaration_list
    ;

declaration_list
    : declaration_list declaration
    | declaration
    ;

declaration
    : typedef_declaration
    | class_declaration
    | enum_declaration
    ;

typedef_declaration
    : KEYWORD_TYPEDEF typedef_name SEMICOLON
    ;

class_declaration
    : KEYWORD_CLASS class_name OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET
    ;

enum_declaration
        : KEYWORD_ENUM enum_name OPEN_CURLY_BRACKET CLOSE_CURLY_BRACKET
    ;
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

template_id
    : IDENTIFIER OP_LESS_THAN template_argument_list OP_GREATER_THAN
    ;

template_argument_list
    : template_argument
    | template_argument_list COMMA template_argument
    ;

template_argument
    : IDENTIFIER
    | INTEGER_LITERAL
    | template_id
    ;

%%

/* Error handler */
void yyerror(const char *s) {
    fprintf(stderr, "Parse Error at line %d: %s\n", yylineno, s);
}

/* simple driver */
int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("fopen");
            return 1;
        }
    } else {
        yyin = stdin;
    }
    yyparse();
    if (yyin && yyin != stdin) fclose(yyin);
    return 0;
}