#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "datatypes.h"
#include "functions.h"
#include "symboltable.h"

void parse_shift_data_processing(StringArray *args, Token *token) {
	printf("Parsing shift: %s %s\n", args->array[0], args->array[1]);
	token->TokenType.DataP.operand2.Op2Type.reg_operand.shift_type = string_to_shift(args->array[0]); // Shift type stored

  if ('#' == args->array[1][0]) {
    // Shift by immediate
    char *number = &args->array[1][1];
    token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.is_imm = 1;
    token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.immediate_shift = parse_immediate_value(number);
  } else if ('r' == args->array[1][0]) {
    // Shift by register
    token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.is_imm = 0; token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.rs = string_to_reg_address(args->array[1]);
  } else {
    fprintf(stderr, "Shift not number or register");
    exit(EXIT_FAILURE);
  }
}

void parse_shift_data_transfer(StringArray *args, Token *token) {
	token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift_type = string_to_shift(args->array[0]);  
  if ('#' == args->array[1][0]) {
    // Shift by immediate
    char *number = &args->array[1][1];
    token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift.is_imm = 1;
    token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift.immediate_shift = parse_immediate_value(number);
  } else if ('r' == args->array[1][0]) {
    // Shift by register
    token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift.is_imm = 0;
    token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift.rs = string_to_reg_address(args->array[1]);
  } else {
    fprintf(stderr, "Shift not number or register");
    exit(EXIT_FAILURE);
  }
}	

void parse_operand_data_processing(StringArray *args, Token *token) {
  for (int i = 0; i < args->size; i++) {
    remove_spaces(args->array[i]);
  }

  //printf("Parsing operands for %s instruction:\n", opcode_to_string(token->opcode));
  char **sections = args->array;
  if ('#' == sections[0][0]) {
    //printf("<#expression> argument detected.\n");
    // Operand2 in the form <#expression>
    if (token->opcode == LSL) {
      // Convert to mov rd, rd, lsl <#expression>
      token->TokenType.DataP.operand2.is_imm = 0;
      token->TokenType.DataP.operand2.Op2Type.reg_operand.rm = token->TokenType.DataP.rd;
      token->TokenType.DataP.operand2.Op2Type.reg_operand.shift_type = LSL_S;
      token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.is_imm = 1;
      token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.immediate_shift = parse_immediate_value(&sections[0][1]);
    } else {
      token->TokenType.DataP.operand2.is_imm = 1;
      
      uint32_t *imm_addr = &token->TokenType.DataP.operand2.Op2Type.imm_operand.immediate;
      *imm_addr = parse_immediate_value(&sections[0][1]);

      int *rotation = &token->TokenType.DataP.operand2.Op2Type.imm_operand.rotation;
      *rotation = 0;

      uint16_t shift = WORD_SIZE;
      if (*imm_addr > 0xFF) {
        while (!(*imm_addr & 0x3)) {
          *imm_addr >>= 2;
          shift -= 2;
        }
        *rotation = shift / 2;
      }
    }
  } else if ('r' == sections[0][0]) {
    // Operand2 in the form Rm{,<shift>}
    token->TokenType.DataP.operand2.is_imm = 0;
    token->TokenType.DataP.operand2.Op2Type.reg_operand.rm = string_to_reg_address(sections[0]);

    if (args->size >= 2) {
      // Rm has shift
      StringArray *shift_tokens = malloc(sizeof(StringArray));

      if (!shift_tokens) {
        perror("Unable to allocate memory for shift_tokens.\n");
        exit(EXIT_FAILURE);
      }

      shift_tokens->array = &sections[1];
      shift_tokens->size = args->size - 1;
      parse_shift_data_processing(shift_tokens, token);
      free(shift_tokens);
    } else {
      // Rm has no shift
      token->TokenType.DataP.operand2.Op2Type.reg_operand.shift_type = 0;
      token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.is_imm = 1;
      token->TokenType.DataP.operand2.Op2Type.reg_operand.shift.immediate_shift = 0;
    }
  } else {
    fprintf(stderr, "Operand not number or register");
    exit(EXIT_FAILURE);
  }
}				

void parse_offset_data_transfer(StringArray *args, Token *token) {
  for (int i = 0; i < args->size; i++) {
    remove_spaces(args->array[i]);
  }
  
  char **sections = args->array;
  if ('#' == sections[0][0]) {
    // Offset in the form <#expression>, 12 bits unsigned
    token->TokenType.SDT.offset.is_imm = 0;
    token->TokenType.SDT.offset.OffsetType.expression = parse_immediate_value(&sections[0][1]);
    printf("Offset expression: %d\n", token->TokenType.SDT.offset.OffsetType.expression); 
  } else if ('r' == sections[0][0]) {
    // Offset in the form Rm{,<shift>}
    token->TokenType.SDT.offset.is_imm = 1;
    token->TokenType.SDT.offset.OffsetType.ShiftedReg.rm = string_to_reg_address(sections[0]);

    if (args->size >= 2) {
      // Rm has shift
      StringArray *shift_tokens = malloc(sizeof(StringArray));

      if (!shift_tokens) {
        perror("Unable to allocate memory for shift_tokens.\n");
        exit(EXIT_FAILURE);
      }

      shift_tokens->array = &sections[1];
      shift_tokens->size = args->size - 1;
      parse_shift_data_transfer(shift_tokens, token);
      free(shift_tokens);
    } else {
      // Rm has no shift
      token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift_type = 0;
      token->TokenType.SDT.offset.up_bit = 0;
      token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift.is_imm = 1;
      token->TokenType.SDT.offset.OffsetType.ShiftedReg.shift.immediate_shift = 0;
    }
  } else {
    fprintf(stderr, "Operand not number or register");
    exit(EXIT_FAILURE);
  }
}

