#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "parser.tab.h" // For token definitions

TacInstr* tac_list_head = NULL;
TacInstr* tac_list_tail = NULL;

static int temp_count = 0;
static int label_count = 0;

/* --- Address Creation Helpers --- */

TacAddr* new_temp() {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_TEMP;
    addr->val.temp_id = temp_count++;
    return addr;
}

TacAddr* new_label_addr() {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_LABEL;
    addr->val.label_id = label_count++;
    return addr;
}

TacAddr* new_const(int val) {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_CONSTANT;
    addr->val.const_val = val;
    return addr;
}

TacAddr* new_var_addr(Symbol* sym) {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_VARIABLE;
    addr->val.var = sym;
    return addr;
}

TacAddr* new_string_addr(char* str_label) {
    TacAddr* addr = (TacAddr*)calloc(1, sizeof(TacAddr));
    addr->kind = ADDR_STRING;
    addr->val.string_label = strdup(str_label);
    return addr;
}

/* --- Core TAC Emitter --- */

void emit(TacOp op, TacAddr* res, TacAddr* arg1, TacAddr* arg2) {
    TacInstr* instr = (TacInstr*)calloc(1, sizeof(TacInstr));
    instr->op = op;
    instr->res = res;
    instr->arg1 = arg1;
    instr->arg2 = arg2;
    instr->next = NULL;
    instr->prev = tac_list_tail;

    if (tac_list_head == NULL) {
        tac_list_head = instr;
        tac_list_tail = instr;
    } else {
        tac_list_tail->next = instr;
        tac_list_tail = instr;
    }
}

/* --- TAC Printing Utility --- */

void print_addr(TacAddr* addr) {
    if (!addr) {
        printf("----");
        return;
    }
    switch (addr->kind) {
        case ADDR_VARIABLE: printf("%s", addr->val.var->name); break;
        case ADDR_TEMP:     printf("t%d", addr->val.temp_id); break;
        case ADDR_CONSTANT: printf("%d", addr->val.const_val); break;
        case ADDR_LABEL:    printf("L%d", addr->val.label_id); break;
        case ADDR_STRING:   printf("%s", addr->val.string_label); break;
        default:            printf("BAD_ADDR"); break;
    }
}

void print_tac() {
    printf("\n--- Three-Address Code ---\n");
    for (TacInstr* instr = tac_list_head; instr; instr = instr->next) {
        if (instr->op == TAC_LABEL) {
            print_addr(instr->res);
            printf(":");
        } else if (instr->op == TAC_BEGIN_FUNC || instr->op == TAC_END_FUNC) {
            printf("%s", (instr->op == TAC_BEGIN_FUNC) ? "begin_func" : "end_func");
        } else {
            printf("\t");
            if (instr->res) {
                print_addr(instr->res);
                printf(" = ");
            }

            switch (instr->op) {
                case TAC_ASSIGN: print_addr(instr->arg1); break;
                case TAC_ADD: print_addr(instr->arg1); printf(" + "); print_addr(instr->arg2); break;
                case TAC_SUB: print_addr(instr->arg1); printf(" - "); print_addr(instr->arg2); break;
                case TAC_MUL: print_addr(instr->arg1); printf(" * "); print_addr(instr->arg2); break;
                case TAC_DIV: print_addr(instr->arg1); printf(" / "); print_addr(instr->arg2); break;
                case TAC_EQ: print_addr(instr->arg1); printf(" == "); print_addr(instr->arg2); break;
                case TAC_NE: print_addr(instr->arg1); printf(" != "); print_addr(instr->arg2); break;
                case TAC_LT: print_addr(instr->arg1); printf(" < "); print_addr(instr->arg2); break;
                case TAC_LE: print_addr(instr->arg1); printf(" <= "); print_addr(instr->arg2); break;
                case TAC_GT: print_addr(instr->arg1); printf(" > "); print_addr(instr->arg2); break;
                case TAC_GE: print_addr(instr->arg1); printf(" >= "); print_addr(instr->arg2); break;
                case TAC_GOTO: printf("goto "); print_addr(instr->res); break;
                case TAC_IFZ: printf("ifz "); print_addr(instr->arg1); printf(" goto "); print_addr(instr->res); break;
                case TAC_RETURN: printf("return "); print_addr(instr->arg1); break;
                // We will be adding more cases here as we implement them
                default: printf("UNKNOWN_OP %d",instr->op); break;
            }
        }
        printf("\n");
    }
    printf("--------------------------\n");
}

