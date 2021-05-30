#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "data_processor.h"
#include "functions.h"
#include "shifter.h"

// Bitwise AND
void and(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn & operand2;
	if (set_conds) {
		set_n_z(cpsr, res);
	}
	*rd = res;
}

// Bitwise exclusive OR
void eor(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn ^ operand2;
	if (set_conds) {
		set_n_z(cpsr, res);
	}
	*rd = res;
}

// Subtract operand2 from register
void sub(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn - operand2;
	if (set_conds) {
		set_c(cpsr, rn > operand2);
		set_n_z(cpsr, res);
	}
	*rd = res;
}

// Subtract register from operand2
void rsb(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = operand2 - rn;
	if (set_conds) {
		set_c(cpsr, operand2 > rn); 
		set_n_z(cpsr, res);
	}
	*rd = res;
}

// Addition
void add(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn + operand2;
	if (set_conds) {
		set_c(cpsr, rn > INT_MAX - operand2);
		set_n_z(cpsr, res);
	}
	*rd = res;
}

// Bitwise AND, result not written
void tst(Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn & operand2;
	if (set_conds) {
		set_n_z(cpsr, res);
	}
}

// Bitwise XOR, result not written
void teq(Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn ^ operand2;
	if (set_conds) {
		set_n_z(cpsr, res);
	}
}

// sub, result not written
void cmp(Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn - operand2;
	if (set_conds) {
		set_c(cpsr, rn >= operand2);
		set_n_z(cpsr, res);
	}
}

// Bitwise OR
void orr(Register *rd, Register rn, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	int res = rn | operand2;
	if (set_conds) {
		set_n_z(cpsr, res);
	}
	*rd = res;
}

// Move operand2 to dest
void mov(Register *rd, uint32_t operand2, Register *cpsr, uint32_t set_conds) {
	if (set_conds) {
		set_n_z(cpsr, operand2);
	}
	*rd = operand2;
}

void execute(int opcode, Register *rd, Register rn, uint32_t operand2, 
		uint32_t set_conds, Register *cpsr) {
	switch(opcode) {
		case 0: and(rd, rn, operand2, cpsr, set_conds); break;
		case 1: eor(rd, rn, operand2, cpsr, set_conds); break;
		case 2: sub(rd, rn, operand2, cpsr, set_conds); break;
		case 3: rsb(rd, rn, operand2, cpsr, set_conds); break;
		case 4: add(rd, rn, operand2, cpsr, set_conds); break;
		case 8: tst(rn, operand2, cpsr, set_conds); break;
		case 9: teq(rn, operand2, cpsr, set_conds); break;
		case 10: cmp(rn, operand2, cpsr, set_conds); break;
		case 12: orr(rd, rn, operand2, cpsr, set_conds); break;
		case 13: mov(rd, operand2, cpsr, set_conds); break;
	}
}

void process(Instruction i, struct Registers *regs) {
	int op_code, shifter_carryout;
	uint32_t is_imm, set_conds, op2_rotated;

	is_imm = is_immediate(i);
	op_code = opcode(i);
	set_conds = is_set(i);
	int op2 = operand2(i);

	int rn_num = rn(i);
	int rd_num = rd(i);
	
	Register *rn = &regs->general_regs[rn_num];
	Register *rd = &regs->general_regs[rd_num];
	Register *cpsr = &regs->cpsr;
	
	if (set_conds) {
		reset_flags(cpsr);
	}

	if (is_imm) {
		// operand2 is an immediate value

		uint32_t rotation = ((op2 & 0xf00) >> 8) * 2;
		uint32_t imm = op2 & 0xff;
		op2_rotated = rotate_right(imm, rotation);
	} else {
		// operand2 is a register
		int rm_num = op2 & 0xf;
		Register rm = regs->general_regs[rm_num];
		int shift = op2 >> 4;
		int shift_type = (shift >> 1) & 0x3;
		int register_shift = shift & 0x1;
		
		int shift_value;
		if (register_shift) {
			// shift specified by register
		} else {
			// shift by constant
			shift_value = shift >> 3;
		}
		op2_rotated = shift_result(rm, shift_type, shift_value); 
		shifter_carryout = carry_output(rm, shift_type, shift_value);
		if (set_conds) set_c(cpsr, shifter_carryout);
	}
	
	execute(op_code, rd, *rn, op2_rotated, set_conds, cpsr);
}

