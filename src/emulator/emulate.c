#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "datatypes.h"
#include "data_processor.h"

void perform(enum InstructionType type, Instruction instruction, struct Registers* reg, Byte* memory) {

  if (!instruction_is_valid(instruction, reg) && !(type == ALLZERO)) {
    return;
  }

  switch(type) {
  case ALLZERO:
    print_registers(reg);
    print_memory(memory, 1<<15);
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

void pipeline(struct Registers* registers, Byte* memory) {
  Instruction fetched;
  enum InstructionType type;
  Instruction toDecode;
  Instruction toExecute;
  int state = 0;

  while (1) {
	  printf("type: %d\n", type);
	  if (type == NOOP) {
	    continue;
	  }
	  if (state > 1) {
	    toExecute = toDecode;
	    toDecode = fetched;
	    fetched = *((Instruction*) (memory + registers->pc));
	    printf("Instruction: %8x\n", fetched);
	    perform(type, toExecute, registers, memory);
	    printf("Performed\n");
	    if ((type == BRANCH) && instruction_is_valid(toExecute, registers)) {
		    state = 0;
	    }
	    type = get_instr_type(toDecode);
	  } else if (state > 0) {
	    toDecode = fetched;
	    fetched = *((Instruction*) (memory + registers->pc));
	    printf("Instruction: %8x", fetched);
	    type = get_instr_type(toDecode);
	    state++;
	  } else {
	    fetched = *((Instruction*) (memory + registers->pc));
	    printf("Instruction: %8x", fetched);
	    state++;
	  }
	  registers->pc+=4;
	  printf("PC: %3d\n", registers->pc);
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

  pipeline(registers, memory);
}
 
