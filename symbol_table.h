#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include "types.h"

#define MAX_SYMBOLS 1024

// extern Scope* current_scope;
typedef enum {
    SYM_VARIABLE,
    SYM_FUNCTION,
    SYM_TYPEDEF,
    SYM_ENUM_CONST
} SymbolKind;

// Represents a single entry (symbol) in the table
typedef struct Symbol {
    char* name;
    Type* type;
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

void enter_scope();
void leave_scope();

Symbol* add_symbol(const char* name, Type* type, SymbolKind kind);
Symbol* find_symbol(const char* name);
Symbol* find_symbol_in_current_scope(const char* name);
void print_symbol_table(Scope* scope, int depth);

#endif // SYMBOL_TABLE