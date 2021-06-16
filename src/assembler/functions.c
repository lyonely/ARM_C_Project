#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"
#include "datatypes.h"

// Writes an array of instructions into a binary file
void write_to_file(Instruction *instructions, int size, char *filename) {
  FILE *fp = fopen(filename, "wb");
  fwrite(instructions, sizeof(Instruction), size, fp);
  printf("instructions written to file %s:\n", filename);
  for (int i = 0; i < size; i++)  { printf("Instruction %d: %x\n", i, instructions[i]);
  }
  fclose(fp);
}

void remove_spaces(char *str) {
  while (isspace(str[0])) {
    memmove(str, &str[1], strlen(str));
  }

  int pos = strlen(str) - 1;
  
  while (isspace(str[pos]) && pos >= 0) {
    pos--;
  }

  if (pos < strlen(str) - 1) {
    str[pos + 1] = '\0';
  }
  printf("Space removed, result: (%s)\n", str);
}

void delete_string_array(StringArray *string_array) {
  for (int i = 0; i < string_array->size; i++) {
    free(string_array->array[i]);
  }
}

// Initialises values for Token struct
void initialise_token(Token *token) {
  token->address = 0;
  token->opcode = 0;
  token->cond = 0;
  token->num_args = 0;
  
  token->TokenType.DataP.rd = 0;
  token->TokenType.DataP.rn = 0;
  token->TokenType.DataP.operand2.is_imm = 0;
 
  token->TokenType.Multiply.rd = 0;
  token->TokenType.Multiply.rm = 0;
  token->TokenType.Multiply.rs = 0;
  token->TokenType.Multiply.rn = 0;

  token->TokenType.Branch.target_address = 0;
  
  token->TokenType.SDT.offset.up_bit = 1;
  token->TokenType.SDT.rd = 0;
  token->TokenType.SDT.rn = 0;
  token->TokenType.SDT.offset.is_imm = 0;
  token->TokenType.SDT.offset.preindex = 0;
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

unsigned int get_opcode(Operation op) {
  if (get_type(op) != DATA_P) {
    perror("Wrong instruction type passed into get_opcode\n");
    exit(EXIT_FAILURE);
  }
  switch(op) {
    case AND: 
    case ANDEQ: return 0;
    case EOR: return 1;
    case SUB: return 2;
    case RSB: return 3;
    case ADD: return 4;
    case ORR: return 12;
    case MOV: 
    case LSL: return 13;
    case TST: return 8;
    case TEQ: return 9;
    default: return 10;
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
  remove_spaces(str);
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
  remove_spaces(str);
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
  printf("String to remove spaces: (%s)\n", str);
  remove_spaces(str);
  printf("Converting (%s) to reg address\n", str);
	return strtol(&str[1], (char **) NULL, 10);
}	

ShiftType string_to_shift(char *str) {
  remove_spaces(str);
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
  remove_spaces(str);
  long result;
	if(strstr(str, "0x")) {
		result = strtol(str, (char**)NULL, 16);
	} else {
		result = strtol(str, (char**)NULL, 10);
	}
	return result;
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