void tokenise_dataprocessing(char *str, Token *token) {
  //printf("Tokenising %s instruction:\n", opcode_to_string(token->opcode));

  StringArray *operand2 = malloc(sizeof(StringArray));
  operand2->array = malloc(token->num_args * sizeof(char*));
  operand2->size = 0;

  char *arg = strtok(str, ",");
  if (token->num_args == 3) { 
    // ADD, ANDEQ, EOR, SUB, RSB, ADD, ORR
    token->TokenType.DataP.rd = string_to_reg_address(arg);
    arg = strtok(NULL, ",");
    token->TokenType.DataP.rn = string_to_reg_address(arg);
  } else {
    if (token->opcode == MOV || token->opcode == LSL) { 
      // MOV, LSL
      token->TokenType.DataP.rd = string_to_reg_address(arg);
    } else {
      // TST, TEQ, CMP
      token->TokenType.DataP.rn = string_to_reg_address(arg);
      printf("token rn field set to %d\n", token->TokenType.DataP.rn);
    }
  }
 
  arg = strtok(NULL, ",");
  //printf("Argument get: %s\n", arg);
  strcpy(operand2->array[0] = malloc(strlen(arg) + 1), arg);
  //printf("Arg 0 stored in op2 array: %s\n", operand2->array[0]);
  operand2->size++;
 
  arg = strtok(NULL, " ,");
  if (arg) {
    strcpy(operand2->array[1] = malloc(strlen(arg) + 1), arg); // rm
    //printf("Arg 1 stored in op2 array: %s\n", operand2->array[1]);
    arg = strtok(NULL, "");
    strcpy(operand2->array[2] = malloc(strlen(arg) + 1), arg); // rs or constant shift amt
    //printf("Arg 2 stored in op2 array: %s\n", operand2->array[2]);
    operand2->size += 2;
  }
  parse_operand_data_processing(operand2, token);
  delete_string_array(operand2);
  free(operand2->array);
  free(operand2);
}

