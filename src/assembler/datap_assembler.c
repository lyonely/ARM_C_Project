#include <stdint.h>
#include "datatypes.h"
#include "functions.h"

// Sets bits 31-28 to 1110 (always cond)
void set_cond_field(Instruction* i) {
	*i &= 0x0fffffff;
	*i |= 0xe0000000;
}

// Sets bits 24-21 to specified opcode
void set_opcode_field(Opcode opcode, Instruction* i) {
	*i |= (opcode << 21);
}

// Sets bit 20 if opcode is tst, teq, cmp
void set_flag_field(Opcode opcode, Instruction* i) {
	switch(opcode) {
		case 8:
		case 9:
		case 10: *i |= 0x00100000; break;
		default: *i &= 0xffefffff;
	}
}

// Sets bits 11-8 to number of rotations
void set_op2imm_rotate_field(int rotation, Instruction* i) {
	*i |= (rotation << 8);
}

// Sets bits 7-0 to immediate value
void set_op2imm_imm_field(int imm, Instruction* i) {
	*i |= imm;
}

void build_datap_instr(Token* token, Instruction* i) {
  // Sets instruction bits if opcode is not ANDEQ (all-zero)
  if (!(token->opcode == ANDEQ && token->DataP.rd == 0 
        && token->DataP.rn == 0 && token->DataP.operand2.reg_operand.rm == 0)) {
    set_cond_field(i);
    set_opcode_field(token->opcode, i);
    set_flag_field(token->opcode, i);
    set_rn_field(token->DataP.rn, i);
    set_rd_field(token->DataP.rd, i);

    if (token->DataP.operand2.is_imm) {
      set_imm_field(i);
      set_op2imm_rotate_field(token->DataP.operand2.imm_operand.rotation, i);
      set_op2imm_imm_field(token->DataP.operand2.imm_operand.immediate, i);
    } else {
      set_op2reg_rm_field(token->DataP.operand2.reg_operand.rm, i);
      set_op2reg_shifttype_field(token->DataP.operand2.reg_operand.shift_type, i);
      if (token->DataP.operand2.reg_operand.shift_by_reg) { 
        set_op2reg_shiftreg_field(token->DataP.operand2.reg_operand.shift.rs, i);
      } else {
        set_op2reg_shiftamt_field(token->DataP.operand2.reg_operand.shift.immediate_shift, i);
      }
    }
  } else {
    *i &= 0x0;
  } 
}

