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
        case TYPE_STRING: strcat(temp, "string"); break;
        case TYPE_ENUM:   strcat(temp, "enum"); break;
        case TYPE_FUNCTION: strcat(temp, "function"); break;
        case TYPE_STRUCT:  strcat(temp, "struct"); break;
        case TYPE_UNION:   strcat(temp, "union"); break;
        case TYPE_POINTER: {
            // Correctly handle recursive call
            char base_buffer[128];
            sprintf(buffer, "%s*", type_to_string_recursive(type->data.base, base_buffer));
            return buffer;
        }
        case TYPE_ARRAY: {
            // Correctly handle recursive call
            char base_buffer_arr[128];
            sprintf(buffer, "%s[]", type_to_string_recursive(type->data.base, base_buffer_arr));
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
    
    // --- THIS IS THE FIX ---
    if (type->kind == TYPE_UNKNOWN) {
        strcpy(buffer, "unknown_type");
        return buffer;
    }
    // --- END FIX ---
    
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
    if (!specifiers) return create_type(TYPE_INT); // Default to int if no specifiers

    Type* type = create_type(TYPE_UNKNOWN);
    int type_specified = 0;
    const char* user_defined_typename = NULL;
    ASTNode* struct_union_spec_node = NULL;

    // First pass: Find the primary type specifier (int, struct, typename, etc.)
    for (ASTNodeList* s = specifiers; s; s = s->next) {
        ASTNode *spec_node = s->node;
        if (spec_node->type == NODE_SPECIFIER) {
            switch (spec_node->data.specifier) {
                case VOID:   if (!type_specified) type->kind = TYPE_VOID;   type_specified = 1; break;
                case BOOL:   if (!type_specified) type->kind = TYPE_BOOL;   type_specified = 1; break;
                case CHAR:   if (!type_specified) type->kind = TYPE_CHAR;   type_specified = 1; break;
                case STRING: if (!type_specified) type->kind = TYPE_STRING; type_specified = 1; break;
                case SHORT:  if (!type_specified) type->kind = TYPE_SHORT;  type_specified = 1; break;
                case INT:    if (!type_specified) type->kind = TYPE_INT;    type_specified = 1; break;
                case LONG:   if (!type_specified) type->kind = TYPE_LONG;   type_specified = 1; break;
                case FLOAT:  if (!type_specified) type->kind = TYPE_FLOAT;  type_specified = 1; break;
                case DOUBLE: if (!type_specified) type->kind = TYPE_DOUBLE; type_specified = 1; break;
                default: break;
            }
        } else if (spec_node->type == NODE_TYPENAME) {
            user_defined_typename = spec_node->data.stringValue;
            type_specified = 1;
        } else if (spec_node->type == NODE_STRUCT_OR_UNION_SPECIFIER || spec_node->type == NODE_ENUM_SPECIFIER) {
            struct_union_spec_node = spec_node;
            type_specified = 1;
        }
    }

    // Second pass: Apply modifiers (const, unsigned)
    for (ASTNodeList* s = specifiers; s; s = s->next) {
        if (s->node->type == NODE_SPECIFIER) {
            if (s->node->data.specifier == UNSIGNED) type->is_unsigned = 1;
            if (s->node->data.specifier == CONST) type->is_const = 1;
        }
    }
    
    // Now, resolve the specific type
    if (user_defined_typename) {
        Symbol* sym = find_symbol(user_defined_typename);
        if (sym && sym->kind == SYM_TYPEDEF) {
            free(type); // Free the temp type
            Type* found_type = copy_type(sym->type);
            found_type->is_const = type->is_const; // Apply qualifiers from this declaration
            found_type->is_unsigned = type->is_unsigned;
            return found_type;
        } else {
            fprintf(stderr, "Semantic Error: Unknown type name '%s'.\n", user_defined_typename);
            semantic_errors++;
            type->kind = TYPE_UNKNOWN;
            return type;
        }
    } else if (struct_union_spec_node) {
        if (struct_union_spec_node->type == NODE_STRUCT_OR_UNION_SPECIFIER) {
            const char* tag_name = struct_union_spec_node->data.struct_or_union_specifier.name;
            ASTNodeList* members = struct_union_spec_node->data.struct_or_union_specifier.members;
            
            // Case 1: Definition (e.g., "struct T { ... }") or Anonymous Definition (e.g., "struct { ... }")
            if (members) {
                type->kind = (struct_union_spec_node->data.struct_or_union_specifier.kind == STRUCT) ? TYPE_STRUCT : TYPE_UNION;
                
                // If it has a name, it defines a new type in the symbol table.
                if (tag_name) {
                    // Check for redefinition in the current scope.
                    if (find_symbol_in_current_scope(tag_name)) {
                        fprintf(stderr, "Semantic Error: Redefinition of 'struct %s'.\n", tag_name);
                        semantic_errors++;
                    }
                    // Add the symbol *before* analyzing members to support recursive structures.
                    // We use SYM_TYPEDEF as a stand-in for a type tag.
                    add_symbol(tag_name, type, SYM_TYPEDEF);
                }
                
                // Now, analyze the members to populate the type information.
                analyze_struct_or_union_specifier(struct_union_spec_node, type);
                return type;
            }
            // Case 2: Usage (e.g., "struct T s;") or Forward Declaration (e.g., "struct T;")
            else if (tag_name) {
                Symbol* sym = find_symbol(tag_name);
                if (sym && sym->kind == SYM_TYPEDEF && (sym->type->kind == TYPE_STRUCT || sym->type->kind == TYPE_UNION)) {
                    free(type); // Free the temporary type.
                    Type* found_type = copy_type(sym->type); // Use the complete type from the symbol table.
                    found_type->is_const = type->is_const; // Apply qualifiers from this declaration
                    return found_type;
                } else {
                    // This could be a forward declaration. Create an incomplete type and add it to the table.
                    fprintf(stderr, "Semantic Warning: Using incomplete type 'struct %s'. Assuming forward declaration.\n", tag_name);
                    type->kind = (struct_union_spec_node->data.struct_or_union_specifier.kind == STRUCT) ? TYPE_STRUCT : TYPE_UNION;
                    type->data.struct_union_info.name = strdup(tag_name);
                    type->data.struct_union_info.members = NULL; // Mark as incomplete
                    if (!find_symbol_in_current_scope(tag_name)) {
                        add_symbol(tag_name, type, SYM_TYPEDEF);
                    }
                    return type;
                }
            }
        } else if (struct_union_spec_node->type == NODE_ENUM_SPECIFIER) {
            type->kind = TYPE_ENUM;
            analyze_enum_specifier(struct_union_spec_node);
            return type;
        }
    }

    if (!type_specified) {
        type->kind = TYPE_INT; // Default to int
    }

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
            return are_types_compatible(type1->data.base, type2->data.base);
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
    if (type1->kind == TYPE_ARRAY && type1->data.base->kind == TYPE_CHAR && type2->kind == TYPE_STRING) {
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