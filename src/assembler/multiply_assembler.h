#ifndef MULTIPLY_ASSEMBLER_H
#define MULTIPLY_ASSEMBLER_H

#include <stdint.h>
#include "datatypes.h"

//Converts multiply struct into Instruction
void build_multiply_instruction(MultiplyInstruction* instr, Instruction* i);

#endif
