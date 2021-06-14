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

void build_branch_instr(Token *token, Instruction* i) {
	set_cond_field(token->cond, i);
	set_bits_27_to_24(i);

  Address curr = token->address;
	curr += 8;

	int32_t offset = BRANCH_OFFSET_MASK & (token->Branch.target_address - curr);
	if (token->Branch.target_address < curr) {
		offset |= 0x2000000;
	} else {
		offset &= 0x0ffffff;
	}
	*i &= 0xff000000;
	*i |= (offset >> 2);

}

