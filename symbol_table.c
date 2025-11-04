#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "types.h"

static Scope* current_scope = NULL;
static int current_scope_level = 0;
extern int yylineno;
// extern int semantic_errors;
void init_symbol_table() {
    if (current_scope) {
        while (current_scope->parent) {
            leave_scope();
        }
        destroy_scope(current_scope);
    }
    current_scope = (Scope*)calloc(1, sizeof(Scope));
    current_scope->parent = NULL;
    current_scope->symbol_count = 0;
    current_scope_level = 0;
}

void destroy_scope(Scope* scope) {
    if (!scope) return;
    for (int i = 0; i < scope->symbol_count; i++) {
        free(scope->symbols[i]->name);
        free(scope->symbols[i]);
    }
    free(scope);
}

void enter_scope() {
    Scope* new_scope = (Scope*)calloc(1, sizeof(Scope));
    new_scope->parent = current_scope;
    current_scope = new_scope;
    current_scope_level++;
}

Scope* get_current_scope(){
    return current_scope;
}
Scope* leave_scope() {
    if (!current_scope) {
        fprintf(stderr, "Fatal: leave_scope() called on NULL scope.\n");
        return NULL;
    }
    if (!current_scope->parent) {
         fprintf(stderr, "Warning: leave_scope() called on global scope.\n");
         return current_scope;
    }

    Scope* popped_scope = current_scope;
    current_scope = current_scope->parent;
    current_scope_level--;
    return popped_scope;
}

Symbol* add_symbol(const char* name, Type* type, SymbolKind kind, int lineno) {
    // Checking if symbol already exists in current scope
    for (int i = 0; i < current_scope->symbol_count; i++) {
        if (strcmp(current_scope->symbols[i]->name, name) == 0) {
            fprintf(stderr, "Semantic Error (Line %d): Redefinition of '%s'.\n", lineno, name);
            semantic_errors++;
            return NULL;
        }
    }

    if (current_scope->symbol_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Fatal: Symbol table overflow in current scope.\n");
        exit(1);
    }

    Symbol* new_symbol = (Symbol*)calloc(1, sizeof(Symbol));
    if (!new_symbol) {
        fprintf(stderr, "Fatal: Memory allocation failed for symbol.\n");
        exit(1);
    }
    new_symbol->name = strdup(name);
    new_symbol->type = type;
    new_symbol->kind = kind;
    new_symbol->line_declared = lineno;

    // initialize
    new_symbol->scope_level = current_scope_level;
    new_symbol->stack_offset = 0;
    new_symbol->next_use = -1;
    new_symbol->is_live = false;

    current_scope->symbols[current_scope->symbol_count++] = new_symbol;

    return new_symbol;
}

Symbol* find_symbol_in_current_scope(const char* name) {
    if (!current_scope) return NULL;
    for (int i = 0; i < current_scope->symbol_count; i++) {
        if (strcmp(current_scope->symbols[i]->name, name) == 0) {
            return current_scope->symbols[i];
        }
    }
    return NULL;
}

Symbol* find_symbol(const char* name) {
    Scope* scope = current_scope;
    while (scope) {
        for (int i = 0; i < scope->symbol_count; i++) {
            if (strcmp(scope->symbols[i]->name, name) == 0) {
                return scope->symbols[i];
            }
        }
        scope = scope->parent;
    }
    return NULL;
}
const char* symbol_kind_to_string(SymbolKind kind) {
    switch(kind) {
        case SYM_VARIABLE: return "Variable";
        case SYM_FUNCTION: return "Function";
        case SYM_TYPEDEF:  return "Typedef";
        case SYM_ENUM_CONST: return "EnumConst";
        case SYM_CONSTANT: return "Constant";
        default: return "Unknown";
    }
}
void print_symbol_table(Scope* scope, int depth) {
    if (!scope) return;

    // Create indentation string
    char indent[depth*2 + 1];
    for (int i=0; i < depth*2; i++) indent[i] = ' ';
    indent[depth*2] = '\0';

    printf("%s--- Scope (Level %d) ---\n", indent, depth);

    for (int i = 0; i < scope->symbol_count; i++) {
        Symbol* sym = scope->symbols[i];

        // Print symbol details
        printf("%s  Name: %-15s, Scope: %-3d, Offset: %-4d, Kind: %-10s, Line: %d, Type: %s\n", // <-- Modified Print
               indent,
               sym->name,
               sym->scope_level,
               sym->stack_offset,
               symbol_kind_to_string(sym->kind),
               sym->line_declared,
               type_to_string(sym->type)); // Ensure type_to_string is robust
        if (sym->kind == SYM_TYPEDEF &&
            (sym->type->kind == TYPE_STRUCT || sym->type->kind == TYPE_UNION) &&
             sym->type->data.struct_union_info.member_scope)
        {
            printf("%s  ... members of %s ...\n", indent, sym->name);
            print_symbol_table(sym->type->data.struct_union_info.member_scope, depth + 1);
        }
    }
}