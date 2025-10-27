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
    arr_type->data.base_info.num_dimensions = 0; 
    // We will add dimensions later in semantic analysis
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
    
    return type_to_string_recursive(type, buffer);
}


Type* get_type_from_specifiers(ASTNodeList* specifiers) {
    if (!specifiers) return create_type(TYPE_INT); 

    Type* type = NULL;
    int type_specified = 0;
    
    // Used to store qualifiers found in the specifier list
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
                    // This is an anonymous struct/union. It must be defined here. The definition itself is handled by the pre-pass in analyze_node.
            
                    type = create_type(spec_node->data.struct_or_union_specifier.kind == STRUCT ? TYPE_STRUCT : TYPE_UNION);
                    type_specified = 1;
                }
                break;
            }
            
            case NODE_CLASS_SPECIFIER: {
                 const char* class_name = spec_node->data.class_specifier.name;
                 Symbol* sym = find_symbol(class_name);
                 if (sym && sym->type->kind == TYPE_STRUCT) { 
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
        type = create_type(TYPE_INT); 
    }
    
    type->is_const = is_const;
    type->is_unsigned = is_unsigned;
    type->storage_class = storage;

    return type;
}

Type* build_type_from_declarator(Type* base_type, ASTNode* declarator) {
    if (!declarator) return base_type;

    Type* final_type = base_type;
    ASTNode* current = declarator;

    // 1. Find the innermost part 
    ASTNode* innermost = declarator;
    while (innermost && innermost->type != NODE_IDENTIFIER && innermost->type != NODE_QUALIFIED_ID) {
        switch(innermost->type) {
            case NODE_POINTER_DECLARATOR:
                innermost = innermost->data.pointer_declarator.base_declarator;
                break;
            case NODE_ARRAY_DECLARATOR:
                innermost = innermost->data.array_declarator.base_declarator;
                break;
            case NODE_FUNCTION_DECLARATOR:
                innermost = innermost->data.function_declarator.base_declarator;
                break;
            case NODE_REFERENCE_DECLARATOR:
                innermost = innermost->data.reference_declarator.base_declarator;
                break;
            default:
                innermost = NULL; 
                break;
        }
    }

    // 2. Build type outwards from the base_type, following the chain back to the original declarator
    Type* current_type = base_type;
    current = declarator;

    while (current && current != innermost) {
        switch (current->type) {
            case NODE_POINTER_DECLARATOR:
                current_type = create_pointer_type(current_type);
                if (current->data.pointer_declarator.pointer && current->data.pointer_declarator.pointer->data.pointer.qualifiers) {
                    for(ASTNodeList* q = current->data.pointer_declarator.pointer->data.pointer.qualifiers; q; q=q->next) {
                         if(q->node->data.specifier == CONST) current_type->is_const = 1;
                        
                    }
                }
                current = current->data.pointer_declarator.base_declarator;
                break;
            case NODE_REFERENCE_DECLARATOR:
                current_type = create_reference_type(current_type);
                current = current->data.reference_declarator.base_declarator;
                break;
            case NODE_ARRAY_DECLARATOR: {
                int size = 0;
                if (current->data.array_declarator.size) {
                    if (current->data.array_declarator.size->type == NODE_CONSTANT) {
                         size = atoi(current->data.array_declarator.size->data.stringValue);
                         if (size <= 0) {
                              fprintf(stderr, "Semantic Error line %d: Array size must be positive.\n", current->lineno);
                              semantic_errors++;
                              size = 0; 
                         }
                    } else {
                         fprintf(stderr, "Semantic Error line %d: Array size must be a constant integer expression.\n", current->lineno);
                         semantic_errors++;
                         size = 0; 
                    }
                } else {
                     size = 0; 
                }

                // Create a NEW array type whose base is the current_type
                Type* new_array_type = create_type(TYPE_ARRAY);
                new_array_type->data.base_info.base = current_type; 
                new_array_type->data.base_info.num_dimensions = 1; 
                new_array_type->data.base_info.dimensions[0] = size;

                // If the element type itself was already an array, copy its dimensions
                if (current_type->kind == TYPE_ARRAY && current_type->data.base_info.num_dimensions > 0) {
                     if (1 + current_type->data.base_info.num_dimensions <= MAX_ARRAY_DIMENSIONS) {
                         new_array_type->data.base_info.num_dimensions += current_type->data.base_info.num_dimensions;
                         for (int i = 0; i < current_type->data.base_info.num_dimensions; ++i) {
                             new_array_type->data.base_info.dimensions[i + 1] = current_type->data.base_info.dimensions[i];
                         }
                     } else {
                          fprintf(stderr, "Semantic Error line %d: Exceeded maximum array dimensions (%d).\n", current->lineno, MAX_ARRAY_DIMENSIONS);
                          semantic_errors++;
                     }
                }

                current_type = new_array_type; 
                current = current->data.array_declarator.base_declarator;
                break;
            }
            case NODE_FUNCTION_DECLARATOR: {
                Type* func_type = create_type(TYPE_FUNCTION);
                func_type->data.function_sig.return_type = current_type; 
                func_type->data.function_sig.params = current->data.function_declarator.parameters;
                
                // Variadic functions
                func_type->data.function_sig.is_variadic = false; 
                if (func_type->data.function_sig.params) {
                    ASTNodeList* p = func_type->data.function_sig.params;
                    while (p->next) {
                        p = p->next;
                    }
                    // If the last item in the list is a NULL node, it's variadic.
                    if (p->node == NULL) {
                        func_type->data.function_sig.is_variadic = true;
                    }
                }
                current_type = func_type;
                current = current->data.function_declarator.base_declarator;
                break;
            }
            default: 
                 current = NULL; 
                 break;
        }
    }

    final_type = current_type;
    final_type->is_const = base_type->is_const;
    final_type->is_unsigned = base_type->is_unsigned;
    final_type->storage_class = base_type->storage_class;

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
        return 1; 
    }
    // Allow assignment between any two arithmetic types 
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
