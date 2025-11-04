/*
 * Definitions for the MIPS Code Generator.
 * Defines Basic Blocks, Register Descriptors, and Address Descriptors.
 */
#ifndef MIPS_H
#define MIPS_H

#include "tac.h"
#include "list.h"

// We will use the 10 temporary registers: $t0 - $t9
#define NUM_TEMP_REGS 10

// We will also use the 8 saved registers: $s0 - $s7
#define NUM_SAVED_REGS 8

/*
 * Register Descriptor (RD)
 * Keeps track of what is currently in each MIPS register.
 */
typedef struct MipsRegister {
    const char* name; // e.g., "$t0"
    List* vars;       // List of TacAddr* (variables) currently in this register
} MipsRegister;

/*
 * Address Descriptor (AD)
 * For each variable, tracks where its current value can be found.
 */
typedef enum {
    LOC_NONE,
    LOC_MEM,
    LOC_REG
} LocationKind;

typedef struct AddrLocation {
    LocationKind kind;
    int reg_index; // If kind == LOC_REG, this is the index in temp_reg_desc
} AddrLocation;

typedef struct AddrDesc {
    TacAddr* var;    // The variable/temporary
    List* locations; // List of AddrLocation*
    struct AddrDesc* next;
} AddrDesc;

/*
 * Basic Block
 * A sequence of 3AC instructions with one entry and one exit.
 */
typedef struct BasicBlock {
    int id;
    TacInstr* start; // Pointer to the first 3AC instruction
    TacInstr* end;   // Pointer to the last 3AC instruction
    
    struct BasicBlock* next; // Next block in the linked list
    
    // Successors for the control flow graph (CFG)
    struct BasicBlock* fall_through_target; // e.g., for 'if'
    struct BasicBlock* branch_target;     // e.g., for 'goto' or 'if'
} BasicBlock;

// Main entry point for MIPS generation
void generate_mips(TacInstr* tac_head, const char* out_filename);

#endif // MIPS_H