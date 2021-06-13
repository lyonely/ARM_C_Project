#ifndef ASSEMBLER_DATATYPES_H
#define ASSEMBLER_DATATYPES_H
#define WORD_SIZE 32
#include <stdint.h>

#define MAX_CHARS_PER_LINE (511)

typedef uint32_t Instruction;

typedef struct {
  Instruction* array;
  uint16_t size;
} InstructionArray;

typedef struct {
    char ** array;
    uint16_t size;
} StringArray;    

typedef uint8_t RegAddress;

typedef uint32_t Word;

typedef uint32_t Address;

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

/* Enum for representing the Mnemonics of different operations */
typedef enum {
    ADD,
    SUB,
    RSB,
    AND,
    EOR,
    ORR,
    MOV,
    TST, TEQ,
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

/* Represents five types of instructions */
typedef enum {
  DATA_P,
  MULTIPLY,
  SDT,
  BRANCH
} Type;

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
  ROR_S = 3
} ShiftType;

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
  int acc;
  int rd;
  int rn;
  int rs;
  int rm;
} MultiplyInstruction;

typedef struct {
  Condition cond;
  char* symbol;
} BranchInstruction;

typedef struct {
  Condition cond;
  int is_imm;
  int preindex;
  int up_bit;
  int load_store;
  int rn;
  int rd;
  int imm_offset;
  ShiftType shift_type;
  int shift_by_reg;
  int shift_amount;
  int rs;
  int rm;
} DataTransferInstruction;

typedef struct {
  int is_imm;
  int immediate_shift;
  int rs;
} Shift;

typedef struct {
  int is_imm;
  
  union {
    struct {
      char immediate;
      int rotation;
    } imm_operand;

    struct {
      int shift_by_reg;
      ShiftType shift_type;
      int rm;
      Shift shift;
    } reg_operand;
  };
} Op2;

typedef struct {
  int is_imm; // = 1 if offset is interpreted as a shifted register (rn)

  union {
    int expression; // offset by <#expression> bytes

    struct {
      int preindex;
      ShiftType shift_type;
      int rm;
      int up_bit; // =1 if offset is added, =0 if offset is subtracted
      Shift shift;
    } ShiftedReg;
  };
} Offset;

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
  };
} Token;

#endif

