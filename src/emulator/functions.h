#include <stdio.h>
#include "datatypes.h"

long readBinary(FILE* file, void *destination);

uint32_t is_set(Instruction instruction);

uint32_t is_immediate(Instruction instruction);

int opcode(Instruction instruction);

int rd(Instruction instruction);

int rn(Instruction instruction);

int operand2(Instruction instruction);

uint32_t is_pre_indexing(Instruction instruction);

uint32_t is_up(Instruction instruction);

uint32_t is_load(Instruction instruction);

int get_cond(Instruction instruction);
/*
Instr_Type get_instr_type(Instr instruction); #todo
*/
int instruction_is_valid(Instruction instruction, Registers* regs);

uint32_t accumulate(Instruction instruction);

/*
void set_n_z(Register *cpsr, int result); 
	
void set_v(Register *cpsr, int value);  

void set_c(Register *cpsr, int value); 

void set_z(Register *cpsr, int value); 

void set_n(Register *cpsr, int value); 
*/
int rnMultiply(Instruction instruction);

int rdMultiply(Instruction instruction);

int rsMultiply(Instruction instruction);

int rmMultiply(Instruction instruction);


/*
void print_registers(Registers *regs); #todo

void display_memory(Byte* memory, int memory_capacity); #todo
*/
int sdt_offset(Instruction instruction); 

