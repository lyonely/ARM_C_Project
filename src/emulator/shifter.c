#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "datatypes.h"

int carry_output(int rm_value, int shift_type, int shift_value) {
	int carry = 0;
	if (shift_value > 0) {
		switch(shift_type) {
			// Logical left
			case 0: carry = (rm_value << (shift_value - 1)) >> 31; break;
			// Logical right and arithmetic right
			case 1: 
			case 2: carry = (rm_value >> (shift_value - 1)) & 0x1; break;
			// Rotate right
			case 3: carry = (rm_value >> (shift_value - 1)) & 0x1; break;
		}
	}	
	return carry;
}

int shift_result(int rm_value, int shift_type, int shift_value) {
	int result;
	switch(shift_type) {
		// Logical left
		case 0: result = rm_value << shift_value; break; 
		// Logical right
		case 1: result = rm_value >> shift_value; break;
		// Arithmetic right
		case 2: result = rm_value >> shift_value; break;
		// Rotate right
		case 3: result = rotate_right(rm_value, shift_value); break;
	}
	return result;
}

// shift_type (2 bits), shift_value (5 bits), rm_value (32 bits)
int shift(int rm_value, int shift_type, int shift_value, int set_cpsr, Register *cpsr) {
	int result;
	int shifter_carry_output;

	switch(shift_type) {
		case 0: 
			// logical left
			result = shift_result(rm_value, shift_type, shift_value);
			shifter_carry_output = carry_output(rm_value, shift_type, shift_value);
			
			if(set_cpsr) {
				set_c(cpsr, shifter_carry_output);	
			}
			break;

		case 1: 
			// logical right
			result = shift_result(rm_value, shift_type, shift_value);

			shifter_carry_output = carry_output(rm_value, shift_type, shift_value);

			if(set_cpsr){
				set_c(cpsr, shifter_carry_output);
			}
			break;

		case 2:
			// first, logical right shift
			result = shift_result(rm_value, shift_type, shift_value);

			shifter_carry_output = carry_output(rm_value, shift_type, shift_value);

			if(set_cpsr){
				set_c(cpsr, shifter_carry_output);
			}

			int sign_bit = rm_value < 0;

			// set high bits of operand2 to sign bit
			if(sign_bit){
				// if sign bit is 1, set higher bits to 1
				int temp = ((2 ^ shift_value) - 1 ) << (32 - shift_value);
				result = temp | result;
			}
			break;

		case 3: 
			// get carry output
			shifter_carry_output = carry_output(rm_value, shift_type, shift_value);
			
			// rotate right
			result = shift_result(rm_value, shift_type, shift_value);

			break;
	}
	return result;
}


int operand2_shifted_reg(int op2, struct Registers* registers, int set_cpsr){
  
  // get pointer to register rm, get shift type & get shift
  int rm = op2 & 0xf;
	int rm_value = registers -> general_regs[rm];
	int op2_shift = op2 >> 4;
	int shift_type = (op2_shift >> 1) & 0x2;

  // extract bit 4: indicates if shift value is a 5-bit uint or a register
  int bit_four = op2_shift & 1;
	
	int shift_value = 0;

    if(bit_four){
    // case when bit_four is 1 is optional
		// Hence blank code body; will revisit if there is sufficient time
    } else {
    // bit_four is 0
		// Shift amount is directly given in bits 7 - 11 of operand2
		// which is bit 4 - 8 of shift variable
		  shift_value = op2_shift >> 3;
    }
    
    Register *cpsr = &(registers -> cpsr);
    // shift function executes a shift operation, returns value of op2
    return shift(rm_value, shift_type, shift_value, set_cpsr, cpsr);
} 

