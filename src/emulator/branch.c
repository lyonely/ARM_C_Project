#include <stdio.h>
#include "datatypes.h"
#include "functions.h"

#define BRANCH_OFFSET_MASK 0xffffff
#define NEGATIVE_SIGN_BIT ((-1)<<26)

/* Executes the branch instruction.
 */
void branch(Instruction instruction, struct Registers* registers){
	int32_t sign_bit = 0;
	int32_t offset = instruction & BRANCH_OFFSET_MASK; 
	if ((offset << 8) < 0) {
		sign_bit = NEGATIVE_SIGN_BIT;
	}	
	offset <<= 2;
	offset += sign_bit;
	registers->pc += offset - 4;
}	
