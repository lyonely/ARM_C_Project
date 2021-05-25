#include <stdio.h>
#include <stdint.h>
#include "data_processor.h"

// Debugging file for data_processor

int main(void) {
	Register source = 0xfff;
	Register dest = 5;
	uint32_t op2 = 0xff;
	
	int result;

	result = and(&dest, &source, op2);
	printf("and -> rn AND op2: %d, rd: %d\n", result, dest);

	result = eor(&dest, &source, op2);
	printf("eor -> rn EOR op2: %d, rd: %d\n", result, dest);

	result = sub(&dest, &source, op2);
	printf("sub -> rn - op2: %d, rd: %d\n", result, dest);
	
	result = rsb(&dest, &source, op2);
	printf("rsb -> op2 - rn: %d, rd: %d\n", result, dest);
	
	result = add(&dest, &source, op2);
	printf("add -> rn + op2: %d, rd: %d\n", result, dest);
	
	result = tst(&source, op2);
	printf("tst -> rn AND op2: %d, rd: %d\n", result, dest);

	result = teq(&source, op2);
	printf("teq -> rn EOR op2: %d, rd: %d\n", result, dest);

	result = cmp(&source, op2);
	printf("cmp -> rn - op2: %d, rd: %d\n", result, dest);

	result = orr(&dest, &source, op2);
	printf("orr -> rn OR op2: %d, rd: %d\n", result, dest);

	result = mov(&dest, op2);
	printf("mov -> op2: %d, rd: %d\n", result, dest);

	return 0;
}
