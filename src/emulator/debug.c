#include <stdio.h>
#include <stdint.h>
#include "data_processor.h"

// Debugging file for data_processor
int main(void) {

	int and_op = 0;
	int eor_op = 1;
	int sub_op = 2;
	int rsb_op = 3;
	int add_op = 4;
	int tst_op = 8;
	int teq_op = 9;
	int cmp_op = 10;
	int orr_op = 12;
	int mov_op = 13;
	struct Registers regs;
	
	regs.pc = 0;
	regs.cpsr = 0;
	regs.general_regs[0] = 0x4;

	printf("CPSR reg: %d\n", regs.cpsr);

	// No cond, immediate, add, set flags, rn = 0, rd = 1
	// rotate = 2, imm = 1000
	Instruction i1 = 0b11100010100100000001001000001000;
	
	/*
	process(i1, &regs);
	printf("rn: %d\n", regs.general_regs[0]);
	printf("rd: %d\n", regs.general_regs[1]);
	printf("cpsr flags: %d\n", regs.cpsr >> 28);	
	*/
	return 0;
}
