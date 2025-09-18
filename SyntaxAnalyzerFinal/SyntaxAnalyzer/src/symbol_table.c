#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global symbol tables
symbol_table *global_scope = NULL;
extern symbol_table *current_scope;

// Hash function for symbol table
unsigned int hash_function(const char *str, int table_size) {
  unsigned int hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % table_size;
}

// Create a new symbol table
symbol_table *create_symbol_table(void) {
  symbol_table *table = malloc(sizeof(symbol_table));
  if (!table)
    return NULL;

  table->size = 101; // Prime number for better hash distribution
  table->table = calloc(table->size, sizeof(symbol *));
  if (!table->table) {
    free(table);
    return NULL;
  }

  table->count = 0;
  table->parent = NULL;
  table->children = NULL;
  table->child_count = 0;
  table->scope_level = 0;
  table->scope_name = NULL;

  return table;
}

// Create a child scope
symbol_table *create_child_scope(symbol_table *parent, const char *scope_name) {
  symbol_table *child = create_symbol_table();
  if (!child)
    return NULL;

  child->parent = parent;
  child->scope_level = parent ? parent->scope_level + 1 : 0;
  if (scope_name) {
    child->scope_name = malloc(strlen(scope_name) + 1);
    if (child->scope_name) {
      strcpy(child->scope_name, scope_name);
    }
  }

  if (parent) {
    parent->children = realloc(parent->children, (parent->child_count + 1) *
                                                     sizeof(symbol_table *));
    if (parent->children) {
      parent->children[parent->child_count] = child;
      parent->child_count++;
    }
  }

  return child;
}

// Destroy symbol table
void destroy_symbol_table(symbol_table *table) {
  if (!table)
    return;

  // Free all symbols
  for (int i = 0; i < table->size; i++) {
    symbol *current = table->table[i];
    while (current) {
      symbol *next = current->next;
      free(current->name);
      if (current->type_info.function_info.param_types) {
        free(current->type_info.function_info.param_types);
      }
      if (current->type_info.composite_info.struct_name) {
        free(current->type_info.composite_info.struct_name);
      }
      if (current->sym_type == SYMBOL_CONSTANT &&
          current->type_info.constant_info.value.string_value) {
        free(current->type_info.constant_info.value.string_value);
      }
      free(current);
      current = next;
    }
  }

  // Free children
  for (int i = 0; i < table->child_count; i++) {
    destroy_symbol_table(table->children[i]);
  }

  free(table->children);
  free(table->table);
  free(table->scope_name);
  free(table);
}

// Create a new symbol
symbol *create_symbol(const char *name, symbol_type sym_type,
                      data_type data_type) {
  symbol *sym = malloc(sizeof(symbol));
  if (!sym)
    return NULL;

  sym->name = malloc(strlen(name) + 1);
  if (!sym->name) {
    free(sym);
    return NULL;
  }
  strcpy(sym->name, name);

  sym->sym_type = sym_type;
  sym->data_type = data_type;
  sym->storage = STORAGE_AUTO;
  sym->access = ACCESS_PUBLIC;
  sym->line_declared = 0;
  sym->line_used = 0;
  sym->is_initialized = 0;
  sym->is_used = 0;
  sym->is_function_defined = 0;
  sym->next = NULL;

  // Initialize type-specific information
  memset(&sym->type_info, 0, sizeof(sym->type_info));

  return sym;
}

// Insert symbol into table
int insert_symbol(symbol_table *table, symbol *sym) {
  if (!table || !sym)
    return 0;

  unsigned int index = hash_function(sym->name, table->size);

  // Check for duplicate in current scope
  symbol *current = table->table[index];
  while (current) {
    if (strcmp(current->name, sym->name) == 0) {
      report_redeclaration_error(sym->name, sym->line_declared,
             current->line_declared);
      return 0;
    }
    current = current->next;
  }

  // Insert at beginning of chain
  sym->next = table->table[index];
  table->table[index] = sym;
  table->count++;

  return 1;
}

