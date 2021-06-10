#include <stdio.h>
#include <stdlib.h>
#include "assemble.h"
#include "datatypes.h"
#include "datap_assembler.h"
#include "multiply_assembler.h"
#include "branch_assembler.h"
#include "sdt_assembler.h"
#include "functions.h"

void assemble(char** assembly_code, int num_lines) {
  // Converted instructions stored here
  Instruction *instructions = calloc(num_lines, sizeof(Instruction));
  
  // TODO: create symbol table (symboltable)
  // TODO: read mnemonic and operands (parser)

  int current_line = 0;
  
  while(current_line < num_lines) {
    Instruction *instr = malloc(sizeof(Instruction));
    Operation op; // TODO: get operation
    
    DataProcessingInstruction *datap_instr;
    MultiplyInstruction *mul_instr;
    BranchInstruction *branch_instr;
    DataTransferInstruction *sdt_instr;

    switch(op) {
      // Data Processing
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
      case LSL:
      case ANDEQ:
        datap_instr = malloc(sizeof(DataProcessingInstruction));
        // TODO: Populate datap_instr with the correct fields
        build_datap_instr(datap_instr, instr);
        free(datap_instr);
        break;

      // Multiply
      case MUL:
      case MLA:
        mul_instr = malloc(sizeof(MultiplyInstruction));
        // TODO: Populate mul_instr with correct fields
        build_multiply_instr(mul_instr, instr);
        free(mul_instr);
        break;
      
      // Single Data Transfer
      case LDR:
      case STR:
        sdt_instr = malloc(sizeof(DataTransferInstruction));
        build_sdt_instr(sdt_instr, instr);
        free(sdt_instr);
        break;

      // Branch
      case BEQ:
      case BNE:
      case BGE:
      case BLT:
      case BGT:
      case BLE:
      case B:
        branch_instr = malloc(sizeof(BranchInstruction));
        // build_branch_instr(branch_instr, instr, curr_addr, symboltable);
        free(branch_instr);
        break;

      default:
        break;
    }

    instructions[current_line] = *instr;
    current_line++;
  }
  
  // Writes instruction array to binary file
  write_to_file(instructions, num_lines);
  free(instructions);
}
