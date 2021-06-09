#ifndef SHIFTER_H
#define SHIFTER_H

#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "datatypes.h"

// Returns carry-out bit from shift result
int carry_output(int rm_value, int shift_type, int shift_value);

// Returns result of shift on register value (does not modify CPSR reg)
int shift_result(int rm_value, int shift_type, int shift_value); 

// shift_type (2 bits), shift_value (5 bits), rm_value (32 bits)
int shift(int rm_value, int shift_type, int shift_value, int set_cpsr, Register *cpsr);

// returns value of operand2 from shifted register
int operand2_shifted_reg(int op2, struct Registers* registers, int set_cpsr);

#endif

