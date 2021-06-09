#include <stdio.h>
#include <stdlib.h>
#include "assemble.h"
#include "datatypes.h"
#include "datap_assembler.h"
#include "functions.h"

void assemble(int num_lines) {
  // Converted instructions stored here
  Instruction *instructions = calloc(num_lines, sizeof(Instruction));
  
  // TODO: create symbol table (symboltable)
  // TODO: read mnemonic and operands (parser)

  int current_line = 0;
  
  while(current_line < num_lines) {
    Instruction *instr = malloc(sizeof(Instruction));
    Operation op; // TODO: get operation
    
    DataProcessingInstruction *datap_instr;

    switch(op) {
      case AND:
      case EOR:
      case SUB:
      case RSB:
      case ADD:
      case ORR:
      case MOV:
      case TST:
      case TEQ:
      case CMP: 
        datap_instr = malloc(sizeof(DataProcessingInstruction));
        // TODO: Populate datap_instr with the correct fields
        build_datap_instr(datap_instr, instr);
        break;
      case MUL:
      case MLA:
      case LDR:
      case STR:
      case BEQ:
      case BNE:
      case BLT:
      case BGT:
      case BLE:
      case B:
      case LSL:
      case ANDEQ:
      default:
        break;
    }

    instructions[current_line] = *instr;

  }
  
  // Writes instruction array to binary file
  write_to_file(instructions, num_lines);
}
