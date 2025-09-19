%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "grammar.tab.h"

int linenum = 1;
int columnnum = 1;

void update_location();
void handle_comment();
void handle_multiline_comment();
char* my_strdup_lex(const char *s);
extern void add_token_to_list(const char *token, const char *type);
extern const char* get_token_name(int token_type);

// This macro does all 3 steps in one
#define TOKEN(t) do { \
    update_location(); \
    add_token_to_list(yytext, get_token_name(t)); \
    return t; \
} while (0)
%}

%option noyywrap
%option yylineno

/* Regular expression definitions */
DIGIT       [0-9]
LETTER      [a-zA-Z_]
ALNUM       [a-zA-Z0-9_]
IDENTIFIER  {LETTER}{ALNUM}*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}*\.{DIGIT}+([eE][+-]?{DIGIT}+)?
CHAR        '([^'\\]|\\.)'
STRING      \"([^"\\]|\\.)*\"
WHITESPACE  [ \t\r]+
NEWLINE     \n

/* Comment patterns */
SINGLE_COMMENT    "//".*
MULTI_COMMENT_START "/*"

%%

{WHITESPACE}      { columnnum += yyleng; }
{NEWLINE}         { linenum++; columnnum = 1; }

{SINGLE_COMMENT} { handle_comment(); }
{MULTI_COMMENT_START} { handle_multiline_comment(); }

"#include"      { update_location(); /* Skip */ }
"#define"       { update_location(); /* Skip */ }
"#"[a-zA-Z_][a-zA-Z0-9_]* { update_location(); /* Skip */ }
"<"[^>]*">"       { update_location(); /* Skip */ }

"if"            { TOKEN(IF); }
"else"          { TOKEN(ELSE); }
"while"         { TOKEN(WHILE); }
"for"           { TOKEN(FOR); }
"do"            { TOKEN(DO); }
"switch"        { TOKEN(SWITCH); }
"case"          { TOKEN(CASE); }
"default"       { TOKEN(DEFAULT); }
"break"         { TOKEN(BREAK); }
"continue"      { TOKEN(CONTINUE); }
"return"        { TOKEN(RETURN); }
"goto"          { TOKEN(GOTO); }

"static"        { TOKEN(STATIC); }
"struct"        { TOKEN(STRUCT); }
"typedef"       { TOKEN(TYPEDEF); }

"int"           { TOKEN(INT); }
"char"          { TOKEN(CHAR); }
"float"         { TOKEN(FLOAT); }
"double"        { TOKEN(DOUBLE); }
"void"          { TOKEN(VOID); }

"class"         { TOKEN(CLASS); }
"public"        { TOKEN(PUBLIC); }
"private"       { TOKEN(PRIVATE); }
"protected"     { TOKEN(PROTECTED); }
"virtual"       { TOKEN(VIRTUAL); }
"override"      { TOKEN(OVERRIDE); }
"new"           { TOKEN(NEW); }
"delete"        { TOKEN(DELETE); }
"this"          { TOKEN(THIS); }

"lambda"        { TOKEN(LAMBDA); }
"auto"          { TOKEN(AUTO); }
"malloc"        { TOKEN(MALLOC); }
"free"          { TOKEN(FREE); }

"++"            { TOKEN(INCREMENT); }
"--"            { TOKEN(DECREMENT); }
"+="            { TOKEN(PLUS_ASSIGN); }
"-="            { TOKEN(MINUS_ASSIGN); }
"*="            { TOKEN(MULT_ASSIGN); }
"/="            { TOKEN(DIV_ASSIGN); }

"=="            { TOKEN(EQUAL); }
"!="            { TOKEN(NOT_EQUAL); }
"<="            { TOKEN(LESS_EQUAL); }
">="            { TOKEN(GREATER_EQUAL); }
"&&"            { TOKEN(LOGICAL_AND); }
"||"            { TOKEN(LOGICAL_OR); }
"<<"            { TOKEN(LEFT_SHIFT); }
">>"            { TOKEN(RIGHT_SHIFT); }
"->"            { TOKEN(ARROW); }
"::"            { TOKEN(SCOPE); }

