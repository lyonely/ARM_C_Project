#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "datatypes.h"
#include "data_processor.h"
#include "multiply.h"
#include "branch.h"
#include "data_transfer.h"

void perform(enum InstructionType type, Instruction instruction, struct Registers* reg, Byte* memory) {

  if (!instruction_is_valid(instruction, reg) && !(type == ALLZERO)) {
    return;
  }

  switch(type) {
  case ALLZERO:
    print_registers(reg);
    print_memory(memory, 1<<16);
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
  enum InstructionType type = ALLZERO;
  Instruction toDecode;
  Instruction toExecute = -1;
  int state = 0;

  while (1) {




	  if (type == NOOP) {
	    continue;
	  }
	  if (state > 1) {
	    toExecute = toDecode;
	    toDecode = fetched;
	    fetched = *((Instruction*) (memory + registers->pc));


	    perform(type, toExecute, registers, memory);

	    if ((type == BRANCH) && instruction_is_valid(toExecute, registers)) {

		perform(type, toExecute, registers, memory);
		if ((type == BRANCH) && instruction_is_valid(toExecute, registers)) {

		    state = 0;

	    }
	    type = get_instr_type(toDecode);
	  
	  } else if (state > 0) {
	    toDecode = fetched;
	    fetched = *((Instruction*) (memory + registers->pc));




	    type = get_instr_type(toDecode);
	    state++;
	  
	  } else {

	    fetched = *((Instruction*) (memory + registers->pc));

	    state++;
	  }
	  registers->pc+=4;


		fetched = *((Instruction*) (memory + registers->pc));
	    state++;
	  }
	  registers->pc+=4;

	}
}

int main(int argc, char *argv[]) {
  FILE *code;

  if (argc != 2) {
	perror("Wrong number of file arguments.\n");
	exit(EXIT_FAILURE);
  }

  code = fopen(argv[1], "rb");

  if (code == NULL) {
    perror("Error opening file\n");
    exit(EXIT_FAILURE);
  }
 

  Byte *memory = calloc(1<<16, 1);
  readBinary(code, memory);

  struct Registers* registers = calloc(1, sizeof(struct Registers));
  registers->pc = 0;
  registers->cpsr = 0;

  fclose(code);

  pipeline(registers, memory);

  free(registers);
  free(memory);

  return 0;
}
 
