#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "functions.h"
#include "datatypes.h"

// Writes an array of instructions into a binary file
void write_to_file(Instruction *instructions, int size, char *filename) {
  FILE *fp = fopen(filename, "wb");
  fwrite(instructions, sizeof(Instruction), size, fp);
  fclose(fp);
}

void delete_string_array(StringArray *string_array) {
  for (int i = 0; i < string_array->size; i++) {
    free(string_array->array[i]);
  }
}

// Converts Instruction from big-endian to little-endian
void flip_endian(Instruction *i) {
  Instruction byte1 = *i & 0xFF000000;
  Instruction byte2 = *i & 0x00FF0000;
  Instruction byte3 = *i & 0x0000FF00;
  Instruction byte4 = *i & 0x000000FF;

  byte1 = byte1 >> 24;
  byte2 = byte2 >> 8;
  byte3 = byte3 << 8;
  byte4 = byte4 << 24;

  *i = byte1 | byte2 | byte3 | byte4;
}

Type get_type(Operation opcode) {
  switch(opcode) {
    case MUL:
    case MLA:
      return MULTIPLY; break;
    case LDR:
    case STR:
      return SDT; break;
    case BEQ:
    case BNE:
    case BGE:
    case BLT:
    case BGT:
    case BLE:
    case B:
      return BRANCH; break;
    default:
      return DATA_P; break;
  }
}

unsigned int get_num_args(Operation opcode) {
  switch(opcode) {
    case MLA:
      return 4;
    case BEQ:
    case BNE:
    case BGE:
    case BLT:
    case BGT:
    case BLE:
    case B:
      return 1;
    case AND:
    case ANDEQ:
    case EOR:
    case SUB:
    case RSB:
    case ADD:
    case ORR:
    case MUL:
      return 3;
    default:
      return 2;
  }
}

Operation string_to_operation(char *str) {
  if (!strcmp(str, "add")) {
    return ADD;
  }
  if (!strcmp(str, "sub")) {
    return SUB;
  }
  if (!strcmp(str, "rsb")) {
    return RSB;
  }
  if (!strcmp(str, "and")) {
    return AND;
  }
  if (!strcmp(str, "eor")) {
    return EOR;
  }
  if (!strcmp(str, "orr")) {
    return ORR;
  }
  if (!strcmp(str, "mov")) {
    return MOV;
  }
  if (!strcmp(str, "tst")) {
    return TST;
  }
  if (!strcmp(str, "teq")) {
    return TEQ;
  }
  if (!strcmp(str, "cmp")) {
    return CMP;
  }
  if (!strcmp(str, "mul")) {
    return MUL;
  }
  if (!strcmp(str, "mla")) {
    return MLA;
  }
  if (!strcmp(str, "ldr")) {
    return LDR;
  }
  if (!strcmp(str, "str")) {
    return STR;
  }
  if (!strcmp(str, "beq")) {
    return BEQ;
  }
  if (!strcmp(str, "bne")) {
    return BNE;
  }
  if (!strcmp(str, "bge")) {
    return BGE;
  }
  if (!strcmp(str, "blt")) {
    return BLT;
  }
  if (!strcmp(str, "bgt")) {
    return BGT;
  }
  if (!strcmp(str, "ble")) {
    return BLE;
  }
  if (!strcmp(str, "b")) {
    return B;
  }
  if (!strcmp(str, "lsl")) {
    return LSL;
  }
  if (!strcmp(str, "andeq")) {
    return ANDEQ;
  }

  fprintf(stderr, "No such mnemonic found.\n");
  exit(EXIT_FAILURE);
}

Condition string_to_condition(char *str) {
  if (!strcmp(str, "eq")) {
    return EQ;
  }
  if (!strcmp(str, "ne")) {
    return NE;
  }
  if (!strcmp(str, "ge")) {
    return GE;
  }
  if (!strcmp(str, "lt")) {
    return LT;
  }
  if (!strcmp(str, "gt")) {
    return GT;
  }
  if (!strcmp(str, "le")) {
    return LE;
  }
  if (!strcmp(str, "al") || 0 == strlen(str)) {
    return AL;
  }
  fprintf(stderr, "No such condition found.\n");
  exit(EXIT_FAILURE);
}

unsigned int string_to_reg_address(char *str) {
	return strtol(&str[1], (char **) NULL, 10);
}	

ShiftType string_to_shift(char *str) {
	if (!strcmp(str, "lsl")) {
		return LSL_S;
	}
	if (!strcmp(str, "lsr")) {
    return LSR_S;
	}
	if (!strcmp(str, "asr")) {
    return ASR_S;
	}
	if (!strcmp(str, "ror")) {
    return ROR_S;
	}
	fprintf(stderr, "No such shift found.\n");
  exit(EXIT_FAILURE);
}

uint32_t parse_immediate_value(char *str) {
	if(strstr(str, "0x")) {
		return strtol(str, (char **)NULL, 16);
	} else {
		return strtol(str, (char **)NULL, 10);
	}
}

char *opcode_to_string(Operation opcode) {
  switch(opcode) {
    case ADD: return "add";
    case SUB: return "sub";
    case RSB: return "rsb";
    case AND: return "and";
    case EOR: return "eor";
    case ORR: return "orr";
    case MOV: return "mov";
    case TST: return "tst";
    case TEQ: return "teq";
    case CMP: return "cmp";
    case MUL: return "mul";
    case MLA: return "mla";
    case LDR: return "ldr";
    case STR: return "str";
    case BEQ: return "beq";
    case BNE: return "bne";
    case BGE: return "bge";
    case BLT: return "blt";
    case BLE: return "ble";
    case BGT: return "bgt";
    case B: return "b";
    case LSL: return "lsl";
    default: return "andeq";
  }
}

