#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H
#include <stdint.h>
#include "functions.h"

// Bitwise AND
uint32_t and(Register *rd, Register *rn, uint32_t operand2);

// Bitwise exclusive OR
uint32_t eor(Register *rd, Register *rn, uint32_t operand2);

// Subtract operand2 from register
uint32_t sub(Register *rd, Register *rn, uint32_t operand2);

// Subtract register from operand2
uint32_t rsb(Register *rd, Register *rn, uint32_t operand2);

// Addition
uint32_t add(Register *rd, Register *rn, uint32_t operand2);

// Bitwise AND, result not written
uint32_t tst(Register *rn, uint32_t operand2);

// Bitwise XOR, result not written
uint32_t teq(Register *rn, uint32_t operand2);

// sub, result not written
uint32_t cmp(Register *rn, uint32_t operand2);

// Bitwise OR
uint32_t orr(Register *rd, Register *rn, uint32_t operand2);

// Move operand2 to dest
uint32_t mov(Register *rd, uint32_t operand2);

// Sets CPSR register flags based on the result, V flag unchanged
void set_flags(int opcode, Register cpsr, uint32_t result, uint32_t carry);

// Get carry bit based on operation and operands
uint32_t get_carry(int opcode, Register rn, uint32_t operand2); 

// Parse instruction into components
void process(Instruction i, struct Registers *regs);

// Calls operation with corresponding registers/operands
void execute(int opcode, Register rd, Register rn, uint32_t operand2, 
		uint32_t set_conds, Register cpsr);


#endif
