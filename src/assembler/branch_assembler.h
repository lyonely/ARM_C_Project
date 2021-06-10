#ifndef BRANCH_ASSEMBLER_H
#define BRANCH_ASSEMBLER_H

#include <stdint.h>
#include "datatypes.h"

void build_branch_instr(BranchInstruction* instr, Instruction* i, \
		Address curr, SymbolTable* symboltable);
	
#endif
