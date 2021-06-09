#include <stdio.h>
#include "datatypes.h"

// Writes an array of instructions into a binary file
void write_to_file(Instruction *instructions, int num_instructions) {
  FILE *fp = fopen("../../out/program.bin", "wb");
  fwrite(instructions, sizeof(Instruction), num_instructions, fp);
  fclose(fp);
}

// Sets bit 25
void set_imm_field(Instruction* i) {
	*i |= 0x02000000;
}

// Sets bits 19-16 to rn number
void set_rn_field(int rn, Instruction* i) {
	*i |= (rn << 16); 
}

// Sets bits 15-12 to rd number
void set_rd_field(int rd, Instruction* i) {
	*i |= (rd << 12);
}

// Sets bits 11-7 to shift amount
void set_op2reg_shiftamt_field(int amt, Instruction* i) {
	*i |= (amt << 7);
}

// Sets bits 6-5 to shift type
void set_op2reg_shifttype_field(Shift type, Instruction* i) {
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


