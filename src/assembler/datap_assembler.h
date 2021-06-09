#ifndef DATAP_ASSEMBLER_H
#define DATAP_ASSEMBLER_H

#include <stdint.h>
#include "datatypes.h"

// Returns opcode from mnemonic
int get_opcode(Operation op);

// Sets bits 31-28 to 1110 (always cond)
void set_cond_field(Instruction* i);

// Sets bit 25
void set_imm_field(Instruction* i);

// Sets bits 24-21 to specified opcode
void set_opcode_field(Opcode opcode, Instruction* i);

// Sets bit 20 if opcode is tst, teq, cmp
void set_flag_field(Opcode opcode, Instruction* i);

// Sets bits 19-16 to rn number
void set_rn_field(int rn, Instruction* i);

// Sets bits 15-12 to rd number
void set_rd_field(int rd, Instruction* i);

// Sets bits 11-8 to number of rotations
void set_op2imm_rotate_field(int rotation, Instruction* i);

// Sets bits 7-0 to immediate value
void set_op2imm_imm_field(int imm, Instruction* i);

// Sets bits 11-7 to shift amount
void set_op2reg_shiftamt_field(int amt, Instruction* i); 

// Sets bits 6-5 to shift type
void set_op2reg_shifttype_field(Shift type, Instruction* i);

// Sets bits 11-8 to rs number, bit 7 = 0 and sets bit 4 (register specified shift)
void set_op2reg_shiftreg_field(int rs, Instruction* i); 

// Sets bits 3-0 to rm number
void set_op2reg_rm_field(int rm, Instruction* i);

// Converts data processing struct into Instruction
void build_datap_instr(DataProcessingInstruction* instr, Instruction* i);

#endif

