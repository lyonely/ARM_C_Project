#include <stdio.h>
#include "datatypes.h"
#include "functions.h"
#define DEBUG 0

void branch(int offset, Register* pc, Instruction* f){
  offset<<=2;
  Register* pc_old = pc;
  *pc += offset;
  if (DEBUG) {
    printf("Exec branch, offset: %d, old PC: %d", offset, *pc_old);
    printf("new PC: %d", *pc); 
  }
}	
