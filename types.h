#ifndef TYPES_H
#define TYPES_H

#include "ast.h"
#include <stdbool.h>

#define MAX_ARRAY_DIMENSIONS 10
// Enum for basic type kinds
typedef enum {
    TYPE_UNKNOWN = 0,
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
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_ENUM,
    TYPE_REFERENCE,
    TYPE_STRING,
} TypeKind;

// Forward declarations
typedef struct Type Type;
struct Scope;

typedef struct StructUnionInfo {
    char *name;
    struct Member *members; // linked list of members for struct/union
    struct Scope* member_scope;
} StructUnionInfo;

// Structure to represent a type in our compiler
struct Type {
    TypeKind kind;
    bool is_const;
    bool is_unsigned;
    int storage_class;  // to store STATIC, EXTERN, etc.

    union {
        struct {
            Type* base;
            int dimensions[MAX_ARRAY_DIMENSIONS]; // Store dimension sizes here
            int num_dimensions;                   // Number of valid dimensions stored
        } base_info;

        // For functions
        struct {
            struct Type* return_type;
            ASTNodeList* params; // We'll link to AST nodes for param types
            bool is_variadic;
        } function_sig;
        
        // For structs and unions
        struct StructUnionInfo struct_union_info;
    } data;
};


// Core type creation
Type* create_type(TypeKind kind);
Type* copy_type(Type* type);

// Type construction helpers
Type* create_pointer_type(Type* base);
Type* create_reference_type(Type* base);
Type* create_array_type(Type* base, int size);
Type *create_enum_type(const char *name);
Type* create_type_struct_union(TypeKind kind, const char* name);
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