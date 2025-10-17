#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h" // For token definitions
extern int yylineno;

// --- Generic Node and List Creation ---
ASTNode* create_node(NodeType type) {
    ASTNode* node = (ASTNode*)calloc(1, sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed in create_node\n");
        exit(1);
    }
    node->type = type;
    node->lineno = yylineno;
    return node;
}

ASTNodeList* create_list_node(ASTNode* node) {
    ASTNodeList* list_node = (ASTNodeList*)malloc(sizeof(ASTNodeList));
    if (!list_node) {
        fprintf(stderr, "Error: Memory allocation failed in create_list_node\n");
        exit(1);
    }
    list_node->node = node;
    list_node->next = NULL;
    return list_node;
}

ASTNodeList* append_to_list(ASTNodeList* list, ASTNode* node) {
    if (!list) return create_list_node(node);
    ASTNodeList* current = list;
    while (current->next) current = current->next;
    current->next = create_list_node(node);
    return list;
}

// --- Specific Node Creators ---
ASTNode* create_identifier_node(char* name){
    ASTNode* node = create_node(NODE_IDENTIFIER);
    node->data.stringValue = strdup(name);
    return node;
}
ASTNode* create_constant_node(char* value){
    ASTNode* node = create_node(NODE_CONSTANT);
    node->data.stringValue = strdup(value);
    return node;
}
ASTNode* create_string_literal_node(char* value){
    ASTNode* node = create_node(NODE_STRING_LITERAL);
    node->data.stringValue = strdup(value);
    return node;
}
ASTNode* create_typename_node(char* name){
    ASTNode* node = create_node(NODE_TYPENAME);
    node->data.stringValue = strdup(name);
    return node;
}
ASTNode* create_binary_expr_node(ASTNode* left, int op, ASTNode* right){
    ASTNode* node = create_node(NODE_BINARY_EXPR);
    node->data.binary_expr.left = left;
    node->data.binary_expr.op = op;
    node->data.binary_expr.right = right;
    return node;
}
ASTNode* create_unary_expr_node(int op, ASTNode* operand){
    ASTNode* node = create_node(NODE_UNARY_EXPR);
    node->data.unary_expr.op = op;
    node->data.unary_expr.operand = operand;
    return node;
}
ASTNode* create_cast_expr_node(ASTNode* type_name, ASTNode* expr){
    ASTNode* node = create_node(NODE_CAST_EXPRESSION);
    node->data.cast_expr.type_name = type_name;
    node->data.cast_expr.expr = expr;
    return node;
}
ASTNode* create_conditional_expr_node(ASTNode* condition, ASTNode* true_expr, ASTNode* false_expr){
    ASTNode* node = create_node(NODE_CONDITIONAL_EXPR);
    node->data.conditional_expr.condition = condition;
    node->data.conditional_expr.true_expr = true_expr;
    node->data.conditional_expr.false_expr = false_expr;
    return node;
}
ASTNode* create_assignment_node(ASTNode* lvalue, int op, ASTNode* rvalue){
    ASTNode* node = create_node(NODE_ASSIGNMENT);
    node->data.assignment.lvalue = lvalue;
    node->data.assignment.op = op;
    node->data.assignment.rvalue = rvalue;
    return node;
}
ASTNode* create_func_call_node(ASTNode* function, ASTNodeList* arguments){
    ASTNode* node = create_node(NODE_FUNC_CALL);
    node->data.func_call.function = function;
    node->data.func_call.arguments = arguments;
    return node;
}
ASTNode* create_array_subscript_node(ASTNode* array, ASTNode* index){
    ASTNode* node = create_node(NODE_ARRAY_SUBSCRIPT);
    node->data.array_subscript.array = array;
    node->data.array_subscript.index = index;
    return node;
}
ASTNode* create_member_access_node(ASTNode* object, char* member_name, int is_pointer){
    ASTNode* node = create_node(NODE_MEMBER_ACCESS);
    node->data.member_access.object = object;
    node->data.member_access.member_name = strdup(member_name);
    node->data.member_access.is_pointer = is_pointer;
    return node;
}
ASTNode* create_compound_statement_node(ASTNodeList* items){
    ASTNode* node = create_node(NODE_COMPOUND_STATEMENT);
    node->data.compound_statement.items = items;
    return node;
}
ASTNode* create_if_statement_node(ASTNode* condition, ASTNode* if_body, ASTNode* else_body){
    ASTNode* node = create_node(NODE_IF_STATEMENT);
    node->data.if_statement.condition = condition;
    node->data.if_statement.if_body = if_body;
    node->data.if_statement.else_body = else_body;
    return node;
}
ASTNode* create_switch_statement_node(ASTNode* expression, ASTNode* body){
    ASTNode* node = create_node(NODE_SWITCH_STATEMENT);
    node->data.switch_statement.expression = expression;
    node->data.switch_statement.body = body;
    return node;
}
ASTNode* create_case_statement_node(ASTNode* expression, ASTNode* body){
    ASTNode* node = create_node(NODE_CASE_STATEMENT);
    node->data.case_statement.expression = expression;
    node->data.case_statement.body = body;
    return node;
}
ASTNode* create_default_statement_node(ASTNode* body){
    ASTNode* node = create_node(NODE_DEFAULT_STATEMENT);
    node->data.default_statement.body = body;
    return node;
}
ASTNode* create_while_statement_node(ASTNode* condition, ASTNode* body){
    ASTNode* node = create_node(NODE_WHILE_STATEMENT);
    node->data.while_statement.condition = condition;
    node->data.while_statement.body = body;
    return node;
}
ASTNode* create_do_while_statement_node(ASTNode* body, ASTNode* condition){
    ASTNode* node = create_node(NODE_DO_WHILE_STATEMENT);
    node->data.do_while_statement.body = body;
    node->data.do_while_statement.condition = condition;
    return node;
}
ASTNode* create_for_statement_node(ASTNode* init, ASTNode* condition, ASTNode* increment, ASTNode* body){
    ASTNode* node = create_node(NODE_FOR_STATEMENT);
    node->data.for_statement.init = init;
    node->data.for_statement.condition = condition;
    node->data.for_statement.increment = increment;
    node->data.for_statement.body = body;
    return node;
}
ASTNode* create_goto_statement_node(char* label){
    ASTNode* node = create_node(NODE_GOTO_STATEMENT);
    node->data.goto_statement.label = strdup(label);
    return node;
}
ASTNode* create_return_statement_node(ASTNode* expression){
    ASTNode* node = create_node(NODE_RETURN_STATEMENT);
    node->data.return_statement.expression = expression;
    return node;
}
ASTNode* create_expression_statement_node(ASTNode* expression){
    ASTNode* node = create_node(NODE_EXPRESSION_STATEMENT);
    node->data.expression_statement.expression = expression;
    return node;
}
ASTNode* create_labeled_statement_node(char* label, ASTNode* statement){
    ASTNode* node = create_node(NODE_LABELED_STATEMENT);
    node->data.labeled_statement.label = strdup(label);
    node->data.labeled_statement.statement = statement;
    return node;
}
ASTNode* create_declaration_node(ASTNodeList* specifiers, ASTNodeList* declarators){
    ASTNode* node = create_node(NODE_DECLARATION);
    node->data.declaration.specifiers = specifiers;
    node->data.declaration.declarators = declarators;
    return node;
}
ASTNode* create_init_declarator_node(ASTNode* declarator, ASTNode* initializer){
    ASTNode* node = create_node(NODE_INIT_DECLARATOR);
    node->data.init_declarator.declarator = declarator;
    node->data.init_declarator.initializer = initializer;
    return node;
}
ASTNode* create_function_definition_node(ASTNodeList* specifiers, ASTNode* declarator, ASTNode* body){
    ASTNode* node = create_node(NODE_FUNCTION_DEFINITION);
    node->data.function_definition.specifiers = specifiers;
    node->data.function_definition.declarator = declarator;
    node->data.function_definition.body = body;
    return node;
}
ASTNode* create_parameter_declaration_node(ASTNodeList* specifiers, ASTNode* declarator){
    ASTNode* node = create_node(NODE_PARAMETER_DECLARATION);
    node->data.parameter_declaration.specifiers = specifiers;
    node->data.parameter_declaration.declarator = declarator;
    return node;
}
ASTNode* create_specifier_node(int specifier_token){
    ASTNode* node = create_node(NODE_SPECIFIER);
    node->data.specifier = specifier_token;
    return node;
}
ASTNode* create_pointer_node(ASTNodeList* qualifiers, ASTNode* next){
    ASTNode* node = create_node(NODE_POINTER);
    node->data.pointer.qualifiers = qualifiers;
    node->data.pointer.next_pointer = next;
    return node;
}
ASTNode* create_pointer_declarator_node(ASTNode* pointer, ASTNode* base_declarator){
    ASTNode* node = create_node(NODE_POINTER_DECLARATOR);
    node->data.pointer_declarator.pointer = pointer;
    node->data.pointer_declarator.base_declarator = base_declarator;
    return node;
}
ASTNode* create_array_declarator_node(ASTNode* base_declarator, ASTNode* size){
    ASTNode* node = create_node(NODE_ARRAY_DECLARATOR);
    node->data.array_declarator.base_declarator = base_declarator;
    node->data.array_declarator.size = size;
    return node;
}
ASTNode* create_function_declarator_node(ASTNode* base_declarator, ASTNodeList* params){
    ASTNode* node = create_node(NODE_FUNCTION_DECLARATOR);
    node->data.function_declarator.base_declarator = base_declarator;
    node->data.function_declarator.parameters = params;
    return node;
}
ASTNode* create_struct_or_union_specifier_node(int kind, char* name, ASTNodeList* members){
    ASTNode* node = create_node(NODE_STRUCT_OR_UNION_SPECIFIER);
    node->data.struct_or_union_specifier.kind = kind;
    node->data.struct_or_union_specifier.name = name ? strdup(name) : NULL;
    node->data.struct_or_union_specifier.members = members;
    return node;
}
ASTNode* create_class_node(char* name, ASTNodeList* base_classes, ASTNodeList* members){
    ASTNode* node = create_node(NODE_CLASS_SPECIFIER);
    node->data.class_specifier.name = name ? strdup(name) : NULL;
    node->data.class_specifier.base_classes = base_classes;
    node->data.class_specifier.members = members;
    return node;
}
ASTNode* create_access_specifier_node(int specifier){
    ASTNode* node = create_node(NODE_ACCESS_SPECIFIER);
    node->data.specifier = specifier;
    return node;
}
ASTNode* create_base_class_node(int specifier, ASTNode* class_name){
    ASTNode* node = create_node(NODE_BASE_CLASS);
    node->data.base_class.access_specifier = specifier;
    node->data.base_class.class_name = class_name;
    return node;
}
ASTNode* create_enum_specifier_node(char* name, ASTNodeList* members){
    ASTNode* node = create_node(NODE_ENUM_SPECIFIER);
    node->data.enum_specifier.name = name ? strdup(name) : NULL;
    node->data.enum_specifier.members = members;
    return node;
}
ASTNode* create_lambda_node(ASTNodeList* capture, ASTNodeList* params, ASTNode* body){
    ASTNode* node = create_node(NODE_LAMBDA_EXPR);
    node->data.lambda_expr.capture = capture;
    node->data.lambda_expr.params = params;
    node->data.lambda_expr.body = body;
    return node;
}
ASTNode* create_new_expr_node(ASTNode* type_name, ASTNode* initializer){
    ASTNode* node = create_node(NODE_NEW_EXPRESSION);
    node->data.new_expr.type_name = type_name;
    node->data.new_expr.initializer = initializer;
    return node;
}
ASTNode* create_delete_expr_node(ASTNode* expr){
    ASTNode* node = create_node(NODE_DELETE_EXPRESSION);
    node->data.delete_expr.expr = expr;
    return node;
}
ASTNode *create_enumerator_node(char *name, ASTNode *value) {
    ASTNode *node = create_node(NODE_ENUMERATOR);
    node->data.enumerator.name = name;
    node->data.enumerator.value = value;
    return node;
}
ASTNode *create_initializer_list_node(ASTNodeList *items) {
    ASTNode *node = create_node(NODE_INITIALIZER_LIST);
    node->data.items_list = items;
    return node;
}

