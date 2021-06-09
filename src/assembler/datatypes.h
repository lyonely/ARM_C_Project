#ifndef ASSEMBLER_DATATYPES_H
#define ASSEMBLER_DATATYPES_H
#include <stdint.h>

typedef uint32_t Instruction;

typedef struct {
    char ** array;
    uint16_t size;
} StringArray;    

typedef uint8_t RegAddress;

typedef uint32_t Word;

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

// used for defining opcode
typedef enum {
  // And. 
  AND_O = 0x0,
  // Exclusive or. 
  EOR_O = 0x1,
  // Subtract. 
  SUB_O = 0x2,
  // Reverse subtract. 
  RSB_O = 0x3,
  // Add. 
  ADD_O = 0x4,
  // And, set flags only. 
  TST_O = 0x8,
  // Exclusive or, set flags only.
  TEQ_O = 0x9,
  // Subtract, set flags only. 
  CMP_O = 0xA,
  // Or. 
  ORR_O = 0xC,
  //Move.
  MOV_O = 0xD,
} Opcode;

//used to identify type of shift for shifter
typedef enum {
  LSL_S = 0,
  LSR_S = 1,
  ASR_S = 2,
  ROR_S = 3,
} Shift;

// used to identify type of condition
typedef enum {
  EQ = 0,
  NE = 1,
  GE = 0xA,
  LT = 0xB,
  GT = 0xC,
  LE = 0xD,
  AL = 0xE,
} Condition;  

typedef struct {
  Opcode opcode;
  int rd;
  int rn;
  int is_imm;
  int rotation; // set when is_imm = 1
  int imm; // set when is_imm = 1
  Shift shift_type; // set when is_imm = 0
  int shift_by_reg; // is_imm = 0
  int shift_amount; // is_imm = 0, shift_by_reg = 0
  int rs; // is_imm = 0, shift_by_reg = 1
  int rm; // is_imm = 0 
} DataProcessingInstruction;


#endif

