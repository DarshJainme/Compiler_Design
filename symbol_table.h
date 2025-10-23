#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

struct Type;
struct ASTNode;
#include "types.h"
#include "semantic.h"

#define MAX_SYMBOLS 1024

// extern Scope* current_scope;
typedef enum {
    SYM_VARIABLE,
    SYM_FUNCTION,
    SYM_TYPEDEF,
    SYM_ENUM_CONST,
    SYM_CONSTANT
} SymbolKind;

// Represents a single entry (symbol) in the table
typedef struct Symbol {
    char* name;
    struct Type* type;
    SymbolKind kind;
    int line_declared;
} Symbol;

// Represents a single scope (e.g., global, function body)
typedef struct Scope {
    Symbol* symbols[MAX_SYMBOLS];
    int symbol_count;
    struct Scope* parent;
} Scope;


void init_symbol_table();
void destroy_scope(Scope* scope);
Scope* get_current_scope();
void enter_scope();
Scope* leave_scope();

Symbol* add_symbol(const char* name, struct Type* type, SymbolKind kind, int lineno);
Symbol* find_symbol(const char* name);
Symbol* find_symbol_in_current_scope(const char* name);
void print_symbol_table(Scope *scope, int depth);
const char* symbol_kind_to_strign(SymbolKind kind);

#endif // SYMBOL_TABLE