// --- AST Printing for Debugging ---

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  "); // Use 2 spaces for indentation
}

const char* token_to_string(int token) {
    switch(token) {
        case '+': return "+"; case '-': return "-"; case '*': return "*"; case '/': return "/";
        case '%': return "%"; case '<': return "<"; case '>': return ">"; case '&': return "&";
        case '|': return "|"; case '^': return "^"; case '!': return "!"; case '=': return "=";
        case LE_OP: return "<="; case GE_OP: return ">="; case EQ_OP: return "=="; case NE_OP: return "!=";
        case LAND_OP: return "&&"; case LOR_OP: return "||"; case INC_OP: return "++"; case DEC_OP: return "--";
        case LSHIFT_OP: return "<<"; case RSHIFT_OP: return ">>"; case ADD_ASSIGN: return "+="; case SUB_ASSIGN: return "-=";
        case MUL_ASSIGN: return "*="; case DIV_ASSIGN: return "/="; case MOD_ASSIGN: return "%=";
        case LSHIFT_ASSIGN: return "<<="; case RSHIFT_ASSIGN: return ">>="; case AND_ASSIGN: return "&=";
        case OR_ASSIGN: return "|="; case XOR_ASSIGN: return "^="; case INT: return "INT"; case CHAR: return "CHAR";
        case VOID: return "VOID"; case FLOAT: return "FLOAT"; case DOUBLE: return "DOUBLE"; case LONG: return "LONG";
        case SHORT: return "SHORT"; case SIGNED: return "SIGNED"; case UNSIGNED: return "UNSIGNED"; case BOOL: return "BOOL";
        case STATIC: return "STATIC"; case EXTERN: return "EXTERN"; case CONST: return "CONST"; case VOLATILE: return "VOLATILE";
        case TYPEDEF: return "TYPEDEF"; case STRUCT: return "STRUCT"; case UNION: return "UNION"; case CLASS: return "CLASS";
        case PUBLIC: return "PUBLIC"; case PRIVATE: return "PRIVATE"; case PROTECTED: return "PROTECTED";
        case STRING: return "STRING";
        default: return "UNKNOWN_TOKEN";
    }
}