void tokenise_datatransfer(char* str, Token *token, Address *memory_address, Instruction *instructions) {

  StringArray* offset = malloc(sizeof(StringArray));
  offset->array = malloc(token->num_args * sizeof(char*));
  offset->size = 0;

  char* arg = strtok(str, ",");
  token->TokenType.SDT.rd = string_to_reg_address(arg); // rd

  arg = strtok(NULL, ",");
  printf("Next argument grabbed: %s\n", arg);
  char* constant_offset = strchr(arg, '=');
  if (constant_offset) {
    // address in the form <=expression>
    uint32_t exp = parse_immediate_value(&arg[1]);
    if (exp <= 0xFF) {
      // convert to mov instruction
      token->TokenType.DataP.rd = token->TokenType.SDT.rd;
      token->opcode = MOV;
      token->num_args = 2;
      token->TokenType.DataP.operand2.is_imm = 1;
      token->TokenType.DataP.operand2.Op2Type.imm_operand.immediate = exp;
      token->TokenType.DataP.operand2.Op2Type.imm_operand.rotation = 0;

    } else {
      token->TokenType.SDT.rn = 15; // rn = PC
      token->TokenType.SDT.offset.preindex = 1;
      token->TokenType.SDT.offset.is_imm = 0;
      token->TokenType.SDT.offset.OffsetType.expression = *memory_address - (token->address + 8);
      instructions[*memory_address / 4] = exp; // Store value in memory
      *memory_address += 4;
    }
  } else {
    // address in the form [Rn] / [Rn, <offset>] / [Rn], <offset>
    char* bracket_end = strchr(arg, ']');
    if (bracket_end) {
      // address in the form [Rn], <offset>; arg = [Rn]
      token->TokenType.SDT.offset.preindex = 0;
      memmove(arg, &arg[1], bracket_end - arg);
      arg[bracket_end - arg - 1] = '\0';
      token->TokenType.SDT.rn = string_to_reg_address(arg);
    } else {
      // address in the form [Rn, <offset>]; arg = [Rn
      token->TokenType.SDT.offset.preindex = 1;
      token->TokenType.SDT.rn = string_to_reg_address(&arg[1]);
    }

    arg = strtok(NULL, ",");
    if (!arg) {
      // no offset, address is [Rn]
      token->TokenType.SDT.offset.preindex = 1;
      offset->array[0] = "#0";
      offset->size++;
    } else {
      remove_spaces(arg);
      // arg = <#expression] / <#expression> / {+/-}Rm 
      // Set up bit
      if (arg[0] == '#' && arg[1] == '-') {
        token->TokenType.SDT.offset.up_bit = 0;
      } else if (arg[0] == '-') {
        token->TokenType.SDT.offset.up_bit = 0;
      } else {
        token->TokenType.SDT.offset.up_bit = 1;
      }
      
      bracket_end = strchr(arg, ']');
      offset->array[0] = malloc(strlen(arg) + 1);
      
      if (bracket_end) {
        // Store #expression into offset array
        // arg = <#expression>]
        if (arg[1] == '-' || arg[1] == '+') {
          memmove(&arg[1], &arg[2], bracket_end - arg - 2);
          arg[bracket_end - arg - 1] = '\0';
          memcpy(offset->array[0], arg, strlen(arg) + 1);
          offset->array[0][bracket_end - arg] = '\0';
        } else {
          memcpy(offset->array[0], arg, bracket_end - arg);
          offset->array[0][bracket_end - arg + 1] = '\0';
        }
        offset->size++;
        printf("Stored arg %s into offset array: offset[0] = %s\n",
            arg, offset->array[0]);
      } else {
        // arg = {+/-}Rm or <#expression>
        
        // Store Rm or <#expression> into offset array
        if (arg[0] == '-' || arg[0] == '+') {
          memcpy(offset->array[0], &arg[1], strlen(arg));
          offset->array[0][strlen(arg)] = '\0';
        } else {
          offset->array[0] = arg;
        }
        offset->size++;

        // Store shifttype and shift arguments into offset array
        arg = strtok(NULL, ",");
        if (arg) {
          remove_spaces(arg);
          bracket_end = strchr(arg, ']');
          char* space = strchr(arg, ' ');
          memcpy(offset->array[1] = malloc(space - arg + 1), arg, space - arg);
          offset->array[1][space - arg] = '\0';
          if (bracket_end) {
            // arg = <shifttype> <shift>]
            memcpy(offset->array[2] = malloc(bracket_end - &space[1] + 1), &space[1], bracket_end - &space[1]);
            offset->array[2][bracket_end - &space[1]] = '\0';
          } else {
            // arg = <shifttype> <shift>
            memcpy(offset->array[2] = malloc(strlen(&space[1]) + 1), &space[1], strlen(&space[1]));
            offset->array[2][strlen(&space[1])] = '\0';
          }
          offset->size += 2;
        }
      }
    }
    parse_offset_data_transfer(offset, token);
  }
  free(offset->array);
  free(offset);
}

void tokenise_multiply(char* str, Token *token) {
  char* args = strcpy(malloc(strlen(str) + 1), str);
  char* operand = strtok(args, ",");
  token->TokenType.Multiply.rd = string_to_reg_address(operand);
  operand = strtok(NULL, ",");
  token->TokenType.Multiply.rm = string_to_reg_address(operand);
  operand = strtok(NULL, ",");
  token->TokenType.Multiply.rs = string_to_reg_address(operand);

  if (token->opcode == MLA) {
    operand = strtok(NULL, ",");
    token->TokenType.Multiply.rn = string_to_reg_address(operand);
  }
  free(args);
}

void tokenise_branch(char* str, Token *token, SymbolTable *symboltable) {
  if (strchr(str, ',')) {
    perror("Wrong number of args provided in tokenise_branch");
    exit(EXIT_FAILURE);
  }
  
  if (str[0] == '#') {
    size_t arg_len = strlen(str);
    char addr[arg_len];
    memcpy(addr, &str[1], arg_len);
    token->TokenType.Branch.target_address = (Address) parse_immediate_value(addr);
  } else {
    token->TokenType.Branch.target_address = lookup_symbol(symboltable, str); 
  }
}

// Tokenises assembly code and parses into Instruction
int tokenise(char *line, Address address, SymbolTable *symboltable, 
    Instruction *instructions, Address *memory_address, Token *token) {

  printf("Tokenising line %s:\n", line);
  char *opcode = strtok(line, " ");
  char *args = strtok(NULL, "");

  printf("Opcode: %s\n", opcode);
  printf("Args: %s\n", args);

  if (!args) {
    perror("Error getting arguments");
    exit(EXIT_FAILURE);
  }

  token->opcode = string_to_operation(opcode);
  token->address = address;
  token->num_args = get_num_args(token->opcode);

  // Condition setting
  if (opcode[0] == 'b' && opcode[1]) {
    char cond[2];
    memcpy(cond, &opcode[1], 2);
    token->cond = string_to_condition(cond);
  } else if (token->opcode == ANDEQ) {
    token->cond = EQ;
  } else {
    token->cond = AL;
  }
  
  switch(get_type(token->opcode)) {
    case DATA_P:
      tokenise_dataprocessing(args, token); break;
    case MULTIPLY:
      tokenise_multiply(args, token); break;
    case SDT:
      tokenise_datatransfer(args, token, memory_address, instructions); break;
    default:
      tokenise_branch(args, token, symboltable); break;
  }
  return 1;
}

