#include <stdio.h>
#include "datatypes.h"
#include "functions.h"

void branch(Instruction instr, struct Registers* reg){
  int offset;
  offset = (instr & 0xffffff)<<2;  
  uint32_t offset32 = offset;
  reg->pc += offset32;
}	
