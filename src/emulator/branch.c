#include <stdio.h>
#include "datatypes.h"
#include "functions.h"

void branch(Instruction instr, struct Registers* reg){
  int32_t signBit = 0;
  int32_t offset = instr & 0xffffff; 
  if ((offset << 8) < 0) {
	signBit = ((-1) << 26);
  }	
  offset *= 4;
  offset += signBit;
  reg->pc += offset - 4;
}	
