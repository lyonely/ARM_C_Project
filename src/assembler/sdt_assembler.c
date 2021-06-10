#include "datatypes.h"
#include "functions.h"

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

void build_sdt_instr(DataTransferInstruction* sdt, Instruction *i) {
  set_fixed_bits(i);
  set_cond_field(sdt->cond, i);
  set_rn_field(sdt->rn, i);
  set_rd_field(sdt->rd, i);

  if (sdt->preindex) {
    set_preindex_field(i);
  }

  if (sdt->up_bit) {
    set_upbit_field(i);
  }

  if (sdt->load_store) {
    set_loadstore_field(i);
  }

  if (sdt->is_imm) {
    set_imm_field(i);
    set_op2reg_shifttype_field(sdt->shift_type, i);
    set_op2reg_rm_field(sdt->rm, i);

    if (sdt->shift_by_reg) {
      set_op2reg_shiftreg_field(sdt->rs, i);
    } else {
      set_op2reg_shiftamt_field(sdt->shift_amount, i);
    }
  } else {
    set_immoffset_field(sdt->imm_offset, i);
  }
}

