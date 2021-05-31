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
//    printf("ignored");
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
  //          printf("State: %1d\n", state);
	    toExecute = toDecode;
	    toDecode = fetched;
	    fetched = *((Instruction*) (memory + registers->pc));
//	    printf("ToDecode: %8x\n", toDecode);
//	    printf("Fetched Instruction: %8x\n", fetched);
//	    printf("To Execute: %8x\n", toExecute);
	    perform(type, toExecute, registers, memory);
//	    printf("Executed\n");
	    if ((type == BRANCH) && instruction_is_valid(toExecute, registers)) {
		    state = 0;
//		    printf("Branch Function\n");
	    }
	    type = get_instr_type(toDecode);
//	    printf("Type: %3d\n", type);

	  } else if (state > 0) {
  //          printf("State: %1d\n", state);
	    toDecode = fetched;
	    fetched = *((Instruction*) (memory + registers->pc));
//	    printf("ToDecode: %8x\n", toDecode);
//	    printf("Fetched Instruction: %8x\n", fetched);
	    type = get_instr_type(toDecode);
//	    printf("Type: %3d\n", type);
	    state++;
  
	  } else {
  //          printf("State: %1d\n", state);
	    fetched = *((Instruction*) (memory + registers->pc));
//	    printf("Fetched Instruction: %8x\n", fetched);
	    state++;
	  }
	  registers->pc+=4;
//	  printf("PC: %4d\n", registers->pc);
//	  printf("CPSR: 0x%8x\n", registers -> cpsr);
	}
}

int main(int argc, char *argv[]) {
//	printf("entered main\n");
  FILE *code;

  if (argc != 2) {
	perror("Wrong number of file arguments.\n");
	exit(EXIT_FAILURE);
  }
//  printf("opening file\n");
  code = fopen(argv[1], "rb");

  if (code == NULL) {
    perror("Error opening file\n");
    exit(EXIT_FAILURE);
  }
 

//  printf("Reading file\n");
  Byte *memory = calloc(1<<16, 1);
  readBinary(code, memory);

  struct Registers* registers = calloc(1, sizeof(struct Registers));
  registers->pc = 0;
  registers->cpsr = 0;

  fclose(code);
//  printf("Entering pipeline\n");
  pipeline(registers, memory);

  free(registers);
  free(memory);

  return 0;
}
 
