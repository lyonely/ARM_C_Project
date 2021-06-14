#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdint.h>
#include "datatypes.h"

// Writes an array of instructions into a binary file
void write_to_file(Instruction *instructions, int size, char *filename);

// Frees members of StringArray struct
void delete_string_array(StringArray *string_array);

// Converts instruction from big-endian to little-endian format
void flip_endian(Instruction *i);

// Gets instruction type of operation
Type get_type(Operation opcode);

// Gets expected number of arguments for operation
unsigned int get_num_args(Operation opcode);

// Returns opcode from string
Operation string_to_operation(char *str);

// Returns condition code from string
Condition string_to_condition(char *str);

// Returns register address from string
unsigned int string_to_reg_address(char *str);

// Returns ShiftType from string
ShiftType string_to_shift(char *str);

// Returns immediate value from string
uint32_t parse_immediate_value(char *str);

#endif

