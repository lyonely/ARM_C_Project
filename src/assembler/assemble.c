#include <stdlib.h>
#include <stdint.h>
#include "assemble.h"
#include "datatypes.h"
#include "functions.h"
#include "symboltable.h"
#include "parser.h"

#define NEGATIVE_SIGN_BIT ((-1)<<26)
#define BRANCH_OFFSET_MASK 0x3ffffff

void build_datap_instr(Token *token, Instruction *i) {
  // Sets instruction bits if opcode is not ANDEQ (all-zero)
  if (!(token->opcode == ANDEQ && token->DataP.rd == 0 
        && token->DataP.rn == 0 && token->DataP.operand2.reg_operand.rm == 0)) {
    
    // Set cond field
    *i &= 0x0fffffff;
	  *i |= (token->cond << 28);

    // Set opcode field
    *i |= (token->opcode << 21);
    
    // Set flags field
    switch(token->opcode) {
      case TST:
      case TEQ:
      case CMP: *i |= 0x00100000; break;
      default: *i &= 0xffefffff; break; }

    // Set rn and rd fields
    *i |= (token->DataP.rn << 16);
    *i |= (token->DataP.rd << 12);

    if (token->DataP.operand2.is_imm) {
      // Set imm field and immediate operand2
      *i |= 0x02000000; // I bit
      *i |= (token->DataP.operand2.imm_operand.rotation << 8); // Rotation
      *i |= token->DataP.operand2.imm_operand.immediate; // Immediate operand
    } else {
      // Set shifted register operand2 fields
      *i |= token->DataP.operand2.reg_operand.rm; // Rm
      *i |= (token->DataP.operand2.reg_operand.shift_type << 5); // Shift type
      if (token->DataP.operand2.reg_operand.shift.is_imm) {
        // Set shift by immediate fields
        *i |= (token->DataP.operand2.reg_operand.shift.immediate_shift << 7);
      } else {
        // Set shift by register fields
        *i |= 0x10; // Shift by register value field
        *i &= 0xFFFFFF7F;
        *i |= (token->DataP.operand2.reg_operand.shift.rs << 8); // Rs
      }
    }
  } else {
    *i &= 0x0;
  } 
}

void build_sdt_instr(Token *token, Instruction *i) {
  // Fixed bits
  *i &= 0xf79fffff;
  *i |= 0x04000000;
  
  // Set cond field
  *i &= 0x0fffffff;
  *i |= (token->cond << 28);

  // Set rn and rd fields
  *i |= (token->SDT.rn << 16);
  *i |= (token->SDT.rd << 12);

  // Set preindex field
  if (token->SDT.offset.preindex) {
    *i |= 0x01000000;
  }

  // Set upbit field
  if (token->SDT.offset.ShiftedReg.up_bit) {
    *i |= 0x00800000;
  }

  // Set load field
  if (token->opcode == LDR) {
    *i |= 0x00100000;
  }

  if (token->SDT.offset.is_imm) {
    // Set fields for register offset
    *i |= 0x02000000; // I bit
    *i |= (token->SDT.offset.ShiftedReg.shift_type << 5); // Shift type
    *i |= token->SDT.offset.ShiftedReg.rm; // Rm

    if (token->SDT.offset.ShiftedReg.shift.is_imm) {
      // Shift by immediate
      *i |= (token->SDT.offset.ShiftedReg.shift.immediate_shift << 7);
    } else {
      // Shift by register value
      *i |= 0x10;
      *i &= 0xFFFFFF7F;
      *i |= (token->SDT.offset.ShiftedReg.shift.rs << 8); // Rs
    }
  } else {
    // Set fields for immediate offset
    *i &= 0xfffff000;
    *i |= token->SDT.offset.expression;
  }
}

void build_multiply_instr(Token *token, Instruction *i) {
  // Set cond field
  *i &= 0x0fffffff;
	*i |= (token->cond << 28);

  // Set flags field
  *i &= 0xffefffff;

  // Set bits 7-4
  *i &= 0xffffff0f;
	*i |= 0x00000090;

  // Set rd, rs, rm fields
	*i |= (token->Multiply.rd << 16);
	*i |= (token->Multiply.rs << 8);
	*i |= token->Multiply.rm;

  // Set accumulate and rn fields for MLA
  if (token->opcode == MLA) {
		*i |= 0x00200000;
		*i |= (token->Multiply.rn << 12);
	} else {
		*i &= 0xffdfffff;
	}
}

void build_branch_instr(Token *token, Instruction *i) {
  // Set cond field
  *i &= 0x0fffffff;
	*i |= (token->cond << 27);
  
  // Set bits 27-24
  *i &= 0xf0ffffff;
	*i |= 0x0a000000;

  Address curr = token->address;
	curr += 8;

	int32_t offset = BRANCH_OFFSET_MASK & (token->Branch.target_address - curr);
	if (token->Branch.target_address < curr) {
		offset |= 0x2000000;
	} else {
		offset &= 0x0ffffff;
	}
	*i &= 0xff000000;
	*i |= (offset >> 2);
}

void assemble(StringArray *source) {
  // Instruction words stored here
  Instruction instructions[2 * source->size]; // Allocate enough memory for LDR instructions

  // First pass - symboltable, labels removed from source
  SymbolTable *symboltable = create_symboltable(source);

  // Second pass - tokenise and build instructions
  Address address = 0;
  Address next_memory_address = source->size;
  int current_line = 0;
  while (current_line < source->size) {
    Token token;
    
    char *line = source->array[current_line];
    
    if (tokenise(line, address, symboltable, &next_memory_address, instructions, &token)) {
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
    } else {
      perror("Error tokenising instruction in assemble");
      exit(EXIT_FAILURE);
    }
    current_line++;
  }

  // Writes instruction array + memory bytes to binary file
  write_to_file(instructions, next_memory_address / 4);
}

