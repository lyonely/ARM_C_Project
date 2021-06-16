#ifndef ASSEMBLER_DATATYPES_H
#define ASSEMBLER_DATATYPES_H

#include <stdint.h>

#define WORD_SIZE (32)
#define MAX_CHARS_PER_LINE (511)

typedef uint32_t Instruction;
typedef uint32_t Address;

typedef struct {
  char **array;
  uint16_t size;
} StringArray;    

/* Struct to represent an entry in the symbol table */
typedef struct {
  char* symbol;
  Address address;
} Symbol;

/* Struct to represent the symbol table container */
typedef struct {
  int size;
  Symbol* table;
} SymbolTable;

/* Operation mnemonics */
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

/* Instruction types*/
typedef enum {
  DATA_P,
  MULTIPLY,
  SDT,
  BRANCH
} Type;

/* Shift types */
typedef enum {
  LSL_S = 0,
  LSR_S = 1,
  ASR_S = 2,
  ROR_S = 3
} ShiftType;

/* Condition codes */
typedef enum {
  EQ = 0,
  NE = 1,
  GE = 0xA,
  LT = 0xB,
  GT = 0xC,
  LE = 0xD,
  AL = 0xE
} Condition;  

/* Struct representing possible shifts */
typedef struct {
  int is_imm;
  int immediate_shift;
  int rs;
} Shift;

/* Struct representing data processing instruction operand2 */
typedef struct {
  int is_imm;
  
  union {
    struct {
      uint32_t immediate;
      int rotation;
    } imm_operand;

    struct {
      ShiftType shift_type;
      int rm;
      Shift shift;
    } reg_operand;
  } Op2Type;
} Op2;

/* Struct representing SDT instruction offset */
typedef struct {
  int is_imm; // = 1 if offset is interpreted as a shifted register (rn)
  int preindex;
  int up_bit; // =1 if offset is added, =0 if offset is subtracted
  union {
    int expression; // offset by <#expression> bytes

    struct {
      ShiftType shift_type;
      int rm;
      Shift shift;
    } ShiftedReg;
  } OffsetType;
} Offset;

/* Struct for storing tokenised assembly code */
typedef struct {
  Address address;
  Operation opcode;
  Condition cond;
  unsigned int num_args;
  union {
    struct {
      int rd;
      int rn;
      Op2 operand2;
    } DataP;

    struct {
      int rd;
      int rm;
      int rs;
      int rn;
    } Multiply;

    struct {
      Address target_address;
    } Branch;

    struct {
      int rd;
      int rn;
      Offset offset;
    } SDT;
  } TokenType;
} Token;

#endif

