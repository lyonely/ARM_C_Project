#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "parser.h"
#include "symboltable.h"
#include "functions.h"

// TODO
void tokenise_operand2(char* op2, Token *token) {
}

void tokenise_dataprocessing(char* str, Token *token) {
  char* args = strcpy(malloc(strlen(str) + 1), str);
  char* operand;
  operand = strtok(args, ",");
  switch(token->opcode) {
    case TST:
    case TEQ:
    case CMP:
      token->instr_token.datap_token.rn = string_to_reg_address(operand);
      tokenise_operand2(strtok(NULL, ","), token);
      break;
    case MOV:
      token->instr_token.datap_token.rd = string_to_reg_address(operand);
      tokenise_operand2(strtok(NULL, ","), token);
      break;
    default:
      token->instr_token.datap_token.rd = string_to_reg_address(operand);
      operand = strtok(NULL, ",");
      token->instr_token.datap_token.rn = string_to_reg_address(operand);
      tokenise_operand2(strtok(NULL, ","), token);
      break;
  }
  free(args);
}

// TODO
void tokenise_datatransfer(char* args, Token *token) {
}

// TODO
void tokenise_multiply(char* args, Token *token) {
}

void tokenise_branch(char* args, Token *token, SymbolTable *symboltable) {
  if (strchr(args, ',')) {
    perror("Wrong number of args provided in tokenise_branch");
    exit(EXIT_FAILURE);
  }
  
  if (args[0] == '#') {
    size_t arg_len = strlen(args);
    char addr[arg_len];
    memcpy(addr, &args[1], arg_len);
    token->instr_token.branch_token.target_address = (Address) parse_immediate_value(addr);
  } else {
    token->instr_token.branch_token.target_address = lookup_symbol(symboltable, args); 
  }
}

// TODO
void tokenise_special(char* args, Token *token) {
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
      case BRANCH:
        tokenise_branch(line, token, symboltable);
      case SPECIAL:
        tokenise_special(line, token);
      default:
        perror("instruction type does not exist in tokenise");
        exit(EXIT_FAILURE);
    }
    tokens[token_pos] = token;
    current_line++;
    token_pos++;
  }
  return tokens;
}

