#include <stdio.h>
#include <stdint.h>
#include "functions.h"

// TODO: check 'Set Condition Codes' bit and set CPSR flags accordingly

// Performs right rotation on a 32-bit binary number
int rotate_right(uint32_t value, unsigned int rotation) {
	unsigned int shifted = value >> rotation;
	unsigned int rotated_bits = value << (32 - rotation);
	return (shifted | rotated_bits);
}

// Bitwise AND
uint32_t and(Register *rd, Register *rn, uint32_t operand2) {
	*rd = *rn & operand2;
	return *rd;
}

// Bitwise exclusive OR
uint32_t eor(Register *rd, Register *rn, uint32_t operand2) {
	*rd = (*rn & ~operand2) | (~*rn & operand2);
	return *rd;
}

// Subtract operand2 from register
uint32_t sub(Register *rd, Register *rn, uint32_t operand2) {
	*rd = *rn - operand2;
	return *rd;
}

// Subtract register from operand2
uint32_t rsb(Register *rd, Register *rn, uint32_t operand2) {
	*rd = operand2 - *rn;
	return *rd;
}


// Addition
uint32_t add(Register *rd, Register *rn, uint32_t operand2) {
	*rd = *rn + operand2;
	return *rd;
}

// Bitwise AND, result not written
uint32_t tst(Register *rn, uint32_t operand2) {
	return *rn & operand2;
}

// Bitwise XOR, result not written
uint32_t teq(Register *rn, uint32_t operand2) {
	return (*rn & ~operand2) | (~*rn & operand2);
}

// sub, result not written
uint32_t cmp(Register *rn, uint32_t operand2) {
	return *rn - operand2;
}

// Bitwise OR
uint32_t orr(Register *rd, Register *rn, uint32_t operand2) {
	*rd = *rn | operand2;
	return *rd;
}

// Move operand2 to dest
uint32_t mov(Register *rd, uint32_t operand2) {
	*rd = operand2;
	return *rd;
}

int main(void) {
	Register dest = 0;
	Register source = 0x3;

	eor(&dest, &source, 0x2);
	printf("%d\n", dest);

	return 0;
}

/*
void process(Instruction i, struct Registers *regs) {
	int cond = get_cond(i);
	uint32_t is_imm = is_immediate(i);
	int op = opcode(i);
	uint32_t set_conds = is_set(i);
	int rn_num = rn(i);
	int rd_num = rd(i);
	int op2 = operand2(i);
	int operand2;

	Register rn = regs -> general_regs[rn_num];
	Register rd = regs -> general_regs[rd_num];

	if (is_imm) {
		// operand2 is an immediate value

		int rotation = ((op2 & 0xf00) >> 8) * 2;
		uint32_t imm = op2 & 0xff;
		operand2 = rotate_right(imm, rotation);
	} else {
		// operand2 is a register
		
		int shift = (op2 & 0xff0) >> 4;
		int rm_num = op2 & 0xf;

		Register rm = regs -> general_regs[rm_num];
	}

	
	switch(op) {
		case 0:	
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
	
	}
}
*/



