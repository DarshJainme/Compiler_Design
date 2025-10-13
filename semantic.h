#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include "types.h"

// The main entry point for semantic analysis. Returns 1 on success, 0 on error.
int analyze_ast(ASTNode* root);

#endif // SEMANTIC_H