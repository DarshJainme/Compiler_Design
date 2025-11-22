#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "types.h"
#include <stdbool.h>
extern int semantic_errors;
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

// Add declarations
// Helper to get name from declarator (used in tac.c)
char* get_name_from_declarator(struct ASTNode* declarator);
// Helper to infer type from constant string (used in tac.c)
struct Type* infer_constant_type(const char* val);

// The main entry point for semantic analysis. Returns 1 on success, 0 on error.
int analyze_ast(ASTNode* root);

#endif // SEMANTIC_H
