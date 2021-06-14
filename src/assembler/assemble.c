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
  // Instruction words stored here
  Instruction instructions[2 * source->size];

  // First pass - symboltable
  SymbolTable *symboltable = create_symboltable(source);

  // Second pass - tokenise and build instructions
  Address address = 0;
  int current_line = 0;
  while (current_line < source->size) {
    Token token;
    
    char *line = source->array[current_line];
    
    if (tokenise(line, address, symboltable, &token)) {
      Instruction instr = 0;
      switch(get_type(token.opcode)) {
        case DATA_P:
          build_datap_instr(&token, &instr);
          break;
        case MULTIPLY:
          build_multiply_instr(&token, &instr);
          break;
        case SDT:
          build_sdt_instr(&token, &instr);
          break;
        default:
          build_branch_instr(&token, &instr);
          break;
      }
      flip_endian(&instr);
      instructions[address / 4] = instr;
      address += 4;
    }
    current_line++;
  }

  // TODO add on stored memory

  // Writes instruction array to binary file
  write_to_file(instructions, address / 4);
}

