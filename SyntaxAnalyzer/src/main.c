#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ast.h"
#include "symbol_table.h"
#include "grammar.tab.h"

extern int yyparse();
extern int yylex();
extern void yyrestart(FILE*);
extern FILE *yyin;
extern int linenum;
extern int columnnum;
extern int error_count;
ast_node *root = NULL;
extern symbol_table *global_scope;
extern symbol_table *current_scope;
extern void print_token_table(); // Declare the function from grammar.y

// Function to get token name from token type (Your existing function is perfect)
const char* get_token_name(int token_type) {
    switch(token_type) {
        case INTEGER_LITERAL: return "INTEGER_LITERAL";
        case FLOAT_LITERAL: return "FLOAT_LITERAL";
        case CHAR_LITERAL: return "CHAR_LITERAL";
        case STRING_LITERAL: return "STRING_LITERAL";
        case IDENTIFIER: return "IDENTIFIER";
        case IF: return "IF";
        case ELSE: return "ELSE";
        case WHILE: return "WHILE";
        case FOR: return "FOR";
        case DO: return "DO";
        case SWITCH: return "SWITCH";
        case CASE: return "CASE";
        case DEFAULT: return "DEFAULT";
        case BREAK: return "BREAK";
        case CONTINUE: return "CONTINUE";
        case RETURN: return "RETURN";
        case GOTO: return "GOTO";
        case PRINTF: return "PRINTF";
        case SCANF: return "SCANF";
        case STATIC: return "STATIC";
        case STRUCT: return "STRUCT";
        case TYPEDEF: return "TYPEDEF";
        case INT: return "INT";
        case CHAR: return "CHAR";
        case FLOAT_TYPE: return "FLOAT_TYPE";
        case DOUBLE: return "DOUBLE";
        case VOID: return "VOID";
        case CLASS: return "CLASS";
        case PUBLIC: return "PUBLIC";
        case PRIVATE: return "PRIVATE";
        case PROTECTED: return "PROTECTED";
        case VIRTUAL: return "VIRTUAL";
        case OVERRIDE: return "OVERRIDE";
        case NEW: return "NEW";
        case DELETE: return "DELETE";
        case THIS: return "THIS";
        case LAMBDA: return "LAMBDA";
        case AUTO: return "AUTO";
        case MALLOC: return "MALLOC";
        case FREE: return "FREE";
        case PLUS: return "OPERATOR";
        case MINUS: return "OPERATOR";
        case MULTIPLY: return "OPERATOR";
        case DIVIDE: return "OPERATOR";
        case MODULO: return "OPERATOR";
        case INCREMENT: return "OPERATOR";
        case DECREMENT: return "OPERATOR";
        case ASSIGN: return "OPERATOR";
        case PLUS_ASSIGN: return "OPERATOR";
        case MINUS_ASSIGN: return "OPERATOR";
        case MULT_ASSIGN: return "OPERATOR";
        case DIV_ASSIGN: return "OPERATOR";
        case EQUAL: return "OPERATOR";
        case NOT_EQUAL: return "OPERATOR";
        case LESS_THAN: return "OPERATOR";
        case GREATER_THAN: return "OPERATOR";
        case LESS_EQUAL: return "OPERATOR";
        case GREATER_EQUAL: return "OPERATOR";
        case LOGICAL_AND: return "OPERATOR";
        case LOGICAL_OR: return "OPERATOR";
        case LOGICAL_NOT: return "OPERATOR";
        case BITWISE_AND: return "OPERATOR";
        case BITWISE_OR: return "OPERATOR";
        case BITWISE_XOR: return "OPERATOR";
        case BITWISE_NOT: return "OPERATOR";
        case LEFT_SHIFT: return "OPERATOR";
        case RIGHT_SHIFT: return "OPERATOR";
        case SEMICOLON: return "SEMICOLON";
        case COMMA: return "COMMA";
        case DOT: return "DOT";
        case ARROW: return "ARROW";
        case SCOPE: return "SCOPE";
        case COLON: return "COLON";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACKET: return "LBRACKET";
        case RBRACKET: return "RBRACKET";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        default: return "UNKNOWN";
    }
}

