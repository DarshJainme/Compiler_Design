%{
    #include <iostream>
    #include <string>
    #include "tokens.h"

    // Counter for errors
    int num_errors = 0;

    // Helper function -> print tokens in a table format
    void print_token(TokenType type){
        // yytext is the global character pointer holding the matched lexeme.
        printf("%-25s %s\n", yytext, token_names[type]);
    }
    // Helper function -> report errors
    void report_error(const char* lexeme){
        fprintf(stderr, "Lexical error at line no - %d : Invalid token '%s'\n\n", yylineno, lexeme);
        num_errors++;
    }
%}

%x COMMENT_STATE

/* Regex Definitions for Readability */
DIGIT [0-9]
LETTER [a-zA-Z]
IDENTIFIER {LETTER}({LETTER}|{DIGIT}|_)*
INTEGER_LITERAL {DIGIT}+
FLOAT_LITERAL {DIGIT}+\.{DIGIT}*([exE][+-]?{DIGIT}+)?|\.{DIGIT}+([eE][+-]?{DIGIT}+)?|{DIGIT}+[eE][+-]?{DIGIT}+
CHAR_LITERAL '(\\.|[^'\\])'
STRING_LITERAL \"(\\.|[^"\\])*\"
WHITESPACE [ \t\r]+
NEWLINE \n


%%
/* Rules Section */

{WHITESPACE} { /* Ignore */ }
{NEWLINE} { /* yylineno is automatically incremented by flex */ }

"//"([^\n])*    { /* Ignore single-line comments */ }
"/*"            { BEGIN(COMMENT_STATE); }
<COMMENT_STATE>"*/" { BEGIN(INITIAL); }
<COMMENT_STATE>.   { /* Ignore everything inside the comment state */ }
<COMMENT_STATE>\n  { /* Also ignore newlines inside the comment */ }
<COMMENT_STATE><<EOF>> { 
    fprintf(stderr, "Lexical Error at line %d: Unterminated multi-line comment\n", yylineno);
    num_errors++;
}

/* Preprocessor Directives */
^"#"([^\n])*    { print_token(PREPROCESSOR_DIRECTIVE); }

/* Keywords (before IDENTIFIER) */
"int"           { print_token(KEYWORD_INT); }
"char"          { print_token(KEYWORD_CHAR); }
"float"         { print_token(KEYWORD_FLOAT); }
"double"        { print_token(KEYWORD_DOUBLE); }
"void"          { print_token(KEYWORD_VOID); }
"if"            { print_token(KEYWORD_IF); }
"else"          { print_token(KEYWORD_ELSE); }
"for"           { print_token(KEYWORD_FOR); }
"while"         { print_token(KEYWORD_WHILE); }
"return"        { print_token(KEYWORD_RETURN); }
"break"         { print_token(KEYWORD_BREAK); }
"continue"      { print_token(KEYWORD_CONTINUE); }
"do"            { print_token(KEYWORD_DO); }
"switch"        { print_token(KEYWORD_SWITCH); }
"case"          { print_token(KEYWORD_CASE); }
"default"       { print_token(KEYWORD_DEFAULT); }
"goto"          { print_token(KEYWORD_GOTO); }
"enum"          { print_token(KEYWORD_ENUM); }
"struct"        { print_token(KEYWORD_STRUCT); }
"union"         { print_token(KEYWORD_UNION); }
"typedef"       { print_token(KEYWORD_TYPEDEF); }
"static"        { print_token(KEYWORD_STATIC); }
"auto"          { print_token(KEYWORD_AUTO); }
"const"         { print_token(KEYWORD_CONST); }
"sizeof"        { print_token(KEYWORD_SIZEOF); }
"class"         { print_token(KEYWORD_CLASS); }
"public"        { print_token(KEYWORD_PUBLIC); }
"private"       { print_token(KEYWORD_PRIVATE); }
"protected"     { print_token(KEYWORD_PROTECTED); }
"new"           { print_token(KEYWORD_NEW); }
"delete"        { print_token(KEYWORD_DELETE); }

/* Literals */
{INTEGER_LITERAL} { print_token(INTEGER_LITERAL); }
{FLOAT_LITERAL}   { print_token(FLOAT_LITERAL); }
{CHAR_LITERAL}    { print_token(CHAR_LITERAL); }
{STRING_LITERAL}  { print_token(STRING_LITERAL); }
{IDENTIFIER}      { print_token(IDENTIFIER); }

/* Operators */
"++"            { print_token(OP_INCREMENT); }
"--"            { print_token(OP_DECREMENT); }
"->"            { print_token(POINTER_ARROW); }
"+"             { print_token(OP_PLUS); }
"-"             { print_token(OP_MINUS); }
"*"             { print_token(OP_MULTIPLY); } /* Context (pointer vs multiply) is for the parser */
"/"             { print_token(OP_DIVIDE); }
"%"             { print_token(OP_MODULO); }
"+="            { print_token(OP_ADD_ASSIGN); }
"-="            { print_token(OP_SUB_ASSIGN); }
"*="            { print_token(OP_MUL_ASSIGN); }
"/="            { print_token(OP_DIV_ASSIGN); }
"%="            { print_token(OP_MOD_ASSIGN); }
"=="            { print_token(OP_EQUAL); }
"!="            { print_token(OP_NOT_EQUAL); }
"<="            { print_token(OP_LESS_THAN_EQUAL); }
">="            { print_token(OP_GREATER_THAN_EQUAL); }
"<"             { print_token(OP_LESS_THAN); }
">"             { print_token(OP_GREATER_THAN); }
"&&"            { print_token(OP_LOGICAL_AND); }
"||"            { print_token(OP_LOGICAL_OR); }
"!"             { print_token(OP_LOGICAL_NOT); }
"&"             { print_token(OP_BITWISE_AND); } /* Context (ref vs bitwise) is for the parser */
"|"             { print_token(OP_BITWISE_OR); }
"^"             { print_token(OP_BITWISE_XOR); }
"~"             { print_token(OP_BITWISE_NOT); }
"<<"            { print_token(OP_LSHIFT); }
">>"            { print_token(OP_RSHIFT); }
"="             { print_token(OP_ASSIGN); }

/* Punctuations */
";"             { print_token(SEMICOLON); }
","             { print_token(COMMA); }
"("             { print_token(OPEN_PARENTHESIS); }
")"             { print_token(CLOSE_PARENTHESIS); }
"{"             { print_token(OPEN_CURLY_BRACKET); }
"}"             { print_token(CLOSE_CURLY_BRACKET); }
"["             { print_token(OPEN_BRACKET); }
"]"             { print_token(CLOSE_BRACKET); }
":"             { print_token(COLON); }
"."             { print_token(DOT); }

/* Error Handling Rule */
.               { report_error(yytext); }

%%