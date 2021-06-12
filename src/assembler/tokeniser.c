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
    arg = strtok(NULL, ","); // rn
    token->DataP.rn = string_to_reg_address(arg);
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
}

// TODO tokenise address into [rn, rm, shift], set up bit and pre/postindex
void tokenise_datatransfer(char* str, Token *token) {
  char* arg = strcpy(malloc(strlen(str) + 1), str);
  StringArray* shift = malloc(sizeof(StringArray));
  shift->size = 0;

  arg = strtok(str, ","); 
  token->SDT.rd = string_to_reg_address(arg); // rd
 
  arg = strtok(NULL, ",");
  // TODO get rn, get rm, store shift into array

  parse_operand_data_transfer(shift, token);

  free(arg);
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

// Tokenises assembly code into array of Tokens
Token **tokenise(StringArray *code, SymbolTable *symboltable) {
  Token **tokens = calloc(code->size, sizeof(Token*));
  if (tokens == NULL) {
    perror("Failed memory allocation in tokenise");
    exit(EXIT_FAILURE);
  }

  // Loop to tokenise each line
  int current_line = 0;
  int token_pos = 0;
  while (current_line < code->size) {
    Token *token = malloc(sizeof(token));

    if (token == NULL) {
      perror("failed memory allocation in tokenise");
      exit(EXIT_FAILURE);
    }

    char *line = code->array[current_line];

    // Opcode
    char *opcode = strtok(line, " ");
    char *args = strtok(NULL, " ");
   
    // Label reached, skip line
    if (!args) {
      free(token);
      current_line++;
      continue;
    }

    token->opcode = string_to_operation(opcode);
    token->address = token_pos * sizeof(Instruction);
    token->num_args = get_num_args(token->opcode);

    // Condition for branch instructions
    if (opcode[0] == 'b' && opcode[1]) {
      char cond[2];
      memcpy(cond, &opcode[1], 2);
      token->cond = string_to_condition(cond);
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
    tokens[token_pos] = token;
    current_line++;
    token_pos++;
  }
  return tokens;
}

