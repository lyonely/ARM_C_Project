#include <stdint.h>
#include <string.h>
#include "datatypes.h"

#define NEGATIVE_SIGN_BIT ((-1)<<26)
#define BRANCH_OFFSET_MASK 0x3ffffff
// Set bits 31-28 to condition
void set_cond_field(Condition cond, Instruction* i) {
	*i &= 0x0fffffff;
	*i |= (cond << 27);
}

// Set bits 27-24 
void set_bits_27_to_24(Instruction* i) {
	*i &= 0xf0ffffff;
	*i |= 0x0a000000;
}

void build_branch_instr(BranchInstruction* instr, Instruction* i, \
		Address curr, SymbolTable* symboltable) {
	set_cond_field(instr->cond, i);
	set_bits_27_to_24(i);
	Address label_address;
	for (int i = 0; i < symboltable->size; i++) {
		Symbol* label = &symboltable->table[i];
		if (strcmp(label->symbol, instr->symbol)) {
			label_address = label->address;
			break;
		}
	}
	curr += 8;
	int32_t offset = BRANCH_OFFSET_MASK & (label_address - curr);
	if (label_address < curr) {
		offset |= 0x2000000;
	} else {
		offset &= 0x0ffffff;
	}
	*i &= 0xff000000;
	*i |= (offset >> 2);

}

