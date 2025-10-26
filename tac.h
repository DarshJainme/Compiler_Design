#ifndef TAC_H
#define TAC_H

#include "symbol_table.h"
// Forward declare ASTNode to avoid circular dependency if tac.h is included elsewhere
struct ASTNode;

// The different kinds of operations in our three-address code
// (Corrected to match tac.c)
typedef enum {
    TAC_UNDEF,
    TAC_BEGIN_FUNC, // Marker for function start (prologue)
    TAC_END_FUNC,   // Marker for function end (epilogue)

    // Assignment and Arithmetic (Integer/General)
    TAC_ASSIGN,     // result = arg1
    TAC_ADD,        // result = arg1 + arg2
    TAC_SUB,        // result = arg1 - arg2
    TAC_MUL,        // result = arg1 * arg2
    TAC_DIV,        // result = arg1 / arg2
    TAC_MOD,        // result = arg1 % arg2
    TAC_NEG,        // result = -arg1 (Unary minus)
    TAC_ADDR,       // result = &arg1 (Address of)
    TAC_DEREF,      // result = *arg1 (Dereference)
    TAC_STORE,      // *arg1 = res (Store value 'res' at address 'arg1')
    TAC_LEA,        // result = arg1 + arg2 (Load Effective Address)

    // Floating Point Arithmetic
    TAC_FADD,       // result = arg1 + arg2 (float)
    TAC_FSUB,       // result = arg1 - arg2 (float)
    TAC_FMUL,       // result = arg1 * arg2 (float)
    TAC_FDIV,       // result = arg1 / arg2 (float)
    TAC_FNEG,       // result = -arg1       (float)

    // Type Conversions
    TAC_CVT_I2F,    // result(float) = (float)arg1(int)
    TAC_CVT_F2I,    // result(int) = (int)arg1(float)
    TAC_CVT_D2F,    // result(float) = (float)arg1(double)  <-- NEW
    TAC_CVT_F2D,    // result(double) = (double)arg1(float) <-- NEW
    
    // Control Flow
    TAC_LABEL,      // Label for jumps (res is label)
    TAC_GOTO,       // Unconditional jump to res (label)
    TAC_IFZ,        // if arg1 == 0 goto res (label)
    TAC_IFNZ,       // if arg1 != 0 goto res (label)
    
    // Relational Ops (result is 0 or 1)
    TAC_EQ,         // result = arg1 == arg2
    TAC_NE,         // result = arg1 != arg2
    TAC_LT,         // result = arg1 < arg2
    TAC_LE,         // result = arg1 <= arg2
    TAC_GT,         // result = arg1 > arg2
    TAC_GE,         // result = arg1 >= arg2
    TAC_FEQ,        // result = arg1 == arg2 (float)
    TAC_FNE,        // result = arg1 != arg2 (float)
    TAC_FLT,        // result = arg1 < arg2  (float)
    TAC_FLE,        // result = arg1 <= arg2 (float)
    TAC_FGT,        // result = arg1 > arg2  (float)
    TAC_FGE,        // result = arg1 >= arg2 (float)

    // Bitwise and Shifts (Integer)
    TAC_LSHIFT,     // result = arg1 << arg2
    TAC_RSHIFT,     // result = arg1 >> arg2
    TAC_BAND,       // result = arg1 & arg2
    TAC_BOR,        // result = arg1 | arg2
    TAC_BXOR,       // result = arg1 ^ arg2
    TAC_BNOT,       // result = ~arg1 (Bitwise complement)
    TAC_NOT,        // result = !arg1 (Logical NOT)

    // Function Calls and Parameters
    TAC_PARAM,      // Push parameter arg1
    TAC_CALL,       // result = call arg1 (func name), arg2 (num params)
    TAC_RETURN,     // return arg1

    // Array and Member Access
    TAC_INDEX_READ, // result = arg1[arg2] (Load from array)
    TAC_INDEX_WRITE,// arg1[arg2] = res (Store to array)

} TacOp;

// The different kinds of operands (addresses)
// (Corrected to match tac.c)
typedef enum {
    ADDR_NONE,          
    ADDR_TEMP,          // A compiler-generated temporary variable (e.g., t1, t2)
    ADDR_VARIABLE,      // A user-defined variable (e.g., a, b, myFunc)
    ADDR_CONSTANT_INT,  // An integer constant (e.g., 5, 100)
    ADDR_CONSTANT_FLOAT,// A float/double constant (e.g., 5.5, 3.14)
    // ADDR_CONSTANT_CHAR is handled as ADDR_CONSTANT_INT in tac.c
    ADDR_LABEL,         // A label (e.g., L1, L2)
    ADDR_STRING,        // A string literal identifier (e.g., S0)
} TacAddrKind;

// A single operand/address in the instruction
// (Corrected to match tac.c)
typedef struct TacAddr {
    TacAddrKind kind;
    struct Type* type; // Type information for casting/promotion
    union {
        // ADDR_TEMP
        int temp_id; 
        
        // ADDR_LABEL
        int label_id; 

        // ADDR_VARIABLE
        Symbol* var;

        // ADDR_CONSTANT_*
        int const_int;
        double const_float;
        
        // ADDR_STRING
        char* string_label; // The label name (e.g., "S0")
    } val; // tac.c uses 'val'
} TacAddr;

// A single three-address instruction (a quadruple)
typedef struct TacInstr {
    TacOp op;
    TacAddr* res;
    TacAddr* arg1;
    TacAddr* arg2;
    struct TacInstr* next;
    struct TacInstr* prev;
} TacInstr;

// --- String Literal Management ---
typedef struct StringLiteral {
    char* label;
    char* value;
    struct StringLiteral* next;
} StringLiteral;

extern StringLiteral* string_literal_list; // List to hold all string literals

// Global list of all instructions
extern TacInstr* tac_list_head;
extern TacInstr* tac_list_tail;

/* --- Core TAC Functions --- */

// Create new addresses/operands
TacAddr* new_temp(struct Type* type); // Pass type info
TacAddr* new_label_addr();
TacAddr* new_const_int(int val);
TacAddr* new_const_float(double val);
TacAddr* new_const_char(char val);
TacAddr* new_var_addr(Symbol* sym);
TacAddr* new_string_addr(char* string_value); // Pass the actual string

// Emit a new instruction to the global list
void emit(TacOp op, TacAddr* res, TacAddr* arg1, TacAddr* arg2);

// Print the entire TAC list for debugging
void print_tac();

// Helper to generate TAC for implicit type conversions
TacAddr* handle_implicit_cast(TacAddr* src_addr, Type* target_type);

/* --- TAC Generation from AST --- */

// Main entry point to generate TAC from the AST
void generate_tac(struct ASTNode* root);
TacAddr* gen_tac_for_expr(struct ASTNode* node, bool lvalue_only);
void gen_tac_for_node(struct ASTNode* node);
void gen_tac_for_declaration(struct ASTNode* node);
void gen_tac_for_assignment(struct ASTNode* node);


/* For different loops, stack management */
void push_loop_labels(TacAddr* continue_label, TacAddr* break_label);
void pop_loop_labels();
TacAddr* get_loop_continue_label();
TacAddr* get_loop_break_label();
#endif // TAC_H