"+"             { TOKEN(PLUS); }
"-"             { TOKEN(MINUS); }
"*"             { TOKEN(MULTIPLY); }
"/"             { TOKEN(DIVIDE); }
"%"             { TOKEN(MODULO); }
"="             { TOKEN(ASSIGN); }
"<"             { TOKEN(LESS_THAN); }
">"             { TOKEN(GREATER_THAN); }
"!"             { TOKEN(LOGICAL_NOT); }
"~"             { TOKEN(BITWISE_NOT); }
"&"             { TOKEN(BITWISE_AND); }
"|"             { TOKEN(BITWISE_OR); }
"^"             { TOKEN(BITWISE_XOR); }

";"             { TOKEN(SEMICOLON); }
","             { TOKEN(COMMA); }
"."             { TOKEN(DOT); }
":"             { TOKEN(COLON); }
"("             { TOKEN(LPAREN); }
")"             { TOKEN(RPAREN); }
"["             { TOKEN(LBRACKET); }
"]"             { TOKEN(RBRACKET); }
"{"             { TOKEN(LBRACE); }
"}"             { TOKEN(RBRACE); }

{INTEGER}       {
    yylval.ival = atoi(yytext);
    TOKEN(INTEGER_LITERAL);
}

{FLOAT}         {
    yylval.fval = atof(yytext);
    TOKEN(FLOAT_LITERAL);
}

{CHAR}          {
    if (strlen(yytext) == 3) {
        yylval.cval = yytext[1];
    } else { /* Handle escape sequences */
        switch (yytext[2]) {
            case 'n': yylval.cval = '\n'; break;
            case 't': yylval.cval = '\t'; break;
            case 'r': yylval.cval = '\r'; break;
            case '\\': yylval.cval = '\\'; break;
            case '\'': yylval.cval = '\''; break;
            default: yylval.cval = yytext[2]; break;
        }
    }
    TOKEN(CHAR_LITERAL);
}

{STRING}        {
    char *str = my_strdup_lex(yytext + 1);
    str[strlen(str) - 1] = '\0';
    yylval.sval = str;
    TOKEN(STRING_LITERAL);
}

{IDENTIFIER}    {
    yylval.sval = my_strdup_lex(yytext);
    TOKEN(IDENTIFIER);
}

'[^']*$        {
    fprintf(stderr, "Lexical error at line %d, column %d: Unterminated character literal\n",
            linenum, columnnum);
    update_location();
}

\"[^"]*$        {
    fprintf(stderr, "Lexical error at line %d, column %d: Unterminated string literal\n",
            linenum, columnnum);
    update_location();
}

{DIGIT}{IDENTIFIER} {
    fprintf(stderr, "Lexical error at line %d, column %d: Invalid identifier starting with digit: %s\n",
            linenum, columnnum, yytext);
    update_location();
}

.               {
    fprintf(stderr, "Lexical error at line %d, column %d: Unknown character '%c'\n",
            linenum, columnnum, yytext[0]);
    update_location();
}

%%

char* my_strdup_lex(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy) {
        memcpy(copy, s, len);
    }
    return copy;
}

void update_location() {
    columnnum += yyleng;
}

void handle_comment() {
    columnnum += yyleng;
}

void handle_multiline_comment() {
    int c;
    int start_line = linenum;
    int start_col = columnnum;
    columnnum += 2;

    while ((c = input()) != 0) {
        if (c == '\n') {
            linenum++;
            columnnum = 1;
        } else {
            columnnum++;
        }

        if (c == '*') {
            c = input();
            if (c == '/') {
                columnnum++;
                return;
            } else {
                unput(c);
            }
        }
    }
    fprintf(stderr, "Lexical error at line %d, column %d: Unterminated comment\n",
            start_line, start_col);
}