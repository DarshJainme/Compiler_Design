%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h" /* Header from Bison */
#include "symbol_table.h"

/* All error reporting will be handled by the parser's yyerror */
void yyerror(const char *s);

%}

/* Use modern Flex options instead of #defines */
%option noinput nounput yylineno

/* Definitions for regular expressions */
digit       [0-9]
letter      [a-zA-Z_]
ws          [ \t\r]+

/* Exclusive start condition for multi-line comments */
%x ML_COMMENT

%%
{ws}                            { /* Ignore whitespace */ }
"\n"                            { /* Flex handles yylineno automatically */ }

    /* --- Numeric Constants --- */
    /* Order is important: float must come before integer */
{digit}+\.{digit}+[fF]?          { yylval.str = strdup(yytext); return CONSTANT; }
{digit}+([eE][+-]?{digit}+)      { yylval.str = strdup(yytext); return CONSTANT; }
0[xX]({digit}|[a-fA-F])+         { yylval.str = strdup(yytext); return CONSTANT; }
0[bB][01]+                      { yylval.str = strdup(yytext); return CONSTANT; }
0[oO]?[0-7]+                    { yylval.str = strdup(yytext); return CONSTANT; }
{digit}+                        { yylval.str = strdup(yytext); return CONSTANT; }

    /* --- Keywords --- */
"auto"                          { return AUTO; }
"bool"                          { return BOOL; }
"break"                         { return BREAK; }
"case"                          { return CASE; }
"catch"                         { return CATCH; }
"char"                          { return CHAR; }
"class"                         { return CLASS; }
"const"                         { return CONST; }
"continue"                      { return CONTINUE; }
"default"                       { return DEFAULT; }
"delete"                        { return DELETE; }
"do"                            { return DO; }
"double"                        { return DOUBLE; }
"else"                          { return ELSE; }
"enum"                          { return ENUM; }
"extern"                        { return EXTERN; }
"false"                         { return BOOL_FALSE; }
"FILE"                          { return FILE_TYPE; }
"final"                         { return FINAL; }
"float"                         { return FLOAT; }
"for"                           { return FOR; }
"friend"                        { return FRIEND; }
"goto"                          { return GOTO; }
"if"                            { return IF; }
"int"                           { return INT; }
"long"                          { return LONG; }
"namespace"                     { return NAMESPACE; }
"new"                           { return NEW; }
"override"                      { return OVERRIDE; }
"private"                       { return PRIVATE; }
"protected"                     { return PROTECTED; }
"public"                        { return PUBLIC; }
"register"                      { return REGISTER; }
"return"                        { return RETURN; }
"short"                         { return SHORT; }
"signed"                        { return SIGNED; }
"sizeof"                        { return SIZEOF; }
"static"                        { return STATIC; }
"string"                        { return STRING; }
"struct"                        { return STRUCT; }
"switch"                        { return SWITCH; }
"throw"                         { return THROW; }
"try"                           { return TRY; }
"true"                          { return BOOL_TRUE; }
"typedef"                       { return TYPEDEF; }
"union"                         { return UNION; }
"unsigned"                      { return UNSIGNED; }
"using"                         { return USING; }
"virtual"                       { return VIRTUAL; }
"void"                          { return VOID; }
"while"                         { return WHILE; }
"until"                         { return UNTIL; }

    /* --- Operators --- */
"=="                            { return EQ_OP; }
"!="                            { return NE_OP; }
">="                            { return GE_OP; }
"<="                            { return LE_OP; }
">>="                           { return RSHIFT_ASSIGN; }
"<<="                           { return LSHIFT_ASSIGN; }
"+="                            { return ADD_ASSIGN; }
"-="                            { return SUB_ASSIGN; }
"*="                            { return MUL_ASSIGN; }
"/="                            { return DIV_ASSIGN; }
"%="                            { return MOD_ASSIGN; }
"&="                            { return AND_ASSIGN; }
"^="                            { return XOR_ASSIGN; }
"|="                            { return OR_ASSIGN; }
">>"                            { return RSHIFT_OP; }
"<<"                            { return LSHIFT_OP; }
"++"                            { return INC_OP; }
"--"                            { return DEC_OP; }
"->"                            { return PTR_OP; }
"&&"                            { return LAND_OP; }
"||"                            { return LOR_OP; }
"..."                           { return ELLIPSIS; }
"::"                            { return SCOPE_OP; }

    /* --- Literals and Identifier --- */
    /* Must be after keywords to ensure correct precedence */
'(\\.|[^'\\])'                  { yylval.str = strdup(yytext); return CHAR_LITERAL; }
\"([^"\\\n]|\\.)*\"              { yylval.str = strdup(yytext); return STRING_LITERAL; }
{letter}({letter}|{digit})*     { if (is_typename(yytext)) {
                                    yylval.str = strdup(yytext);
                                    return TYPE_NAME;
                                } else {
                                    yylval.str = strdup(yytext);
                                    return IDENTIFIER;
                                }
        }

    /* --- Comments --- */
\/\/.*                          { /* Ignore single line comment */ }
"/*"                            { BEGIN(ML_COMMENT); }
<ML_COMMENT>{
"*/"                            { BEGIN(INITIAL); }
[^*\n]+                         { /* Eat up text */ }
"*"                             { /* Eat up a star */ }
\n                              { /* Still in comment, but count line */ }
<<EOF>>                         { yyerror("Unterminated multi-line comment"); return 0; }
}

    /* --- All other single characters --- */
    /* These can be returned as themselves */
[;,{}()\[\].?:=*/%&|^!~+-<>]      { return yytext[0]; }

    /* --- Error Rule --- */
.                               { yyerror("Unexpected character"); }
%%

int yywrap(void) {
    return 1;
}
