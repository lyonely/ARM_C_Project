#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include "datatypes.h"

// Writes an array of instructions into a binary file (/out/program.bin)
void write_to_file(Instruction *instructions, int num_instructions);

// Converts instruction from big-endian to little-endian format
void get_little_endian(Instruction *i);

// Gets instruction type of operation
Type get_type(Operation opcode);

// Sets bit 25 of instruction
void set_imm_field(Instruction* i);

// Sets bits 19-16 to rn number
void set_rn_field(int rn, Instruction* i);

// Sets bits 15-12 to rd number
void set_rd_field(int rd, Instruction* i);

// Sets bits 11-7 to shift amount
void set_op2reg_shiftamt_field(int amt, Instruction* i);

// Sets bits 6-5 to shift type
void set_op2reg_shifttype_field(Shift type, Instruction* i);

// Sets bits 11-8 to rs number, sets bit 7 = 0 and sets bit 4
// (register specified shift)
void set_op2reg_shiftreg_field(int rs, Instruction* i);

// Sets bits 3-0 to rm number
void set_op2reg_rm_field(int rm, Instruction* i);

#endif

