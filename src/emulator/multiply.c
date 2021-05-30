#include <stdio.h>
#include "datatypes.h"
#include "functions.h"

void multiply(Instruction instr, struct Registers* rs) {
  //store values in 32 bit variables	
  uint32_t op1 = rs->general_regs[rmMultiply(instr)];
  uint32_t op2 = rs->general_regs[rsMultiply(instr)];
  uint32_t op3 = rs->general_regs[rnMultiply(instr)];
  //multiply and accumulate
  uint64_t res = op1 * op2;

  if(accumulate(instr)) {
    res += op3;
  }
  //truncate to 32 bits
  uint32_t res32 = res;
  //store result in rd
  rs->general_regs[rdMultiply(instr)] = res32;
  //set cpsr flags
  if (is_set(instr)) {
  	set_n_z(&rs->cpsr, res32);
  }  
}
