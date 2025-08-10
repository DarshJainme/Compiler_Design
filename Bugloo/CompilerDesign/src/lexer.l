%{
    #include <stdio.h>
    #include <string.h>
    #include <limits.h>
    #include <stdlib.h>
    #include "tokens.h"

    // Counter for errors
    int num_errors = 0;

    // Declare external variables
    extern char* yytext;
    extern int yylineno;
    extern FILE* yyin;

    // Helper function to check integer overflow
    int check_integer_overflow(const char* str) {
        char* endptr;
        long val = strtol(str, &endptr, 10);
        return (val > INT_MAX || val < INT_MIN || *endptr != '\0');
    }

    // Helper function to check identifier length
    int check_identifier_length(const char* str) {
        return strlen(str) > 255; // Common limit for identifiers
    }
%}

%option yylineno
%option noyywrap

%x COMMENT_STATE

DIGIT [0-9]
LETTER [a-zA-Z_]
IDENTIFIER {LETTER}({LETTER}|{DIGIT})*
INTEGER_LITERAL {DIGIT}+
CHAR_LITERAL '(\\.|[^'\\])'
EXPONENTIAL_PART [eE][+-]?{DIGIT}+
FLOAT_LITERAL ({DIGIT}+\.{DIGIT}*{EXPONENTIAL_PART}?|\.{DIGIT}+{EXPONENTIAL_PART}?|{DIGIT}+{EXPONENTIAL_PART})
STRING_LITERAL \"(\\.|[^\"\\])*\"
WHITESPACE [ \t\r]+
NEWLINE \n

INVALID_IDENTIFIER_START {DIGIT}({LETTER}|{DIGIT})*
INVALID_NUMBER {DIGIT}+[a-zA-Z_]+{DIGIT}*
INVALID_CHAR_IN_NUMBER {DIGIT}+[$@#%&*!^~`]({DIGIT}|[a-zA-Z_])*

%%

{WHITESPACE}    { /* Ignore */ }
{NEWLINE}       { /* yylineno is automatically incremented by flex */ }

"/*"                            { BEGIN(COMMENT_STATE); }
<COMMENT_STATE>"*/"             { BEGIN(INITIAL); }
<COMMENT_STATE>.                { /* Ignore everything inside the comment state */ }
<COMMENT_STATE>\n               { /* Also ignore newlines inside the comment */ }
<COMMENT_STATE><<EOF>>          {
    fprintf(stderr, "Lexical Error at line %d: Unterminated multi-line comment\n", yylineno);
    num_errors++;
}

"//"[^\n]*                      { /* Ignore single-line comments */ }

^"#"[^\n]*                      { return PREPROCESSOR_DIRECTIVE; }

"int"           { return KEYWORD_INT; }
"char"          { return KEYWORD_CHAR; }
"float"         { return KEYWORD_FLOAT; }
"double"        { return KEYWORD_DOUBLE; }
"void"          { return KEYWORD_VOID; }
"if"            { return KEYWORD_IF; }
"else"          { return KEYWORD_ELSE; }
"for"           { return KEYWORD_FOR; }
"while"         { return KEYWORD_WHILE; }
"return"        { return KEYWORD_RETURN; }
"break"         { return KEYWORD_BREAK; }
"continue"      { return KEYWORD_CONTINUE; }
"do"            { return KEYWORD_DO; }
"switch"        { return KEYWORD_SWITCH; }
"case"          { return KEYWORD_CASE; }
"default"       { return KEYWORD_DEFAULT; }
"goto"          { return KEYWORD_GOTO; }
"enum"          { return KEYWORD_ENUM; }
"struct"        { return KEYWORD_STRUCT; }
"union"         { return KEYWORD_UNION; }
"typedef"       { return KEYWORD_TYPEDEF; }
"static"        { return KEYWORD_STATIC; }
"auto"          { return KEYWORD_AUTO; }
"const"         { return KEYWORD_CONST; }
"sizeof"        { return KEYWORD_SIZEOF; }
"class"         { return KEYWORD_CLASS; }
"public"        { return KEYWORD_PUBLIC; }
"private"       { return KEYWORD_PRIVATE; }
"protected"     { return KEYWORD_PROTECTED; }
"new"           { return KEYWORD_NEW; }
"delete"        { return KEYWORD_DELETE; }
"using"         { return KEYWORD_USING; }
"namespace"     { return KEYWORD_NAMESPACE; }
"bool"          { return KEYWORD_BOOL; }
"long"          { return KEYWORD_LONG; }
"true"          { return KEYWORD_TRUE; }
"false"         { return KEYWORD_FALSE; }

{INTEGER_LITERAL}               { 
    if (check_integer_overflow(yytext)) {
        fprintf(stderr, "Lexical Error at line %d: Integer constant '%s' exceeds maximum value\n", yylineno, yytext);
        num_errors++;
        return ERROR_TOKEN;
    }
    return INTEGER_LITERAL; 
}
{FLOAT_LITERAL}                 { return FLOAT_LITERAL; }
{CHAR_LITERAL}                  { return CHAR_LITERAL; }
{STRING_LITERAL}                { return STRING_LITERAL; }
\"([^\"\\\\]|\\\\.)*$            {
    fprintf(stderr, "Lexical Error at line %d: Unterminated string literal\n", yylineno);
    num_errors++;
    return ERROR_TOKEN;
}
'[^']*$                         {
    fprintf(stderr, "Lexical Error at line %d: Unterminated character literal\n", yylineno);
    num_errors++;
    return ERROR_TOKEN;
}

