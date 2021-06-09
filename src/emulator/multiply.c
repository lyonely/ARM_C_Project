#include <stdio.h>
#include "datatypes.h"
#include "functions.h"

/* Executes the multiply instruction.
 */
void multiply(Instruction instruction, struct Registers* registers) {
	//store values in 32 bit variables	
	uint32_t op1 = registers->general_regs[rm_multiply(instruction)];
	uint32_t op2 = registers->general_regs[rs_multiply(instruction)];
	uint32_t op3 = registers->general_regs[rn_multiply(instruction)];
  
	//multiply and accumulate
	uint64_t res = op1 * op2;
	
	if(accumulate(instruction)) {
		res += op3;
	}
	
	//truncate to 32 bits
	uint32_t res32 = res;
	
	//store result in rd
	registers->general_regs[rd_multiply(instruction)] = res32;
  
	//set cpsr flags
	if (is_set(instruction)) {
		set_n_z(&registers->cpsr, res32);
	}  
}

