#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "parser.tab.h"
#include "semantic.h"
#include "symbol_table.h"
#include "ast.h"

Type* create_type(TypeKind kind) {
    Type* new_type = (Type*)calloc(1, sizeof(Type));
    if (!new_type) {
        fprintf(stderr, "Fatal: Memory allocation failed for type.\n");
        exit(1);
    }
    new_type->kind = kind;
    return new_type;
}

Type *create_type_struct_union(TypeKind k, const char *name) {
    Type *t = create_type(k);
    if (name) t->data.struct_union_info.name = strdup(name);
    return t;
}

Type *create_enum_type(const char *name) {
    Type *t = create_type(TYPE_ENUM);
    if (name) t->data.struct_union_info.name = strdup(name);
    t->data.struct_union_info.members = NULL;
    return t;
}

Type* copy_type(Type* type) {
    if (!type) return NULL;
    Type* new_type = (Type*)malloc(sizeof(Type));
    memcpy(new_type, type, sizeof(Type));
    return new_type;
}

Type* create_pointer_type(Type* base) {
    Type* ptr_type = create_type(TYPE_POINTER);
    ptr_type->data.base_info.base = base;
    return ptr_type;
}

Type* create_reference_type(Type* base) {
    Type* ref_type = create_type(TYPE_REFERENCE);
    ref_type->data.base_info.base = base;
    return ref_type;
}

Type* create_array_type(Type* base, int size) {
    Type* arr_type = create_type(TYPE_ARRAY);
    arr_type->data.base_info.base = base;
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
        case TYPE_STRING: strcat(temp, "string"); break;
        case TYPE_ENUM:   strcat(temp, "enum"); break;
        case TYPE_FUNCTION: strcat(temp, "function"); break;
        case TYPE_STRUCT:
            if (type->data.struct_union_info.name) {
                sprintf(temp, "struct %s", type->data.struct_union_info.name);
            } else {
                strcat(temp, "struct");
            }
            break;
        case TYPE_UNION:
            if (type->data.struct_union_info.name) {
                sprintf(temp, "union %s", type->data.struct_union_info.name);
            } else {
                strcat(temp, "union");
            }
            break;
        case TYPE_POINTER: {
            char base_buffer[128];
            sprintf(buffer, "%s*", type_to_string_recursive(type->data.base_info.base, base_buffer));
            return buffer;
        }
        case TYPE_REFERENCE: {
            char base_buffer[128];
            sprintf(buffer, "%s&", type_to_string_recursive(type->data.base_info.base, base_buffer));
            return buffer;
        }
        case TYPE_ARRAY: {
            char base_buffer_arr[128];
            sprintf(buffer, "%s[]", type_to_string_recursive(type->data.base_info.base, base_buffer_arr));
            return buffer;
        }
        default: strcat(temp, "unknown_type"); break;
    }
    strcpy(buffer, temp);
    return buffer;
}

char* type_to_string(Type* type) {
    static char buffer[256];
    buffer[0] = '\0';
    if (!type) {
        strcpy(buffer, "null_type");
        return buffer;
    }
    
    if (type->kind == TYPE_UNKNOWN) {
        strcpy(buffer, "unknown_type");
        return buffer;
    }
    
    // Now call the recursive helper for valid types
    return type_to_string_recursive(type, buffer);
}

// case VOID:   type->kind = TYPE_VOID;   type_specified = 1; break;
//                 case BOOL:   type->kind = TYPE_BOOL;   type_specified = 1; break;
//                 case CHAR:   type->kind = TYPE_CHAR;   type_specified = 1; break;
//                 case STRING: type->kind = TYPE_STRING; type_specified = 1; break;
//                 case SHORT:  type->kind = TYPE_SHORT;  type_specified = 1; break;
//                 case INT:    type->kind = TYPE_INT;    type_specified = 1; break;
//                 case LONG:   type->kind = TYPE_LONG;   type_specified = 1; break;
//                 case FLOAT:  type->kind = TYPE_FLOAT;  type_specified = 1; break;
//                 case DOUBLE: type->kind = TYPE_DOUBLE; type_specified = 1; break;
//                 case UNSIGNED: type->is_unsigned = 1; break;
//                 case CONST:    type->is_const = 1; break;
// In semantic.c

