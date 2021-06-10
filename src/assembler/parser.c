#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

Opcode mnemonic_to_opcode(Operation operation) {
	switch (operation) {
    case ADD:
      return ADD_O;
      break;
    case SUB:
      return SUB_O;
      break;
    case RSB:
      return RSB_O;
      break;
    case AND:
      return AND_O;
      break;
    case EOR:
      return EOR_O;
      break;
    case ORR:
      return ORR_O;
      break;
    case MOV:
      return MOV_O;
      break;
    case TST:
      return TST_O;
      break;
    case TEQ:
      return TEQ_O;
      break;
    case CMP:
      return CMP_O;
      break;
    default:
      fprintf(stderr, "No such opcode found.\n");
      exit(EXIT_FAILURE);
      break;
  }			
}	

RegAddress string_to_reg_address(char *str) {
	return strtol(&str[1], (char **) NULL, 10);
}	

Shift string_to_shift(char *str) {
	if(!strcmp(str, "lsl")) {
		return LSL_S;
	}
	fprintf(stderr, "No such shift found.\n");
  exit(EXIT_FAILURE);
}	

void parse_shift(StringArray *tokens, Instruction *instruction) {
	
}

void parse_operand(StringArray *tokens, Instruction *instruction) {

}				

Word parse_immediate_value(char *str) {
	if(strstr(str, "0x")) {
		return strtol(str, (char **)NULL, 16);
	} else {
		return strtol(str, (char **)NULL, 10);
	}
}	
