#ifndef DATATYPES_H
#define DATATYPES_H
#include <stdio.h>
#include <stdint.h>

typedef unsigned char Byte;
typedef uint32_t Instruction;
typedef uint32_t Register;

enum Cond {
	EQ = 0b0000,
       	NE = 0b0001, 
	GE = 0b1010, 
	LT = 0b1011, 
	GT = 0b1100, 
	LE = 0b1101, 
	AL = 0b1110,

};

enum InstructionType {
	DP, MUL, SDT, BRANCH, ALLZERO
};

struct Registers {
	Register general_regs[13];
	Register pc;
	Register cpsr;
};


#endif

