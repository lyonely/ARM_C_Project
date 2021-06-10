#ifndef DATAP_ASSEMBLER_H
#define DATAP_ASSEMBLER_H

#include "datatypes.h"

// Converts data processing struct into Instruction
void build_datap_instr(DataProcessingInstruction* instr, Instruction* i);

#endif

