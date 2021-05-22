#include <stdio.h>
#include "functions.h"
#include <stdint.h>

int rotate_right(uint32_t value, unsigned int rotation) {
	unsigned int shifted = value >> rotation;
	unsigned int rotated_bits = value << (32 - rotation);
	return (shifted | rotated_bits);
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

	}

	
	switch(opcode) {
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
int main(void) {
	uint32_t x = 8;
	unsigned int rotation = 5; 
	
	printf("%d\n", rotate_right(x, rotation));

	return 0;
}