/* --- TAC Generation from AST (To be implemented) --- */

void gen_tac_for_node(ASTNode* node);

void generate_tac(ASTNode* root) {
    if (!root) return;
    // Reset counters for fresh generation
    temp_count = 0;
    label_count = 0;
    gen_tac_for_node(root);
}

TacAddr* gen_tac_for_expr(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case NODE_CONSTANT:
            return new_const(atoi(node->data.stringValue));

        case NODE_IDENTIFIER: {
            Symbol* sym = find_symbol(node->data.stringValue);
            if (!sym) {
                // This should be caught by semantic analysis, but as a safeguard:
                fprintf(stderr, "Error: Symbol '%s' not found during TAC generation.\n", node->data.stringValue);
                return NULL;
            }
            return new_var_addr(sym);
        }

        case NODE_BINARY_EXPR: {
            TacAddr* left = gen_tac_for_expr(node->data.binary_expr.left);
            TacAddr* right = gen_tac_for_expr(node->data.binary_expr.right);
            TacAddr* result = new_temp();
            TacOp op = TAC_UNDEF;
            switch (node->data.binary_expr.op) {
                case '+': op = TAC_ADD; break;
                case '-': op = TAC_SUB; break;
                case '*': op = TAC_MUL; break;
                case '/': op = TAC_DIV; break;
                // Add cases for ==, !=, <, >, etc.
                case EQ_OP: op = TAC_EQ; break;
                case NE_OP: op = TAC_NE; break;
                case '<':   op = TAC_LT; break;
                case LE_OP: op = TAC_LE; break;
                case '>':   op = TAC_GT; break;
                case GE_OP: op = TAC_GE; break;
            }
            emit(op, result, left, right);
            return result;
        }
        case NODE_UNARY_EXPR: {
            TacAddr* operand = gen_tac_for_expr(node->data.unary_expr.operand);
            TacAddr* one = new_const(1);
            TacAddr* result = NULL;

            // NOTE: This assumes your AST can't distinguish pre/post increment.
            // This implementation defaults to PRE-increment/decrement behavior.
            // a = ++b; -> b = b + 1; a = b;
            // a = b++; -> t = b; b = b + 1; a = t;
            switch(node->data.unary_expr.op) {
                case INC_OP:
                    emit(TAC_ADD, operand, operand, one);
                    result = operand; // Pre-increment returns the new value
                    break;
                case DEC_OP:
                    emit(TAC_SUB, operand, operand, one);
                    result = operand; // Pre-decrement returns the new value
                    break;
                default:
                     fprintf(stderr, "Warning: TAC generation not implemented for unary operator %d\n", node->data.unary_expr.op);
                     return NULL;
            }
            return result;
        }
        case NODE_PREFIX_UNARY_EXPR: { // For ++b
            TacAddr* operand = gen_tac_for_expr(node->data.unary_expr.operand);
            TacAddr* one = new_const(1);
            TacOp op = (node->data.unary_expr.op == INC_OP) ? TAC_ADD : TAC_SUB;
            
            emit(op, operand, operand, one); // b = b + 1
            return operand;                  // return the new value of b
        }
        case NODE_POSTFIX_UNARY_EXPR: { // For b++
            TacAddr* operand = gen_tac_for_expr(node->data.unary_expr.operand);
            TacAddr* result = new_temp();
            TacAddr* one = new_const(1);
            TacOp op = (node->data.unary_expr.op == INC_OP) ? TAC_ADD : TAC_SUB;

            emit(TAC_ASSIGN, result, operand, NULL); // t = b (save original value)
            emit(op, operand, operand, one);         // b = b + 1
            return result;                           // return t (the original value)
        }


        case NODE_ASSIGNMENT: {
            TacAddr* lvalue = gen_tac_for_expr(node->data.assignment.lvalue);
            TacAddr* rvalue = gen_tac_for_expr(node->data.assignment.rvalue);
            emit(TAC_ASSIGN, lvalue, rvalue, NULL);
            return lvalue;
        }
        
        // Add cases for all other expression types...
        default:
            fprintf(stderr, "Warning: TAC generation not implemented for expression node type %d\n", node->type);
            return NULL;
    }
}

