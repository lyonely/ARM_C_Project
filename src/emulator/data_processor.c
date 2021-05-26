#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "data_processor.h"
#include "functions.h"
#include "shifter.h"

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

// Sets CPSR register flags based on the result, V flag unchanged
void set_flags(int opcode, Register cpsr, uint32_t result, uint32_t carry) {
	// Carry flag
	set_c(&cpsr, carry);

	// Zero flag
	if (result == 0) {
		set_z(&cpsr, 1);
	}

	// Negative flag
	set_n(&cpsr, result >> 31);
}

// Gets carry value based on the operation and operands
uint32_t get_carry(int opcode, Register rn, uint32_t operand2, int shifter_carryout) {
	uint32_t carry = 0;
	
	switch(opcode) {	
		// add
		case 4: if (rn > INT_MAX - operand2) { carry = 1; } break;
		// sub, cmp
		case 2:
		case 10: if (rn > operand2) { carry = 1; } break;
		// rsb
		case 3: if (operand2 > rn) { carry = 1; } break;
		default: carry = shifter_carryout; break;
	}

	return carry;
}

void execute(int opcode, Register rd, Register rn, uint32_t operand2, 
		uint32_t set_conds, Register cpsr, int shifter_carryout) {
	
	uint32_t carry = get_carry(opcode, rn, operand2, shifter_carryout);
	uint32_t result;

	switch(opcode) {
		case 0: result = and(&rd, &rn, operand2); break;
		case 1: result = eor(&rd, &rn, operand2); break;
		case 2: result = sub(&rd, &rn, operand2); break;
		case 3: result = rsb(&rd, &rn, operand2); break;
		case 4: result = add(&rd, &rn, operand2); break;
		case 8: result = tst(&rn, operand2); break;
		case 9: result = teq(&rn, operand2); break;
		case 10: result = cmp(&rn, operand2); break;
		case 12: result = orr(&rd, &rn, operand2); break;
		case 13: result = mov(&rd, operand2); break;
	}

	// Set CPSR flags
	if (set_conds) {
		set_flags(opcode, cpsr, result, carry);
	}
}


void process(Instruction i, struct Registers *regs) {
	if (instruction_is_valid(i, regs)) {
		uint32_t is_imm = is_immediate(i);
		int op_code = opcode(i);
		uint32_t set_conds = is_set(i);
		int rn_num = rn(i);
		int rd_num = rd(i);
		int op2 = operand2(i);
		uint32_t operand2;
		int shifter_carryout;

		Register rn = regs -> general_regs[rn_num];
		Register rd = regs -> general_regs[rd_num];
		Register cpsr = regs -> cpsr;

		if (is_imm) {
			// operand2 is an immediate value

			uint32_t rotation = ((op2 & 0xf00) >> 8) * 2;
			uint32_t imm = op2 & 0xff;
			operand2 = rotate_right(imm, rotation);
		} else {
			// operand2 is a register
			int rm_num = op2 & 0xf;
			Register rm = regs -> general_regs[rm_num];

			int shift = op2 >> 4;
			int shift_type = (shift >> 1) & 0x2;
			int register_shift = shift & 0x1;
			
			int shift_value;
			if (register_shift) {
				// shift specified by register
			} else {
				// shift by constant
				shift_value = shift >> 3;
			}
			operand2 = shift_result(rm, shift_type, shift_value); 
			shifter_carryout = carry_output(rm, shift_type, shift_value);
		}

		execute(op_code, rd, rn, operand2, set_conds, cpsr, shifter_carryout);
	}
}

