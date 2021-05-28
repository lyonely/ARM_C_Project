#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H
#include <stdint.h>
#include "functions.h"
#include "shifter.h"

// Bitwise AND
void and(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Bitwise exclusive OR
void eor(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Subtract operand2 from register
void sub(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Subtract register from operand2
void rsb(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Addition
void add(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Bitwise AND, result not written
void tst(Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Bitwise XOR, result not written
void teq(Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// sub, result not written
void cmp(Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Bitwise OR
void orr(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds);

// Move operand2 to dest
void mov(Register *rd, uint32_t operand2, Register *cpsr, uint32_t set_conds);


// Parse instruction into components
void process(Instruction i, struct Registers *regs);

// Calls operation with corresponding registers/operands
void execute(int opcode, Register *rd, Register rn, uint32_t operand2, 
		uint32_t set_conds, Register *cpsr);


#endif
