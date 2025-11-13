#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "types.h"
#include "ast.h"         
#include "parser.tab.h"

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

    Type* int_type = create_type(TYPE_INT);

    ASTNode* const_spec = create_specifier_node(CONST);
    ASTNode* char_spec = create_specifier_node(CHAR);
    ASTNodeList* spec_list = create_list_node(const_spec);
    append_to_list(spec_list, char_spec);


    ASTNode* ident_node = create_node(NODE_IDENTIFIER);
    ident_node->data.stringValue = strdup("format"); 
    
    ASTNode* ptr_decl = create_pointer_declarator_node(NULL, ident_node); // Creates the '*'
    
    ASTNode* param_node = create_parameter_declaration_node(spec_list, ptr_decl);
    
    ASTNodeList* param_list = create_list_node(param_node);

    // 3. Define 'printf'
    Type* printf_type = create_type(TYPE_FUNCTION);
    printf_type->data.function_sig.return_type = int_type;
    printf_type->data.function_sig.is_variadic = true;
    printf_type->data.function_sig.params = param_list; // Assign the param list
    
    add_symbol("printf", printf_type, SYM_FUNCTION, 0);

    // 4. Define 'scanf'
    Type* scanf_type = create_type(TYPE_FUNCTION);
    scanf_type->data.function_sig.return_type = int_type;
    scanf_type->data.function_sig.is_variadic = true;
    scanf_type->data.function_sig.params = param_list; // Can reuse the same param list
    
    add_symbol("scanf", scanf_type, SYM_FUNCTION, 0);

    // --- 5. Define 'malloc' ---
    // Signature: void* malloc(int size)

    // 5a. Create return type: void*
    Type* void_type = create_type(TYPE_VOID);
    Type* void_ptr_type = create_pointer_type(void_type);

    // 5b. Create AST for parameter: (int size)
    // We need 'int' specifiers (INT comes from parser.tab.h)
    ASTNode* int_spec = create_specifier_node(INT);
    ASTNodeList* int_spec_list = create_list_node(int_spec);

    // We need the 'size' identifier
    ASTNode* size_ident = create_node(NODE_IDENTIFIER);
    size_ident->data.stringValue = strdup("size"); // Use stringValue from ast.h

    // We need the full parameter node
    ASTNode* size_param_node = create_parameter_declaration_node(int_spec_list, size_ident);

    // Finally, the parameter list (with just one parameter)
    ASTNodeList* malloc_param_list = create_list_node(size_param_node);

    // 5c. Create malloc function type
    Type* malloc_type = create_type(TYPE_FUNCTION);
    malloc_type->data.function_sig.return_type = void_ptr_type;
    malloc_type->data.function_sig.params = malloc_param_list;
    malloc_type->data.function_sig.is_variadic = false;

    // 5d. Add to symbol table
    add_symbol("malloc", malloc_type, SYM_FUNCTION, 0);

    // --- 6. Define 'calloc' ---
    // Signature: void* calloc(int num, int size)

    // 6a. Create AST for parameter: (int num)
    // We can reuse the int_spec_list from above
    ASTNode* num_ident = create_node(NODE_IDENTIFIER);
    num_ident->data.stringValue = strdup("num");
    ASTNode* num_param_node = create_parameter_declaration_node(int_spec_list, num_ident);

    // 6b. Create the parameter list: (int num, int size)
    // We can reuse size_param_node from malloc
    ASTNodeList* calloc_param_list = create_list_node(num_param_node);
    append_to_list(calloc_param_list, size_param_node);

    // 6c. Create calloc function type
    // We reuse void_ptr_type from malloc
    Type* calloc_type = create_type(TYPE_FUNCTION);
    calloc_type->data.function_sig.return_type = void_ptr_type;
    calloc_type->data.function_sig.params = calloc_param_list;
    calloc_type->data.function_sig.is_variadic = false;

    // 6d. Add to symbol table
    add_symbol("calloc", calloc_type, SYM_FUNCTION, 0);


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

Symbol* add_symbol_to_scope(Scope* target_scope, const char* name, Type* type, SymbolKind kind, int lineno) {
    if (!target_scope) return NULL;
    
    // Check if symbol already exists in target scope
    for (int i = 0; i < target_scope->symbol_count; i++) {
        if (strcmp(target_scope->symbols[i]->name, name) == 0) {
            fprintf(stderr, "Semantic Error (Line %d): Redefinition of '%s'.\n", lineno, name);
            semantic_errors++;
            return NULL;
        }
    }

    if (target_scope->symbol_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Fatal: Symbol table overflow in scope.\n");
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
    new_symbol->scope_level = 0; // Global level
    new_symbol->stack_offset = 0;
    new_symbol->next_use = -1;
    new_symbol->is_live = false;

    target_scope->symbols[target_scope->symbol_count++] = new_symbol;

    return new_symbol;
}

Scope* get_global_scope() {
    Scope* scope = current_scope;
    while (scope && scope->parent != NULL) {
        scope = scope->parent;
    }
    return scope;
}