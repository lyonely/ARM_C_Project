#include <stdio.h>
#include "datatypes.h"
#include "functions.h"
#include <stdint.h>

void process(Instruction i) {
	int cond = get_cond(i);
	uint32_t is_imm = is_immediate(i);
	int opcode = opcode(i);
	uint32_t set_conds = is_set(i);
	int rn = rn(i);
	int rd = rd(i);
	int op2 = operand2(i);
}