{INVALID_IDENTIFIER_START}      {
    fprintf(stderr, "Lexical Error at line %d: Invalid identifier '%s' - identifiers cannot start with a digit\n", yylineno, yytext);
    num_errors++;
    return ERROR_TOKEN;
}

{INVALID_NUMBER}                {
    fprintf(stderr, "Lexical Error at line %d: Invalid numeric constant '%s' - contains alphabetic characters\n", yylineno, yytext);
    num_errors++;
    return ERROR_TOKEN;
}

{INVALID_CHAR_IN_NUMBER}        {
    fprintf(stderr, "Lexical Error at line %d: Invalid numeric constant '%s' - contains illegal characters\n", yylineno, yytext);
    num_errors++;
    return ERROR_TOKEN;
}

{IDENTIFIER}                    { 
    if (check_identifier_length(yytext)) {
        fprintf(stderr, "Lexical Error at line %d: Identifier '%s' exceeds maximum length (255 characters)\n", yylineno, yytext);
        num_errors++;
        return ERROR_TOKEN;
    }
    return IDENTIFIER; 
}

"++"            { return OP_INCREMENT; }
"--"            { return OP_DECREMENT; }
"->"            { return POINTER_ARROW; }
"+"             { return OP_PLUS; }
"-"             { return OP_MINUS; }
"*"             { return OP_MULTIPLY; }
"/"             { return OP_DIVIDE; }
"%"             { return OP_MODULO; }
"+="            { return OP_ADD_ASSIGN; }
"-="            { return OP_SUB_ASSIGN; }
"*="            { return OP_MUL_ASSIGN; }
"/="            { return OP_DIV_ASSIGN; }
"%="            { return OP_MOD_ASSIGN; }
"=="            { return OP_EQUAL; }
"!="            { return OP_NOT_EQUAL; }
"<="            { return OP_LESS_THAN_EQUAL; }
">="            { return OP_GREATER_THAN_EQUAL; }
"<"             { return OP_LESS_THAN; }
">"             { return OP_GREATER_THAN; }
"&&"            { return OP_LOGICAL_AND; }
"||"            { return OP_LOGICAL_OR; }
"!"             { return OP_LOGICAL_NOT; }
"&"             { return OP_BITWISE_AND; }
"|"             { return OP_BITWISE_OR; }
"^"             { return OP_BITWISE_XOR; }
"~"             { return OP_BITWISE_NOT; }
"<<"            { return OP_LSHIFT; }
">>"            { return OP_RSHIFT; }
"="             { return OP_ASSIGN; }

";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return OPEN_PARENTHESIS; }
")"             { return CLOSE_PARENTHESIS; }
"{"             { return OPEN_CURLY_BRACKET; }
"}"             { return CLOSE_CURLY_BRACKET; }
"["             { return OPEN_BRACKET; }
"]"             { return CLOSE_BRACKET; }
":"             { return COLON; }
"."             { return DOT; }

[$@#`\\]           { 
    fprintf(stderr, "Lexical Error at line %d: Illegal character '%s'\n", yylineno, yytext);
    num_errors++;
    return ERROR_TOKEN;
}

.               { 
    fprintf(stderr, "Lexical Error at line %d: Invalid token '%s'\n", yylineno, yytext);
    num_errors++;
    return ERROR_TOKEN;
}

%%
