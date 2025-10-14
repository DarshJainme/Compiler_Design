#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "types.h"

struct ASTNode;
void analyze_enum_specifier(struct ASTNode* node);
void analyze_struct_or_union_specifier(struct ASTNode *node, Type* type_being_built); 

// The main entry point for semantic analysis. Returns 1 on success, 0 on error.
int analyze_ast(ASTNode* root);

#endif // SEMANTIC_H