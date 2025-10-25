#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tac.h"
#include "parser.tab.h" // For token definitions

TacInstr* tac_list_head = NULL;
TacInstr* tac_list_tail = NULL;

static int temp_count = 0;
static int label_count = 0;

// A simple stack to keep track of loop labels for break/continue
#define MAX_LOOP_DEPTH 50
static TacAddr* continue_stack[MAX_LOOP_DEPTH];
static TacAddr* break_stack[MAX_LOOP_DEPTH];
static int loop_stack_ptr = -1;

void push_loop_labels(TacAddr* continue_label, TacAddr* break_label) {
    if (loop_stack_ptr < MAX_LOOP_DEPTH - 1) {
        loop_stack_ptr++;
        continue_stack[loop_stack_ptr] = continue_label;
        break_stack[loop_stack_ptr] = break_label;
    } else {
        fprintf(stderr, "Fatal: Exceeded maximum loop nesting depth.\n");
        exit(1);
    }
}

void pop_loop_labels() {
    if (loop_stack_ptr >= 0) {
        loop_stack_ptr--;
    }
}

TacAddr* get_loop_continue_label() {
    if (loop_stack_ptr >= 0) {
        return continue_stack[loop_stack_ptr];
    }
    return NULL;
}

TacAddr* get_loop_break_label() {
    if (loop_stack_ptr >= 0) {
        return break_stack[loop_stack_ptr];
    }
    return NULL;
}

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
            // Only print "res = " for non-jump/non-control instructions
            if (instr->res && instr->op != TAC_GOTO && instr->op != TAC_IFZ && instr->op != TAC_IFNZ && instr->op != TAC_RETURN && instr->op != TAC_PARAM) {
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
                // Correction in printing jump instructions
                case TAC_GOTO: printf("goto "); print_addr(instr->res); break;
                case TAC_IFZ: printf("ifz "); print_addr(instr->arg1); printf(" goto "); print_addr(instr->res); break;
                case TAC_IFNZ: printf("ifnz "); print_addr(instr->arg1); printf(" goto "); print_addr(instr->res); break;
                case TAC_RETURN: printf("return "); 
                    if (instr->res) {
                        // return can have an optional value
                        printf(" ");
                        print_addr(instr->res);
                    }
                    break;
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
    // This function is now only called for simple cases that aren't
    // control-flow statements. The complex logic is in semantic.c.
    if (!node) return;

    switch (node->type) {
        case NODE_DECLARATION: {
            // This handles declarations like `int x = 5;`
            if (!node->data.declaration.declarators) break;

            for (ASTNodeList* d_item = node->data.declaration.declarators; d_item; d_item = d_item->next) {
                ASTNode* init_decl = d_item->node;
                if (init_decl->data.init_declarator.initializer) {
                    // gen_tac_for_expr works because the symbols were already added by semantic.c
                    TacAddr* lvalue = gen_tac_for_expr(init_decl->data.init_declarator.declarator);
                    TacAddr* rvalue = gen_tac_for_expr(init_decl->data.init_declarator.initializer);
                    if (lvalue && rvalue) {
                        emit(TAC_ASSIGN, lvalue, rvalue, NULL);
                    }
                }
            }
            break;
        }
        // NOTE: There are no cases for IF, FOR, WHILE, etc. here anymore!
        // They are handled entirely by semantic.c.
        default:
            // This function can be left empty or print a warning for unhandled simple nodes.
            break;
    }
}