void gen_tac_for_node(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_TRANSLATION_UNIT:
            for (ASTNodeList* item = node->data.items_list; item; item = item->next) {
                gen_tac_for_node(item->node);
            }
            break;

        case NODE_DECLARATION: {
            // Handle declarations
            if (!node->data.declaration.declarators) break;

            for (ASTNodeList* d_item = node->data.declaration.declarators; d_item; d_item = d_item->next) {
                ASTNode* init_decl = d_item->node;
                if (init_decl->data.init_declarator.initializer) {
                    // This is a declaration with an initializer, e.g., int x = 5;
                    // We generate TAC for the assignment part.
                    TacAddr* lvalue = gen_tac_for_expr(init_decl->data.init_declarator.declarator);
                    TacAddr* rvalue = gen_tac_for_expr(init_decl->data.init_declarator.initializer);
                    emit(TAC_ASSIGN, lvalue, rvalue, NULL);
                }
            }
            break;
        }
        case NODE_FUNCTION_DEFINITION:
            emit(TAC_BEGIN_FUNC, NULL, NULL, NULL);
            // In a real implementation, you'd handle parameters here
            gen_tac_for_node(node->data.function_definition.body);
            emit(TAC_END_FUNC, NULL, NULL, NULL);
            break;

        case NODE_COMPOUND_STATEMENT:
            for (ASTNodeList* item = node->data.compound_statement.items; item; item = item->next) {
                gen_tac_for_node(item->node);
            }
            break;

        case NODE_EXPRESSION_STATEMENT:
            if (node->data.expression_statement.expression) {
                gen_tac_for_expr(node->data.expression_statement.expression);
            }
            break;

        case NODE_IF_STATEMENT: {
            TacAddr* else_label = new_label_addr();
            TacAddr* end_label = new_label_addr();
            
            // Condition
            TacAddr* cond = gen_tac_for_expr(node->data.if_statement.condition);
            emit(TAC_IFZ, else_label, cond, NULL); // If condition is false, jump to else

            // If body
            gen_tac_for_node(node->data.if_statement.if_body);
            emit(TAC_GOTO, end_label, NULL, NULL); // Skip else part

            // Else part
            emit(TAC_LABEL, else_label, NULL, NULL);
            if (node->data.if_statement.else_body) {
                gen_tac_for_node(node->data.if_statement.else_body);
            }

            // End label
            emit(TAC_LABEL, end_label, NULL, NULL);
            break;
        }

        case NODE_WHILE_STATEMENT: {
            TacAddr* start_label = new_label_addr();
            TacAddr* end_label = new_label_addr();

            emit(TAC_LABEL, start_label, NULL, NULL); // Label for loop start

            // Condition
            TacAddr* cond = gen_tac_for_expr(node->data.while_statement.condition);
            emit(TAC_IFZ, end_label, cond, NULL); // If condition is false, jump to end

            // Body
            gen_tac_for_node(node->data.while_statement.body);
            emit(TAC_GOTO, start_label, NULL, NULL); // Jump back to the start

            // End label
            emit(TAC_LABEL, end_label, NULL, NULL);
            break;
        }

        case NODE_UNTIL_STATEMENT: {
            TacAddr* start_label = new_label_addr();
            TacAddr* end_label = new_label_addr();

            emit(TAC_LABEL, start_label, NULL, NULL); // Label for loop start

            // Condition
            TacAddr* cond = gen_tac_for_expr(node->data.until_statement.condition);
            // Create a temporary to hold the inverted condition
            TacAddr* temp_not_cond = new_temp();
            emit(TAC_EQ, temp_not_cond, cond, new_const(0)); // temp = (cond == 0)
            emit(TAC_IFZ, end_label, temp_not_cond, NULL); // If temp is false (i.e., cond was true), jump to end

            // Body
            gen_tac_for_node(node->data.until_statement.body);
            emit(TAC_GOTO, start_label, NULL, NULL); // Jump back to the start

            // End label
            emit(TAC_LABEL, end_label, NULL, NULL);
            break;
        }

        case NODE_RETURN_STATEMENT: {
            TacAddr* retval = NULL;
            if (node->data.return_statement.expression) {
                retval = gen_tac_for_expr(node->data.return_statement.expression);
            }
            emit(TAC_RETURN, NULL, retval, NULL);
            break;
        }

        default:
            break;
    }
}