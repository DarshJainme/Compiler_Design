#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "types.h"
#include <stdbool.h>
static int semantic_errors = 0;
typedef struct Label {
    char* name;
    bool is_case_label; // To distinguish case labels from goto labels
    struct Label* next;
} Label;

// New context for analyzing a single function
typedef struct FunctionAnalysisContext {
    Label* labels; // A list of all labels defined in the function
    struct Type* return_type;
    bool in_loop;
    bool in_switch;
} FunctionAnalysisContext;

struct ASTNode;
void analyze_enum_specifier(struct ASTNode* node);
void analyze_struct_or_union_specifier(struct ASTNode *node); 

struct Type *process_enum_specifier(struct ASTNode* node);
struct Type *process_struct_or_union_specifier(struct ASTNode *node);

// The main entry point for semantic analysis. Returns 1 on success, 0 on error.
int analyze_ast(ASTNode* root);

#endif // SEMANTIC_H