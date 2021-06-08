#ifndef ASSEMBLER_DATATYPES_H
#define ASSEMBLER_DATATYPES_H
#include <stdint.h>

typedef uint32_t Instruction;


/* Enum for representing the Mnemonics of different operations */
typedef enum {
    ADD,
    SUB,
    RSB,
    AND,
    EOR,
    ORR,
    MOV,
    TST,
    TEQ,
    CMP,
    MUL,
    MLA,
    LDR,
    STR,
    BEQ,
    BNE,
    BGE,
    BLT,
    BGT,
    BLE,
    B,
    LSL,
    ANDEQ
} Operation;


#endif