Type* get_type_from_specifiers(ASTNodeList* specifiers) {
    if (!specifiers) return create_type(TYPE_INT); // C default

    Type* type = NULL;
    int type_specified = 0;
    
    // These will store qualifiers found in the specifier list
    int is_const = 0;
    int is_unsigned = 0;
    int storage = 0;

    for (ASTNodeList* s = specifiers; s; s = s->next) {
        ASTNode *spec_node = s->node;
        if (!spec_node) continue;

        switch(spec_node->type) {
            case NODE_SPECIFIER:
                switch (spec_node->data.specifier) {
                    case VOID:   if (!type_specified) { type = create_type(TYPE_VOID); type_specified = 1; } break;
                    case BOOL:   if (!type_specified) { type = create_type(TYPE_BOOL); type_specified = 1; } break;
                    case CHAR:   if (!type_specified) { type = create_type(TYPE_CHAR); type_specified = 1; } break;
                    case STRING: if (!type_specified) { type = create_type(TYPE_STRING); type_specified = 1; } break;
                    case INT:    if (!type_specified) { type = create_type(TYPE_INT); type_specified = 1; } break;
                    case LONG:   if (!type_specified) { type = create_type(TYPE_LONG); type_specified = 1; } break;
                    case FLOAT:  if (!type_specified) { type = create_type(TYPE_FLOAT); type_specified = 1; } break;
                    case DOUBLE: if (!type_specified) { type = create_type(TYPE_DOUBLE); type_specified = 1; } break;
                    case UNSIGNED: is_unsigned = 1; break;
                    case CONST:    is_const = 1; break;
                    case STATIC: storage = STATIC; break;
                    case EXTERN: storage = EXTERN; break;
                    case AUTO: storage = AUTO; break;
                    case REGISTER: storage = REGISTER; break;
                    case TYPEDEF: storage = TYPEDEF; break;
                    default: break;
                }
                break;
            case NODE_TYPENAME: {
                Symbol* sym = find_symbol(spec_node->data.stringValue);
                if (sym && sym->kind == SYM_TYPEDEF) {
                    if (!type_specified) {
                        type = copy_type(sym->type);
                        type_specified = 1;
                    }
                } else {
                    fprintf(stderr, "Semantic Error (Line %d): Unknown type name '%s'.\n", spec_node->lineno, spec_node->data.stringValue);
                    semantic_errors++;
                }
                break;
            }

            case NODE_STRUCT_OR_UNION_SPECIFIER: {
                const char* tag_name = spec_node->data.struct_or_union_specifier.name;
                if (tag_name) {
                    Symbol* sym = find_symbol(tag_name);
                    if (sym && (sym->type->kind == TYPE_STRUCT || sym->type->kind == TYPE_UNION)) {
                        if (!type_specified) {
                            type = copy_type(sym->type);
                            type_specified = 1;
                        }
                    } else {
                         fprintf(stderr, "Semantic Error (Line %d): Unknown struct/union '%s'.\n", spec_node->lineno, tag_name);
                         semantic_errors++;
                    }
                } else if (!type_specified) {
                    // This is an anonymous struct/union. It must be defined here.
                    // The definition itself is handled by the pre-pass in analyze_node.
                    // Here, we create a placeholder. The calling context (analyze_declaration)
                    // will not be able to use this effectively without a name, which is correct.
                    type = create_type(spec_node->data.struct_or_union_specifier.kind == STRUCT ? TYPE_STRUCT : TYPE_UNION);
                    type_specified = 1;
                }
                break;
            }
            
            case NODE_CLASS_SPECIFIER: {
                 const char* class_name = spec_node->data.class_specifier.name;
                 Symbol* sym = find_symbol(class_name);
                 if (sym && sym->type->kind == TYPE_STRUCT) { // Classes are stored as structs
                     if (!type_specified) {
                         type = copy_type(sym->type);
                         type_specified = 1;
                     }
                 } else {
                     fprintf(stderr, "Semantic Error (Line %d): Unknown class name '%s'.\n", spec_node->lineno, class_name);
                     semantic_errors++;
                 }
                 break;
            }

            default:
                break;
        }
    }
    if (!type) {
        type = create_type(TYPE_INT); // Default to int if no other type was found
    }
    
    // Apply qualifiers at the end
    type->is_const = is_const;
    type->is_unsigned = is_unsigned;
    type->storage_class = storage;

    return type;
}

Type* build_type_from_declarator(Type* base_type, ASTNode* declarator) {
    if (!declarator) return base_type;
    
    Type* final_type = base_type;
    ASTNode* current = declarator;
    
    // This loop peels layers off the declarator, building the type from the inside out.
    while (current && current->type != NODE_IDENTIFIER) {
        switch (current->type) {
            case NODE_POINTER_DECLARATOR:
                final_type = create_pointer_type(final_type);
                current = current->data.pointer_declarator.base_declarator;
                break;
            case NODE_REFERENCE_DECLARATOR:
                final_type = create_reference_type(final_type);
                current = current->data.reference_declarator.base_declarator;
                break;
            case NODE_ARRAY_DECLARATOR:
                final_type = create_array_type(final_type, 0); // Size not handled yet
                current = current->data.array_declarator.base_declarator;
                break;
            case NODE_FUNCTION_DECLARATOR: {
                Type* func_type = create_type(TYPE_FUNCTION);
                func_type->data.function_sig.return_type = final_type; // The type built so far is the return type
                func_type->data.function_sig.params = current->data.function_declarator.parameters;
                final_type = func_type;
                current = current->data.function_declarator.base_declarator;
                break;
            }
            default: 
                current = NULL; // Stop for other node types
                break;
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
            return are_types_compatible(type1->data.base_info.base, type2->data.base_info.base);
        }
        return 1; // Basic types are the same
    }
    // Allow assignment between any two arithmetic types (e.g., int = float)
    if (is_arithmetic_type(type1) && is_arithmetic_type(type2)) {
        return 1;
    }
    // allowing an integer to be assigned to an enum type.
    if (type1->kind == TYPE_ENUM && is_integer_type(type2)) {
        return 1;
    }
    // Allow char array = string literal
    if (type1->kind == TYPE_ARRAY && type1->data.base_info.base->kind == TYPE_CHAR && type2->kind == TYPE_STRING) {
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