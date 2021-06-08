#include <stdlib.h>
#include <stdio.h>
#include "functions.h"
#include "datatypes.h"
#include "data_processor.h"
#include "multiply.h"
#include "branch.h"
#include "data_transfer.h"

/* Executes the instruction based on its type.
 *
 * The instruction is only executed if its condition holds.
 *
 * In the case that the instruction type is ALLZERO, we print the 
 * state of the registers and non-zero memory and exit the program.
 */
void perform(enum InstructionType type, Instruction instruction, struct Registers* registers, Byte* memory) {
	if (!instruction_is_valid(instruction, registers) && !(type == ALLZERO)) {
		return;
	}
	switch(type) {
		case ALLZERO:
			print_registers(registers);
			print_memory(memory);
			exit(EXIT_SUCCESS);
		case DP:
			process(instruction, registers);
			return;
		case MUL:
			multiply(instruction, registers);
			return;
		case SDT:
			single_data_transfer(instruction, registers, memory);
			return;
		case BRANCH:
			branch(instruction, registers);
			return;
	}
}

/* Implements the three stage pipeline.
 *
 * State 0 - Empty pipeline, 
 *           new instruction is being fetched
 *
 * State 1 - Fetched instruction is being decoded, 
 *           new instruction is being fetched
 *
 * State 2 - Decoded instruction is to be exectuted, 
 *           fetched instruction is being decoded,
 *           new instruction is being fetched
 *
 * In the case of a branch instruction, the pipeline resets and starts at state 0 again.
 * The instruction being fetched will depend on the result of the branch instruction.
 */
void pipeline(struct Registers* registers, Byte* memory) {
	Instruction fetched;
	enum InstructionType type = ALLZERO;
	Instruction toDecode;
	Instruction toExecute;
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
	}
}

/* Reads the binary instructions from the input file, allocates the 
 * required memory for the registers and memory, and starts the 
 * three stage pipeline.
 */
int main(int argc, char *argv[]) {
	FILE *code;
	
	code = fopen(argv[1], "rb");
	
	if (code == NULL) {
		perror("Error opening file\n");
		exit(EXIT_FAILURE);
	}
	
	Byte *memory = calloc(MEMORY_CAPACITY, 1);
	readBinary(code, memory);
	
	struct Registers* registers = calloc(1, sizeof(struct Registers));
	
	// sets the pc and cpsr registers to 0
	registers->pc = 0;
	registers->cpsr = 0;
	
	fclose(code);
	pipeline(registers, memory);
	return 0;
}
 
