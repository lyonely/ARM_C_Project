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

void parse_shift_data_processing(StringArray *tokens, DataProcessingInstruction *instruction) {
	instruction->shift_type = string_to_shift(tokens->array[0]);
  if ('#' == tokens->array[1][0]) {
    // In the form <#expression>
    char *number = &tokens->array[1][1];
    instruction->shift_amount = parse_immediate_value(number);
  } else if ('r' == tokens->array[1][0]) {
    // Is a register
    instruction->rs = string_to_reg_address(tokens->array[1]);
  } else {
    fprintf(stderr, "Shift not number or register");
    exit(EXIT_FAILURE);
  }
}

void parse_shift_data_transfer(StringArray *tokens, DataTransferInstruction *instruction) {
	instruction->shift_type = string_to_shift(tokens->array[0]);
  if ('#' == tokens->array[1][0]) {
    char *number = &tokens->array[1][1];
    instruction->shift_amount = parse_immediate_value(number);
  } else if ('r' == tokens->array[1][0]) {
    instruction->rs = string_to_reg_address(tokens->array[1]);
  } else {
    fprintf(stderr, "Shift not number or register");
    exit(EXIT_FAILURE);
  }
}	

void parse_operand_data_processing(StringArray *tokens, DataProcessingInstruction *instruction) {
  char **sections = tokens->array;
  if ('#' == sections[0][0]) {
    // In the form <#expression>
    instruction->imm = parse_immediate_value(&sections[0][1]);

    uint16_t shift = WORD_SIZE;
    if (instruction->imm > 0xFF) {
      while (!(instruction->imm & 0x3)) {
          instruction->imm >>= 2;
          shift--;
      }
    }

    instruction->shift_amount = shift;

  } else if ('r' == sections[0][0]) {
    // In the form Rm{,<shift>}
    instruction->rm = string_to_reg_address(sections[0]);

    if (tokens->size >= 2) {
      // Has shift
      StringArray *shift_tokens = malloc(sizeof(StringArray));

      if (!shift_tokens) {
        perror("Unable to allocate memory for shift_tokens.\n");
        exit(EXIT_FAILURE);
      }

      // Pass the <shift> section into parse_shift
      shift_tokens->array = &sections[1];
      shift_tokens->size = tokens->size - 1;
      parse_shift_data_processing(shift_tokens, instruction);
      free(shift_tokens);
    }
  } else {
    fprintf(stderr, "Operand not number or register");
    exit(EXIT_FAILURE);
  }
}				

void parse_operand_data_transfer(StringArray *tokens, DataTransferInstruction *instruction) {
  char **sections = tokens->array;
  if ('#' == sections[0][0]) {
    // In the form <#expression>
    instruction->imm_offset = parse_immediate_value(&sections[0][1]);

    uint16_t shift = WORD_SIZE;
    if (instruction->imm_offset > 0xFF) {
      while (!(instruction->imm_offset & 0x3)) {
          instruction->imm_offset >>= 2;
          shift--;
      }
    }

    instruction->shift_amount = shift;

  } else if ('r' == sections[0][0]) {
    // In the form Rm{,<shift>}
    instruction->rm = string_to_reg_address(sections[0]);

    if (tokens->size >= 2) {
      // Has shift
      StringArray *shift_tokens = malloc(sizeof(StringArray));

      if (!shift_tokens) {
        perror("Unable to allocate memory for shift_tokens.\n");
        exit(EXIT_FAILURE);
      }

      // Pass the <shift> section into parse_shift
      shift_tokens->array = &sections[1];
      shift_tokens->size = tokens->size - 1;
      parse_shift_data_transfer(shift_tokens, instruction);
      free(shift_tokens);
    }
  } else {
    fprintf(stderr, "Operand not number or register");
    exit(EXIT_FAILURE);
  }
}

Word parse_immediate_value(char *str) {
	if(strstr(str, "0x")) {
		return strtol(str, (char **)NULL, 16);
	} else {
		return strtol(str, (char **)NULL, 10);
	}
}	
