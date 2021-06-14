#include <stdint.h>
#include "datatypes.h"

// Sets bits 31-28 to 1110 (always cond)
void set_cond_field(Instruction* i) {
	*i &= 0x0fffffff;
	*i |= 0xe0000000;
}

//Sets bit 20
void set_flag_field(Instruction* i) {
	*i &= 0xffefffff;
}

//Sets bit 7-4 to 1001
void set_bits_seven_to_four(Instruction* i) {
	*i &= 0xffffff0f;
	*i |= 0x00000090;
}

void set_acc_field(Operation opcode, Instruction* i) {
	if (opcode == MLA) {
		*i |= 0x00200000;
	} else {
		*i &= 0xffdfffff;
	}
}
void set_rd_field(int rd, Instruction* i) {
	*i |= (rd << 16);
}

void set_rm_field(int rm, Instruction* i) {
	*i |= rm;
}

void set_rs_field(int rs, Instruction* i) {
	*i |= (rs << 8);
}

void set_rn_field(Operation opcode, int rn, Instruction* i) {
	if (opcode == MLA) {
		*i |= (rn << 12);
	}
}

void build_multiply_instr(Token *token, Instruction *i) {
	set_cond_field(i);
	set_flag_field(i);
	set_bits_seven_to_four(i);
	set_rd_field(token->Multiply.rd, i);
	set_rm_field(token->Multiply.rm, i);
	set_rs_field(token->Multiply.rs, i);
	set_acc_field(token->opcode, i);
	set_rn_field(token->opcode, token->Multiply.rn, i);
}
