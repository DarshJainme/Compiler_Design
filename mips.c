/*
 * MODIFIED FILE: mips.c
 *
 * 1.  FIXED: Compilation error for 'reg' undeclared in get_reg() by
 * adding the necessary register finding/spilling logic to the
 * ADDR_CONSTANT_FLOAT block. Changed 'l.s' to 'lw' as we are
 * (incorrectly, for now) loading into a GPR.
 * 2.  FIXED: 'unused variable' warning by removing the 'func_sym'
 * declaration in assign_stack_offsets().
 * 3.  IMPROVED: Corrected liveness analysis as requested.
 * - assign_stack_offsets() now returns the list of local variables
 * and outputs the list of temporaries.
 * - generate_mips() loop now manages these lists.
 * - compute_liveness() now takes the list of function variables
 * and properly initializes all 'is_live' and 'next_use' fields
 * to false/-1 *before* the backward pass.
 * - The 'HACK' comment is removed because the implementation is now correct.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> // For va_list
#include <assert.h>
#include "mips.h"
#include "tac.h"
#include "types.h"
#include "symbol_table.h"
#include "list.h"
#include "ast.h" // For get_type_size

// --- Globals for the MIPS Generator ---

// The output file
static FILE* mips_file;

// GPR Descriptors (RD)
static MipsRegister temp_reg_desc[NUM_TEMP_REGS]; // $t0 - $t9
static const char* temp_reg_names[NUM_TEMP_REGS] = {
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9"
};

// --- NEW: Float Register Descriptors ---
static MipsRegister float_reg_desc[NUM_FLOAT_REGS]; // $f4 - $f15
static const char* float_reg_names[NUM_FLOAT_REGS] = {
    "$f4", "$f6", "$f8", "$f10", "$f12", "$f14", // Use even registers
    "$f16", "$f18", "$f20", "$f22", "$f24", "$f26" 
};
// We need a dedicated temp float register for conversions
#define TEMP_FPR_REG "$f28" 
#define TEMP_FPR_ZERO "$f30" // Used for 0.0

// Address Descriptor (AD)
static AddrDesc* ad_list_head = NULL;

// Basic Block list
static BasicBlock* block_list_head = NULL;

// Stack frame management
static int current_stack_offset; // Keeps track of the bottom of the stack for a function

// Map of temporaries (t_X) to their stack offsets
typedef struct TempOffset {
    int temp_id;
    int offset;
    struct TempOffset* next;
} TempOffset;
static TempOffset* temp_offset_map = NULL;

// --- NEW LIVENESS TRACKING FOR TEMPS ---
// This is the "next-use table" for all compiler temporaries (t0, t1, ...)
// It parallels the 'is_live' and 'next_use' fields in the Symbol struct.
typedef struct {
    bool is_live;
    int next_use;
} TempLiveness;

static TempLiveness* temp_live_info = NULL; // Array, will be sized to temp_count
static int max_temp_id = 0; // The highest t# used in the function

// --- Forward Declarations ---
static void gen_mips_for_instr(TacInstr* instr, Symbol* func_sym);
static const char* getGPR(TacInstr* instr, TacAddr* var, bool is_res);
static const char* getFPR(TacInstr* instr, TacAddr* var, bool is_res);
static void mips_emit(const char* format, ...);
static List* assign_stack_offsets(BasicBlock* func_start_block, List** temps_list_out);
static void compute_liveness(BasicBlock* block, List* vars_in_func);
static void init_float_reg_desc();
static int find_fpr_to_spill();
static void emit_load_address(const char* dst_reg, TacAddr* addr);

/*
================================================================================
 UTILITY AND HELPER FUNCTIONS
================================================================================
*/

// Formatted MIPS emitter
static void mips_emit(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(mips_file, format, args);
    va_end(args);
    fprintf(mips_file, "\n");
}

// Overloaded emitter for comments
static void mips_comment(const char* format, ...) {
    fprintf(mips_file, "# ");
    va_list args;
    va_start(args, format);
    vfprintf(mips_file, format, args);
    va_end(args);
    fprintf(mips_file, "\n");
}

// Get the size of a type (using the function from your types.c)
extern int get_type_size(Type* type); // Already defined in tac.c
static char* get_mem_loc_str(char* buffer, int size, TacAddr* addr);

// Initialize GPR Descriptors ($t0..$t9)
static void init_gpr_desc() {
    for (int i = 0; i < NUM_TEMP_REGS; i++) {
        if (temp_reg_desc[i].vars == NULL) {
            temp_reg_desc[i].vars = list_create();
        } else {
            list_clear(temp_reg_desc[i].vars);
        }
        temp_reg_desc[i].name = temp_reg_names[i];
    }
}

// --- NEW: Initialize FPR Descriptors ---
static void init_float_reg_desc() {
     for (int i = 0; i < NUM_FLOAT_REGS; i++) {
        if (float_reg_desc[i].vars == NULL) {
            float_reg_desc[i].vars = list_create();
        } else {
            list_clear(float_reg_desc[i].vars);
        }
        float_reg_desc[i].name = float_reg_names[i];
    }
}

// // Get a string for a variable's memory location (e.g., "-8($fp)" or "g_var")
// static char* get_mem_loc_str(char* buffer, int size, TacAddr* addr) {
//     if (addr == NULL) {
//         snprintf(buffer, size, "NULL_ADDR");
//         return buffer;
//     }

//     switch (addr->kind) {
//         case ADDR_VARIABLE:
//             if (addr->val.var->scope_level == 0) {
//                 // Global variable
//                 snprintf(buffer, size, "%s", addr->val.var->name);
//             } else {
//                 // Local variable or parameter
//                 snprintf(buffer, size, "%d($fp)", addr->val.var->stack_offset);
//             }
//             break;
//         case ADDR_TEMP: {
//             // Find the temporary in our offset map
//             TempOffset* t = temp_offset_map;
//             while(t) {
//                 if (t->temp_id == addr->val.temp_id) {
//                     snprintf(buffer, size, "%d($fp)", t->offset);
//                     return buffer;
//                 }
//                 t = t->next;
//             }
//             // This should not happen if assign_stack_offsets() worked
//             snprintf(buffer, size, "TEMP_NOT_FOUND(t%d)", addr->val.temp_id);
//             break;
//         }
//         case ADDR_STRING:
//             snprintf(buffer, size, "%s", addr->val.string_label);
//             break;
//         default:
//             snprintf(buffer, size, "INVALID_MEM_LOC");
//             break;
//     }
//     return buffer;
// }


/*
================================================================================
 DESCRIPTOR MANAGEMENT (REGISTER & ADDRESS)
================================================================================
*/

// Initialize Register Descriptors ($t0..$t9)
// static void init_reg_desc() {
//     for (int i = 0; i < NUM_TEMP_REGS; i++) {
//         if (temp_reg_desc[i].vars == NULL) {
//             temp_reg_desc[i].vars = list_create();
//         } else {
//             list_clear(temp_reg_desc[i].vars);
//         }
//         temp_reg_desc[i].name = temp_reg_names[i];
//     }
// }

// Clear the Address Descriptor list
static void init_addr_desc() {
    AddrDesc* current = ad_list_head;
    while (current) {
        AddrDesc* next = current->next;
        // Free the list of locations
        ListNode* loc_node = list_begin(current->locations);
        while (loc_node) {
            free(list_get_data(loc_node)); // Free the AddrLocation struct
            loc_node = list_next(loc_node);
        }
        list_destroy(current->locations);
        free(current);
        current = next;
    }
    ad_list_head = NULL;
}

