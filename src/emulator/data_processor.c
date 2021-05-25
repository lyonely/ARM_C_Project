#include <stdio.h>
#include <stdint.h>
#include "data_processor.h"
#include "functions.h"

// TODO: check 'Set Condition Codes' bit and set CPSR flags accordingly

// Performs right rotation on a 32-bit binary number
uint32_t rotate_right(uint32_t value, uint32_t rotation) {
	uint32_t shifted = value >> rotation;
	uint32_t rotated_bits = value << (32 - rotation);
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

// Sets CPSR register flags based on the result
void set_flags(Register cpsr, uint32_t result) {
	// Carry flag

	// Zero flag
	if (result == 0) {
		set_z(&cpsr, 1);
	}

	// Negative flag
	set_n(&cpsr, result >> 31);
}

void execute(int opcode, Register rd, Register rn, uint32_t operand2, 
		uint32_t set_conds) {
	
	uint32_t result;

	switch(opcode) {
		case 0: result = and(&rd, &rn, operand2); break;
		case 1: result = eor(&rd, &rn, operand2); break;
		case 2: result = sub(&rd, &rn, operand2); break;
		case 3: result = rsb(&rd, &rn, operand2); break;
		case 4: result = add(&rd, &rn, operand2); break;
		case 5: result = tst(&rn, operand2); break;
		case 6: result = teq(&rn, operand2); break;
		case 7: result = cmp(&rn, operand2); break;
		case 8: result = orr(&rd, &rn, operand2); break;
		case 9:	result = mov(&rd, operand2); break;
	}

	// Set CPSR flags
	if (set_conds) {
		// TODO: check the value of flags to be set		
	}
}


void process(Instruction i, struct Registers *regs) {
	int cond = get_cond(i);
	uint32_t is_imm = is_immediate(i);
	int op_code = opcode(i);
	uint32_t set_conds = is_set(i);
	int rn_num = rn(i);
	int rd_num = rd(i);
	int op2 = operand2(i);
	uint32_t operand2;

	Register rn = regs -> general_regs[rn_num];
	Register rd = regs -> general_regs[rd_num];

	if (is_imm) {
		// operand2 is an immediate value

		uint32_t rotation = ((op2 & 0xf00) >> 8) * 2;
		uint32_t imm = op2 & 0xff;
		operand2 = rotate_right(imm, rotation);
	} else {
		// operand2 is a register
		
		int shift = (op2 & 0xff0) >> 4;
		int rm_num = op2 & 0xf;

		Register rm = regs -> general_regs[rm_num];
	
		// TODO: apply shift to register
	}

	execute(op_code, rd, rn, operand2, set_conds);
}

