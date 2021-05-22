#include <stdio.h>
#include "functions.h"

long readBinary(FILE* file, void *destination) {
  fseek(file, 0, SEEK_END); // seek to end of file
  long size = ftell(file) + 1; // get current file pointer + 1 for zero instruction at end of file
  fseek(file, 0, SEEK_SET); // seek back to beginning of file for reading
  fread(destination, 1, sizeof(destination), file);
  return size;
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

uint32_t setConditionCodes(Instruction instruction) {
  return (instruction & 1 << 20);
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
  return (instruction >> 28l);
}

