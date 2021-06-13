#include <stdio.h>
#include <stdlib.h>
#include "assemble.h"
#include "datatypes.h"
#include "datap_assembler.h"
#include "multiply_assembler.h"
#include "branch_assembler.h"
#include "sdt_assembler.h"
#include "functions.h"
#include "symboltable.h"
#include "parser.h"
#include "tokeniser.h"

void assemble(StringArray *source) {
  // Converted instructions stored here
  Instruction *instructions = calloc(source->size, sizeof(Instruction));

  // Symbol Table
  SymbolTable *symboltable = create_symboltable(source);
  
  // TODO: read mnemonic and operands (tokeniser)
  TokenArray* tokens = tokenise(source, symboltable);

  int current_token = 0;
  Token* token;

  while(current_token < tokens->size) {
    Instruction *instr = malloc(sizeof(Instruction));
    token = tokens->array[current_token];
    
    // TODO: set up instruction building for additional stored bytes
    switch(get_type(token->opcode)) {
      case DATA_P:
        // build_datap_instr(token, instr);
        break;
      case MULTIPLY:
        // build_multiply_instr(token, instr);
        break;
      case SDT:
        // build_sdt_instr(token, instr);
        break;
      default:
        // build_branch_instr(token, instr);
        break;
    }
    instructions[current_token] = *instr;
    current_token++;
  }
  
  // Writes instruction array to binary file
  write_to_file(instructions, source->size);
  free(instructions);
}
