#include <stdio.h>
#include "datatypes.h"
#include "functions.h"

void branch(int offset, Register* pc, Instruction* f){
  offset<<=2;
  uint32_t offset32 = offset;
  *pc += offset32;
}	
