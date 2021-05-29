#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "datatypes.h"
#include "data_processor.h"

void perform(Instruction* fetched, Instruction instruction, struct Registers* reg, Byte* memory, int *stop) {

  if (!instruction) {
    *stop = 1;	
    return;
  }

  if (!instruction_is_valid(instruction, reg)) {
    return;
  }

  enum InstructionType type;

  type = get_instr_type(instruction);

  switch(type) {
  case DP:
    process(instruction, reg);
    return;
  case MUL:
    multiply(instruction, reg);
    return;
  case SDT:
    single_data_transfer(instruction, reg, memory);
    return;
  case BRANCH:
    branch(instruction, reg);
    *fetched = NOOP;
    return;
  case NOOP:
    return;
  default:
    printf("Invalid instruction");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char **argv) {
  FILE *code;
  
  if ((code = fopen(argv[argc - 1], "r")) == NULL) {
    perror("Error opening file\n");
    exit(EXIT_FAILURE);
  }
 
  struct Registers* registers = calloc(1, sizeof(struct Registers));

  Byte *memory = calloc(1<<15, 1);
  readBinary(code, memory);

  fclose(code);

  Instruction fetched;
  Instruction decoded;
  int stop = 0;

  do {
    fetched = *((Instruction*) (memory+registers->pc));
    registers->pc+=4;
    perform(&fetched, decoded, registers, memory, &stop);
    decoded = fetched;
  } while (!stop);

  print_registers(registers);
  print_memory(memory, 1<<15);
  free(registers);
  exit(EXIT_SUCCESS);
}
 