// Lookup symbol in current scope only
symbol *lookup_symbol_current_scope(symbol_table *table, const char *name) {
  if (!table || !name)
    return NULL;

  unsigned int index = hash_function(name, table->size);
  symbol *current = table->table[index];

  while (current) {
    if (strcmp(current->name, name) == 0) {
      return current;
    }
    current = current->next;
  }

  return NULL;
}

// Lookup symbol in all scopes (current and parents)
symbol *lookup_symbol_all_scopes(symbol_table *table, const char *name) {
  symbol *sym = lookup_symbol_current_scope(table, name);
  if (sym)
    return sym;

  if (table->parent) {
    return lookup_symbol_all_scopes(table->parent, name);
  }

  return NULL;
}

// Generic lookup function
symbol *lookup_symbol(symbol_table *table, const char *name) {
  return lookup_symbol_all_scopes(table, name);
}

// Mark symbol as used
void mark_symbol_used(symbol_table *table, const char *name, int line) {
  symbol *sym = lookup_symbol_all_scopes(table, name);
  if (sym) {
    sym->is_used = 1;
    sym->line_used = line;
  }
}

// Mark symbol as initialized
void mark_symbol_initialized(symbol_table *table, const char *name) {
  symbol *sym = lookup_symbol_all_scopes(table, name);
  if (sym) {
    sym->is_initialized = 1;
  }
}

// Update symbol type
int update_symbol_type(symbol_table *table, const char *name,
                       data_type new_type) {
  symbol *sym = lookup_symbol_current_scope(table, name);
  if (sym) {
    sym->data_type = new_type;
    return 1;
  }
  return 0;
}

// Type compatibility checking
int are_types_compatible(data_type type1, data_type type2) {
  if (type1 == type2)
    return 1;

  // Allow implicit conversions between numeric types
  if ((type1 == TYPE_INT || type1 == TYPE_CHAR || type1 == TYPE_FLOAT ||
       type1 == TYPE_DOUBLE) &&
      (type2 == TYPE_INT || type2 == TYPE_CHAR || type2 == TYPE_FLOAT ||
       type2 == TYPE_DOUBLE)) {
    return 1;
  }

  return 0;
}

// Assignment type checking
int can_assign_types(data_type dest_type, data_type src_type) {
  return are_types_compatible(dest_type, src_type);
}

// Type to string conversion
const char *type_to_string(data_type type) {
  switch (type) {
  case TYPE_VOID:
    return "void";
  case TYPE_INT:
    return "int";
  case TYPE_CHAR:
    return "char";
  case TYPE_FLOAT:
    return "float";
  case TYPE_DOUBLE:
    return "double";
  case TYPE_POINTER:
    return "pointer";
  case TYPE_ARRAY:
    return "array";
  case TYPE_FUNCTION:
    return "function";
  case TYPE_STRUCT:
    return "struct";
  case TYPE_CLASS:
    return "class";
  case TYPE_CUSTOM:
    return "custom";
  default:
    return "unknown";
  }
}

// Symbol type to string
const char *symbol_type_to_string(symbol_type type) {
  switch (type) {
  case SYMBOL_VARIABLE:
    return "variable";
  case SYMBOL_FUNCTION:
    return "function";
  case SYMBOL_PARAMETER:
    return "parameter";
  case SYMBOL_CLASS:
    return "class";
  case SYMBOL_STRUCT:
    return "struct";
  case SYMBOL_TYPEDEF:
    return "typedef";
  case SYMBOL_LABEL:
    return "label";
  case SYMBOL_ENUM:
    return "enum";
  case SYMBOL_CONSTANT:
    return "constant";
  default:
    return "unknown";
  }
}

// Storage class to string
const char *storage_class_to_string(storage_class storage) {
  switch (storage) {
  case STORAGE_AUTO:
    return "auto";
  case STORAGE_STATIC:
    return "static";
  case STORAGE_EXTERN:
    return "extern";
  case STORAGE_REGISTER:
    return "register";
  default:
    return "unknown";
  }
}

