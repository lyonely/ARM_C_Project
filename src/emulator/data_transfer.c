#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "datatypes.h"
#include "shifter.h"

// PC is register 15, total 17 registers (0 - 16)
#define PC_Reg 15

void single_data_transfer(Instruction instr, struct Registers* registers){
	// Executes a single data transfer instruction which loads data from memory 
  	// to register OR stores data from registers to memory
	printf("ok");
	// Pointer to baseRegister
  	Register *baseRegister = 0;

    // initialize baseRegister pointer
    if(rn(instr) == PC_Reg){
		baseRegister = &(registers -> pc);
	} else {
		baseRegister = &(registers -> general_regs[rn(instr)]);
  	} 


    uint32_t offset = 0;

	if(is_immediate(instr)){
		// offset interpreted as a shifted register
	  
	    // Assume single data transfer does not need to set cpsr
	    int set_cpsr = 0;
	    offset = operand2_shiftedReg(operand2(instr), registers, set_cpsr);

	} else {
        // offset is an unsigned 12 bit immediate offset
        offset = operand2(instr);
	}

    if(is_pre_indexing(instr)){
      // add or subtract offset from base register before data transfer
	  // pre indexing does not change the value of the base register
		
		// pointer to an int
		uint32_t* address;

 		if (is_up(instr)){
        	// add offset to base register (result is a memory address)
        	address = (uint32_t*) (uintptr_t) (*baseRegister + offset);
      	} else {
        	// subtract offset from base register
        	address = (uint32_t*) (uintptr_t) (*baseRegister - offset);
      	}

      	if (is_load(instr)){
        	// word is loaded from memory
        	registers -> general_regs[rd(instr)] = *address;
     		return;
    	} else {
        	// word stored into memory
        	*address = registers -> general_regs[rd(instr)];
        	return;
      	}

    } else {
		// post-indexing
		if (is_up(instr)){
			*baseRegister = *baseRegister + offset;
		} else {
			*baseRegister = *baseRegister - offset;
		}

		if (is_load(instr)){
        	// word is loaded from memory
        	registers -> general_regs[rd(instr)] = *baseRegister;
     		return;
    	} else {
        	// word stored into memory
        	*baseRegister = registers -> general_regs[rd(instr)];
        	return;
      	}

	}

} 


int main(void){

	printf(" first");
	// make Regsiters struct
	struct Registers registers;
	registers.pc = (Register) 15;
	registers.cpsr = (Register) 0;
	registers.general_regs[0] = (Register) 1;
	registers.general_regs[1] = (Register) 3;

	for(int i = 2; i < 13; i++){
		registers.general_regs[i] = (Register) 0;
	}

	struct Registers *registers_ptr = &registers;

	
	// 0000 01 0 (I) 1 (P) 1 (U) 00 0 (L) 0001 (Rn, base) 0000 (Rd, source) 0000 0000 0001 (offset)
	// 0000 0101 1000 0001 0000 0000 0000 0001
	Instruction instr = (unsigned int) 92340225;

	printf(" OK");
	single_data_transfer(instr, registers_ptr);


	return 0;
}
