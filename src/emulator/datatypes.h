#include <stdio.h>
#include <stdbool.h>

typedef unsigned char Byte;
typedef uint32_t Instruction;
typedef uint32_t Register;

enum Cond {
	EQ, NE, GE, LT, GT, LE, AL
};

enum InstructionType {
	DP, MUL, SDT, BRANCH
}

typedef struct {
	Register general_regs[13];
	Register pc;
	Register cpsr;
} Registers;