// Access modifier to string
const char *access_modifier_to_string(access_modifier access) {
  switch (access) {
  case ACCESS_PUBLIC:
    return "public";
  case ACCESS_PRIVATE:
    return "private";
  case ACCESS_PROTECTED:
    return "protected";
  default:
    return "unknown";
  }
}

// Specialized symbol creation functions
symbol *create_array_symbol(const char *name, data_type element_type, int size,
                            int dimensions) {
  symbol *sym = create_symbol(name, SYMBOL_VARIABLE, TYPE_ARRAY);
  if (sym) {
    sym->type_info.array_info.element_type = element_type;
    sym->type_info.array_info.size = size;
    sym->type_info.array_info.dimensions = dimensions;
  }
  return sym;
}

symbol *create_pointer_symbol(const char *name, data_type pointed_type,
                              int level) {
  symbol *sym = create_symbol(name, SYMBOL_VARIABLE, TYPE_POINTER);
  if (sym) {
    sym->type_info.pointer_info.pointed_type = pointed_type;
    sym->type_info.pointer_info.level = level;
  }
  return sym;
}

symbol *create_function_symbol(const char *name, data_type return_type,
                               int param_count, data_type *param_types) {
  symbol *sym = create_symbol(name, SYMBOL_FUNCTION, TYPE_FUNCTION);
  if (sym) {
    sym->type_info.function_info.return_type = return_type;
    sym->type_info.function_info.param_count = param_count;
    if (param_count > 0 && param_types) {
      sym->type_info.function_info.param_types =
          malloc(param_count * sizeof(data_type));
      if (sym->type_info.function_info.param_types) {
        memcpy(sym->type_info.function_info.param_types, param_types,
               param_count * sizeof(data_type));
      }
    }
  }
  return sym;
}

symbol *create_struct_symbol(const char *name, symbol_table *members) {
  symbol *sym = create_symbol(name, SYMBOL_STRUCT, TYPE_STRUCT);
  if (sym) {
    sym->type_info.composite_info.struct_name = malloc(strlen(name) + 1);
    if (sym->type_info.composite_info.struct_name) {
      strcpy(sym->type_info.composite_info.struct_name, name);
    }
    sym->type_info.composite_info.members = members;
  }
  return sym;
}

symbol *create_class_symbol(const char *name, symbol_table *members) {
  symbol *sym = create_symbol(name, SYMBOL_CLASS, TYPE_CLASS);
  if (sym) {
    sym->type_info.composite_info.struct_name = malloc(strlen(name) + 1);
    if (sym->type_info.composite_info.struct_name) {
      strcpy(sym->type_info.composite_info.struct_name, name);
    }
    sym->type_info.composite_info.members = members;
  }
  return sym;
}

// Error reporting functions
void report_redeclaration_error(const char *name, int line, int prev_line) {
  fprintf(stderr,
          "Error at line %d: Redeclaration of '%s' (previously declared at "
          "line %d)\n",
          line, name, prev_line);
}

void report_undeclared_error(const char *name, int line) {
  fprintf(stderr, "Error at line %d: Undeclared identifier '%s'\n", line, name);
}

void report_type_mismatch_error(const char *name, int line, data_type expected,
                                data_type actual) {
  fprintf(stderr,
          "Error at line %d: Type mismatch for '%s' - expected %s, got %s\n",
          line, name, type_to_string(expected), type_to_string(actual));
}

void report_unused_variable_warning(const char *name, int line) {
  fprintf(stderr, "Warning at line %d: Unused variable '%s'\n", line, name);
}

// Print symbol table
void print_symbol_table(symbol_table *table) {
  print_symbol_table_to_file(table, stdout);
}

