#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "parser.tab.h"

Type* create_type(TypeKind kind) {
    Type* new_type = (Type*)calloc(1, sizeof(Type));
    if (!new_type) {
        fprintf(stderr, "Fatal: Memory allocation failed for type.\n");
        exit(1);
    }
    new_type->kind = kind;
    return new_type;
}

Type* copy_type(Type* type) {
    if (!type) return NULL;
    Type* new_type = (Type*)malloc(sizeof(Type));
    memcpy(new_type, type, sizeof(Type));
    return new_type;
}

Type* create_pointer_type(Type* base) {
    Type* ptr_type = create_type(TYPE_POINTER);
    ptr_type->data.base = base;
    return ptr_type;
}

Type* create_array_type(Type* base, int size) {
    Type* arr_type = create_type(TYPE_ARRAY);
    arr_type->data.base = base;
    // You would store the size here if it's known
    return arr_type;
}

char* type_to_string_recursive(Type* type, char* buffer) {
    if (!type) return "null_type";
    
    char temp[128] = "";
    if (type->is_const) strcat(temp, "const ");
    if (type->is_unsigned) strcat(temp, "unsigned ");

    switch(type->kind) {
        case TYPE_VOID:   strcat(temp, "void"); break;
        case TYPE_BOOL:   strcat(temp, "bool"); break;
        case TYPE_CHAR:   strcat(temp, "char"); break;
        case TYPE_SHORT:  strcat(temp, "short"); break;
        case TYPE_INT:    strcat(temp, "int"); break;
        case TYPE_LONG:   strcat(temp, "long"); break;
        case TYPE_FLOAT:  strcat(temp, "float"); break;
        case TYPE_DOUBLE: strcat(temp, "double"); break;
        case TYPE_POINTER:
            sprintf(buffer, "%s*", type_to_string_recursive(type->data.base, temp));
            return buffer;
        case TYPE_ARRAY:
            sprintf(buffer, "%s[]", type_to_string_recursive(type->data.base, temp));
            return buffer;
        default: strcat(temp, "unknown_type"); break;
    }
    strcpy(buffer, temp);
    return buffer;
}

char* type_to_string(Type* type) {
    static char buffer[256];
    buffer[0] = '\0';
    return type_to_string_recursive(type, buffer);
}

Type* get_type_from_specifiers(ASTNodeList* specifiers) {
    if (!specifiers) return create_type(TYPE_INT); // Default int

    Type* type = create_type(TYPE_UNKNOWN);
    int type_specified = 0;

    for (ASTNodeList* s = specifiers; s; s = s->next) {
        if (s->node->type == NODE_SPECIFIER) {
            switch (s->node->data.specifier) {
                case VOID:   type->kind = TYPE_VOID;   type_specified = 1; break;
                case BOOL:   type->kind = TYPE_BOOL;   type_specified = 1; break;
                case CHAR:   type->kind = TYPE_CHAR;   type_specified = 1; break;
                case SHORT:  type->kind = TYPE_SHORT;  type_specified = 1; break;
                case INT:    type->kind = TYPE_INT;    type_specified = 1; break;
                case LONG:   type->kind = TYPE_LONG;   type_specified = 1; break;
                case FLOAT:  type->kind = TYPE_FLOAT;  type_specified = 1; break;
                case DOUBLE: type->kind = TYPE_DOUBLE; type_specified = 1; break;
                case UNSIGNED: type->is_unsigned = 1; break;
                case CONST:    type->is_const = 1; break;
            }
        } else if (s->node->type == NODE_TYPENAME) {
             // A real compiler would look up the typedef here.
             // For now, we assume it's some form of int.
             type->kind = TYPE_INT;
             type_specified = 1;
        }
    }
    if (!type_specified) {
        type->kind = TYPE_INT; // Default to int if only qualifiers like 'const' are present
    }
    return type;
}

Type* build_type_from_declarator(Type* base_type, ASTNode* declarator) {
    if (!declarator) return base_type;
    
    Type* final_type = base_type;
    ASTNode* current = declarator;
    
    // This loop peels layers off the declarator, building the type from the inside out.
    while (current->type != NODE_IDENTIFIER) {
        switch (current->type) {
            case NODE_POINTER_DECLARATOR:
                final_type = create_pointer_type(final_type);
                current = current->data.pointer_declarator.base_declarator;
                break;
            case NODE_ARRAY_DECLARATOR:
                final_type = create_array_type(final_type, 0); // Size not handled yet
                current = current->data.array_declarator.base_declarator;
                break;
            default: // Function declarators, etc.
                return final_type; // Stop for now
        }
    }
    return final_type;
}

int is_integer_type(Type* type) {
    if (!type) return 0;
    return type->kind >= TYPE_BOOL && type->kind <= TYPE_LONG;
}

int is_arithmetic_type(Type* type) {
    if (!type) return 0;
    return type->kind >= TYPE_BOOL && type->kind <= TYPE_DOUBLE;
}

int is_scalar_type(Type* type) {
    if (!type) return 0;
    return is_arithmetic_type(type) || type->kind == TYPE_POINTER;
}

int are_types_compatible(Type* type1, Type* type2) {
    if (!type1 || !type2) return 0;
    if (type1->kind == type2->kind) {
        if (type1->kind == TYPE_POINTER || type1->kind == TYPE_ARRAY) {
            return are_types_compatible(type1->data.base, type2->data.base);
        }
        return 1; // Basic types are the same
    }
    // Allow assignment between any two arithmetic types (e.g., int = float)
    if (is_arithmetic_type(type1) && is_arithmetic_type(type2)) {
        return 1;
    }
    return 0;
}

Type* get_common_arithmetic_type(Type* type1, Type* type2) {
    if (type1->kind == TYPE_DOUBLE || type2->kind == TYPE_DOUBLE) return create_type(TYPE_DOUBLE);
    if (type1->kind == TYPE_FLOAT || type2->kind == TYPE_FLOAT) return create_type(TYPE_FLOAT);
    if (type1->kind == TYPE_LONG || type2->kind == TYPE_LONG) return create_type(TYPE_LONG);
    return create_type(TYPE_INT);
}