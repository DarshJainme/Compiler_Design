#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static Scope* current_scope = NULL;
extern int yylineno;

void init_symbol_table() {
    if (current_scope) {
        // This should not happen, but as a safeguard
        while (current_scope->parent) {
            leave_scope();
        }
        destroy_scope(current_scope);
    }
    current_scope = (Scope*)calloc(1, sizeof(Scope));
    current_scope->parent = NULL;
    current_scope->symbol_count = 0;
}

void destroy_scope(Scope* scope) {
    if (!scope) return;
    for (int i = 0; i < scope->symbol_count; i++) {
        free(scope->symbols[i]->name);
        // Type memory is managed elsewhere for now
        free(scope->symbols[i]);
    }
    free(scope);
}

void enter_scope() {
    Scope* new_scope = (Scope*)calloc(1, sizeof(Scope));
    new_scope->parent = current_scope;
    current_scope = new_scope;
}

void leave_scope() {
    if (current_scope && current_scope->parent) {
        Scope* parent_scope = current_scope->parent;
        destroy_scope(current_scope);
        current_scope = parent_scope;
    }
}

Symbol* add_symbol(const char* name, Type* type, SymbolKind kind) {
    if (find_symbol_in_current_scope(name)) {
        fprintf(stderr, "Semantic Error (Line %d): Redeclaration of '%s'.\n", yylineno, name);
        return NULL;
    }
    if (current_scope->symbol_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table overflow in current scope.\n");
        return NULL;
    }

    Symbol* new_symbol = (Symbol*)malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = type;
    new_symbol->kind = kind;
    new_symbol->line_declared = yylineno;
    
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

void print_symbol_table(Scope* scope, int depth) {
    if (!scope) return;
    printf("Scope Level %d:\n", depth);
    for (int i = 0; i < scope->symbol_count; i++) {
        Symbol* sym = scope->symbols[i];
        printf("  Name: %s, Type: %s, Kind: %d, Declared at line: %d\n",
               sym->name,
               type_to_string(sym->type),
               sym->kind,
               sym->line_declared);
    }
    if (scope->parent) {
        print_symbol_table(scope->parent, depth + 1);
    }
}