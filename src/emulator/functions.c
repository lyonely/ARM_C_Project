#include <stdio.h>
#include "functions.h"

long readBinary(FILE* file, void *destination) {
  fseek(file, 0, SEEK_END); // seek to end of file
  long size = ftell(file) + 1; // get current file pointer + 1 for zero instruction at end of file
  fseek(file, 0, SEEK_SET); // seek back to beginning of file for reading
  fread(destination, 1, sizeof(destination), file);
  return size;
}

int instruction_is_valid(Instruction instruction, Registers* regs) {
  
  uint32_t N_flag = 1 << 31;
  uint32_t Z_flag = 1 << 30;
  uint32_t V_flag = 1 << 28;
  uint32_t cpsr = regs->cpsr;

  uint32_t N_equals_V = (N_flag & cpsr) == ((V_flag & cpsr) << 3);
  uint32_t Z_set = Z_flag & cpsr;
  
  int isValid = 0;

  int cond = get_cond(Instruction instruction);

  switch(cond) {
    case EQ: isValid = Z_set; break;
    case NE: isValid = !Z_set; break;
    case GE: isValid = N_equals_V; break;
    case LT: isValid = !N_equals_V; break;
    case GT: isValid = (!Z_set) && N_equals_V; break;
    case LE: isValid = Z_set || (!N_equals_V); break;
    case AL: isValid = 1; break;
    default: isValid = 0;
  }
  return isValid;
}


/*data processing instructions*/

uint32_t is_set(Instruction instruction) {
  return (instruction & 1 << 20);
}

uint32_t is_immediate(Instruction instruction) {
  return (instruction & 1 << 25);
}

int opcode(Instruction instruction) {
  return (instruction & 0x1e00000) >> 21;
}

int rn(Instruction instruction) {
  return (instruction & 0xf0000) >> 16;
}

int rd(Instruction instruction) {
  return (instruction & 0xf000) >> 12;
}
				
int operand2(Instruction instruction) {
  return (instruction & 0xfff);
}

/*multiply instruction*/

uint32_t accumulate(Instruction instruction) {
  return (instruction & 1 << 21);
}

int rnMultiply(Instruction instruction) {
  return (instruction & 0xf000) << 12;
}

int rdMultiply(Instruction instruction){
  return (instruction & 0xf0000) << 16;
}

int rsMultiply(Instruction instruction) {
  return (instruction & 0xf00) << 8;
}

int rmMultiply(Instruction instruction) {
  return (instruction & 0xf);
}

/*single data transfer*/
uint32_t is_pre_indexing(Instruction instruction) {
  return (instruction & 1 << 24);
}

uint32_t is_up(Instruction instruction) {
  return (instruction & 1 << 23);
}

uint32_t is_load(Instruction instruction) {
  return (instruction & 1 << 20);
}

int sdt_offset(Instruction instruction) {
  return (instruction & 0xfff);
}

int get_cond(Instruction instruction) {
  return (instruction >> 28);
}

int instruction_is_valid(Instruction instruction, struct Registers* regs) {
  
  uint32_t N_flag = 1 << 31;
  uint32_t Z_flag = 1 << 30;
  uint32_t V_flag = 1 << 28;
  uint32_t cpsr = regs->cpsr;

  uint32_t N_equals_V = (N_flag & cpsr) == ((V_flag & cpsr) << 3);
  uint32_t Z_set = Z_flag & cpsr;
  
  int isValid = 0;

  int cond = get_cond(Instruction instruction);

  switch(cond) {
    case EQ: isValid = Z_set; break;
    case NE: isValid = !Z_set; break;
    case GE: isValid = N_equals_V; break;
    case LT: isValid = !N_equals_V; break;
    case GT: isValid = (!Z_set) && N_equals_V; break;
    case LE: isValid = Z_set || (!N_equals_V); break;
    case AL: isValid = 1; break;
    default: isValid = 0;
  }
  return isValid;
}

void set_v(Register *cpsr, int value) {
  (value) ? *cpsr = *cpsr | (1 << 28) : *cpsr & 0xefffffff;
}

void set_c(Register *cpsr, int value) {
  (value) ? *cpsr = *cpsr | (1 << 29) : *cpsr &  0xdfffffff;
}

void set_z(Register *cpsr, int value) {
  (value) ? *cpsr = *cpsr | (1 << 30) : *cpsr & 0xbfffffff;
}

void set_n(Register *cpsr, int value) {
  (value) ? *cpsr = *cpsr | (1 << 31) : *cpsr & 0x7fffffff;
}

InstructionType get_instr_type(Instruction instruction) {
  if (!instruction) {
    return ALLZERO;
  }

  if (instruction & (1<<27)) {
    return BRANCH; 
  }

  if (instruction & (1<<26)) {
    return SDT;
  }

  if (instruction & (1<<25) || !(instruction & (1<<7)) || !(instruction & (1<<4))) {
    return DP;
  }
  
  return MUL;
}
