#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"
#include "symbol_table.h"
#include "parser.tab.h"
#include "st.h"

extern int semantic_errors;
// static Type *current_function_return_type = NULL;

// Forward Declarations for Traversal
void analyze_node(ASTNode *node, FunctionAnalysisContext* context);
Type *analyze_expression(ASTNode *node);
void analyze_declaration(ASTNode *node);
void collect_labels(ASTNode* node, FunctionAnalysisContext* context);
void analyze_statement_with_context(ASTNode* node, FunctionAnalysisContext* context);

// --- First Pass: Label Collection ---
void collect_labels(ASTNode* node, FunctionAnalysisContext* context) {
    if (!node) return;

    switch(node->type) {
        case NODE_LABELED_STATEMENT: {
            char* label_name = node->data.labeled_statement.label;
            // Check for duplicate labels
            for (Label* l = context->labels; l; l = l->next) {
                if (!l->is_case_label && strcmp(l->name, label_name) == 0) {
                    fprintf(stderr, "Semantic Error (Line %d): Duplicate label '%s'.\n", node->lineno, label_name);
                    semantic_errors++;
                    break;
                }
            }
            Label* new_label = calloc(1, sizeof(Label));
            new_label->name = strdup(label_name);
            new_label->is_case_label = false;
            new_label->next = context->labels;
            context->labels = new_label;
            collect_labels(node->data.labeled_statement.statement, context);
            break;
        }
        case NODE_CASE_STATEMENT:
             // We don't need to collect case labels by value in this pass,
             // just note that we are inside a switch.
             collect_labels(node->data.case_statement.body, context);
             break;
        case NODE_DEFAULT_STATEMENT:
             collect_labels(node->data.default_statement.body, context);
             break;
        case NODE_COMPOUND_STATEMENT:
            for (ASTNodeList* item = node->data.compound_statement.items; item; item = item->next) {
                collect_labels(item->node, context);
            }
            break;
        case NODE_IF_STATEMENT:
            collect_labels(node->data.if_statement.if_body, context);
            if (node->data.if_statement.else_body) {
                collect_labels(node->data.if_statement.else_body, context);
            }
            break;
        case NODE_SWITCH_STATEMENT:
            collect_labels(node->data.switch_statement.body, context);
            break;
        case NODE_WHILE_STATEMENT:
        case NODE_DO_WHILE_STATEMENT:
        case NODE_FOR_STATEMENT:
            collect_labels(node->data.for_statement.body, context); // Assuming union has same member name
            break;
        default:
            // Other statements (expressions, jumps) don't contain labels.
            break;
    }
}