// Helper to find or create an Address Descriptor entry for a variable
static AddrDesc* get_or_create_ad_entry(TacAddr* var) {
    // We must compare by content, not pointer, for temps/consts
    AddrDesc* ad = ad_list_head;
    while (ad) {
        if (ad->var->kind == var->kind) {
            switch(var->kind) {
                case ADDR_VARIABLE:
                    if (ad->var->val.var == var->val.var) return ad;
                    break;
                case ADDR_TEMP:
                    if (ad->var->val.temp_id == var->val.temp_id) return ad;
                    break;
                case ADDR_CONSTANT_INT:
                    if (ad->var->val.const_int == var->val.const_int) return ad;
                    break;
                case ADDR_CONSTANT_FLOAT:
                    if (ad->var->val.const_float == var->val.const_float) return ad;
                    break;
                case ADDR_STRING:
                    // We need to compare the string labels
                    if (ad->var->val.string_label && var->val.string_label &&
                        strcmp(ad->var->val.string_label, var->val.string_label) == 0) return ad;
                    break;
                default:
                    // Other types (like ADDR_LABEL) might not be cached
                    break;
            }
        }
        ad = ad->next;
    }

    // Not found, create it
    AddrDesc* new_ad = (AddrDesc*)calloc(1, sizeof(AddrDesc));
    new_ad->var = var;
    new_ad->locations = list_create();
    new_ad->next = ad_list_head; // Prepend
    ad_list_head = new_ad;
    
    // By default, it's in memory (this will be set by the caller)
    return new_ad;
}

// // Update AD to show 'var' is now in 'reg' (and *only* in 'reg')
// static void ad_set_var_in_reg(TacAddr* var, int reg_index) {
//     AddrDesc* ad = get_or_create_ad_entry(var);
    
//     // Clear old locations
//     ListNode* loc_node = list_begin(ad->locations);
//     while (loc_node) {
//         free(list_get_data(loc_node));
//         loc_node = list_next(loc_node);
//     }
//     list_clear(ad->locations);

//     // Add new register location
//     AddrLocation* new_loc = (AddrLocation*)calloc(1, sizeof(AddrLocation));
//     new_loc->kind = LOC_REG;
//     new_loc->reg_index = reg_index;
//     list_append(ad->locations, new_loc);
// }

static void ad_set_var_in_gpr(TacAddr* var, int reg_index) {
    AddrDesc* ad = get_or_create_ad_entry(var);
    list_clear_and_free_data(ad->locations); // Clear old locations
    AddrLocation* new_loc = (AddrLocation*)calloc(1, sizeof(AddrLocation));
    new_loc->kind = LOC_GPR;
    new_loc->reg_index = reg_index;
    list_append(ad->locations, new_loc);
}

// --- NEW: Update AD to show 'var' is now in FPR 'reg' ---
static void ad_set_var_in_fpr(TacAddr* var, int reg_index) {
    AddrDesc* ad = get_or_create_ad_entry(var);
    list_clear_and_free_data(ad->locations); // Clear old locations
    AddrLocation* new_loc = (AddrLocation*)calloc(1, sizeof(AddrLocation));
    new_loc->kind = LOC_FPR;
    new_loc->reg_index = reg_index;
    list_append(ad->locations, new_loc);
}

// Update AD to show 'var' is in memory
static void ad_set_var_in_mem(TacAddr* var) {
    AddrDesc* ad = get_or_create_ad_entry(var);
    // Add memory location if not already present
    bool in_mem = false;
    for (ListNode* n = list_begin(ad->locations); n; n = list_next(n)) {
        if (((AddrLocation*)list_get_data(n))->kind == LOC_MEM) {
            in_mem = true;
            break;
        }
    }
    if (!in_mem) {
        AddrLocation* new_loc = (AddrLocation*)calloc(1, sizeof(AddrLocation));
        new_loc->kind = LOC_MEM;
        list_append(ad->locations, new_loc);
    }
}

// Remove 'reg' as a location for 'var'
static void ad_remove_reg_loc(TacAddr* var, int reg_index, LocationKind kind) {
    AddrDesc* ad = get_or_create_ad_entry(var);
    ListNode* n = list_begin(ad->locations);
    while (n) {
        AddrLocation* loc = (AddrLocation*)list_get_data(n);
        if (loc->kind == kind && loc->reg_index == reg_index) {
            list_remove(ad->locations, loc);
            free(loc);
            break; 
        }
        n = list_next(n);
    }
}

// // Update RD to show 'reg' now holds 'var'
// static void rd_add_var_to_reg(TacAddr* var, int reg_index) {
//     if (!list_contains(temp_reg_desc[reg_index].vars, var)) {
//         list_append(temp_reg_desc[reg_index].vars, var);
//     }
// }

// // Update RD to show 'reg' no longer holds 'var'
// static void rd_remove_var_from_reg(TacAddr* var, int reg_index) {
//     list_remove(temp_reg_desc[reg_index].vars, var);
// }

// // Clear a register in the RD and update AD for all vars it contained
// static void rd_clear_reg(int reg_index) {
//     List* vars_in_reg = temp_reg_desc[reg_index].vars;
//     ListNode* n = list_begin(vars_in_reg);
//     while (n) {
//         TacAddr* var = (TacAddr*)list_get_data(n);
//         ad_remove_reg_loc(var, reg_index);
//         n = list_next(n);
//     }
//     list_clear(vars_in_reg);
// }

// Update GPR-RD to show 'reg' now holds 'var'
static void rd_add_var_to_gpr(TacAddr* var, int reg_index) {
    if (!list_contains(temp_reg_desc[reg_index].vars, var)) {
        list_append(temp_reg_desc[reg_index].vars, var);
    }
}
// Update GPR-RD to show 'reg' no longer holds 'var'
static void rd_remove_var_from_gpr(TacAddr* var, int reg_index) {
    list_remove(temp_reg_desc[reg_index].vars, var);
}
// Clear a GPR in the RD and update AD for all vars it contained
static void rd_clear_gpr(int reg_index) {
    List* vars_in_reg = temp_reg_desc[reg_index].vars;
    ListNode* n = list_begin(vars_in_reg);
    while (n) {
        TacAddr* var = (TacAddr*)list_get_data(n);
        ad_remove_reg_loc(var, reg_index, LOC_GPR);
        n = list_next(n);
    }
    list_clear(vars_in_reg);
}

// --- NEW: FPR Descriptor helpers ---
static void rd_add_var_to_fpr(TacAddr* var, int reg_index) {
    if (!list_contains(float_reg_desc[reg_index].vars, var)) {
        list_append(float_reg_desc[reg_index].vars, var);
    }
}
static void rd_remove_var_from_fpr(TacAddr* var, int reg_index) {
    list_remove(float_reg_desc[reg_index].vars, var);
}
static void rd_clear_fpr(int reg_index) {
    List* vars_in_reg = float_reg_desc[reg_index].vars;
    ListNode* n = list_begin(vars_in_reg);
    while (n) {
        TacAddr* var = (TacAddr*)list_get_data(n);
        ad_remove_reg_loc(var, reg_index, LOC_FPR);
        n = list_next(n);
    }
    list_clear(vars_in_reg);
}

/*
================================================================================
 BASIC BLOCK & LIVENESS
================================================================================
*/

