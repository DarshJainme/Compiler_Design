#ifndef AST_H
#define AST_H

// Enum to identify the type of AST node
typedef enum {
    // Literals and identifiers
    NODE_UNKNOWN = 0,
    NODE_TRANSLATION_UNIT,
    NODE_IDENTIFIER,
    NODE_CONSTANT,
    NODE_STRING_LITERAL,
    NODE_TYPENAME,

    // Expressions
    NODE_BINARY_EXPR,
    NODE_UNARY_EXPR,
    NODE_POSTFIX_UNARY_EXPR, // For b++ and b--
    NODE_PREFIX_UNARY_EXPR,  // For ++b and --b
    NODE_CONDITIONAL_EXPR,
    NODE_ASSIGNMENT,
    NODE_FUNC_CALL,
    NODE_ARRAY_SUBSCRIPT, // e.g., arr[i]
    NODE_MEMBER_ACCESS,   // e.g., obj.field or obj->field
    NODE_CAST_EXPRESSION,
    NODE_NEW_EXPRESSION,
    NODE_DELETE_EXPRESSION,
    NODE_LAMBDA_EXPR,

    // Statements
    NODE_COMPOUND_STATEMENT,
    NODE_EXPRESSION_STATEMENT,
    NODE_IF_STATEMENT,
    NODE_SWITCH_STATEMENT,
    NODE_CASE_STATEMENT,
    NODE_DEFAULT_STATEMENT,
    NODE_WHILE_STATEMENT,
    NODE_UNTIL_STATEMENT,
    NODE_DO_WHILE_STATEMENT,
    NODE_FOR_STATEMENT,
    NODE_GOTO_STATEMENT,
    NODE_CONTINUE_STATEMENT,
    NODE_BREAK_STATEMENT,
    NODE_RETURN_STATEMENT,
    NODE_LABELED_STATEMENT,

    // Declarations
    NODE_DECLARATION,
    NODE_FUNCTION_DEFINITION,
    NODE_PARAMETER_DECLARATION,
    NODE_INITIALIZER_LIST,
    NODE_INIT_DECLARATOR,

    // Declarator types
    NODE_POINTER_DECLARATOR,
    NODE_ARRAY_DECLARATOR,
    NODE_FUNCTION_DECLARATOR,
    NODE_REFERENCE_DECLARATOR,
    
    // Specifiers and Qualifiers
    NODE_SPECIFIER,
    NODE_TYPE_QUALIFIER,
    NODE_STORAGE_CLASS_SPECIFIER,
    NODE_TYPE_SPECIFIER,
    NODE_POINTER,
    NODE_STRUCT_OR_UNION_SPECIFIER,
    NODE_ENUM_SPECIFIER,
    NODE_ENUMERATOR,   // for RED, GREEN, BLUE in an enum list
    
    // Class-related nodes
    NODE_CLASS_SPECIFIER,
    NODE_ACCESS_SPECIFIER,
    NODE_BASE_CLASS,

    // added for scope operators
    NODE_QUALIFIED_ID,
    // Utility nodes
    NODE_ARGUMENT_LIST,    // Generic list for function arguments
    NODE_LIST_ITEM         // Generic list for everything else
} NodeType;

struct ASTNode;
// A generic list node
typedef struct ASTNodeList {
    struct ASTNode* node;
    struct ASTNodeList* next;
} ASTNodeList;

// this struct will be used inside the type system for structs/unions
typedef struct Member {
    char *name;
    struct Type *type;
    struct Member *next;
} Member;

