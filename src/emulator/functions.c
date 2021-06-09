#include <stdio.h>
#include "functions.h"

#define N_FLAG 1<<31
#define Z_FLAG 1<<30
#define C_FLAG 1<<29
#define V_FLAG 1<<28

#define SET_CONDITION_MASK 1<<20
#define IMMEDIATE_MASK 1<<25
#define OPCODE_MASK 0x1e00000
#define OPCODE_SHIFT 21
#define RN_MASK 0xf0000
#define RN_SHIFT 16
#define RD_MASK 0xf000
#define RD_SHIFT 12
#define OPERAND2_MASK 0xfff

#define ACCUMULATE_MASK 1<<21
#define RN_MULTIPLY_MASK 0xf000
#define RN_MULTIPLY_SHIFT 12
#define RD_MULTIPLY_MASK 0xf0000
#define RD_MULTIPLY_SHIFT 16
#define RS_MULTIPLY_MASK 0xf00
#define RS_MULTIPLY_SHIFT 8
#define RM_MULTIPLY_MASK 0xf

#define PRE_INDEXING_SHIFT 24
#define UP_SHIFT 23
#define LOAD_SHIFT 20
#define OFFSET_MASK 0xfff
#define COND_SHIFT 28

#define V0_MASK 0xefffffff
#define C0_MASK 0xdfffffff
#define Z0_MASK 0xbfffffff
#define N0_MASK 0x7fffffff
#define BIT_31_MASK 1<<31
#define RESET_FLAGS_MASK 0x0fffffff

#define MULTIPLY_BITS_7_4 9
#define MULTIPLY_BITS_27_22 0
#define BIT_27_MASK 1<<27
#define BIT_26_MASK 1<<26
#define BIT_25_MASK 1<<25
#define BIT_7_MASK 1<<7
#define BIT_4_MASK 1<<4

long read_binary(FILE* file, void *destination) {
  fseek(file, 0, SEEK_END); // seek to end of file
  long size = ftell(file) + 1; // get current file pointer + 1 for zero instruction at end of file
  fseek(file, 0, SEEK_SET); // seek back to beginning of file for reading
  fread(destination, 1, (size_t) size, file);
  return size;
}

int instruction_is_valid(Instruction instruction, struct Registers* regs) {
  
  uint32_t cpsr = regs->cpsr;

  uint32_t N_equals_V = (N_FLAG & cpsr) == ((V_FLAG & cpsr) << 3);
  uint32_t Z_set = Z_FLAG & cpsr;
  
  int is_valid = 0;

  int cond = get_cond(instruction);

  switch(cond) {
    case EQ: is_valid = Z_set; break;
    case NE: is_valid = !Z_set; break;
    case GE: is_valid = N_equals_V; break;
    case LT: is_valid = !N_equals_V; break;
    case GT: is_valid = (!Z_set) && N_equals_V; break;
    case LE: is_valid = Z_set || (!N_equals_V); break;
    case AL: is_valid = 1; break;
    default: is_valid = 0;
  }
  return is_valid;
}

/*data processing instructions*/

// Performs right rotation on a 32-bit binary number
uint32_t rotate_right(uint32_t value, uint32_t rotation) {
	uint32_t shifted = value >> rotation;
	uint32_t rotated_bits = value << (32 - rotation);
	return (shifted | rotated_bits);
}

int is_set(Instruction instruction) {
  return (instruction & SET_CONDITION_MASK);
}

int is_immediate(Instruction instruction) {
  return (instruction & IMMEDIATE_MASK);
}

int opcode(Instruction instruction) {
  return (instruction & OPCODE_MASK) >> OPCODE_SHIFT;
}

int rn(Instruction instruction) {
  return (instruction & RN_MASK) >> RN_SHIFT;
}

int rd(Instruction instruction) {
  return (instruction & RD_MASK) >> RD_SHIFT;
}
				
