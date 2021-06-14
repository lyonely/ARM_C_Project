#include "datatypes.h"
#include "functions.h"

// TODO put bit setting functions in build_sdt_instr
void set_fixed_bits(Instruction *i) {
  *i &= 0xf79fffff;
  *i |= 0x04000000;
}

void set_cond_field(Condition cond, Instruction *i) {
  *i &= 0x0fffffff;
  *i |= (cond << 28);
}

void set_preindex_field(Instruction *i) {
  *i |= 0x01000000;
}

void set_upbit_field(Instruction *i) {
  *i |= 0x00800000;
}

void set_loadstore_field(Instruction *i) {
  *i |= 0x00100000;
}

void set_immoffset_field(int offset, Instruction *i) {
  *i &= 0xfffff000;
  *i |= offset;
}

void build_sdt_instr(Token *token, Instruction *i) {
  set_fixed_bits(i);
  set_cond_field(token->cond, i);
  set_rn_field(token->SDT.rn, i);
  set_rd_field(token->SDT.rd, i);

  if (token->SDT.offset.ShiftedReg.preindex) {
    set_preindex_field(i);
  }

  if (token->SDT.offset.ShiftedReg.up_bit) {
    set_upbit_field(i);
  }

  if (token->opcode == LDR) {
    set_loadstore_field(i);
  }

  if (token->SDT.offset.is_imm) {
    set_imm_field(i);
    set_op2reg_shifttype_field(token->SDT.offset.ShiftedReg.shift_type, i);
    set_op2reg_rm_field(token->SDT.offset.ShiftedReg.rm, i);

    if (token->SDT.offset.ShiftedReg.shift.is_imm) {
      set_op2reg_shiftamt_field(token->SDT.offset.ShiftedReg.shift.immediate_shift, i);
    } else {
      set_op2reg_shiftreg_field(token->SDT.offset.ShiftedReg.shift.rs, i);
    }
  } else {
    set_immoffset_field(token->SDT.offset.expression, i);
  }
}