// Main AST node structure
typedef struct ASTNode {
    NodeType type;
    int lineno; // Line number for error reporting

    union {
        // Literals and identifiers
        char* stringValue; // For identifiers, constants, string literals
        // Expressions
        struct {
            struct ASTNode* left;
            int op;
            struct ASTNode* right;
        } binary_expr;

        struct {
            int op;
            struct ASTNode* operand;
        } unary_expr;

        struct {
            struct ASTNode* type_name;
            struct ASTNode* expr;
        } cast_expr;
        
        struct {
            struct ASTNode* condition;
            struct ASTNode* true_expr;
            struct ASTNode* false_expr;
        } conditional_expr;

        struct {
            struct ASTNode* lvalue;
            int op;
            struct ASTNode* rvalue;
        } assignment;

        struct {
            struct ASTNode* function;
            ASTNodeList* arguments;
        } func_call;

        struct {
            struct ASTNode* array;
            struct ASTNode* index;
        } array_subscript;

        struct {
            struct ASTNode* object;
            char* member_name;
            int is_pointer;
        } member_access;
        
        struct {
            ASTNodeList* capture;
            ASTNodeList* params;
            struct ASTNode* body;
        } lambda_expr;

        struct {
            struct ASTNode* type_name;
            struct ASTNode* initializer;
        } new_expr;

        struct {
            struct ASTNode* expr;
        } delete_expr;

        // Statements
        struct {
            ASTNodeList* items;
        } compound_statement;

        struct {
            struct ASTNode* expression;
        } expression_statement;

        struct {
            struct ASTNode* condition;
            struct ASTNode* if_body;
            struct ASTNode* else_body;
        } if_statement;

        struct {
            struct ASTNode* expression;
            struct ASTNode* body;
        } switch_statement;

        struct {
            struct ASTNode* expression;
            struct ASTNode* body;
        } case_statement;

        struct {
            struct ASTNode* body;
        } default_statement;

        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_statement;

        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } until_statement;

        struct {
            struct ASTNode* body;
            struct ASTNode* condition;
        } do_while_statement;

        struct {
            struct ASTNode* init;
            struct ASTNode* condition;
            struct ASTNode* increment;
            struct ASTNode* body;
        } for_statement;

        struct {
            char* label;
        } goto_statement;

        struct {
            struct ASTNode* expression;
        } return_statement;

        struct {
            char* label;
            struct ASTNode* statement;
        } labeled_statement;

        // Declarations
        struct {
            ASTNodeList* specifiers;
            ASTNodeList* declarators;
        } declaration;
        
        struct {
            struct ASTNode* declarator;
            struct ASTNode* initializer;
        } init_declarator;

        struct {
            ASTNodeList* specifiers;
            struct ASTNode* declarator;
            struct ASTNode* body;
        } function_definition;

        struct {
            ASTNodeList* specifiers;
            struct ASTNode* declarator;
        } parameter_declaration;

        // Declarator specifics
        struct {
            struct ASTNode* base_declarator;
            struct ASTNode* size;
        } array_declarator;

        struct {
            struct ASTNode* base_declarator;
            ASTNodeList* parameters;
        } function_declarator;

        struct {
            struct ASTNode* pointer;
            struct ASTNode* base_declarator;
        } pointer_declarator;

        struct {
            struct ASTNode* base_declarator;
        } reference_declarator;// for pass by refeerence operarator
        
        struct {
            ASTNodeList* qualifiers;
            struct ASTNode* next_pointer;
        } pointer;

        // Specifiers
        int specifier; // For simple specifiers like PUBLIC, PRIVATE, INT, etc.

        struct {
            int kind; // STRUCT or UNION token
            char* name;
            ASTNodeList* members;
        } struct_or_union_specifier;

        struct {
            char *name;
            ASTNodeList *members;
        } enum_specifier;

        // Add this new struct for an enumerator member
        struct {
            char *name;
            struct ASTNode *value; // Can be NULL
        } enumerator;

        struct {
            char* name;
            ASTNodeList* base_classes;
            ASTNodeList* members;
        } class_specifier;
        
        struct {
            int access_specifier;
            struct ASTNode* class_name;
        } base_class;

        struct {
            ASTNodeList* qualifiers;
            struct ASTNode* identifier;
        } qualified_id;
        // Generic list for the root and other lists
        ASTNodeList* items_list;

    } data;
} ASTNode;


// Helper functions to create various AST nodes
ASTNode* create_node(NodeType type);
ASTNodeList* create_list_node(ASTNode* node);
ASTNodeList* append_to_list(ASTNodeList* list, ASTNode* node);

