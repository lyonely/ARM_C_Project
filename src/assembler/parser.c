#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "datatypes.h"
#include "functions.h"
#include "symboltable.h"

void parse_shift_data_processing(StringArray *args, Token *token) {
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
  char **sections = args->array;
  if ('#' == sections[0][0]) {
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
      char *imm_addr = &token->TokenType.DataP.operand2.Op2Type.imm_operand.immediate;
      int *rotation = &token->TokenType.DataP.operand2.Op2Type.imm_operand.rotation;
      
      *imm_addr = parse_immediate_value(&sections[0][1]);
      uint16_t shift = WORD_SIZE;
      if (*imm_addr > 0xFF) {
        while (!(*imm_addr & 0x3)) {
          *imm_addr >>= 2;
          shift--;
        }
        *rotation = shift;
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
    }
  } else {
    fprintf(stderr, "Operand not number or register");
    exit(EXIT_FAILURE);
  }
}				

void parse_offset_data_transfer(StringArray *args, Token *token) {
  char **sections = args->array;
  if ('#' == sections[0][0]) {
    // Offset in the form <#expression>, 12 bits unsigned
    token->TokenType.SDT.offset.is_imm = 0;
    token->TokenType.SDT.offset.OffsetType.expression = parse_immediate_value(&sections[0][1]);
 
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
    }
  } else {
    fprintf(stderr, "Operand not number or register");
    exit(EXIT_FAILURE);
  }
}

void tokenise_dataprocessing(char* str, Token *token) {
  char* arg = strcpy(malloc(strlen(str) + 1), str);
  StringArray* operand2 = malloc(sizeof(StringArray));
  operand2->size = 0;

  arg = strtok(str, ","); 
  token->TokenType.DataP.rd = string_to_reg_address(arg);
  
  if (token->num_args == 3) { 
    // ADD, ANDEQ, EOR, SUB, RSB, ADD, ORR
    arg = strtok(NULL, ",");
    token->TokenType.DataP.rd = string_to_reg_address(arg);
    token->TokenType.DataP.rn = string_to_reg_address(arg);
  } else {
    if (token->opcode == MOV || token->opcode == LSL) { 
      // MOV, LSL
      token->TokenType.DataP.rd = string_to_reg_address(arg);
    } else {
      // TST, TEQ, CMP
      token->TokenType.DataP.rn = string_to_reg_address(arg);
    }
  }
 
  arg = strtok(NULL, ",");
  operand2->array[0] = arg; // constant or rm
  operand2->size++;
 
  arg = strtok(NULL, " ,");
  if (arg) {
    operand2->array[1] = arg; // shift type
    arg = strtok(NULL, "");
    operand2->array[2] = arg; // rs or constant shift amount
    operand2->size += 2;
  }
  parse_operand_data_processing(operand2, token);
  free(arg);
  free(operand2);
}

void tokenise_datatransfer(char* str, Token *token, Address *memory_address, Instruction *instructions) {
  char* arg = strcpy(malloc(strlen(str) + 1), str);
  StringArray* offset = malloc(sizeof(StringArray));
  offset->size = 0;

  arg = strtok(str, ","); 
  token->TokenType.SDT.rd = string_to_reg_address(arg); // rd
 
  arg = strtok(NULL, ",");
  char* constant_offset = strchr(arg, '=');
  if (constant_offset) {
    // address in the form <=expression>
    uint32_t exp = parse_immediate_value(&arg[1]);
    if (exp <= 0xFF) {
      // convert to mov instruction
      token->TokenType.DataP.rd = token->TokenType.SDT.rd;
      token->opcode = MOV;
      token->num_args = 2;
    } else {
      token->TokenType.SDT.rn = 15; // rn = PC
      token->TokenType.SDT.offset.preindex = 1;
      token->TokenType.SDT.offset.is_imm = 1;
      token->TokenType.SDT.offset.OffsetType.expression = *memory_address - token->address;
      flip_endian(&exp);
      instructions[*memory_address / 4] = exp; // Store value in memory
      *memory_address += 4;
    }
  } else {
    // address in the form [Rn] / [Rn, <offset>] / [Rn], <offset>
    char* bracket_end = strchr(arg, ']');
    if (bracket_end) {
      // address in the form [Rn], <offset>; arg = [Rn]
      token->TokenType.SDT.offset.preindex = 0;
      memcpy(arg, &arg[1], bracket_end - &arg[1]);
      token->TokenType.SDT.rn = string_to_reg_address(arg);
    } else {
      // address in the form [Rn, <offset>]; arg = [Rn
      token->TokenType.SDT.offset.preindex = 1; 
    }

    arg = strtok(NULL, ",");
    if (!arg) {
      // no offset, address is [Rn]
      offset->array[0] = "#0";
      offset->size++;
    } else {
      bracket_end = strchr(arg, ']');
      if (bracket_end) {

        // Store #expression into offset array
        // address in the form [Rn, <#expression>], arg = <#expression>]
        memcpy(offset->array[0], arg, bracket_end - arg);
        offset->size++;
      } else {
        arg = strtok(NULL, ","); // arg = {+/-}Rm or <#expression>
        
        // Set up bit
        if (arg[0] == '+') {
          token->TokenType.SDT.offset.OffsetType.ShiftedReg.up_bit = 1;
        } else {
          token->TokenType.SDT.offset.OffsetType.ShiftedReg.up_bit = 0;
        }

        // Store Rm or <#expression> into offset array
        memcpy(offset->array[0], &arg[1], strlen(arg) - 1);
        offset++;

        // Store shifttype and shift arguments into offset array
        arg = strtok(NULL, ",");
        if (arg) {
          bracket_end = strchr(arg, ']');
          char* space = strchr(arg, ' ');
          memcpy(offset->array[1], arg, space - arg);
          if (bracket_end) {
            // arg = <shifttype> <shift>]
            memcpy(offset->array[2], &space[1], bracket_end - &space[1]);
          } else {
            // arg = <shifttype> <shift>
            memcpy(offset->array[2], &space[1], strlen(&space[1]));
          }
          offset->size += 2;
        }
      }
    }
    parse_offset_data_transfer(offset, token);
  }
  free(arg);
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
  char *opcode = strtok(line, " ");
  char *args = strtok(NULL, " ");
 
  if (!args) {
    return 0;
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
      tokenise_dataprocessing(line, token); break;
    case MULTIPLY:
      tokenise_multiply(line, token); break;
    case SDT:
      tokenise_datatransfer(line, token, memory_address, instructions); break;
    default:
      tokenise_branch(line, token, symboltable); break;
  }
  return 1;
}