void print_symbol_table_to_file(symbol_table *table, FILE *file) {
  if (!table || !file)
    return;

  fprintf(file, "\n=== Symbol Table ===\n");
  fprintf(file, "Scope: %s (Level %d)\n",
          table->scope_name ? table->scope_name : "global", table->scope_level);
  fprintf(file, "Symbols: %d\n\n", table->count);

  fprintf(file, "%-15s %-12s %-10s %-8s %-10s %-8s\n", "Name", "Type",
          "Data Type", "Line", "Storage", "Used");
  fprintf(file,
          "---------------------------------------------------------------\n");

  for (int i = 0; i < table->size; i++) {
    symbol *current = table->table[i];
    while (current) {
      fprintf(file, "%-15s %-12s %-10s %-8d %-10s %-8s\n", current->name,
              symbol_type_to_string(current->sym_type),
              type_to_string(current->data_type), current->line_declared,
              storage_class_to_string(current->storage),
              current->is_used ? "Yes" : "No");
      current = current->next;
    }
  }
  fprintf(file, "\n");
}

void print_all_scopes(symbol_table *table, int depth) {
  if (!table)
    return;

  for (int i = 0; i < depth; i++) {
    printf("  ");
  }

  printf("Scope: %s (Level %d, %d symbols)\n",
         table->scope_name ? table->scope_name : "global", table->scope_level,
         table->count);

  for (int i = 0; i < table->child_count; i++) {
    print_all_scopes(table->children[i], depth + 1);
  }
}

// Semantic analysis helpers
int check_function_signature(symbol *func_sym, data_type return_type,
                             int param_count, data_type *param_types) {
  if (!func_sym || func_sym->sym_type != SYMBOL_FUNCTION)
    return 0;

  if (func_sym->type_info.function_info.return_type != return_type)
    return 0;
  if (func_sym->type_info.function_info.param_count != param_count)
    return 0;

  for (int i = 0; i < param_count; i++) {
    if (!are_types_compatible(func_sym->type_info.function_info.param_types[i],
                              param_types[i])) {
      return 0;
    }
  }

  return 1;
}

int check_array_bounds(symbol *array_sym, int index) {
  if (!array_sym || array_sym->data_type != TYPE_ARRAY)
    return 0;

  if (index < 0 || index >= array_sym->type_info.array_info.size) {
    return 0;
  }

  return 1;
}

int check_member_access(symbol *struct_sym, const char *member_name,
                        access_modifier current_access) {
  if (!struct_sym || (struct_sym->data_type != TYPE_STRUCT && struct_sym->data_type != TYPE_CLASS)) {
    return 0;
  }

  symbol_table *members = struct_sym->type_info.composite_info.members;
  if (!members)
    return 0;

  symbol *member = lookup_symbol_current_scope(members, member_name);
  if (!member)
    return 0;

  // Check access permissions for class members
  if (struct_sym->data_type == TYPE_CLASS) {
    if (member->access == ACCESS_PRIVATE && current_access != ACCESS_PRIVATE) {
      return 0;
    }
    if (member->access == ACCESS_PROTECTED &&
        current_access != ACCESS_PRIVATE &&
        current_access != ACCESS_PROTECTED) {
      return 0;
    }
  }

  return 1;
}

// Utility functions for adding members
int add_struct_member(symbol *struct_sym, symbol *member) {
  if (!struct_sym || !member || struct_sym->data_type != TYPE_STRUCT) {
    return 0;
  }

  if (!struct_sym->type_info.composite_info.members) {
    struct_sym->type_info.composite_info.members = create_symbol_table();
  }

  return insert_symbol(struct_sym->type_info.composite_info.members, member);
}

int add_class_member(symbol *class_sym, symbol *member,
                     access_modifier access) {
  if (!class_sym || !member || class_sym->data_type != TYPE_CLASS) {
    return 0;
  }

  if (!class_sym->type_info.composite_info.members) {
    class_sym->type_info.composite_info.members = create_symbol_table();
  }

  member->access = access;
  return insert_symbol(class_sym->type_info.composite_info.members, member);
}
void print_all_tables_recursively(symbol_table *table, int depth) {
    if (!table) return;

    // This part prints the current table's contents using your existing function.
    print_symbol_table_to_file(table, stdout);

    // This part recursively prints all child tables.
    for (int i = 0; i < table->child_count; i++) {
        print_all_tables_recursively(table->children[i], depth + 1);
    }
}