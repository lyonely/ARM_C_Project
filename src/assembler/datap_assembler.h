#include <stdint.h>
#include "datatypes.h"

typedef struct DataProcessingInstruction {
  int opcode;
  int rd;
  int rn;
  int is_imm;
  int rotation; // set when is_imm = 1
  int imm; // set when is_imm = 1
  int shift_type; // set when is_imm = 0
  int shift_by_reg; // is_imm = 0
  int shift_amount; // is_imm = 0, shift_by_reg = 0
  int rs; // is_imm = 0, shift_by_reg = 1
  int rm; // is_imm = 0 
} datap_instr_t;

// Sets bits 31-28 to 1110 (always cond)
void set_cond_field(Instruction* i);

// Sets bit 25
void set_imm_field(Instruction* i);

// Sets bits 24-21 to specified opcode
void set_opcode_field(int opcode, Instruction* i);

// Sets bit 20 if opcode is tst, teq, cmp
void set_flag_field(int opcode, Instruction* i);

// Sets bits 19-16 to rn number
void set_rn_field(int rn, Instruction* i);

// Sets bits 15-12 to rd number
void set_rd_field(int rd, Instruction* i);

// Sets bits 11-8 to number of rotations
void set_op2imm_rotate_field(int rotation, Instruction* i);

// Sets bits 7-0 to immediate value
void set_op2imm_imm_field(int imm, Instruction* i);

// Sets bits 11-7 to shift amount
void set_op2reg_shiftamt_field(int amt, Instruction* i); 

// Sets bits 6-5 to shift type
void set_op2reg_shifttype_field(int type, Instruction* i);

// Sets bits 11-8 to rs number, bit 7 = 0 and sets bit 4 (register specified shift)
void set_op2reg_shiftreg_field(int rs, Instruction* i); 

// Sets bits 3-0 to rm number
void set_op2reg_rm_field(int rm, Instruction* i);

void build_datap_instr(datap_instr_t* instr, Instruction* i);

