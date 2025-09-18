#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Symbol types */
typedef enum {
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_PARAMETER,
    SYMBOL_CLASS,
    SYMBOL_STRUCT,
    SYMBOL_TYPEDEF,
    SYMBOL_LABEL,
    SYMBOL_ENUM,
    SYMBOL_CONSTANT
} symbol_type;

/* Data types */
typedef enum {
    TYPE_VOID,
    TYPE_INT,
    TYPE_CHAR,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_FUNCTION,
    TYPE_STRUCT,
    TYPE_CLASS,
    TYPE_CUSTOM
} data_type;

/* Storage classes */
typedef enum {
    STORAGE_AUTO,
    STORAGE_STATIC,
    STORAGE_EXTERN,
    STORAGE_REGISTER
} storage_class;

/* Access modifiers for classes */
typedef enum {
    ACCESS_PUBLIC,
    ACCESS_PRIVATE,
    ACCESS_PROTECTED
} access_modifier;

/* Symbol table entry */
typedef struct symbol {
    char *name;                    /* Symbol name */
    symbol_type sym_type;          /* Symbol type */
    data_type data_type;          /* Data type */
    storage_class storage;         /* Storage class */
    access_modifier access;        /* Access modifier (for class members) */
    
    int line_declared;             /* Line where symbol was declared */
    int line_used;                 /* Line where symbol was last used */
    
    /* Type-specific information */
    union {
        struct {
            int size;              /* Size for arrays */
            int dimensions;        /* Number of dimensions */
            data_type element_type; /* Element type for arrays */
        } array_info;
        
        struct {
            data_type return_type; /* Return type for functions */
            int param_count;       /* Number of parameters */
            data_type *param_types; /* Parameter types */
            int is_varargs;        /* Variable arguments function */
        } function_info;
        
        struct {
            int level;             /* Pointer level (*, **, etc.) */
            data_type pointed_type; /* Type being pointed to */
        } pointer_info;
        
        struct {
            char *struct_name;     /* Name of struct/class */
            struct symbol_table *members; /* Member symbol table */
        } composite_info;
        
        struct {
            data_type actual_type; /* The actual type for typedef */
        } typedef_info;
        
        struct {
            union {
                int int_value;
                float float_value;
                char char_value;
                char *string_value;
            } value;
        } constant_info;
    } type_info;
    
    int is_initialized;            /* Whether variable is initialized */
    int is_used;                   /* Whether symbol has been used */
    int is_function_defined;       /* Whether function is defined or just declared */
    
    struct symbol *next;           /* Next symbol in hash chain */
} symbol;

/* Symbol table structure */
typedef struct symbol_table {
    symbol **table;                /* Hash table array */
    int size;                      /* Size of hash table */
    int count;                     /* Number of symbols */
    struct symbol_table *parent;   /* Parent scope */
    struct symbol_table **children; /* Child scopes */
    int child_count;               /* Number of child scopes */
    int scope_level;               /* Nesting level */
    char *scope_name;              /* Name of scope (function name, etc.) */
} symbol_table;

/* Function prototypes */

/* Symbol table management */
symbol_table *create_symbol_table(void);
symbol_table *create_child_scope(symbol_table *parent, const char *scope_name);
void destroy_symbol_table(symbol_table *table);
void print_symbol_table(symbol_table *table);
void print_symbol_table_to_file(symbol_table *table, FILE *file);
void print_all_scopes(symbol_table *table, int depth);
void print_all_tables_recursively(symbol_table *table, int depth);

/* Symbol operations */
symbol *create_symbol(const char *name, symbol_type sym_type, data_type data_type);
int insert_symbol(symbol_table *table, symbol *sym);
symbol *lookup_symbol(symbol_table *table, const char *name);
symbol *lookup_symbol_current_scope(symbol_table *table, const char *name);
symbol *lookup_symbol_all_scopes(symbol_table *table, const char *name);

/* Symbol modification */
void mark_symbol_used(symbol_table *table, const char *name, int line);
void mark_symbol_initialized(symbol_table *table, const char *name);
int update_symbol_type(symbol_table *table, const char *name, data_type new_type);

/* Type checking utilities */
int are_types_compatible(data_type type1, data_type type2);
int can_assign_types(data_type dest_type, data_type src_type);
const char *type_to_string(data_type type);
const char *symbol_type_to_string(symbol_type type);
const char *storage_class_to_string(storage_class storage);
const char *access_modifier_to_string(access_modifier access);

/* Array and pointer utilities */
symbol *create_array_symbol(const char *name, data_type element_type, int size, int dimensions);
symbol *create_pointer_symbol(const char *name, data_type pointed_type, int level);
symbol *create_function_symbol(const char *name, data_type return_type, int param_count, data_type *param_types);

/* Structure and class utilities */
symbol *create_struct_symbol(const char *name, symbol_table *members);
symbol *create_class_symbol(const char *name, symbol_table *members);
int add_struct_member(symbol *struct_sym, symbol *member);
int add_class_member(symbol *class_sym, symbol *member, access_modifier access);

/* Error reporting */
void report_redeclaration_error(const char *name, int line, int prev_line);
void report_undeclared_error(const char *name, int line);
void report_type_mismatch_error(const char *name, int line, data_type expected, data_type actual);
void report_unused_variable_warning(const char *name, int line);

/* Hash function */
unsigned int hash_function(const char *str, int table_size);

/* Semantic analysis helpers */
int check_function_signature(symbol *func_sym, data_type return_type, int param_count, data_type *param_types);
int check_array_bounds(symbol *array_sym, int index);
int check_member_access(symbol *struct_sym, const char *member_name, access_modifier current_access);

/* Global symbol table for program scope */
extern symbol_table *global_scope;
extern symbol_table *current_scope;

#endif /* SYMBOL_TABLE_H */
