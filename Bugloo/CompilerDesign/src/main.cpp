#include <iostream>
#include <cstdio>
#include "tokens.h"

extern int yylex();
extern FILE* yyin;
extern int yylineno;
extern int num_errors;
extern char* yytext;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    yyin = file;

    // Print table header
    printf("%-25s %s\n", "Lexeme", "Token");
    printf("----------------------------------------\n");

    int token;
    while ((token = yylex())) {
        if (token == ERROR_TOKEN) {
            // Error already reported by yylex, just increment counter
            // num_errors is already incremented in lexer
        } else {
            // Print lexeme and token
            printf("%-25s %s\n", yytext, token_to_string((TokenType)token));
        }
    }

    fclose(file);

    if (num_errors > 0) {
        std::cerr << "\nLexical analysis failed with " << num_errors << " errors." << std::endl;
        return 1;
    } else {
        std::cout << "\nLexical analysis completed successfully." << std::endl;
        return 0;
    }
}