// Node creation functions
ASTNode* create_identifier_node(char* name);
ASTNode* create_constant_node(char* value);
ASTNode* create_string_literal_node(char* value);
ASTNode* create_typename_node(char* name);
ASTNode* create_binary_expr_node(ASTNode* left, int op, ASTNode* right);
ASTNode* create_unary_expr_node(int op, ASTNode* operand);
ASTNode* create_cast_expr_node(ASTNode* type_name, ASTNode* expr);
ASTNode* create_conditional_expr_node(ASTNode* condition, ASTNode* true_expr, ASTNode* false_expr);
ASTNode* create_assignment_node(ASTNode* lvalue, int op, ASTNode* rvalue);
ASTNode* create_func_call_node(ASTNode* function, ASTNodeList* arguments);
ASTNode* create_array_subscript_node(ASTNode* array, ASTNode* index);
ASTNode* create_member_access_node(ASTNode* object, char* member_name, int is_pointer);
ASTNode* create_if_statement_node(ASTNode* condition, ASTNode* if_body, ASTNode* else_body);
ASTNode* create_for_statement_node(ASTNode* init, ASTNode* condition, ASTNode* increment, ASTNode* body);
ASTNode* create_compound_statement_node(ASTNodeList* items);
ASTNode* create_switch_statement_node(ASTNode* expression, ASTNode* body);
ASTNode* create_case_statement_node(ASTNode* expression, ASTNode* body);
ASTNode* create_default_statement_node(ASTNode* body);
ASTNode* create_while_statement_node(ASTNode* condition, ASTNode* body);
ASTNode* create_until_statement_node(ASTNode* condition, ASTNode* body);
ASTNode* create_do_while_statement_node(ASTNode* body, ASTNode* condition);
ASTNode* create_goto_statement_node(char* label);
ASTNode* create_return_statement_node(ASTNode* expression);
ASTNode* create_labeled_statement_node(char* label, ASTNode* statement);
ASTNode* create_expression_statement_node(ASTNode* expression);
ASTNode* create_declaration_node(ASTNodeList* specifiers, ASTNodeList* declarators);
ASTNode* create_init_declarator_node(ASTNode* declarator, ASTNode* initializer);
ASTNode* create_function_definition_node(ASTNodeList* specifiers, ASTNode* declarator, ASTNode* body);
ASTNode* create_parameter_declaration_node(ASTNodeList* specifiers, ASTNode* declarator);
ASTNode* create_specifier_node(int specifier_token);
ASTNode* create_pointer_node(ASTNodeList* qualifiers, ASTNode* next);
ASTNode* create_pointer_declarator_node(ASTNode* pointer, ASTNode* base_declarator);
ASTNode* create_array_declarator_node(ASTNode* base_declarator, ASTNode* size);
ASTNode* create_function_declarator_node(ASTNode* base_declarator, ASTNodeList* params);
ASTNode* create_reference_declarator_node(ASTNode* base_declarator);
ASTNode* create_struct_or_union_specifier_node(int kind, char* name, ASTNodeList* members);
ASTNode* create_class_node(char* name, ASTNodeList* base_classes, ASTNodeList* members);
ASTNode* create_access_specifier_node(int specifier);
ASTNode* create_base_class_node(int specifier, ASTNode* class_name);
ASTNode* create_enum_specifier_node(char* name, ASTNodeList* members);
ASTNode* create_lambda_node(ASTNodeList* capture, ASTNodeList* params, ASTNode* body);
ASTNode* create_new_expr_node(ASTNode* type_name, ASTNode* initializer);
ASTNode* create_delete_expr_node(ASTNode* expr);
ASTNode *create_enumerator_node(char *name, ASTNode *value);
ASTNode *create_initializer_list_node(ASTNodeList *items);
// added for the scope resoluion operator.
ASTNode* create_qualified_id_node(ASTNodeList* qualifiers, ASTNode* identifier);

// Function to print the AST for debugging
void print_ast(ASTNode* node, int indent);
const char* token_to_string(int token);

#endif // AST_H