int operand2(Instruction instruction) {
  return (instruction & OPERAND2_MASK);
}

/*multiply instruction*/

int accumulate(Instruction instruction) {
  return (instruction & ACCUMULATE_MASK);
}

int rn_multiply(Instruction instruction) {
  return (instruction & RN_MULTIPLY_MASK) >> RN_MULTIPLY_SHIFT;
}

int rd_multiply(Instruction instruction){
  return (instruction & RD_MULTIPLY_MASK) >> RD_MULTIPLY_SHIFT;
}

int rs_multiply(Instruction instruction) {
  return (instruction & RS_MULTIPLY_MASK) >> RS_MULTIPLY_SHIFT;
}

int rm_multiply(Instruction instruction) {
  return (instruction & RM_MULTIPLY_MASK);
}

/*single data transfer*/
int is_pre_indexing(Instruction instruction) {
  return (instruction >> PRE_INDEXING_SHIFT) & 1;
}

int is_up(Instruction instruction) {
  return (instruction >> UP_SHIFT) & 1;
}

int is_load(Instruction instruction) {
  return (instruction >> LOAD_SHIFT) & 1;
}

int sdt_offset(Instruction instruction) {
  return (instruction & OFFSET_MASK);
}

int get_cond(Instruction instruction) {
  return (instruction >> COND_SHIFT);
}

void set_v(Register *cpsr, int value) {
	if (value) {
		*cpsr |= V_FLAG;
	} else {
		*cpsr &= V0_MASK;
	}
}

void set_c(Register *cpsr, int value) {
	if (value) {
		*cpsr |= C_FLAG;
	} else {
		*cpsr &= C0_MASK;
	}
}

void set_z(Register *cpsr, int value) {
	if (value) {
		*cpsr |= Z_FLAG;
	} else {
		*cpsr &= Z0_MASK;
	}
}

void set_n(Register *cpsr, int value) {
	if (value) {
		*cpsr |= N_FLAG;
	} else {
		*cpsr &= N0_MASK;
	}
}

void set_n_z(Register *cpsr, int result) {
  set_z(cpsr, !result);
  set_n(cpsr, BIT_31_MASK & result);
}

void reset_flags(Register *cpsr) {
	*cpsr &= RESET_FLAGS_MASK;
}

enum InstructionType get_instr_type(Instruction instruction) {
  if (!instruction) {
    return ALLZERO;
  }

  if (((instruction >> 4) & ((1 << 4) -1)) == MULTIPLY_BITS_7_4 && (((instruction >> 22) & ((1 << 6) -1)) == MULTIPLY_BITS_27_22)) {
    return MUL;
  }

  if (instruction & BIT_27_MASK) {
    return BRANCH; 
  }

  if (instruction & BIT_26_MASK) {
    return SDT;
  }

  if (instruction & BIT_25_MASK || !(instruction & BIT_7_MASK) || !(instruction & BIT_4_MASK)) {
    return DP;
  }
  
  return NOOP;
}

void print_registers(struct Registers *reg) {
  printf("Registers:\n");

  for (int i = 0; i < 13; i++) {
    if (i < 10) {
      printf("$%d  : %10d (0x%08x)\n", i, reg->general_regs[i], reg->general_regs[i]);
    } else {
      printf("$%d : %10d (0x%08x)\n", i, reg->general_regs[i], reg->general_regs[i]);
    }
  }

  printf("PC  : %10d (0x%08x)\n", reg->pc, reg->pc);
  printf("CPSR: %10d (0x%08x)\n", reg->cpsr, reg->cpsr);
}

void print_memory(Byte* memory) {
  printf("Non-zero memory:\n");

  for (int i = 0; i < MEMORY_CAPACITY; i+=4) {

    if (*((uint32_t*) (memory + i))) {
      printf("0x%08x: 0x", i);
      
      for (int j = 0; j < 4; j++) {
        printf("%02x", memory[i+j]);
      }
      printf("\n");
    }
  }
}