// Find all instruction leaders and build the basic block list
static void build_basic_blocks(TacInstr* tac_head) {
    if (!tac_head) return;
    
    // We need to know the total number of instructions
    int num_instrs = 0;
    for (TacInstr* instr = tac_head; instr; instr = instr->next) {
        num_instrs++;
    }
    if (num_instrs == 0) return;

    // 'leaders' flags which instruction is a leader
    bool* leaders = (bool*)calloc(num_instrs, sizeof(bool));
    // 'instr_map' maps an index (lineno) to the instruction pointer
    TacInstr** instr_map = (TacInstr**)calloc(num_instrs, sizeof(TacInstr*));
    if (!leaders || !instr_map) {
        fprintf(stderr, "Fatal: could not allocate memory for basic block analysis.\n");
        exit(1);
    }

    // Pass 1: Find leaders
    for (TacInstr* instr = tac_head; instr; instr = instr->next) {
        int i = instr->lineno; // Use the line number from tac.c
        instr_map[i] = instr; // Map index to instruction
        
        if (i == 0) {
            leaders[i] = true; // First instruction is a leader
            continue;
        }

        switch (instr->op) {
            case TAC_LABEL:
            case TAC_BEGIN_FUNC: // <-- *** ADDED THIS LINE ***
                leaders[i] = true; // Target of a jump or start of a func is a leader
                break;
            default: break;
        }
        
        // Instruction *after* a jump/return is a leader
        switch (instr->prev->op) {
            case TAC_GOTO:
            case TAC_IFZ:
            case TAC_IFNZ:
            case TAC_RETURN:
            case TAC_END_FUNC:
                leaders[i] = true;
                break;
            default: break;
        }
    }

    // Pass 2: Create blocks
    BasicBlock* current_block = NULL;
    BasicBlock* prev_block = NULL;
    int block_id = 0;

    for (int i = 0; i < num_instrs; i++) {
        if (leaders[i]) {
            // End previous block
            if (current_block) {
                current_block->end = instr_map[i-1];
            }
            
            // Start new block
            current_block = (BasicBlock*)calloc(1, sizeof(BasicBlock));
            current_block->id = block_id++;
            current_block->start = instr_map[i];
            
            if (prev_block) {
                prev_block->next = current_block;
            } else {
                block_list_head = current_block;
            }
            prev_block = current_block;
        }
    }
    // End the very last block
    if (current_block) {
        current_block->end = instr_map[num_instrs - 1];
    }
    
    // Free the maps
    free(leaders);
    free(instr_map);
}

// ** CORRECTED LIVENESS ANALYSIS **
// Compute Liveness / Next-Use for a single basic block
static void compute_liveness(BasicBlock* block, List* vars_in_func) {
    // 1. Initialize liveness for all variables and temporaries in the function
    if (temp_live_info) {
        memset(temp_live_info, 0, sizeof(TempLiveness) * (max_temp_id + 1));
        for(int i=0; i <= max_temp_id; i++) {
            temp_live_info[i].next_use = -1;
        }
    }
    
    if (vars_in_func) {
        for (ListNode* n = list_begin(vars_in_func); n; n = list_next(n)) {
            Symbol* s = (Symbol*)list_get_data(n);
            s->is_live = false;
            s->next_use = -1;
        }
    }
    
    // 2. Backward pass
    TacInstr* instr = block->end;
    bool done = false;
    
    while (!done) {
        if (instr == block->start->prev) {
            done = true;
            break;
        }
        
        int current_line = instr->lineno;

        // 1. For `res = arg1 op arg2` (or any instruction with a result):
        //    Set `res` to not live, next_use = -1
        if (instr->res) {
            if (instr->res->kind == ADDR_VARIABLE && instr->res->val.var) {
                instr->res->val.var->is_live = false;
                instr->res->val.var->next_use = -1;
            } else if (instr->res->kind == ADDR_TEMP) {
                temp_live_info[instr->res->val.temp_id].is_live = false;
                temp_live_info[instr->res->val.temp_id].next_use = -1;
            }
        }
        
        // 2. Set `arg1` to live, next_use = this instruction's line
        if (instr->arg1) {
             if (instr->arg1->kind == ADDR_VARIABLE && instr->arg1->val.var) {
                instr->arg1->val.var->is_live = true;
                instr->arg1->val.var->next_use = current_line;
            } else if (instr->arg1->kind == ADDR_TEMP) {
                temp_live_info[instr->arg1->val.temp_id].is_live = true;
                temp_live_info[instr->arg1->val.temp_id].next_use = current_line;
            }
        }

        // 3. Set `arg2` to live, next_use = this instruction's line
        if (instr->arg2) {
             if (instr->arg2->kind == ADDR_VARIABLE && instr->arg2->val.var) {
                instr->arg2->val.var->is_live = true;
                instr->arg2->val.var->next_use = current_line;
            } else if (instr->arg2->kind == ADDR_TEMP) {
                temp_live_info[instr->arg2->val.temp_id].is_live = true;
                temp_live_info[instr->arg2->val.temp_id].next_use = current_line;
            }
        }
        
        // Special case: TAC_STORE (*res = arg1)
        // 'res' is used as an address (like an arg1)
        if (instr->op == TAC_STORE) {
            if (instr->res) {
                 if (instr->res->kind == ADDR_VARIABLE && instr->res->val.var) {
                    instr->res->val.var->is_live = true;
                    instr->res->val.var->next_use = current_line;
                } else if (instr->res->kind == ADDR_TEMP) {
                    temp_live_info[instr->res->val.temp_id].is_live = true;
                    temp_live_info[instr->res->val.temp_id].next_use = current_line;
                }
            }
        }

        instr = instr->prev;
    }
}

// Assign stack offsets and return lists of variables/temporaries in the function
static List* assign_stack_offsets(BasicBlock* func_start_block, List** temps_list_out) {
    current_stack_offset = 0; // Reset for new function
    
    // Clear temp offset map (this IS per-function)
    while (temp_offset_map) {
        TempOffset* next = temp_offset_map->next;
        free(temp_offset_map);
        temp_offset_map = next;
    }
    
    // temp_live_info is now global and allocated ONCE. Do not free/alloc here.
    
    List* vars_in_func = list_create();
    List* temps_in_func = list_create();

    BasicBlock* block = func_start_block;
    while(block) {
        TacInstr* instr = block->start;
        bool done = false;
        while (!done) {
            if (instr == block->end->next) done = true;
            if (done) break;

            TacAddr* addrs[] = { instr->res, instr->arg1, instr->arg2 };
            for (int i=0; i < 3; i++) {
                if (addrs[i] == NULL) continue;
                if (addrs[i]->kind == ADDR_VARIABLE) {
                    // Only allocate stack space for locals/params
                    if (addrs[i]->val.var->scope_level > 0 && !list_contains(vars_in_func, addrs[i]->val.var)) {
                        list_append(vars_in_func, addrs[i]->val.var);
                    }
                } else if (addrs[i]->kind == ADDR_TEMP) {
                    // max_temp_id is now global, do not update it here
                    if (!list_contains(temps_in_func, (void*)(long)addrs[i]->val.temp_id)) {
                        list_append(temps_in_func, (void*)(long)addrs[i]->val.temp_id);
                    }
                }
            }
            
            if (instr->op == TAC_END_FUNC) {
                block = NULL; // End of function
                break;
            }
            instr = instr->next;
        }
        if (block) block = block->next;
    }

    // --- Assign offsets ---
    // Assign offsets to locals
    for (ListNode* n = list_begin(vars_in_func); n; n = list_next(n)) {
        Symbol* s = (Symbol*)list_get_data(n);
        int size = get_type_size(s->type);
        if (size == 0) size = 4; // Default to 4 bytes if size is unknown (e.g. void*)
        current_stack_offset -= size; // Allocate space
        s->stack_offset = current_stack_offset;
        mips_comment("Assigning stack offset %d to %s (size %d)", s->stack_offset, s->name, size);
    }
    
    // Assign offsets to temporaries
    for (ListNode* n = list_begin(temps_in_func); n; n = list_next(n)) {
        int temp_id = (int)(long)list_get_data(n);
        current_stack_offset -= 4; // Assume all temps are 4 bytes (int/float/ptr)
        
        TempOffset* t_off = (TempOffset*)calloc(1, sizeof(TempOffset));
        t_off->temp_id = temp_id;
        t_off->offset = current_stack_offset;
        t_off->next = temp_offset_map;
        temp_offset_map = t_off;
        mips_comment("Assigning stack offset %d to t%d", t_off->offset, t_off->temp_id);
    }
    
    // --- Liveness table allocation is REMOVED from here ---

    *temps_list_out = temps_in_func;
    return vars_in_func;
}

/*
================================================================================
 MIPS EMITTERS (PROLOGUE, EPILOGUE, DATA)
================================================================================
*/

