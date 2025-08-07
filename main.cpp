#include <iostream>
#include <fstream>
#include <cstdio>

extern int yylex();
extern FILE* yyin;
extern int yylineno;
extern int num_errors;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        std::cerr << "Error: Could not open file '" << argv[1] << "'" << std::endl;
        return 1;
    }
    printf("%-25s %s\n", "Lexeme", "Token");
    printf("--------------------------------------------------\n");
    yylex();
    printf("--------------------------------------------------\n");
    if (num_errors > 0) {
        printf("Lexical analysis finished with %d error(s).\n", num_errors);
    } else {
        printf("Lexical analysis completed successfully. Total lines: %d\n", yylineno);
    }
    fclose(yyin);
    return 0;
}
