#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "parser.h"
#include "symboltable.h"
#include "functions.h"

void tokenise_dataprocessing(char* str, Token *token) {
  char* arg = strcpy(malloc(strlen(str) + 1), str);
  StringArray* operand2 = malloc(sizeof(StringArray));
  operand2->size = 0;

  arg = strtok(str, ","); 
  token->DataP.rd = string_to_reg_address(arg);
  
  if (token->num_args == 3) { 
    // ADD, ANDEQ, EOR, SUB, RSB, ADD, ORR
    arg = strtok(NULL, ",");
    token->DataP.rd = string_to_reg_address(arg);
    token->DataP.rn = string_to_reg_address(arg);
  } else {
    if (token->opcode == MOV || token->opcode == LSL) { 
      // MOV, LSL
      token->DataP.rd = string_to_reg_address(arg);
    } else {
      // TST, TEQ, CMP
      token->DataP.rn = string_to_reg_address(arg);
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

void tokenise_datatransfer(char* str, Token *token) {
  char* arg = strcpy(malloc(strlen(str) + 1), str);
  StringArray* offset = malloc(sizeof(StringArray));
  offset->size = 0;

  arg = strtok(str, ","); 
  token->SDT.rd = string_to_reg_address(arg); // rd
 
  arg = strtok(NULL, ",");
  char* constant_offset = strchr(arg, '=');
  if (constant_offset) {
    // address in the form <=expression>
    uint32_t exp = parse_immediate_value(&arg[1]);
    if (exp <= 0xFF) {
      // convert to mov instruction
      token->DataP.rd = token->SDT.rd;
      token->opcode = MOV;
      token->num_args = 2;
    } else {
      // TODO: offset thing
      token->SDT.rn = 15; // rn = PC 
    }
  } else {
    // address in the form [Rn] / [Rn, <offset>] / [Rn], <offset>
    char* bracket_end = strchr(arg, ']');
    if (bracket_end) {
      // address in the form [Rn], <offset>; arg = [Rn]
      token->SDT.offset.ShiftedReg.preindex = 0;
      memcpy(arg, &arg[1], bracket_end - &arg[1]);
      token->SDT.rn = string_to_reg_address(arg);
    } else {
      // address in the form [Rn, <offset>]; arg = [Rn
      token->SDT.offset.ShiftedReg.preindex = 1; 
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
          token->SDT.offset.ShiftedReg.up_bit = 1;
        } else {
          token->SDT.offset.ShiftedReg.up_bit = 0;
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
  token->Multiply.rd = string_to_reg_address(operand);
  operand = strtok(NULL, ",");
  token->Multiply.rm = string_to_reg_address(operand);
  operand = strtok(NULL, ",");
  token->Multiply.rs = string_to_reg_address(operand);

  if (token->opcode == MLA) {
    operand = strtok(NULL, ",");
    token->Multiply.rn = string_to_reg_address(operand);
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
    token->Branch.target_address = (Address) parse_immediate_value(addr);
  } else {
    token->Branch.target_address = lookup_symbol(symboltable, str); 
  }
}

// Tokenises assembly code and parses into Instruction
int tokenise(char *line, Address address, SymbolTable *symboltable, Token *token) {
  char *opcode = strtok(line, " ");
  char *args = strtok(NULL, " ");
 
  // Label reached, skip line
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
      tokenise_dataprocessing(line, token);
    case MULTIPLY:
      tokenise_multiply(line, token);
    case SDT:
      tokenise_datatransfer(line, token);
    default:
      tokenise_branch(line, token, symboltable);
  }
  return 1;
}

