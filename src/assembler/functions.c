#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"

// Writes an array of instructions into a binary file
void write_to_file(Instruction *instructions, int size) {
  FILE *fp = fopen("../../out/program.bin", "wb");
  fwrite(instructions, sizeof(Instruction), size, fp);
  fclose(fp);
}

// Converts Instruction from big-endian to little-endian
void flip_endian(Instruction *i) {
  Instruction byte1 = *i & 0xFF000000;
  Instruction byte2 = *i & 0x00FF0000;
  Instruction byte3 = *i & 0x0000FF00;
  Instruction byte4 = *i & 0x000000FF;

  byte1 = byte1 >> 24;
  byte2 = byte2 >> 8;
  byte3 = byte3 << 8;
  byte4 = byte4 << 24;

  *i = byte1 | byte2 | byte3 | byte4;
}

Type get_type(Operation opcode) {
  switch(opcode) {
    case MUL:
    case MLA:
      return MULTIPLY; break;
    case LDR:
    case STR:
      return SDT; break;
    case BEQ:
    case BNE:
    case BGE:
    case BLT:
    case BGT:
    case BLE:
    case B:
      return BRANCH; break;
    default:
      return DATA_P; break;
  }
}

unsigned int get_num_args(Operation opcode) {
  switch(opcode) {
    case MLA:
      return 4;
    case BEQ:
    case BNE:
    case BGE:
    case BLT:
    case BGT:
    case BLE:
    case B:
      return 1;
    case AND:
    case ANDEQ:
    case EOR:
    case SUB:
    case RSB:
    case ADD:
    case ORR:
    case MUL:
      return 3;
    default:
      return 2;
  }
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
void set_op2reg_shifttype_field(ShiftType type, Instruction* i) {
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

