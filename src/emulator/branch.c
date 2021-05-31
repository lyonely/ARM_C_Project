#include <stdio.h>
#include "datatypes.h"
#include "functions.h"

#define BRANCH_OFFSET_MASK 0xffffff
#define NEGATIVE_SIGN_BIT ((-1)<<26)
void branch(Instruction instr, struct Registers* reg){
  int32_t signBit = 0;
  int32_t offset = instr & BRANCH_OFFSET_MASK; 
  if ((offset << 8) < 0) {
	signBit = NEGATIVE_SIGN_BIT;
  }	
  offset <<= 2;
  offset += signBit;
  reg->pc += offset - 4;
}	