static void mips_emit_data_segment(TacInstr* tac_head) {
    mips_emit(".data");
    
    // Emit string literals (already collected by tac.c)
    extern StringLiteral* string_literal_list;
    // We need to reverse the list for printing in order
    StringLiteral* prev = NULL;
    StringLiteral* curr = string_literal_list;
    while (curr) {
        StringLiteral* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    string_literal_list = prev;
    
    for (StringLiteral* sl = string_literal_list; sl; sl = sl->next) {
        mips_emit("%s: .asciiz \"%s\"", sl->label, sl->value);
    }

    // --- NEW: Add 0.0 float constant for comparisons ---
    mips_emit("f_const_zero: .float 0.0");
    // Emit global variables
    mips_emit(".align 2");
    
    List* globals_done = list_create();
    for (TacInstr* instr = tac_head; instr; instr = instr->next) {
        TacAddr* addrs[] = {instr->res, instr->arg1, instr->arg2};
        for (int i=0; i<3; i++) {
            if (addrs[i] && addrs[i]->kind == ADDR_VARIABLE && addrs[i]->val.var->scope_level == 0) {
                Symbol* s = addrs[i]->val.var;
                if (s->type->kind != TYPE_FUNCTION && !list_contains(globals_done, s)) {
                    mips_emit("%s: .space %d", s->name, get_type_size(s->type));
                    list_append(globals_done, s);
                }
            }
        }
    }
    list_destroy(globals_done);
    mips_emit(""); // Newline
}

static void mips_emit_text_segment() {
    mips_emit(".text");
    mips_emit(".globl main"); // Assume main is the entry point
    mips_emit("");
    
    // --- NEW: Load 0.0 into its dedicated register ---
    mips_emit("l.s %s, f_const_zero", TEMP_FPR_ZERO);
}

static void mips_emit_prologue(Symbol* func_sym) {
    mips_emit("\n# --- Function Prologue: %s ---", func_sym->name);
    mips_emit("%s:", func_sym->name); // <-- Add function label
    
    // 1. Save return address and old frame pointer
    mips_emit("subu $sp, $sp, 8"); // Make space for $ra, $fp
    mips_emit("sw $ra, 4($sp)");
    mips_emit("sw $fp, 0($sp)");
    
    // 2. Set new frame pointer
    mips_emit("move $fp, $sp");
    
    // 3. Allocate space for all locals and temporaries
    // current_stack_offset already holds the *negative* size
    int frame_size = -current_stack_offset;
    if (frame_size > 0) {
        mips_emit("subu $sp, $sp, %d", frame_size);
        mips_comment("Stack frame size: %d bytes (for locals/temps)", frame_size);
    }
}

static void mips_emit_epilogue(Symbol* func_sym) {
    // Epilogue is often emitted by TAC_RETURN, but we need a
    // dedicated label for functions that fall off the end.
    mips_emit("\n.L%s_epilogue:", func_sym->name);
    mips_comment("# --- Function Epilogue: %s ---", func_sym->name);
    
    // 1. Deallocate local/temp stack frame
    mips_emit("move $sp, $fp"); // Restore SP to where FP was
    
    // 2. Restore $fp and $ra
    mips_emit("lw $ra, 4($sp)");
    mips_emit("lw $fp, 0($sp)");
    mips_emit("addu $sp, $sp, 8");
    
    // 3. Return
    mips_emit("jr $ra");
    mips_emit("");
}

/*
================================================================================
 REGISTER ALLOCATION (The Core)
================================================================================
*/

// // Spill a register: Write all vars it contains back to memory
// static void spill_reg(int reg_index) {
//     List* vars_in_reg = temp_reg_desc[reg_index].vars;
//     ListNode* n = list_begin(vars_in_reg);
//     char loc_buf[64];
    
//     if (list_is_empty(vars_in_reg)) return;
    
//     mips_comment("Spilling register %s", temp_reg_names[reg_index]);
    
//     while (n) {
//         TacAddr* var = (TacAddr*)list_get_data(n);
//         get_mem_loc_str(loc_buf, 64, var);
        
//         mips_emit("sw %s, %s", temp_reg_names[reg_index], loc_buf);
//         ad_set_var_in_mem(var); // Update AD: var is now (also) in memory
        
//         n = list_next(n);
//     }
// }

// Spill a GPR: Write all vars it contains back to memory
static void spill_gpr(int reg_index) {
    List* vars_in_reg = temp_reg_desc[reg_index].vars;
    ListNode* n = list_begin(vars_in_reg);
    char loc_buf[64];
    
    if (list_is_empty(vars_in_reg)) return;
    
    mips_comment("Spilling GPR %s", temp_reg_names[reg_index]);
    
    while (n) {
        TacAddr* var = (TacAddr*)list_get_data(n);

        // --- NEW CHECK: Can this var be spilled to memory? ---
        bool has_spill_loc = false;
        if (var->kind == ADDR_VARIABLE) {
            has_spill_loc = true; // All variables have a location (global or stack)
        } else if (var->kind == ADDR_TEMP) {
            // Check if it's in the temp_offset_map
            for (TempOffset* t = temp_offset_map; t; t = t->next) {
                if (t->temp_id == var->val.temp_id) {
                    has_spill_loc = true;
                    break;
                }
            }
        }
        // --- END CHECK ---

        if (has_spill_loc) {
            get_mem_loc_str(loc_buf, 64, var);
            // Ensure location wasn't an error
            if (strncmp(loc_buf, "TEMP_NOT_FOUND", 14) != 0) {
                mips_emit("sw %s, %s", temp_reg_names[reg_index], loc_buf);
                ad_set_var_in_mem(var); 
            } else {
                mips_comment("ERROR: Spill location for t%d not found, spill skipped.", var->val.temp_id);
            }
        } else {
            // This var (e.g., global t0) is in a register but has
            // no home on the stack. We can't spill it.
            mips_comment("WARNING: Evicting temp t%d from %s without spilling (no stack loc).", var->val.temp_id, temp_reg_names[reg_index]);
        }
        
        n = list_next(n);
    }
    // The caller (e.g., getGPR) will call rd_clear_gpr() to finalize.
}

// --- NEW: Spill an FPR ---
static void spill_fpr(int reg_index) {
    List* vars_in_reg = float_reg_desc[reg_index].vars;
    ListNode* n = list_begin(vars_in_reg);
    char loc_buf[64];
    
    if (list_is_empty(vars_in_reg)) return;
    
    mips_comment("Spilling FPR %s", float_reg_names[reg_index]);
    
    while (n) {
        TacAddr* var = (TacAddr*)list_get_data(n);

        // --- NEW CHECK: Can this var be spilled to memory? ---
        bool has_spill_loc = false;
        if (var->kind == ADDR_VARIABLE) {
            has_spill_loc = true; // All variables have a location (global or stack)
        } else if (var->kind == ADDR_TEMP) {
            // Check if it's in the temp_offset_map
            for (TempOffset* t = temp_offset_map; t; t = t->next) {
                if (t->temp_id == var->val.temp_id) {
                    has_spill_loc = true;
                    break;
                }
            }
        }
        // --- END CHECK ---

        if (has_spill_loc) {
            get_mem_loc_str(loc_buf, 64, var);
            // Ensure location wasn't an error
            if (strncmp(loc_buf, "TEMP_NOT_FOUND", 14) != 0) {
                mips_emit("s.s %s, %s", float_reg_names[reg_index], loc_buf); // Store Single
                ad_set_var_in_mem(var);
            } else {
                mips_comment("ERROR: Spill location for t%d not found, spill skipped.", var->val.temp_id);
            }
        } else {
            mips_comment("WARNING: Evicting temp t%d from %s without spilling (no stack loc).", var->val.temp_id, float_reg_names[reg_index]);
        }
        
        n = list_next(n);
    }
    // The caller (e.g., getFPR) will call rd_clear_fpr() to finalize.
}

// Find the best register to spill using Next-Use info
static int find_gpr_to_spill() {
    int best_reg = -1;
    int max_next_use = -1; // We want to spill the var with the *furthest* next use

    for (int i = 0; i < NUM_TEMP_REGS; i++) {
        List* vars_in_reg = temp_reg_desc[i].vars;
        if (list_is_empty(vars_in_reg)) {
            return i; // Found an empty register, no spill needed
        }
        
        // Check next-use of all vars in this reg
        int reg_max_next_use = -1;
        bool all_dead = true;
        
        for (ListNode* n = list_begin(vars_in_reg); n; n = list_next(n)) {
            TacAddr* var = (TacAddr*)list_get_data(n);
            bool is_live = false;
            int next_use = -1;
            
            if (var->kind == ADDR_VARIABLE && var->val.var) {
                is_live = var->val.var->is_live;
                next_use = var->val.var->next_use;
            } else if (var->kind == ADDR_TEMP && temp_live_info) {
                is_live = temp_live_info[var->val.temp_id].is_live;
                next_use = temp_live_info[var->val.temp_id].next_use;
            }

            if (is_live) {
                all_dead = false;
                if (next_use == -1) { // Live, but no next use (e.g., live-out)
                    next_use = 999999; // Treat as very far
                }
                if (next_use > reg_max_next_use) {
                    reg_max_next_use = next_use;
                }
            }
        }
        
        if (all_dead) {
            return i; // All vars in this reg are dead, perfect
        }

        if (reg_max_next_use > max_next_use) {
            max_next_use = reg_max_next_use;
            best_reg = i;
        }
    }
    
    return (best_reg != -1) ? best_reg : 0; // Default to $t0 if all else fails
}

static int find_fpr_to_spill() {
    int best_reg = -1;
    int max_next_use = -1; // We want to spill the var with the *furthest* next use

    for (int i = 0; i < NUM_FLOAT_REGS; i++) {
        List* vars_in_reg = float_reg_desc[i].vars;
        if (list_is_empty(vars_in_reg)) {
            return i; // Found an empty register, no spill needed
        }
        
        // Check next-use of all vars in this reg
        int reg_max_next_use = -1;
        bool all_dead = true;
        
        for (ListNode* n = list_begin(vars_in_reg); n; n = list_next(n)) {
            TacAddr* var = (TacAddr*)list_get_data(n);
            bool is_live = false;
            int next_use = -1;
            
            if (var->kind == ADDR_VARIABLE && var->val.var) {
                is_live = var->val.var->is_live;
                next_use = var->val.var->next_use;
            } else if (var->kind == ADDR_TEMP && temp_live_info) {
                is_live = temp_live_info[var->val.temp_id].is_live;
                next_use = temp_live_info[var->val.temp_id].next_use;
            }

            if (is_live) {
                all_dead = false;
                if (next_use == -1) { // Live, but no next use (e.g., live-out)
                    next_use = 999999; // Treat as very far
                }
                if (next_use > reg_max_next_use) {
                    reg_max_next_use = next_use;
                }
            }
        }
        
        if (all_dead) {
            return i; // All vars in this reg are dead, perfect
        }

        if (reg_max_next_use > max_next_use) {
            max_next_use = reg_max_next_use;
            best_reg = i;
        }
    }
    
    return (best_reg != -1) ? best_reg : 0; // Default to $t0 if all else fails
}

// The main `getGPR` algorithm
static const char* getGPR(TacInstr* instr, TacAddr* var, bool is_res) {
    char loc_buf[64];
    
    if (var->kind == ADDR_CONSTANT_INT) {
        int reg_index = find_gpr_to_spill();
        MipsRegister* reg = &temp_reg_desc[reg_index];
        spill_gpr(reg_index);
        rd_clear_gpr(reg_index);
        mips_emit("li %s, %d", reg->name, var->val.const_int);
        return reg->name;
    }

    if (var->kind == ADDR_STRING) {
        int reg_index = find_gpr_to_spill();
        MipsRegister* reg = &temp_reg_desc[reg_index];
        spill_gpr(reg_index);
        rd_clear_gpr(reg_index);
        mips_emit("la %s, %s", reg->name, var->val.string_label);
        // Don't track string constants in AD/RD, just load them
        return reg->name;
    }

    AddrDesc* ad = get_or_create_ad_entry(var);
    for (ListNode* n = list_begin(ad->locations); n; n = list_next(n)) {
        AddrLocation* loc = (AddrLocation*)list_get_data(n);
        if (loc->kind == LOC_GPR) {
            return temp_reg_names[loc->reg_index]; // Found it!
        }
    }

    int reg_index = find_gpr_to_spill();
    MipsRegister* reg = &temp_reg_desc[reg_index];
    spill_gpr(reg_index);
    rd_clear_gpr(reg_index); 

    if (!is_res) {
        mips_emit("lw %s, %s", reg->name, get_mem_loc_str(loc_buf, 64, var));
    }
    
    ad_set_var_in_gpr(var, reg_index);
    rd_add_var_to_gpr(var, reg_index);

    return reg->name;
}

// --- NEW: The main `getFPR` algorithm ---
static const char* getFPR(TacInstr* instr, TacAddr* var, bool is_res) {
    char loc_buf[64];
    
    if (var->kind == ADDR_CONSTANT_FLOAT) {
        int reg_index = find_fpr_to_spill();
        MipsRegister* reg = &float_reg_desc[reg_index];
        spill_fpr(reg_index);
        rd_clear_fpr(reg_index);

        // Load float constant from memory
        mips_emit(".data");
        mips_emit("f_const_%d: .float %f", label_count, var->val.const_float);
        mips_emit(".text");
        mips_emit("l.s %s, f_const_%d", reg->name, label_count);
        label_count++;
        return reg->name;
    }

    AddrDesc* ad = get_or_create_ad_entry(var);
    for (ListNode* n = list_begin(ad->locations); n; n = list_next(n)) {
        AddrLocation* loc = (AddrLocation*)list_get_data(n);
        if (loc->kind == LOC_FPR) {
            return float_reg_names[loc->reg_index]; // Found it!
        }
    }

    int reg_index = find_fpr_to_spill();
    MipsRegister* reg = &float_reg_desc[reg_index];
    spill_fpr(reg_index);
    rd_clear_fpr(reg_index); 

    if (!is_res) {
        mips_emit("l.s %s, %s", reg->name, get_mem_loc_str(loc_buf, 64, var));
    }
    
    ad_set_var_in_fpr(var, reg_index);
    rd_add_var_to_fpr(var, reg_index);

    return reg->name;
}

// The main `getReg` algorithm
// static const char* get_reg(TacInstr* instr, TacAddr* var, bool is_res) {
//     char loc_buf[64];
    
//     // 0. Handle constants directly
//     if (var->kind == ADDR_CONSTANT_INT) {
//         // Find a free register to load this into
//         int reg_index = find_reg_to_spill();
//         MipsRegister* reg = &temp_reg_desc[reg_index];
//         spill_reg(reg_index);
//         rd_clear_reg(reg_index);
//         mips_emit("li %s, %d", reg->name, var->val.const_int);
//         // We don't update AD/RD for constants, they are "pure"
//         return reg->name;
//     }
//     if (var->kind == ADDR_CONSTANT_FLOAT) {
//         // FIX: Add these lines to declare 'reg'
//         int reg_index = find_reg_to_spill();
//         MipsRegister* reg = &temp_reg_desc[reg_index];
//         spill_reg(reg_index);
//         rd_clear_reg(reg_index);

//         mips_comment("Loading float constant %f", var->val.const_float);
//         mips_emit(".data");
//         mips_emit("f_const_%d: .float %f", label_count, var->val.const_float);
//         mips_emit(".text");
//         // FIX: Use lw (load word) into the GPR. This is semantically
//         // wrong (should use FPR and l.s) but will compile.
//         mips_emit("lw %s, f_const_%d", reg->name, label_count);
//         label_count++;
//         return reg->name; // FIX: Add return
//     }

//     // 1. Check if 'var' is already in a register
//     AddrDesc* ad = get_or_create_ad_entry(var);
//     for (ListNode* n = list_begin(ad->locations); n; n = list_next(n)) {
//         AddrLocation* loc = (AddrLocation*)list_get_data(n);
//         if (loc->kind == LOC_REG) {
//             return temp_reg_names[loc->reg_index]; // Found it!
//         }
//     }

//     // 2. Not in a register. Find one to load it into.
//     int reg_index = find_reg_to_spill();
//     MipsRegister* reg = &temp_reg_desc[reg_index];

//     // 3. Spill the chosen register (if it holds anything)
//     spill_reg(reg_index);
//     rd_clear_reg(reg_index); // Clear RD entry

//     // 4. Load 'var' into the register
//     //    (But only if we're not using this for a result)
//     if (!is_res) {
//         mips_emit("lw %s, %s", reg->name, get_mem_loc_str(loc_buf, 64, var));
//     }
    
//     // 5. Update Descriptors
//     ad_set_var_in_reg(var, reg_index);
//     rd_add_var_to_reg(var, reg_index);

//     return reg->name;
// }

// After an op, check if operand registers can be freed
static void free_dead_regs(TacInstr* instr) {
    TacAddr* addrs[] = { instr->arg1, instr->arg2 };
    int current_line = instr->lineno;

    for (int i=0; i<2; i++) {
        TacAddr* var = addrs[i];
        if (var == NULL) continue;
        
        bool is_live = true; 
        int next_use = -1;

        if (var->kind == ADDR_VARIABLE && var->val.var) {
            is_live = var->val.var->is_live;
            next_use = var->val.var->next_use;
        } else if (var->kind == ADDR_TEMP && temp_live_info) {
            is_live = temp_live_info[var->val.temp_id].is_live;
            next_use = temp_live_info[var->val.temp_id].next_use;
        } else if (var->kind == ADDR_CONSTANT_INT || var->kind == ADDR_CONSTANT_FLOAT) {
            is_live = false; 
        } else if (var->kind == ADDR_STRING) {
            is_live = false;
        }
        
        if (!is_live || next_use == current_line) {
            AddrDesc* ad = get_or_create_ad_entry(var);
            ListNode* n = list_begin(ad->locations);
            while (n) {
                AddrLocation* loc = (AddrLocation*)list_get_data(n);
                if (loc->kind == LOC_GPR) {
                    rd_remove_var_from_gpr(var, loc->reg_index);
                    n = list_next(n); // Get next before removing
                    list_remove(ad->locations, loc);
                    free(loc);
                } else if (loc->kind == LOC_FPR) {
                    rd_remove_var_from_fpr(var, loc->reg_index);
                    n = list_next(n); // Get next before removing
                    list_remove(ad->locations, loc);
                    free(loc);
                } else {
                    n = list_next(n);
                }
            }
        }
    }
}


/*
================================================================================
 INSTRUCTION-BY-INSTRUCTION TRANSLATION
================================================================================
*/

static void gen_mips_for_instr(TacInstr* instr, Symbol* func_sym) { // <-- MODIFY THIS
    const char *r_res, *r_arg1, *r_arg2;
    
    // Check type of operands
    bool is_float_op = false;
    if (instr->arg1 && instr->arg1->type && 
       (instr->arg1->type->kind == TYPE_FLOAT || instr->arg1->type->kind == TYPE_DOUBLE)) {
        is_float_op = true;
    }
    if (instr->res && instr->res->type && 
       (instr->res->type->kind == TYPE_FLOAT || instr->res->type->kind == TYPE_DOUBLE)) {
        is_float_op = true;
    }

    // Handle float-specific ops separately
    switch(instr->op) {
        case TAC_FADD: case TAC_FSUB: case TAC_FMUL: case TAC_FDIV: case TAC_FNEG:
        case TAC_FLT: case TAC_FLE: case TAC_FGT: case TAC_FGE: case TAC_FEQ: case TAC_FNE:
        case TAC_CVT_I2F: case TAC_CVT_F2I: case TAC_CVT_D2F: case TAC_CVT_F2D:
            is_float_op = true;
            break;
        default:
            // For ops like TAC_ASSIGN, we must check the type
            if (instr->op == TAC_ASSIGN && is_float_op) break;
            // All other ops are integer
            is_float_op = false; 
    }
    
    switch(instr->op) {
        // --- Function Start/End ---
        case TAC_BEGIN_FUNC:
            mips_emit_prologue(instr->res->val.var);
            break;
        case TAC_END_FUNC:
            mips_emit_epilogue(instr->res->val.var);
            break;

        // --- Labels and Jumps ---
        case TAC_LABEL:
            mips_emit("L%d:", instr->res->val.label_id);
            break;
        case TAC_GOTO:
            mips_emit("j L%d", instr->res->val.label_id);
            break;
        case TAC_IFZ: // if arg1 == 0 goto label
            if (is_float_op) {
                r_arg1 = getFPR(instr, instr->arg1, false);
                mips_emit("c.eq.s %s, %s", r_arg1, TEMP_FPR_ZERO); // Compare to 0.0
                mips_emit("bc1t L%d", instr->res->val.label_id); // Branch if true (is zero)
            } else {
                r_arg1 = getGPR(instr, instr->arg1, false);
                mips_emit("beqz %s, L%d", r_arg1, instr->res->val.label_id);
            }
            free_dead_regs(instr);
            break;
        case TAC_IFNZ: // if arg1 != 0 goto label
            if (is_float_op) {
                r_arg1 = getFPR(instr, instr->arg1, false);
                mips_emit("c.eq.s %s, %s", r_arg1, TEMP_FPR_ZERO); // Compare to 0.0
                mips_emit("bc1f L%d", instr->res->val.label_id); // Branch if false (is not zero)
            } else {
                r_arg1 = getGPR(instr, instr->arg1, false);
                mips_emit("bnez %s, L%d", r_arg1, instr->res->val.label_id);
            }
            free_dead_regs(instr);
            break;

        // --- Assignment ---
        case TAC_ASSIGN: // res = arg1
            if (is_float_op) {
                r_arg1 = getFPR(instr, instr->arg1, false);
                r_res = getFPR(instr, instr->res, true); 
                mips_emit("mov.s %s, %s", r_res, r_arg1);
            } else {
                r_arg1 = getGPR(instr, instr->arg1, false);
                r_res = getGPR(instr, instr->res, true); 
                mips_emit("move %s, %s", r_res, r_arg1);
            }
            free_dead_regs(instr);
            break;
            
        // --- Integer Arithmetic ---
        case TAC_ADD: 
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true); 
            mips_emit("addu %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_SUB:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("subu %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_MUL:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("mul %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_DIV:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("div %s, %s", r_arg1, r_arg2);
            mips_emit("mflo %s", r_res); // Get result from LO
            free_dead_regs(instr);
            break;
        case TAC_MOD: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("div %s, %s", r_arg1, r_arg2);
            mips_emit("mfhi %s", r_res); // Get remainder from HI
            free_dead_regs(instr);
            break;
        case TAC_NEG:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("subu %s, $zero, %s", r_res, r_arg1);
            free_dead_regs(instr);
            break;
            
        // --- Float Arithmetic ---
        case TAC_FADD: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getFPR(instr, instr->res, true);
            mips_emit("add.s %s, %s, %s", r_res, r_arg1, r_arg2); // .s for single
            free_dead_regs(instr);
            break;
        case TAC_FSUB: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getFPR(instr, instr->res, true);
            mips_emit("sub.s %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_FMUL: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getFPR(instr, instr->res, true);
            mips_emit("mul.s %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_FDIV: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getFPR(instr, instr->res, true);
            mips_emit("div.s %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_FNEG: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_res = getFPR(instr, instr->res, true);
            mips_emit("neg.s %s, %s", r_res, r_arg1);
            free_dead_regs(instr);
            break;

        // --- Type Conversions ---
        case TAC_CVT_F2I: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("cvt.w.s %s, %s", TEMP_FPR_REG, r_arg1); // Convert float to word (int) in FPR
            mips_emit("mfc1 %s, %s", r_res, TEMP_FPR_REG); // Move from coprocessor 1 to GPR
            free_dead_regs(instr);
            break;
        case TAC_CVT_I2F: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_res = getFPR(instr, instr->res, true);
            mips_emit("mtc1 %s, %s", r_arg1, TEMP_FPR_REG); // Move from GPR to coprocessor 1
            mips_emit("cvt.s.w %s, %s", r_res, TEMP_FPR_REG); // Convert word (int) to single (float)
            free_dead_regs(instr);
            break;
        case TAC_CVT_D2F: // <-- NEW (Assume .d for double, .s for float)
            r_arg1 = getFPR(instr, instr->arg1, false); // Pretend we got it
            r_res = getFPR(instr, instr->res, true);
            mips_emit("cvt.s.d %s, %s", r_res, r_arg1);
            free_dead_regs(instr);
            break;
        case TAC_CVT_F2D: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_res = getFPR(instr, instr->res, true);
            mips_emit("cvt.d.s %s, %s", r_res, r_arg1);
            free_dead_regs(instr);
            break;
            
        // --- Relational Ops (Integer) ---
        case TAC_LT:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("slt %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_LE:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("sle %s, %s, %s", r_res, r_arg1, r_arg2); // Pseudo-op
            free_dead_regs(instr);
            break;
        case TAC_GT:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("sgt %s, %s, %s", r_res, r_arg1, r_arg2); // Pseudo-op
            free_dead_regs(instr);
            break;
        case TAC_GE:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("sge %s, %s, %s", r_res, r_arg1, r_arg2); // Pseudo-op
            free_dead_regs(instr);
            break;
        case TAC_EQ:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("seq %s, %s, %s", r_res, r_arg1, r_arg2); // Pseudo-op
            free_dead_regs(instr);
            break;
        case TAC_NE:
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("sne %s, %s, %s", r_res, r_arg1, r_arg2); // Pseudo-op
            free_dead_regs(instr);
            break;

        // --- Relational Ops (Float) ---
        case TAC_FLT: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true); // Result is 0 or 1 (int)
            mips_emit("c.lt.s %s, %s", r_arg1, r_arg2);
            mips_emit("li %s, 0", r_res); // Assume false
            mips_emit("bc1t L%d", label_count);
            mips_emit("li %s, 1", r_res); // Is true
            mips_emit("L%d:", label_count++);
            free_dead_regs(instr);
            break;
        case TAC_FEQ: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("c.eq.s %s, %s", r_arg1, r_arg2);
            mips_emit("li %s, 0", r_res);
            mips_emit("bc1t L%d", label_count);
            mips_emit("li %s, 1", r_res);
            mips_emit("L%d:", label_count++);
            free_dead_regs(instr);
            break;
        case TAC_FLE: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("c.le.s %s, %s", r_arg1, r_arg2);
            mips_emit("li %s, 0", r_res);
            mips_emit("bc1t L%d", label_count);
            mips_emit("li %s, 1", r_res);
            mips_emit("L%d:", label_count++);
            free_dead_regs(instr);
            break;
        case TAC_FGT: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("c.gt.s %s, %s", r_arg1, r_arg2);
            mips_emit("li %s, 0", r_res);
            mips_emit("bc1t L%d", label_count);
            mips_emit("li %s, 1", r_res);
            mips_emit("L%d:", label_count++);
            free_dead_regs(instr);
            break;
        case TAC_FGE: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("c.ge.s %s, %s", r_arg1, r_arg2);
            mips_emit("li %s, 0", r_res);
            mips_emit("bc1t L%d", label_count);
            mips_emit("li %s, 1", r_res);
            mips_emit("L%d:", label_count++);
            free_dead_regs(instr);
            break;
        case TAC_FNE: // <-- NEW
            r_arg1 = getFPR(instr, instr->arg1, false);
            r_arg2 = getFPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("c.eq.s %s, %s", r_arg1, r_arg2);
            mips_emit("li %s, 0", r_res);
            mips_emit("bc1t L%d", label_count);
            mips_emit("li %s, 1", r_res);
            mips_emit("L%d:", label_count++);
            free_dead_regs(instr);
            break;

        // --- Bitwise and Logical ---
        case TAC_LSHIFT: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("sllv %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_RSHIFT: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("srav %s, %s, %s", r_res, r_arg1, r_arg2); // Arithmetic shift
            free_dead_regs(instr);
            break;
        case TAC_BAND: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("and %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_BOR: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("or %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_BXOR: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_arg2 = getGPR(instr, instr->arg2, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("xor %s, %s, %s", r_res, r_arg1, r_arg2);
            free_dead_regs(instr);
            break;
        case TAC_BNOT: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("nor %s, %s, $zero", r_res, r_arg1); // a NOR 0 == NOT a
            free_dead_regs(instr);
            break;
        case TAC_NOT: // <-- NEW
            r_arg1 = getGPR(instr, instr->arg1, false);
            r_res = getGPR(instr, instr->res, true);
            mips_emit("seq %s, %s, $zero", r_res, r_arg1); // Set if $r_arg1 == 0
            free_dead_regs(instr);
            break;

        // --- Function Calls ---
        case TAC_PARAM:
            // This is complex. Standard MIPS passes first 4 args in $a0-$a3.
            // We'll stick to the simple (but slow) stack-only version for now.
            if (is_float_op) {
                r_arg1 = getFPR(instr, instr->arg1, false);
                mips_emit("subu $sp, $sp, 4");
                mips_emit("s.s %s, 0($sp)", r_arg1);
            } else {
                r_arg1 = getGPR(instr, instr->arg1, false);
                mips_emit("subu $sp, $sp, 4"); 
                mips_emit("sw %s, 0($sp)", r_arg1); 
            }
            free_dead_regs(instr);
            break;
            
        case TAC_CALL:
            // NOTE: This does not yet handle runtime lib calls like strcat
            mips_emit("jal %s", instr->arg1->val.var->name);
            if (instr->arg2->kind == ADDR_CONSTANT_INT) {
                int num_params = instr->arg2->val.const_int;
                if (num_params > 0) {
                    mips_emit("addu $sp, $sp, %d", num_params * 4);
                }
            }
            if (instr->res) {
                if(is_float_op) {
                    r_res = getFPR(instr, instr->res, true);
                    mips_emit("mov.s %s, $f0", r_res); // Float return is in $f0
                } else {
                    r_res = getGPR(instr, instr->res, true);
                    mips_emit("move %s, $v0", r_res); // Int return is in $v0
                }
            }
            break;

        case TAC_RETURN:
            if (instr->arg1) {
                if(is_float_op) {
                    r_arg1 = getFPR(instr, instr->arg1, false);
                    mips_emit("mov.s $f0, %s", r_arg1); // Put float return val in $f0
                } else {
                    r_arg1 = getGPR(instr, instr->arg1, false);
                    mips_emit("move $v0, %s", r_arg1); // Put int return val in $v0
                }
            }
            // Symbol* func_sym = block->start->res->val.var; // <-- THIS IS THE CRASH, DELETE IT
            if (func_sym) { // <-- USE THE PASSED-IN SYMBOL
                mips_emit("j .L%s_epilogue", func_sym->name);
            } else {
                mips_comment("ERROR: Could not find function for return, using unknown epilogue");
                mips_emit("j .L_unknown_epilogue");
            }
            break;

        // --- Memory and Pointers ---
        case TAC_ADDR: // res = &arg1
            r_res = getGPR(instr, instr->res, true);
            emit_load_address(r_res, instr->arg1);
            free_dead_regs(instr);
            break;

        case TAC_DEREF: // res = *arg1 (load)
            r_arg1 = getGPR(instr, instr->arg1, false); // Get pointer value
            if (is_float_op) {
                r_res = getFPR(instr, instr->res, true);   // Get reg for float result
                mips_emit("l.s %s, 0(%s)", r_res, r_arg1); // Load Single
            } else {
                r_res = getGPR(instr, instr->res, true);   // Get reg for int result
                mips_emit("lw %s, 0(%s)", r_res, r_arg1);
            }
            free_dead_regs(instr);
            break;
            
        case TAC_STORE: // *res = arg1 (store)
            r_res = getGPR(instr, instr->res, false);  // Get address
            if (is_float_op) {
                r_arg1 = getFPR(instr, instr->arg1, false); // Get float value
                mips_emit("s.s %s, 0(%s)", r_arg1, r_res); // Store Single
            } else {
                r_arg1 = getGPR(instr, instr->arg1, false); // Get int value
                mips_emit("sw %s, 0(%s)", r_arg1, r_res);
            }
            free_dead_regs(instr);
            break;

        default:
            mips_comment("WARNING: MIPS generation not implemented for op %d", instr->op);
            break;
    }
}

// At the end of a basic block, write all "dirty" registers back to memory.
static void store_regs_at_block_end(BasicBlock* block) {
    char loc_buf[64];
    mips_comment("--- End of Basic Block %d ---", block->id);
    
    bool is_exit = (block->end->op == TAC_GOTO || block->end->op == TAC_RETURN);

    // --- Spill GPRs ---
    for (int i = 0; i < NUM_TEMP_REGS; i++) {
        List* vars_in_reg = temp_reg_desc[i].vars;
        if (list_is_empty(vars_in_reg)) continue;
        if (is_exit) {
            spill_gpr(i);
        } else {
            for (ListNode* n = list_begin(vars_in_reg); n; n = list_next(n)) {
                // ... (liveness check is unchanged) ...
                TacAddr* var = (TacAddr*)list_get_data(n);
                bool is_live = false;
                if (var->kind == ADDR_VARIABLE && var->val.var) {
                    is_live = var->val.var->is_live;
                } else if (var->kind == ADDR_TEMP && temp_live_info) {
                    is_live = temp_live_info[var->val.temp_id].is_live;
                }
                if (is_live) {
                    mips_emit("sw %s, %s", temp_reg_names[i], get_mem_loc_str(loc_buf, 64, var));
                    ad_set_var_in_mem(var);
                }
            }
        }
        rd_clear_gpr(i); // Clear after block
    }
    
    // --- NEW: Spill FPRs ---
    for (int i = 0; i < NUM_FLOAT_REGS; i++) {
        List* vars_in_reg = float_reg_desc[i].vars;
        if (list_is_empty(vars_in_reg)) continue;
        if (is_exit) {
            spill_fpr(i);
        } else {
            for (ListNode* n = list_begin(vars_in_reg); n; n = list_next(n)) {
                // ... (liveness check is identical) ...
                TacAddr* var = (TacAddr*)list_get_data(n);
                bool is_live = false;
                if (var->kind == ADDR_VARIABLE && var->val.var) {
                    is_live = var->val.var->is_live;
                } else if (var->kind == ADDR_TEMP && temp_live_info) {
                    is_live = temp_live_info[var->val.temp_id].is_live;
                }
                if (is_live) {
                    mips_emit("s.s %s, %s", float_reg_names[i], get_mem_loc_str(loc_buf, 64, var));
                    ad_set_var_in_mem(var);
                }
            }
        }
        rd_clear_fpr(i); // Clear after block
    }
}


/*
================================================================================
 MAIN ENTRY POINT
================================================================================
*/
void generate_mips(TacInstr* tac_head, const char* out_filename) {
    mips_file = fopen(out_filename, "w");
    if (!mips_file) {
        perror("Could not open MIPS output file");
        return;
    }

    mips_emit_data_segment(tac_head);
    mips_emit_text_segment(); // This will now init TEMP_FPR_ZERO
    build_basic_blocks(tac_head);

    // --- NEW PRE-SCAN FOR ALL TEMPS ---
    max_temp_id = -1; // Start at -1
    for (TacInstr* instr = tac_head; instr; instr = instr->next) {
        TacAddr* addrs[] = { instr->res, instr->arg1, instr->arg2 };
        for (int i=0; i < 3; i++) {
            if (addrs[i] && addrs[i]->kind == ADDR_TEMP) {
                if (addrs[i]->val.temp_id > max_temp_id) {
                    max_temp_id = addrs[i]->val.temp_id;
                }
            }
        }
    }
    
    // --- NEW GLOBAL ALLOCATION FOR LIVENESS ---
    if (max_temp_id >= 0) {
        temp_live_info = (TempLiveness*)calloc(max_temp_id + 1, sizeof(TempLiveness));
        if (!temp_live_info) {
            fprintf(stderr, "Fatal: Could not allocate memory for temp liveness info.\n");
            exit(1);
        }
    } else {
        temp_live_info = NULL; // No temps used at all
    }
    // --- END NEW ---

    Symbol* current_function_symbol = NULL;
    List* current_func_vars = NULL;
    List* current_func_temps = NULL;

    for (BasicBlock* block = block_list_head; block; block = block->next) {
        
        mips_comment("\n# --- Basic Block %d (Start: L%d, End: L%d) ---", 
            block->id, block->start->lineno, block->end->lineno);
        
        if (block->start->op == TAC_BEGIN_FUNC) {
            if (current_func_vars) list_destroy(current_func_vars);
            if (current_func_temps) list_destroy(current_func_temps);
            
            current_function_symbol = block->start->res->val.var;
            
            // This function now just assigns offsets and collects lists
            current_func_vars = assign_stack_offsets(block, &current_func_temps);
        }

        // This is now SAFE to call on global blocks because temp_live_info is allocated
        compute_liveness(block, current_func_vars);
        
        init_gpr_desc();
        init_float_reg_desc();
        init_addr_desc(); 

        TacInstr* instr = block->start;
        bool done = false;
        while (!done) {
            if (instr == block->end->next) done = true;
            if (done) break;

            mips_comment("L%d:", instr->lineno);
            gen_mips_for_instr(instr, current_function_symbol);

            if (instr->op == TAC_END_FUNC) {
                current_function_symbol = NULL; 
                
                // Clear function-specific data
                if (current_func_vars) {
                    list_destroy(current_func_vars);
                    current_func_vars = NULL;
                }
                 if (current_func_temps) {
                    list_destroy(current_func_temps);
                    current_func_temps = NULL;
                }
            }
            
            instr = instr->next;
        }
        
        store_regs_at_block_end(block);
    }
    
    // Final cleanup
    if (current_func_vars) list_destroy(current_func_vars);
    if (current_func_temps) list_destroy(current_func_temps);
    if (temp_live_info) {
        free(temp_live_info);
    }

    fclose(mips_file);
}

static char* get_mem_loc_str(char* buffer, int size, TacAddr* addr) {
    if (addr == NULL) {
        snprintf(buffer, size, "NULL_ADDR");
        return buffer;
    }

    switch (addr->kind) {
        case ADDR_VARIABLE:
            if (addr->val.var->scope_level == 0) {
                snprintf(buffer, size, "%s", addr->val.var->name);
            } else {
                snprintf(buffer, size, "%d($fp)", addr->val.var->stack_offset);
            }
            break;
        case ADDR_TEMP: {
            TempOffset* t = temp_offset_map;
            while(t) {
                if (t->temp_id == addr->val.temp_id) {
                    snprintf(buffer, size, "%d($fp)", t->offset);
                    return buffer;
                }
                t = t->next;
            }
            snprintf(buffer, size, "TEMP_NOT_FOUND(t%d)", addr->val.temp_id);
            break;
        }
        case ADDR_STRING:
            snprintf(buffer, size, "%s", addr->val.string_label);
            break;
        default:
            snprintf(buffer, size, "INVALID_MEM_LOC");
            break;
    }
    return buffer;
}
static void emit_load_address(const char* dst_reg, TacAddr* addr) {
    char buf[64];
    get_mem_loc_str(buf, sizeof buf, addr);
    char* paren = strchr(buf, '(');
    if (paren && strcmp(paren, "($fp)") == 0) {
        mips_emit("addiu %s, $fp, %d", dst_reg, atoi(buf));
    } else if (paren && strcmp(paren, "($sp)") == 0) {
        mips_emit("addiu %s, $sp, %d", dst_reg, atoi(buf));
    } else {
        mips_emit("la %s, %s", dst_reg, buf);
    }
}
