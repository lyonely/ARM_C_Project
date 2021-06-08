#include <stdint.h>
#include "datatypes.h"
#include "datap_assembler.h"

int get_opcode(Operation op) {
  switch(op) {
    case AND: return 0b0000;
    case EOR: return 0b0001;
    case SUB: return 0b0010;
    case RSB: return 0b0011;
    case ADD: return 0b0100;
    case TST: return 0b1000;
    case TEQ: return 0b1001;
    case CMP: return 0b1010;
    case ORR: return 0b1100;
    case MOV: return 0b1101;
    default: return 0b1111; // unused opcode value
  }
}

// Sets bits 31-28 to 1110 (always cond)
void set_cond_field(Instruction* i) {
	*i &= 0x0fffffff;
	*i |= 0xe0000000;
}

// Sets bit 25
void set_imm_field(Instruction* i) {
	*i |= 0x02000000;
}

// Sets bits 24-21 to specified opcode
void set_opcode_field(int opcode, Instruction* i) {
	*i |= (opcode << 21);
}

// Sets bit 20 if opcode is tst, teq, cmp
void set_flag_field(int opcode, Instruction* i) {
	switch(opcode) {
		case 8:
		case 9:
		case 10: *i |= 0x00100000; break;
		default: *i &= 0xffefffff;
	}
}

// Sets bits 19-16 to rn number
void set_rn_field(int rn, Instruction* i) {
	*i |= (rn << 16); 
}

// Sets bits 15-12 to rd number
void set_rd_field(int rd, Instruction* i) {
	*i |= (rd << 12);
}

// Sets bits 11-8 to number of rotations
void set_op2imm_rotate_field(int rotation, Instruction* i) {
	*i |= (rotation << 8);
}

// Sets bits 7-0 to immediate value
void set_op2imm_imm_field(int imm, Instruction* i) {
	*i |= imm;
}

// Sets bits 11-7 to shift amount
void set_op2reg_shiftamt_field(int amt, Instruction* i) {
	*i |= (amt << 7);
}

// Sets bits 6-5 to shift type
void set_op2reg_shifttype_field(int type, Instruction* i) {
	*i |= (type << 5);
}

// Sets bits 11-8 to rs number, sets bit 7 = 0 and sets bit 4
// (register specified shift)
void set_op2reg_shiftreg_field(int rs, Instruction* i) {
	*i |= 0x10;
	*i &= 0xFFFFFF7F;
	*i |= (rs << 8);
}

// Sets bits 3-0 to rm number
void set_op2reg_rm_field(int rm, Instruction* i) {
	*i |= rm;
}

void build_datap_instr(datap_instr_t* instr, Instruction* i) {
  set_cond_field(i);
  set_opcode_field(instr->opcode, i);
  set_flag_field(instr->opcode, i);
  set_rn_field(instr->rn, i);
  set_rd_field(instr->rd, i);
  
  if (instr->is_imm) {
    set_imm_field(i);
    set_op2imm_rotate_field(instr->rotation, i);
    set_op2imm_imm_field(instr->imm, i);
  } else {
    set_op2reg_rm_field(instr->rm, i);
    set_op2reg_shifttype_field(instr->shift_type, i);
    if (instr->shift_by_reg) { 
      set_op2reg_shiftreg_field(instr->rs, i);
    } else {
      set_op2reg_shiftamt_field(instr->shift_amount, i);
    }
  }
}

