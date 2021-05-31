#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "datatypes.h"
#include "shifter.h"

#define PC_Reg 15

void single_data_transfer(Instruction instr, struct Registers *registers, Byte* memory){
	// Executes a single data transfer instruction which loads data from memory 
	// to register OR stores data from registers to memory
	// baseRegister stores value in baseRegister
	uint32_t baseRegister = 0;
	
	// initialize baseRegister pointer
	if((rn(instr) == PC_Reg)){
		baseRegister = (registers -> pc);
	} else {
		baseRegister = (registers -> general_regs[rn(instr)]);
  	} 

	uint32_t offset = 0;
	
	if(is_immediate(instr)){
		// Offset interpreted as a shifted register
		// Assume single data transfer does not need to set cpsr
		int set_cpsr = 0;
		offset = (unsigned int) operand2_shiftedReg(operand2(instr), registers, set_cpsr);
	} else {
		// offset is an unsigned 12 bit immediate offset
		offset = operand2(instr);
	}

    if(is_pre_indexing(instr)){
      // add or subtract offset from base register before data transfer
	  // pre indexing does not change the value of the base register

		
 		if (is_up(instr)){
        	// add offset to base register (result is a memory address)
			baseRegister += offset;

      		} else {
        	// subtract offset from base register
        		baseRegister -= offset;
      		}

      	if (is_load(instr)){
		if (baseRegister >= MEMORY_CAPACITY) {
			printf("Error: Out of bounds memory access at address 0x%08x\n", baseRegister);
			return;
		}
			// word is loaded from memory
        	registers -> general_regs[rd(instr)] = *((uint32_t*) (memory + baseRegister));
     		return;
    	} else {
		if (baseRegister >= MEMORY_CAPACITY) {
			printf("Error: Out of bounds memory access at address 0x%08x\n", baseRegister);
			return;
		}
			// word stored into memory
        	*((uint32_t*)(memory + baseRegister)) = registers -> general_regs[rd(instr)]; 
        	return;
      	}

    } else {
		// post-indexing
	if (is_load(instr)){
		if (baseRegister >= MEMORY_CAPACITY) {
			printf("Error: Out of bounds memory access at address 0x%08x\n", baseRegister);
			return;
		}
			// word is loaded from memory into destination register
			registers -> general_regs[rd(instr)] = *((uint32_t*) (baseRegister + memory));
			
		} else {
		if (baseRegister >= MEMORY_CAPACITY) {
			printf("Error: Out of bounds memory access at address 0x%08x\n", baseRegister);
			return;
		}
			// word stored into memory
			*((uint32_t*) (baseRegister + memory)) = registers -> general_regs[rd(instr)];
			
		}

		// update value of base register (rn) using offset
		if (is_up(instr)){
			registers -> general_regs[rn(instr)] = baseRegister + offset;
		} else {
			registers -> general_regs[rn(instr)] = baseRegister - offset;
		}
	}
} 

