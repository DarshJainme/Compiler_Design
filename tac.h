#ifndef TAC_H
#define TAC_H

#include "symbol_table.h"

// The different kinds of operations in our three-address code
typedef enum {
    TAC_UNDEF,
    TAC_BEGIN_FUNC,
    TAC_END_FUNC,

    // Assignment and Copying
    TAC_ASSIGN,     // result = arg1
    TAC_ADD,        // result = arg1 + arg2
    TAC_SUB,        // result = arg1 - arg2
    TAC_MUL,        // result = arg1 * arg2
    TAC_DIV,        // result = arg1 / arg2
    
    // Logical and Relational
    TAC_EQ,         // result = (arg1 == arg2)
    TAC_NE,         // result = (arg1 != arg2)
    TAC_LT,         // result = (arg1 < arg2)
    TAC_LE,         // result = (arg1 <= arg2)
    TAC_GT,         // result = (arg1 > arg2)
    TAC_GE,         // result = (arg1 >= arg2)

    // Jumps and Labels
    TAC_LABEL,      // L1:
    TAC_GOTO,       // goto L1
    TAC_IFZ,        // ifz arg1 goto L1 (For if arg1 is zero)
    TAC_IFNZ,       // if non zero so that code part becomes simpler.
    
    // Function calls
    TAC_PARAM,      // param arg1
    TAC_CALL,       // call arg1, result
    TAC_RETURN,     // return arg1

    // Memory and Pointers
    TAC_ADDR,       // result = &arg1 (address of)
    TAC_DEREF,      // result = *arg1 (dereference)
    TAC_STORE       // *result = arg1 (store via pointer)

} TacOp;

// The different kinds of "addresses" or operands
typedef enum {
    ADDR_UNDEF,
    ADDR_VARIABLE,  // A variable from the symbol table
    ADDR_TEMP,      // A temporary variable (t1, t2, ...)
    ADDR_CONSTANT,  // A literal integer constant
    ADDR_LABEL,     // A label (L1, L2, ...)
    ADDR_STRING     // A string literal
} AddrKind;

// Represents an operand in a TAC instruction
typedef struct TacAddr {
    AddrKind kind;
    union {
        Symbol* var;        // For ADDR_VARIABLE
        int temp_id;        // For ADDR_TEMP
        int const_val;      // For ADDR_CONSTANT
        int label_id;       // For ADDR_LABEL
        char* string_label; // For ADDR_STRING
    } val;
} TacAddr;

// Represents a single TAC instruction (a quadruple)
typedef struct TacInstr {
    TacOp op;
    TacAddr* res;
    TacAddr* arg1;
    TacAddr* arg2;
    struct TacInstr* next;
    struct TacInstr* prev;
} TacInstr;

// Global list of all instructions
extern TacInstr* tac_list_head;
extern TacInstr* tac_list_tail;

/* --- Core TAC Functions --- */

// Create new addresses/operands
TacAddr* new_temp();
TacAddr* new_label_addr();
TacAddr* new_const(int val);
TacAddr* new_var_addr(Symbol* sym);
TacAddr* new_string_addr(char* str_label);

// Emit a new instruction to the global list
void emit(TacOp op, TacAddr* res, TacAddr* arg1, TacAddr* arg2);

// Print the entire TAC list for debugging
void print_tac();

/* --- TAC Generation from AST --- */

// Main entry point to generate TAC from the AST
void generate_tac(ASTNode* root);

// Recursive function to generate TAC for an expression
TacAddr* gen_tac_for_expr(ASTNode* node);
void gen_tac_for_node(ASTNode* node);

/* For different loops, stack management */
void push_loop_labels(TacAddr* continue_label, TacAddr* break_label);
void pop_loop_labels();
TacAddr* get_loop_continue_label();
TacAddr* get_loop_break_label();
#endif // TAC_H