// --- Second Pass: Full Analysis with Context ---
void analyze_statement_with_context(ASTNode* node, FunctionAnalysisContext* context) {
    if (!node) return;

    switch (node->type) {
        case NODE_COMPOUND_STATEMENT:
            enter_scope();
            for (ASTNodeList *item = node->data.compound_statement.items; item; item = item->next) {
                // Use analyze_node which will delegate back to this function for statements
                analyze_node(item->node, context); 
            }
            leave_scope();
            break;

        case NODE_EXPRESSION_STATEMENT:
            if (node->data.expression_statement.expression) {
                analyze_expression(node->data.expression_statement.expression);
            }
            break;

        case NODE_IF_STATEMENT: {
            Type *cond_type = analyze_expression(node->data.if_statement.condition);
            if (cond_type && !is_scalar_type(cond_type)) { /* error */ }
            analyze_statement_with_context(node->data.if_statement.if_body, context);
            if (node->data.if_statement.else_body) {
                analyze_statement_with_context(node->data.if_statement.else_body, context);
            }
            break;
        }
        case NODE_WHILE_STATEMENT:
        case NODE_DO_WHILE_STATEMENT: {
            bool was_in_loop = context->in_loop; // Save previous state
            context->in_loop = true;

            Type *cond_type = NULL;
            if (node->type == NODE_WHILE_STATEMENT)
            {
                cond_type = analyze_expression(node->data.while_statement.condition);
                analyze_statement_with_context(node->data.while_statement.body, context);
            }
            else // NODE_DO_WHILE_STATEMENT
            {
                analyze_statement_with_context(node->data.do_while_statement.body, context);
                cond_type = analyze_expression(node->data.do_while_statement.condition);
            }

            if (cond_type && !is_scalar_type(cond_type))
            {
                fprintf(stderr, "Semantic Error (Line %d): Condition of statement must be a scalar type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
                semantic_errors++;
            }

            context->in_loop = was_in_loop; // Restore previous state
            break;
        }
        case NODE_FOR_STATEMENT: {
            bool was_in_loop = context->in_loop;
            context->in_loop = true;
            enter_scope(); // Scope for the loop variable and body
            // 1. Analyze the INIT part
            if (node->data.for_statement.init)
            {
                if (node->data.for_statement.init->type == NODE_DECLARATION)
                {
                    // If it's a declaration, analyze it as such
                    analyze_declaration(node->data.for_statement.init);
                }
                else
                {
                    // If it's an expression statement, unwrap and analyze the expression
                    ASTNode *init_expr = node->data.for_statement.init->data.expression_statement.expression;
                    if (init_expr)
                    {
                        analyze_expression(init_expr);
                    }
                }
            }

            // 2. Analyze the CONDITION part
            if (node->data.for_statement.condition)
            {
                // Unwrap the expression from the expression statement
                ASTNode *cond_expr = node->data.for_statement.condition->data.expression_statement.expression;
                if (cond_expr)
                {
                    Type *cond_type = analyze_expression(cond_expr);
                    if (cond_type && !is_scalar_type(cond_type))
                    {
                        fprintf(stderr, "Semantic Error (Line %d): Condition of for-loop must be a scalar type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
                        semantic_errors++;
                    }
                }
            }

            // 3. Analyze the INCREMENT part
            if (node->data.for_statement.increment)
            {
                // The increment part is a raw expression, so no unwrapping is needed here
                analyze_expression(node->data.for_statement.increment);
            }

            // 4. Analyze the BODY
            analyze_statement_with_context(node->data.for_statement.body, context); // Use the context-aware version
            leave_scope();
            context->in_loop = was_in_loop;
            break;
        }

        case NODE_SWITCH_STATEMENT: {
            Type *cond_type = analyze_expression(node->data.switch_statement.expression);
            if (!is_integer_type(cond_type)) { 
                fprintf(stderr, "Semantic Error (Line %d): Condition of switch statement must be an integer type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
                semantic_errors++;
            }
            context->in_switch = true;
            analyze_statement_with_context(node->data.switch_statement.body, context);
            context->in_switch = false;
            break;
        }

        case NODE_LABELED_STATEMENT:
            // The label was already validated in the first pass. Just analyze the statement.
            analyze_statement_with_context(node->data.labeled_statement.statement, context);
            break;

        case NODE_CASE_STATEMENT:
            if (!context->in_switch) {
                fprintf(stderr, "Semantic Error (Line %d): 'case' statement not in a switch.\n", node->lineno);
                semantic_errors++;
            }
            // Analyze the constant expression and the body
            analyze_expression(node->data.case_statement.expression);
            analyze_statement_with_context(node->data.case_statement.body, context);
            break;
        case NODE_DEFAULT_STATEMENT:
            if (!context->in_switch) {
                fprintf(stderr, "Semantic Error (Line %d): 'default' statement not in a switch.\n", node->lineno);
                semantic_errors++;
            }
            analyze_statement_with_context(node->data.default_statement.body, context);
            break;

        case NODE_GOTO_STATEMENT: {
            bool found = false;
            for (Label* l = context->labels; l; l = l->next) {
                if (!l->is_case_label && strcmp(l->name, node->data.goto_statement.label) == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                fprintf(stderr, "Semantic Error (Line %d): Use of undeclared label '%s'.\n", node->lineno, node->data.goto_statement.label);
                semantic_errors++;
            }
            break;
        }
        case NODE_CONTINUE_STATEMENT:
            if (!context->in_loop) {
                fprintf(stderr, "Semantic Error (Line %d): 'continue' statement not in a loop.\n", node->lineno);
                semantic_errors++;
            }
            break;
        case NODE_BREAK_STATEMENT:
            if (!context->in_loop && !context->in_switch) {
                fprintf(stderr, "Semantic Error (Line %d): 'break' statement not in a loop or switch.\n", node->lineno);
                semantic_errors++;
            }
            break;
        case NODE_RETURN_STATEMENT: {
            if (!context || !context->return_type)
            {
                fprintf(stderr, "Semantic Error (Line %d): return statement not in a function.\n", node->lineno);
                semantic_errors++;
            }
            else
            {
                Type *return_expr_type = node->data.return_statement.expression ? analyze_expression(node->data.return_statement.expression) : create_type(TYPE_VOID);
                if (!are_types_compatible(context->return_type, return_expr_type))
                {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible return type. Expected '%s' but got '%s'.\n", node->lineno, type_to_string(context->return_type), type_to_string(return_expr_type));
                    semantic_errors++;
                }
                else if (context->return_type->kind != return_expr_type->kind)
                {
                    // Insert cast node for implicit conversion
                    node->data.return_statement.expression = create_cast_expr_node(
                        create_typename_node(type_to_string(context->return_type)),
                        node->data.return_statement.expression);
                }
            }
            break;
        }
        
        default:
             break;
    }
}


// Helper function to extract the name from a declarator
const char *get_name_from_declarator(ASTNode *declarator)
{
    ASTNode *current = declarator;
    while (current)
    {
        if (current->type == NODE_IDENTIFIER)
        {
            return current->data.stringValue;
        }
        else if (current->type == NODE_POINTER_DECLARATOR)
        {
            current = current->data.pointer_declarator.base_declarator;
        }
        else if (current->type == NODE_ARRAY_DECLARATOR)
        {
            current = current->data.array_declarator.base_declarator;
        }
        else if (current->type == NODE_FUNCTION_DECLARATOR)
        {
            current = current->data.function_declarator.base_declarator;
        }
        else
        {
            return NULL; // Should not happen for valid declarators
        }
    }
    return NULL;
}

// Helper to infer type from constant string
Type *infer_constant_type(const char *val)
{
    int len = strlen(val);
    // Floating point: contains '.' or 'e' or 'E'
    if (strchr(val, '.') || strchr(val, 'e') || strchr(val, 'E'))
    {
        // Float suffix
        if (len > 1 && (val[len - 1] == 'f' || val[len - 1] == 'F'))
        {
            return create_type(TYPE_FLOAT);
        }
        // Long double suffix (not implemented, treat as double)
        if (len > 1 && (val[len - 1] == 'l' || val[len - 1] == 'L'))
        {
            return create_type(TYPE_DOUBLE);
        }
        // Default: double
        return create_type(TYPE_DOUBLE);
    }
    // Integer suffixes
    if (len > 2 && ((val[len - 2] == 'u' || val[len - 2] == 'U') && (val[len - 1] == 'l' || val[len - 1] == 'L')))
    {
        Type *t = create_type(TYPE_LONG);
        t->is_unsigned = 1;
        return t;
    }
    if (len > 2 && ((val[len - 2] == 'l' || val[len - 2] == 'L') && (val[len - 1] == 'u' || val[len - 1] == 'U')))
    {
        Type *t = create_type(TYPE_LONG);
        t->is_unsigned = 1;
        return t;
    }
    if (len > 1 && (val[len - 1] == 'l' || val[len - 1] == 'L'))
    {
        return create_type(TYPE_LONG);
    }
    if (len > 1 && (val[len - 1] == 'u' || val[len - 1] == 'U'))
    {
        Type *t = create_type(TYPE_INT);
        t->is_unsigned = 1;
        return t;
    }
    // Default: integer
    return create_type(TYPE_INT);
}

// Recursively check array initializer
int check_array_initializer(Type *array_type, ASTNode *initializer)
{
    if (!array_type || array_type->kind != TYPE_ARRAY)
        return 0;
    if (!initializer)
        return 1;
    if (initializer->type == NODE_INITIALIZER_LIST)
    {
        ASTNodeList *elems = initializer->data.items_list;
        for (ASTNodeList *e = elems; e; e = e->next)
        {
            if (array_type->data.base->kind == TYPE_ARRAY)
            {
                if (!check_array_initializer(array_type->data.base, e->node))
                    return 0;
            }
            else
            {
                Type *elem_type = analyze_expression(e->node);
                if (!are_types_compatible(array_type->data.base, elem_type))
                    return 0;
            }
        }
        return 1;
    }
    else
    {
        // Single value for array
        Type *elem_type = analyze_expression(initializer);
        return are_types_compatible(array_type->data.base, elem_type);
    }
}

// This function should be exactly as follows.
void analyze_enum_specifier(ASTNode *node) {
    if (!node || node->type != NODE_ENUM_SPECIFIER) return;

    ASTNodeList *members = node->data.enum_specifier.members;
    int current_value = 0;

    for (ASTNodeList *m = members; m; m = m->next) {
        ASTNode *member_node = m->node;
        // The parser fix ensures this node is now NODE_ENUMERATOR
        if (member_node && member_node->type == NODE_ENUMERATOR) {
            const char *name = member_node->data.enumerator.name;

            if (member_node->data.enumerator.value) {
                // For this assignment, we'll assume the value is a simple integer constant.
                // A full compiler would have a function to evaluate constant expressions here.
                ASTNode* val_node = member_node->data.enumerator.value;
                if(val_node->type == NODE_CONSTANT) {
                    current_value = atoi(val_node->data.stringValue);
                } else {
                     fprintf(stderr, "Semantic Error (Line %d): Enumerator value for '%s' is not a constant integer.\n", member_node->lineno, name);
                     semantic_errors++;
                }
            }
            
            // Create an integer type for the constant
            Type *enum_const_type = create_type(TYPE_INT);
            // Add the enumerator (e.g., GREEN) to the current symbol table scope
            add_symbol(name, enum_const_type, SYM_CONSTANT);
            
            current_value++; // Increment for the next enumerator
        }
    }
}

void analyze_struct_or_union_specifier(ASTNode *node, Type* type_being_built) {
    if (!node || node->type != NODE_STRUCT_OR_UNION_SPECIFIER) return;

    // According to the C standard, struct/union members live in their own scope.
    enter_scope(); 

    // This is the list of 'struct_declaration' nodes.
    ASTNodeList *member_decls = node->data.struct_or_union_specifier.members;
    Member *head = NULL, *tail = NULL;
    
    // Iterate through each member declaration (e.g., 'int i;', 'float f;')
    for (ASTNodeList *decl_item = member_decls; decl_item; decl_item = decl_item->next) {
        ASTNode *decl_node = decl_item->node;
        if(decl_node->type != NODE_DECLARATION) continue;

        // **CRITICAL:** We can now re-use the main analyze_declaration function.
        // It will add the member symbols ('i', 'f') to the *current scope*, 
        // which is the temporary scope we just created for the union.
        analyze_declaration(decl_node);
    }

    // Now, copy the symbols from the temporary scope into the Member list for the Type.
    Scope* member_scope = get_current_scope();
    for (int i = 0; i < member_scope->symbol_count; i++) {
        Symbol* member_symbol = member_scope->symbols[i];
        
        Member *new_member = (Member*)calloc(1, sizeof(Member));
        new_member->name = strdup(member_symbol->name);
        new_member->type = copy_type(member_symbol->type); // Important to copy
        
        if (!head) {
            head = tail = new_member;
        } else {
            tail->next = new_member;
            tail = new_member;
        }
    }
    
    leave_scope(); // Destroy the temporary scope for the members.

    // Attach the completed member list to the union's type information.
    type_being_built->data.struct_union_info.members = head;

    // Finally, if the struct/union has a tag name (like 'Data'), add it to the PARENT scope.
    const char* tag_name = node->data.struct_or_union_specifier.name;
    if (tag_name) {
        // We must add the type to the outer scope, not the member scope we just left.
        add_symbol(tag_name, type_being_built, SYM_TYPEDEF);
    }
}


// Add all function parameters to the current scope
void add_function_parameters(ASTNode *declarator)
{
    if (!declarator)
        return;
    ASTNode *current = declarator;
    // Find the function declarator node
    while (current)
    {
        if (current->type == NODE_FUNCTION_DECLARATOR)
        {
            // For each parameter, add to symbol table
            ASTNodeList *params = current->data.function_declarator.parameters;
            for (ASTNodeList *p = params; p; p = p->next)
            {
                ASTNode *param_decl = p->node;
                if (!param_decl)
                    continue;
                Type *param_type = get_type_from_specifiers(param_decl->data.parameter_declaration.specifiers);
                if (param_decl->data.parameter_declaration.declarator)
                {
                    param_type = build_type_from_declarator(param_type, param_decl->data.parameter_declaration.declarator);
                    const char *pname = get_name_from_declarator(param_decl->data.parameter_declaration.declarator);
                    if (pname)
                    {
                        add_symbol(pname, param_type, SYM_VARIABLE);
                    }
                }
            }
            break;
        }
        else if (current->type == NODE_POINTER_DECLARATOR)
        {
            current = current->data.pointer_declarator.base_declarator;
        }
        else if (current->type == NODE_ARRAY_DECLARATOR)
        {
            current = current->data.array_declarator.base_declarator;
        }
        else
        {
            break;
        }
    }
}

void analyze_declaration(ASTNode *node)
{
    Type *base_type = get_type_from_specifiers(node->data.declaration.specifiers);

    // Check if this is a typedef
    int is_typedef = 0;
    for (ASTNodeList *spec = node->data.declaration.specifiers; spec; spec = spec->next)
    {
        if (spec->node->type == NODE_SPECIFIER && spec->node->data.specifier == TYPEDEF)
        {
            is_typedef = 1;
            break;
        }
    }
    if (!node->data.declaration.declarators) {
        free(base_type); // The type is processed, we can free the temporary one.
        return;
    }
    if (node->data.declaration.declarators)
    {
        for (ASTNodeList *d = node->data.declaration.declarators; d; d = d->next)
        {
            ASTNode *init_decl = d->node;
            ASTNode *declarator = init_decl->data.init_declarator.declarator;
            const char *name = get_name_from_declarator(declarator);

            if (!name)
            {
                fprintf(stderr, "Semantic Error (Line %d): Declarator is missing a name.\n", node->lineno);
                semantic_errors++;
                continue;
            }

            Type *final_type = build_type_from_declarator(copy_type(base_type), declarator);

            if (is_typedef)
            {
                add_symbol(name, final_type, SYM_TYPEDEF);
                add_typename(name);
            }
            else
            {
                add_symbol(name, final_type, SYM_VARIABLE);
            }
            if (final_type->kind == TYPE_ARRAY && init_decl->data.init_declarator.initializer &&
                init_decl->data.init_declarator.initializer->type == NODE_INITIALIZER_LIST)
            {
                if (!check_array_initializer(final_type, init_decl->data.init_declarator.initializer))
                {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible types in array initialization of '%s'.\n", init_decl->lineno, name);
                    semantic_errors++;
                    continue;
                }
                // If you want, you can also check the number of elements matches the array size
                continue; // Skip the normal type check for arrays
            }
            if (init_decl->data.init_declarator.initializer)
            {
                Type *initializer_type = analyze_expression(init_decl->data.init_declarator.initializer);
                if (!are_types_compatible(final_type, initializer_type))
                {
                    fprintf(stderr, "Semantic Error (Line %d): Incompatible types in initialization of '%s'. Cannot assign '%s' to '%s'.\n",
                            init_decl->lineno, name, type_to_string(initializer_type), type_to_string(final_type));
                    semantic_errors++;
                }
                else if (final_type->kind != initializer_type->kind)
                {
                    // Insert cast node for implicit conversion
                    ASTNode *cast_node = create_cast_expr_node(create_typename_node(type_to_string(final_type)), init_decl->data.init_declarator.initializer);
                    init_decl->data.init_declarator.initializer = cast_node;
                }
            }
        }
    }
    free(base_type);
}

// In semantic.c, replace the entire function
void analyze_function_definition(ASTNode *node)
{
    Type *return_type = get_type_from_specifiers(node->data.function_definition.specifiers);
    const char *name = get_name_from_declarator(node->data.function_definition.declarator);
    if (!name) { 
        fprintf(stderr, "Semantic Error (Line %d): Function definition is missing a name.\n", node->lineno);
        semantic_errors++;
        return; 
    }

    Type *func_type = build_type_from_declarator(return_type, node->data.function_definition.declarator);
    add_symbol(name, func_type, SYM_FUNCTION);

    // --- NEW TWO-PASS ANALYSIS ---
    FunctionAnalysisContext context = {0};
    context.return_type = func_type->data.function_sig.return_type;

    // 1. First Pass: Collect all labels (goto, case, default)
    collect_labels(node->data.function_definition.body, &context);
    
    // 2. Second Pass: Full statement analysis
    enter_scope();
    add_function_parameters(node->data.function_definition.declarator);
    analyze_statement_with_context(node->data.function_definition.body, &context);
    leave_scope();

    // Free the collected labels
    Label* current = context.labels;
    while(current) {
        Label* next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}


// void analyze_statement(ASTNode *node)
// {
//     if (!node)
//         return;

//     switch (node->type)
//     {
//     case NODE_COMPOUND_STATEMENT:
//         enter_scope();
//         for (ASTNodeList *item = node->data.compound_statement.items; item; item = item->next)
//         {
//             analyze_node(item->node);
//         }
//         leave_scope();
//         break;
//     case NODE_EXPRESSION_STATEMENT:
//         if (node->data.expression_statement.expression)
//         {
//             analyze_expression(node->data.expression_statement.expression);
//         }
//         break;
//     case NODE_FOR_STATEMENT:
//     {
//         enter_scope(); // Scope for the loop variable and body

//         // 1. Analyze the INIT part
//         if (node->data.for_statement.init)
//         {
//             if (node->data.for_statement.init->type == NODE_DECLARATION)
//             {
//                 // If it's a declaration, analyze it as such
//                 analyze_declaration(node->data.for_statement.init);
//             }
//             else
//             {
//                 // If it's an expression statement, unwrap and analyze the expression
//                 ASTNode *init_expr = node->data.for_statement.init->data.expression_statement.expression;
//                 if (init_expr)
//                 {
//                     analyze_expression(init_expr);
//                 }
//             }
//         }

//         // 2. Analyze the CONDITION part
//         if (node->data.for_statement.condition)
//         {
//             // Unwrap the expression from the expression statement
//             ASTNode *cond_expr = node->data.for_statement.condition->data.expression_statement.expression;
//             if (cond_expr)
//             {
//                 Type *cond_type = analyze_expression(cond_expr);
//                 if (cond_type && !is_scalar_type(cond_type))
//                 {
//                     fprintf(stderr, "Semantic Error (Line %d): Condition of for-loop must be a scalar type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
//                     semantic_errors++;
//                 }
//             }
//         }

//         // 3. Analyze the INCREMENT part
//         if (node->data.for_statement.increment)
//         {
//             // The increment part is a raw expression, so no unwrapping is needed here
//             analyze_expression(node->data.for_statement.increment);
//         }

//         // 4. Analyze the BODY
//         analyze_statement(node->data.for_statement.body);

//         leave_scope(); // Exit the loop's scope
//         break;
//     }

//     case NODE_IF_STATEMENT:
//     case NODE_WHILE_STATEMENT:
//     case NODE_DO_WHILE_STATEMENT:
//     {
//         Type *cond_type = NULL;
//         if (node->type == NODE_IF_STATEMENT)
//         {
//             cond_type = analyze_expression(node->data.if_statement.condition);
//             analyze_statement(node->data.if_statement.if_body);
//             if (node->data.if_statement.else_body)
//                 analyze_statement(node->data.if_statement.else_body);
//         }
//         else if (node->type == NODE_WHILE_STATEMENT)
//         {
//             cond_type = analyze_expression(node->data.while_statement.condition);
//             analyze_statement(node->data.while_statement.body);
//         }
//         else if (node->type == NODE_DO_WHILE_STATEMENT)
//         {
//             analyze_statement(node->data.do_while_statement.body);
//             cond_type = analyze_expression(node->data.do_while_statement.condition);
//         }
//         if (cond_type && !is_scalar_type(cond_type))
//         {
//             fprintf(stderr, "Semantic Error (Line %d): Condition of statement must be a scalar type, but got '%s'.\n", node->lineno, type_to_string(cond_type));
//             semantic_errors++;
//         }
//         break;
//     }
//     case NODE_RETURN_STATEMENT:
//         if (!current_function_return_type)
//         {
//             fprintf(stderr, "Semantic Error (Line %d): return statement not in a function.\n", node->lineno);
//             semantic_errors++;
//         }
//         else
//         {
//             Type *return_expr_type = node->data.return_statement.expression ? analyze_expression(node->data.return_statement.expression) : create_type(TYPE_VOID);
//             if (!are_types_compatible(current_function_return_type, return_expr_type))
//             {
//                 fprintf(stderr, "Semantic Error (Line %d): Incompatible return type. Expected '%s' but got '%s'.\n", node->lineno, type_to_string(current_function_return_type), type_to_string(return_expr_type));
//                 semantic_errors++;
//             }
//             else if (current_function_return_type->kind != return_expr_type->kind)
//             {
//                 // Insert cast node for implicit conversion
//                 node->data.return_statement.expression = create_cast_expr_node(
//                     create_typename_node(type_to_string(current_function_return_type)),
//                     node->data.return_statement.expression);
//             }
//         }
//         break;
//     default:
//         // For simple statements like break, continue, etc., there's no analysis needed.
//         break;
//     }
// }

Type *analyze_expression(ASTNode *node)
{
    if (!node)
        return create_type(TYPE_VOID);

    switch (node->type)
    {
    case NODE_CONSTANT:
        return infer_constant_type(node->data.stringValue);
    case NODE_STRING_LITERAL:
        return create_type(TYPE_STRING);

    case NODE_IDENTIFIER:
    {
        Symbol *sym = find_symbol(node->data.stringValue);
        if (!sym)
        {
            fprintf(stderr, "Semantic Error (Line %d): Use of undeclared identifier '%s'.\n", node->lineno, node->data.stringValue);
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }
        if (sym->kind == SYM_TYPEDEF)
        {
            fprintf(stderr, "Semantic Error (Line %d): Cannot use a typename '%s' as an expression.\n", node->lineno, node->data.stringValue);
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }
        return sym->type;
    }

    case NODE_BINARY_EXPR:
    {
        Type *left_type = analyze_expression(node->data.binary_expr.left);
        Type *right_type = analyze_expression(node->data.binary_expr.right);

        // String concatenation
        if (node->data.binary_expr.op == '+')
        {
            if ((left_type->kind == TYPE_STRING || (left_type->kind == TYPE_POINTER && left_type->data.base->kind == TYPE_CHAR)) ||
                (right_type->kind == TYPE_STRING || (right_type->kind == TYPE_POINTER && right_type->data.base->kind == TYPE_CHAR)))
            {
                return create_type(TYPE_STRING);
            }
        }

        // Insert cast nodes for arithmetic type promotion
        if (is_arithmetic_type(left_type) && is_arithmetic_type(right_type))
        {
            Type *result_type = get_common_arithmetic_type(left_type, right_type);
            if (left_type->kind != result_type->kind)
            {
                node->data.binary_expr.left = create_cast_expr_node(
                    create_typename_node(type_to_string(result_type)),
                    node->data.binary_expr.left);
            }
            if (right_type->kind != result_type->kind)
            {
                node->data.binary_expr.right = create_cast_expr_node(
                    create_typename_node(type_to_string(result_type)),
                    node->data.binary_expr.right);
            }
            return result_type;
        }

        // Basic pointer arithmetic: ptr + int
        if (left_type->kind == TYPE_POINTER && is_integer_type(right_type))
            return left_type;
        if (is_integer_type(left_type) && right_type->kind == TYPE_POINTER)
            return right_type;

        fprintf(stderr, "Semantic Error (Line %d): Invalid operands for binary operator '%s'. Types are '%s' and '%s'.\n",
                node->lineno, token_to_string(node->data.binary_expr.op), type_to_string(left_type), type_to_string(right_type));
        semantic_errors++;
        return create_type(TYPE_UNKNOWN);
    }

    case NODE_ASSIGNMENT:
    {
        Type *lvalue_type = analyze_expression(node->data.assignment.lvalue);
        Type *rvalue_type = analyze_expression(node->data.assignment.rvalue);
        if (!are_types_compatible(lvalue_type, rvalue_type))
        {
            fprintf(stderr, "Semantic Error (Line %d): Incompatible types in assignment. Cannot assign '%s' to '%s'.\n",
                    node->lineno, type_to_string(rvalue_type), type_to_string(lvalue_type));
            semantic_errors++;
        }
        else if (lvalue_type->kind != rvalue_type->kind)
        {
            // Insert cast node for implicit conversion
            node->data.assignment.rvalue = create_cast_expr_node(
                create_typename_node(type_to_string(lvalue_type)),
                node->data.assignment.rvalue);
        }
        return lvalue_type;
    }

    case NODE_UNARY_EXPR:
    {
        Type *operand_type = analyze_expression(node->data.unary_expr.operand);
        switch (node->data.unary_expr.op)
        {
        case '&':
            return create_pointer_type(operand_type);
        case '*':
            if (operand_type->kind != TYPE_POINTER)
            {
                fprintf(stderr, "Semantic Error (Line %d): Cannot dereference a non-pointer type ('%s').\n", node->lineno, type_to_string(operand_type));
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }
            return operand_type->data.base;
        case '!':
            return create_type(TYPE_INT); // Logical NOT always returns an int (0 or 1)
        default:
            return operand_type; // For +/-/~/++/--
        }
    }

    case NODE_FUNC_CALL:
    {
        Type *func_expr_type = analyze_expression(node->data.func_call.function);
        if (func_expr_type->kind == TYPE_POINTER && func_expr_type->data.base->kind == TYPE_FUNCTION)
        {
            // Calling a function pointer
            return func_expr_type->data.base->data.function_sig.return_type;
        }
        if (func_expr_type->kind != TYPE_FUNCTION)
        {
            fprintf(stderr, "Semantic Error (Line %d): Called object is not a function or function pointer.\n", node->lineno);
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }
        return func_expr_type->data.function_sig.return_type;
    }

    case NODE_CONDITIONAL_EXPR:
    {
        Type *cond_type = analyze_expression(node->data.conditional_expr.condition);
        Type *true_type = analyze_expression(node->data.conditional_expr.true_expr);
        Type *false_type = analyze_expression(node->data.conditional_expr.false_expr);
        if (!is_scalar_type(cond_type))
        {
            fprintf(stderr, "Semantic Error (Line %d): Condition in ternary operator must be scalar, got '%s'.\n", node->lineno, type_to_string(cond_type));
            semantic_errors++;
        }
        if (are_types_compatible(true_type, false_type))
        {
            Type *result_type = get_common_arithmetic_type(true_type, false_type);
            // Insert cast nodes if needed
            if (true_type->kind != result_type->kind)
            {
                node->data.conditional_expr.true_expr = create_cast_expr_node(
                    create_typename_node(type_to_string(result_type)),
                    node->data.conditional_expr.true_expr);
            }
            if (false_type->kind != result_type->kind)
            {
                node->data.conditional_expr.false_expr = create_cast_expr_node(
                    create_typename_node(type_to_string(result_type)),
                    node->data.conditional_expr.false_expr);
            }
            return result_type;
        }
        else
        {
            fprintf(stderr, "Semantic Error (Line %d): Types in ternary operator branches are incompatible: '%s' and '%s'.\n", node->lineno, type_to_string(true_type), type_to_string(false_type));
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }
    }

    case NODE_CAST_EXPRESSION:
    {
        // Already a cast node, just return the type
        ASTNode *type_name_node = node->data.cast_expr.type_name;
        if (type_name_node && type_name_node->type == NODE_DECLARATION)
        {
            return get_type_from_specifiers(type_name_node->data.declaration.specifiers);
        }
        // Fallback: treat as unknown
        return create_type(TYPE_UNKNOWN);
    }

    case NODE_ARRAY_SUBSCRIPT:
    {
        Type *array_type = analyze_expression(node->data.array_subscript.array);
        Type *index_type = analyze_expression(node->data.array_subscript.index);

        if (array_type->kind != TYPE_ARRAY && array_type->kind != TYPE_POINTER) {
            fprintf(stderr, "Semantic Error (Line %d): Subscripted value is not an array or pointer.\n", node->lineno);
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }

        if (!is_integer_type(index_type)) {
            fprintf(stderr, "Semantic Error (Line %d): Array subscript is not an integer.\n", node->lineno);
            semantic_errors++;
        }

        return array_type->data.base; // The type of the result is the base type of the array
    }
    case NODE_MEMBER_ACCESS:
    {
        Type *object_type = analyze_expression(node->data.member_access.object);
        const char *member_name = node->data.member_access.member_name;

        if (node->data.member_access.is_pointer) {
            if (object_type->kind != TYPE_POINTER) {
                fprintf(stderr, "Semantic Error (Line %d): Request for member '%s' in something not a pointer to a structure or union.\n", node->lineno, member_name);
                semantic_errors++;
                return create_type(TYPE_UNKNOWN);
            }
            object_type = object_type->data.base; // Dereference the pointer to get the struct/union type
        }

        if (object_type->kind != TYPE_STRUCT && object_type->kind != TYPE_UNION) {
            fprintf(stderr, "Semantic Error (Line %d): Request for member '%s' in something not a structure or union.\n", node->lineno, member_name);
            semantic_errors++;
            return create_type(TYPE_UNKNOWN);
        }

        // Find the member in the type's member list
        for (Member *m = object_type->data.struct_union_info.members; m; m = m->next) {
            if (strcmp(m->name, member_name) == 0) {
                return m->type; // Found it, return its type
            }
        }

        fprintf(stderr, "Semantic Error (Line %d): No member named '%s' in struct or union.\n", node->lineno, member_name);
        semantic_errors++;
        return create_type(TYPE_UNKNOWN);
    }
    default:
        return create_type(TYPE_UNKNOWN);
    }
}

void analyze_node(ASTNode *node, FunctionAnalysisContext* context) // FIX: Pass context down
{
    if (!node)
        return;
    switch (node->type)
    {
    case NODE_TRANSLATION_UNIT:
        for (ASTNodeList *item = node->data.items_list; item; item = item->next)
        {
            analyze_node(item->node, NULL); // FIX: Pass NULL context for global scope
        }
        break;
    case NODE_DECLARATION:
        analyze_declaration(node);
        break;
    case NODE_FUNCTION_DEFINITION:
        analyze_function_definition(node);
        break;
    default:
        // If it's a statement, analyze it as such.
        // Expressions are handled recursively by statement analyzers.
        if (context) { // FIX: Only call this if we are inside a function
             analyze_statement_with_context(node, context);
        }
        break;
    }
}

int analyze_ast(ASTNode *root)
{
    if (!root)
        return 1; // Nothing to analyze
    semantic_errors = 0;
    init_symbol_table();
    analyze_node(root, NULL);
    return semantic_errors == 0;
}