void print_ast(ASTNode* node, int indent) {
    if (!node) {
        return;
    }

    print_indent(indent);

    switch (node->type) {
        case NODE_TRANSLATION_UNIT:
            printf("Program [line %d]\n", node->lineno);
            for (ASTNodeList* item = node->data.items_list; item; item = item->next) {
                print_ast(item->node, indent + 1);
            }
            break;
        case NODE_IDENTIFIER: printf("Identifier: %s\n", node->data.stringValue); break;
        case NODE_CONSTANT: printf("Constant: %s\n", node->data.stringValue); break;
        case NODE_STRING_LITERAL: printf("String Literal: %s\n", node->data.stringValue); break;
        case NODE_TYPENAME: printf("Typename: %s\n", node->data.stringValue); break;
        case NODE_EXPRESSION_STATEMENT:
            printf("Expression Statement [line %d]\n", node->lineno);
            print_ast(node->data.expression_statement.expression, indent + 1);
            break;

        case NODE_ASSIGNMENT:
             printf("Assignment (op: %s) [line %d]\n", token_to_string(node->data.assignment.op), node->lineno);
             print_indent(indent + 1); printf("L-Value:\n");
             print_ast(node->data.assignment.lvalue, indent + 2);
             print_indent(indent + 1); printf("R-Value:\n");
             print_ast(node->data.assignment.rvalue, indent + 2);
             break;

        case NODE_UNARY_EXPR:
            printf("Unary Expr (op: %s) [line %d]\n", token_to_string(node->data.unary_expr.op), node->lineno);
            print_indent(indent + 1); printf("Operand:\n");
            print_ast(node->data.unary_expr.operand, indent + 2);
            break;
        
        case NODE_BINARY_EXPR:
            printf("Binary Expr (op: %s) [line %d]\n", token_to_string(node->data.binary_expr.op), node->lineno);
            print_indent(indent + 1); printf("Left:\n");
            print_ast(node->data.binary_expr.left, indent + 2);
            print_indent(indent + 1); printf("Right:\n");
            print_ast(node->data.binary_expr.right, indent + 2);
            break;
            
        case NODE_CONDITIONAL_EXPR:
            printf("Conditional Expr (?:) [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Condition:\n");
            print_ast(node->data.conditional_expr.condition, indent+2);
            print_indent(indent + 1); printf("True-Expr:\n");
            print_ast(node->data.conditional_expr.true_expr, indent+2);
            print_indent(indent + 1); printf("False-Expr:\n");
            print_ast(node->data.conditional_expr.false_expr, indent+2);
            break;
        
        case NODE_FUNC_CALL:
            printf("Function Call [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Function:\n");
            print_ast(node->data.func_call.function, indent + 2);
            print_indent(indent + 1); printf("Arguments:\n");
            for (ASTNodeList* arg = node->data.func_call.arguments; arg; arg = arg->next) {
                print_ast(arg->node, indent + 2);
            }
            break;

        case NODE_DECLARATION:
            printf("Declaration [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Specifiers:\n");
            for (ASTNodeList* spec = node->data.declaration.specifiers; spec; spec = spec->next) {
                print_ast(spec->node, indent + 2);
            }
            print_indent(indent + 1); printf("Declarators:\n");
            for (ASTNodeList* decl = node->data.declaration.declarators; decl; decl = decl->next) {
                print_ast(decl->node, indent + 2);
            }
            break;

        case NODE_INIT_DECLARATOR:
             printf("Init Declarator\n");
             print_indent(indent + 1); printf("Declarator:\n");
             print_ast(node->data.init_declarator.declarator, indent+2);
             print_indent(indent + 1); printf("Initializer:\n");
             print_ast(node->data.init_declarator.initializer, indent+2);
             break;

        case NODE_FUNCTION_DEFINITION:
            printf("Function Definition [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Specifiers:\n");
            for (ASTNodeList* spec = node->data.function_definition.specifiers; spec; spec = spec->next) {
                print_ast(spec->node, indent + 2);
            }
            print_indent(indent + 1); printf("Declarator:\n");
            print_ast(node->data.function_definition.declarator, indent + 2);
            print_indent(indent + 1); printf("Body:\n");
            print_ast(node->data.function_definition.body, indent + 2);
            break;

        case NODE_COMPOUND_STATEMENT:
            printf("Compound Statement [line %d]\n", node->lineno);
            for (ASTNodeList* item = node->data.compound_statement.items; item; item = item->next) {
                print_ast(item->node, indent + 1);
            }
            break;

        case NODE_SPECIFIER:
            printf("Specifier: %s\n", token_to_string(node->data.specifier));
            break;

        case NODE_POINTER_DECLARATOR:
            printf("Pointer Declarator\n");
            print_ast(node->data.pointer_declarator.pointer, indent + 2);
            print_ast(node->data.pointer_declarator.base_declarator, indent + 2);
            break;

        case NODE_POINTER:
             printf("Pointer\n");
             if(node->data.pointer.qualifiers){
                 print_indent(indent + 2); printf("Qualifiers:\n");
                 for (ASTNodeList* q = node->data.pointer.qualifiers; q; q=q->next){
                     print_ast(q->node, indent+4);
                 }
             }
             if(node->data.pointer.next_pointer){
                 print_ast(node->data.pointer.next_pointer, indent + 2);
             }
             break;

        case NODE_ARRAY_DECLARATOR:
            printf("Array Declarator\n");
            print_ast(node->data.array_declarator.base_declarator, indent + 2);
            print_indent(indent + 2); printf("Size:\n");
            print_ast(node->data.array_declarator.size, indent + 4);
            break;

        case NODE_FUNCTION_DECLARATOR:
            printf("Function Declarator\n");
            print_ast(node->data.function_declarator.base_declarator, indent + 2);
            print_indent(indent + 2); printf("Parameters:\n");
            for(ASTNodeList* p = node->data.function_declarator.parameters; p; p=p->next){
                print_ast(p->node, indent + 4);
            }
            break;

        case NODE_PARAMETER_DECLARATION:
            printf("Parameter Declaration\n");
            print_indent(indent + 2); printf("Specifiers:\n");
            for (ASTNodeList* spec = node->data.parameter_declaration.specifiers; spec; spec = spec->next) {
                print_ast(spec->node, indent + 4);
            }
            print_indent(indent + 2); printf("Declarator:\n");
            print_ast(node->data.parameter_declaration.declarator, indent + 4);
            break;
            
        case NODE_IF_STATEMENT:
            printf("If Statement [line %d]\n", node->lineno);
            print_indent(indent + 2); printf("Condition:\n");
            print_ast(node->data.if_statement.condition, indent+4);
            print_indent(indent + 2); printf("If Body:\n");
            print_ast(node->data.if_statement.if_body, indent+4);
            if(node->data.if_statement.else_body){
                print_indent(indent + 2); printf("Else Body:\n");
                print_ast(node->data.if_statement.else_body, indent+4);
            }
            break;

        case NODE_FOR_STATEMENT:
            printf("For Statement [line %d]\n", node->lineno);
            print_indent(indent + 2); printf("Init:\n");
            print_ast(node->data.for_statement.init, indent+4);
            print_indent(indent + 2); printf("Condition:\n");
            print_ast(node->data.for_statement.condition, indent+4);
            print_indent(indent + 2); printf("Increment:\n");
            print_ast(node->data.for_statement.increment, indent+4);
            print_indent(indent + 2); printf("Body:\n");
            print_ast(node->data.for_statement.body, indent+4);
            break;

        case NODE_WHILE_STATEMENT:
            printf("While Statement [line %d]\n", node->lineno);
            print_indent(indent + 2); printf("Condition:\n");
            print_ast(node->data.while_statement.condition, indent+4);
            print_indent(indent + 2); printf("Body:\n");
            print_ast(node->data.while_statement.body, indent+4);
            break;

        case NODE_DO_WHILE_STATEMENT:
            printf("Do-While Statement [line %d]\n", node->lineno);
            print_indent(indent + 2); printf("Body:\n");
            print_ast(node->data.do_while_statement.body, indent+4);
            print_indent(indent + 2); printf("Condition:\n");
            print_ast(node->data.do_while_statement.condition, indent+4);
            break;
        case NODE_SWITCH_STATEMENT:
            printf("Switch Statement [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Expression:\n");
            print_ast(node->data.switch_statement.expression, indent + 2);
            print_indent(indent + 1); printf("Body:\n");
            print_ast(node->data.switch_statement.body, indent + 2);
            break;
        case NODE_CASE_STATEMENT:
            printf("Case Statement [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Value:\n");
            print_ast(node->data.case_statement.expression, indent + 2);
            print_indent(indent + 1); printf("Body:\n");
            print_ast(node->data.case_statement.body, indent + 2);
            break;
        case NODE_DEFAULT_STATEMENT:
            printf("Default Statement [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Body:\n");
            print_ast(node->data.default_statement.body, indent + 2);
            break;

        case NODE_LABELED_STATEMENT:
            printf("Labeled Statement (label: %s) [line %d]\n", node->data.labeled_statement.label, node->lineno);
            print_ast(node->data.labeled_statement.statement, indent + 1);
            break;

        case NODE_GOTO_STATEMENT:
            printf("Goto Statement (label: %s) [line %d]\n", node->data.goto_statement.label, node->lineno);
            break;
        case NODE_BREAK_STATEMENT: printf("Break Statement\n"); break;
        case NODE_CONTINUE_STATEMENT: printf("Continue Statement\n"); break;
        
        case NODE_RETURN_STATEMENT:
            printf("Return Statement\n");
            print_ast(node->data.return_statement.expression, indent + 2);
            break;

        case NODE_CLASS_SPECIFIER:
            printf("Class: %s [line %d]\n", node->data.class_specifier.name, node->lineno);
            if(node->data.class_specifier.base_classes){
                print_indent(indent+2); printf("Base Classes:\n");
                for(ASTNodeList* b = node->data.class_specifier.base_classes; b; b=b->next){
                    print_ast(b->node, indent+4);
                }
            }
            print_indent(indent+2); printf("Members:\n");
            for(ASTNodeList* m = node->data.class_specifier.members; m; m=m->next){
                print_ast(m->node, indent+4);
            }
            break;
            
        case NODE_BASE_CLASS:
            printf("Base Class: %s\n", token_to_string(node->data.base_class.access_specifier));
            print_ast(node->data.base_class.class_name, indent+2);
            break;
            
        case NODE_ACCESS_SPECIFIER:
            printf("Access Specifier: %s\n", token_to_string(node->data.specifier));
            break;
        
        case NODE_CAST_EXPRESSION:
            printf("Cast Expression [line %d]\n", node->lineno);
            print_indent(indent + 1);
            printf("Type:\n");
            // The 'type_name' node itself is a declaration node, so we can print it
            print_ast(node->data.cast_expr.type_name, indent + 2); 
            print_indent(indent + 1);
            printf("Expr:\n");
            print_ast(node->data.cast_expr.expr, indent + 2);
            break;
        case NODE_ARRAY_SUBSCRIPT:
            printf("Array Subscript [line %d]\n", node->lineno);
            print_indent(indent + 1); printf("Array:\n");
            print_ast(node->data.array_subscript.array, indent + 2);
            print_indent(indent + 1); printf("Index:\n");
            print_ast(node->data.array_subscript.index, indent + 2);
            break;
            
        case NODE_MEMBER_ACCESS:
            printf("Member Access (op: %s) [line %d]\n", node->data.member_access.is_pointer ? "->" : ".", node->lineno);
            print_indent(indent + 1); printf("Object:\n");
            print_ast(node->data.member_access.object, indent + 2);
            print_indent(indent + 1); printf("Member: %s\n", node->data.member_access.member_name);
            break;

        case NODE_INITIALIZER_LIST:
            printf("Initializer List [line %d]\n", node->lineno);
            for (ASTNodeList* item = node->data.items_list; item; item = item->next) {
                print_ast(item->node, indent + 1);
            }
            break;

        case NODE_ENUM_SPECIFIER:
            printf("Enum Specifier: %s [line %d]\n", node->data.enum_specifier.name ? node->data.enum_specifier.name : "(anonymous)", node->lineno);
            for (ASTNodeList* item = node->data.enum_specifier.members; item; item = item->next) {
                print_ast(item->node, indent + 1);
            }
            break;
        
        case NODE_ENUMERATOR:
            printf("Enumerator: %s\n", node->data.enumerator.name);
            if (node->data.enumerator.value) {
                print_indent(indent + 1); printf("Value:\n");
                print_ast(node->data.enumerator.value, indent + 2);
            }
            break;
            
        case NODE_STRUCT_OR_UNION_SPECIFIER:
            printf("%s Specifier: %s [line %d]\n", 
                   node->data.struct_or_union_specifier.kind == STRUCT ? "Struct" : "Union", 
                   node->data.struct_or_union_specifier.name ? node->data.struct_or_union_specifier.name : "(anonymous)", 
                   node->lineno);
            if(node->data.struct_or_union_specifier.members) {
                 for (ASTNodeList* item = node->data.struct_or_union_specifier.members; item; item = item->next) {
                    print_ast(item->node, indent + 1);
                }
            }
            break;

        default:
            printf("Unknown node type: %d [line %d]\n", node->type, node->lineno);
            break;
    }
}