// Token analysis mode - this is only used for the -t flag now
int analyze_tokens_only(FILE *input) {
    // This function remains unchanged, it's correct for its purpose.
    // ... (your existing analyze_tokens_only function) ...
     int token;
    extern char *yytext;
    extern YYSTYPE yylval;

    printf("Token\tToken_Type\n");

    yyrestart(input);
    linenum = 1;
    columnnum = 1;

    while ((token = yylex()) != 0) {
        switch(token) {
            case INTEGER_LITERAL:
                printf("%d\t%s\n", yylval.ival, get_token_name(token));
                break;
            case FLOAT_LITERAL:
                printf("%.2f\t%s\n", yylval.fval, get_token_name(token));
                break;
            case CHAR_LITERAL:
                printf("%c\t%s\n", yylval.cval, get_token_name(token));
                break;
            case STRING_LITERAL:
                printf("\"%s\"\t%s\n", yylval.sval, get_token_name(token));
                break;
            case IDENTIFIER:
                printf("%s\t%s\n", yylval.sval, get_token_name(token));
                break;
            default:
                printf("%s\t%s\n", yytext, get_token_name(token));
                break;
        }
    }

    return 0;
}

// === THIS IS THE CORRECTED FUNCTION ===
int analyze_syntax(FILE *input) {
    // Initialize symbol tables
    global_scope = create_symbol_table();
    current_scope = global_scope;

    yyrestart(input);
    linenum = 1;
    columnnum = 1;
    error_count = 0;

    int parse_result = yyparse();

    if (error_count == 0 && parse_result == 0) {
        printf("Syntax analysis successful!\n");
        
        // 1. Print the token table that was built during the parse.
        print_token_table();

        // 2. Print the symbol table to see the results of your semantic analysis.
        printf("\n\n## Symbol Table Analysis ##\n");
        print_all_tables_recursively(global_scope, 0); // Use the new recursive function

        return 0; // Success
    } else {
        // Syntax errors were already reported by yyerror.
        printf("\nSyntax analysis failed with %d error(s).\n", error_count);
        return 1; // Failure
    }
}

// ... (The rest of your main.c file: save_ast_to_file, save_symbol_table_to_file, print_usage, and main function remain unchanged) ...
// Save AST to file
void save_ast_to_file(ast_node *ast, const char *input_filename) {
    if (!ast || !input_filename) return;

    char *output_filename = malloc(strlen(input_filename) + 10);
    if (!output_filename) return;

    strcpy(output_filename, input_filename);
    char *dot = strrchr(output_filename, '.');
    int base_len = dot ? (dot - output_filename) : strlen(output_filename);
    strcpy(output_filename + base_len, ".ast");

    FILE *ast_file = fopen(output_filename, "w");
    if (ast_file) {
        fprintf(ast_file, "=== Abstract Syntax Tree ===\n");
        print_ast_to_file(ast, 0, ast_file);
        fclose(ast_file);
        printf("AST saved to: %s\n", output_filename);
    }

    free(output_filename);
}

// Save symbol table to file
void save_symbol_table_to_file(void *table, const char *input_filename) {
    if (!table || !input_filename) return;

    char *output_filename = malloc(strlen(input_filename) + 10);
    if (!output_filename) return;

    strcpy(output_filename, input_filename);
    char *dot = strrchr(output_filename, '.');
    int base_len = dot ? (dot - output_filename) : strlen(output_filename);
    strcpy(output_filename + base_len, ".sym");

    FILE *sym_file = fopen(output_filename, "w");
    if (sym_file) {
        print_symbol_table_to_file((symbol_table*)table, sym_file);
        fclose(sym_file);
        printf("Symbol table saved to: %s\n", output_filename);
    }

    free(output_filename);
}

// Print usage information
void print_usage(const char *program_name) {
    printf("Usage: %s [options] <input_file>\n", program_name);
    printf("Options:\n");
    printf("  -v, --verbose    Enable verbose output (AST and symbol table)\n");
    printf("  -t, --tokens     Only perform lexical analysis (tokens only)\n");
    printf("  -h, --help       Show this help message\n");
    printf("\nThe analyzer will:\n");
    printf("- If no syntax errors: Print token table\n");
    printf("- If syntax errors found: Report the errors\n");
    printf("- With -v: Also save AST and symbol table to files\n");
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    int tokens_only = 0;
    char *input_filename = NULL;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--tokens") == 0) {
            tokens_only = 1;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (argv[i][0] != '-') {
            input_filename = argv[i];
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (!input_filename) {
        fprintf(stderr, "Error: No input file specified\n");
        print_usage(argv[0]);
        return 1;
    }

    // Open input file
    FILE *input = fopen(input_filename, "r");
    if (!input) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", input_filename);
        return 1;
    }

    int result;

    if (tokens_only) {
        result = analyze_tokens_only(input);
    } else {
        result = analyze_syntax(input);

        // Save additional files if verbose mode and analysis was attempted
        if (verbose && global_scope) {
            save_symbol_table_to_file(global_scope, input_filename);
            if (root) {
                save_ast_to_file(root, input_filename);
            }
        }
    }

    fclose(input);

    // Cleanup
    if (global_scope) {
        destroy_symbol_table(global_scope);
    }
    if (root) {
        free_ast(root);
    }

    return result;
}