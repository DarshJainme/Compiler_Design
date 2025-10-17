#ifndef TYPES_H
#define TYPES_H

#include "ast.h"

// Enum for basic type kinds
typedef enum {
    TYPE_VOID,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_SHORT,
    TYPE_INT,
    TYPE_LONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_ARRAY,
    TYPE_POINTER,
    TYPE_FUNCTION,
    TYPE_STRUCT_UNION,
    TYPE_ENUM,
    TYPE_REFERENCE,
    TYPE_UNKNOWN
} TypeKind;

// Forward declarations
struct Type;
struct Symbol;

// Structure to represent a type in our compiler
typedef struct Type {
    TypeKind kind;
    unsigned int is_const : 1;
    unsigned int is_unsigned : 1;

    union {
        // For pointers and arrays
        struct Type* base;

        // For functions
        struct {
            struct Type* return_type;
            ASTNodeList* params; // We'll link to AST nodes for param types
        } function_sig;
        
        // For structs and unions
        struct {
            char* name;
            struct Scope* members; // A dedicated scope for struct members
        } struct_union_info;
    } data;
} Type;


// Core type creation
Type* create_type(TypeKind kind);
Type* copy_type(Type* type);

// Type construction helpers
Type* create_pointer_type(Type* base);
Type* create_reference_type(Type* base);
Type* create_array_type(Type* base, int size);
Type* build_type_from_declarator(Type* base_type, ASTNode* declarator);

// Type information and checking
Type* get_type_from_specifiers(ASTNodeList* specifiers);
int is_integer_type(Type* type);
int is_arithmetic_type(Type* type);
int is_scalar_type(Type* type);
int are_types_compatible(Type* type1, Type* type2);
Type* get_common_arithmetic_type(Type* type1, Type* type2);

// Utility
char* type_to_string(Type* type);

#endif // TYPES_H