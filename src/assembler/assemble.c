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
  InstructionArray *instructions = malloc(sizeof(InstructionArray));
  instructions->array = calloc(source->size, sizeof(Instruction));
  instructions->size = 0;

  // Symbol Table
  SymbolTable *symboltable = create_symboltable(source);
 
  int address = 0;
  int current_line = 0;
  while (current_line < source->size) {
    Token *token = malloc(sizeof(Token));
    if (token == NULL) {
      perror("Failed memory allocation in assemble");
      exit(EXIT_FAILURE);
    }
    
    char *line = source->array[current_line];
    
    if (tokenise(line, address, symboltable, token)) {
      Instruction instr;
      switch(get_type(token->opcode)) {
        case DATA_P:
          // build_datap_instr
          break;
        case MULTIPLY:
          // build_multiply_instr
          break;
        case SDT:
          // build_sdt_instr
          break;
        default:
          // build_branch_instr
          break;
      }
      instructions->array[instructions->size] = instr;
      instructions->size++;
      address += sizeof(Instruction);
    }
    current_line++;
  }

  // TODO add on stored memory?

  // Writes instruction array to binary file
  write_to_file(instructions);
  free(instructions->array);
  free(instructions);
}
