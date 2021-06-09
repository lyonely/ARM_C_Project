#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include "datatypes.h"

long read_binary(FILE* file, void *destination);

// performs right rotation on a 32-bit binary number
uint32_t rotate_right(uint32_t value, uint32_t rotation); 

// checks if CPSR flags should be updated
int is_set(Instruction instruction);

// (for data processing) checks if operand2 is an immediate constant
int is_immediate(Instruction instruction);

// (for data processing) returns the opcode
int opcode(Instruction instruction);

// returns the first operand register
int rd(Instruction instruction);

// returns the destination register
int rn(Instruction instruction);

// (for data processing) returns operand2
int operand2(Instruction instruction);

// (for single data transfer) checks if pre-indexing flag is set
int is_pre_indexing(Instruction instruction);

// (for single data transfer) checks if up bit is set
int is_up(Instruction instruction);

// (for single data transfer) checks if load bit is set
int is_load(Instruction instruction);

// returns the condition field
int get_cond(Instruction instruction);

// returns the instruction type
enum InstructionType get_instr_type(Instruction instruction);

// checks if an instruction is valid based on the cond
int instruction_is_valid(Instruction instruction, struct Registers* regs);

//checks if the accumulate bit is set
int accumulate(Instruction instruction);

//sets the v bit if the value is not zero
void set_v(Register *cpsr, int value);  

//sets the c bit if the value is not zero
void set_c(Register *cpsr, int value); 

//sets the z bit if the value is not zero
void set_z(Register *cpsr, int value); 

//sets the n bit if the value is not zero
void set_n(Register *cpsr, int value); 

//sets the n and z bit
void set_n_z(Register *cpsr, int result);

// resets CPSR register flags to 0
void reset_flags(Register *cpsr);

//(for multiply) returns the rn operand register
int rn_multiply(Instruction instruction);

//(for multiply) returns the destination register
int rd_multiply(Instruction instruction);

//(for multiply) returns the rs operand register
int rs_multiply(Instruction instruction);

//(for multiply) returns the rm operand register
int rm_multiply(Instruction instruction);

//prints the state of the registers
void print_registers(struct Registers *regs);

//prints non-zero memory
void print_memory(Byte* memory);

//(for branch) returns the offset
int sdt_offset(Instruction instruction); 

#endif

