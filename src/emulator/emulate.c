#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "datatypes.h"
#include "data_processor.h"

void perform(enum InstructionType type, Instruction instruction, struct Registers *reg, 
		Byte* memory, int memory_capacity) {

  if (!instruction_is_valid(instruction, reg)) {
	return;
  }

  switch(type) {
  case ALLZERO:
    print_registers(reg);
    print_memory(memory, memory_capacity);
    exit(EXIT_SUCCESS);
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
    return;
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
  Instruction toDecode;
  enum InstructionType type;
  Instruction toExecute;
  int state = 0;

  while(1) {
    if (state == 2) {
      toExecute = toDecode;	  
      toDecode = fetched;
      fetched = *((Instruction*) (memory+registers->pc));
      perform(type, toExecute, registers, memory, 1<<15);
      if (type == BRANCH && instruction_is_valid(toExecute, registers)) {
        state = 0;
      }
      type = get_instr_type(toDecode);
    } else if (state == 1) {
      toDecode = fetched;
      fetched = *((Instruction*) (memory+registers->pc));
      type = get_instr_type(toDecode);
      state = 2;
    } else {
      fetched = *((Instruction*) (memory+registers->pc));
      state = 1;
    }
    registers->pc+=4;
  }

}

