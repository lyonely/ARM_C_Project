#include <stdio.h>
#include "datatypes.h"

long readBinary(FILE* file, void *destination);

// checks if CPSR flags should be updated
uint32_t is_set(Instruction instruction);

// (for data processing) checks if operand2 is an immediate constant
uint32_t is_immediate(Instruction instruction);

// (for data processing) returns the opcode
int opcode(Instruction instruction);

// returns the first operand register
int rd(Instruction instruction);

// returns the destination register
int rn(Instruction instruction);

// (for data processing) returns operand2
int operand2(Instruction instruction);

// (for single data transfer) checks if pre-indexing flag is set
uint32_t is_pre_indexing(Instruction instruction);

// (for single data transfer) checks if up bit is set
uint32_t is_up(Instruction instruction);

// (for single data transfer) checks if load bit is set
uint32_t is_load(Instruction instruction);

// returns the condition field
int get_cond(Instruction instruction);

// returns the instruction type
enum InstructionType get_instr_type(Instruction instruction);

// checks if an instruction is valid based on the cond
int instruction_is_valid(Instruction instruction, struct Registers* regs);

//checks if the accumulate bit is set
uint32_t accumulate(Instruction instruction);

//sets the v bit if the value is not zero
void set_v(Register *cpsr, int value);  

//sets the c bit if the value is not zero
void set_c(Register *cpsr, int value); 

//sets the z bit if the value is not zero
void set_z(Register *cpsr, int value); 

//sets the n bit if the value is not zero
void set_n(Register *cpsr, int value); 

//(for multiply) returns the rn operand register
int rnMultiply(Instruction instruction);

//(for multiply) returns the destination register
int rdMultiply(Instruction instruction);

//(for multiply) returns the rs operand register
int rsMultiply(Instruction instruction);

//(for multiply) returns the rm operand register
int rmMultiply(Instruction instruction);


/*
void print_registers(Registers *regs); #todo

void display_memory(Byte* memory, int memory_capacity); #todo
*/

//(for branch) returns the offset
int sdt_offset(Instruction instruction); 

