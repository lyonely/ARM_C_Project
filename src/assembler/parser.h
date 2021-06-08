#ifndef PARSER_H
#define PARSER_H
#include "datatypes.h"


//Returns mnemonic for a given string
Mnemonic string_to_mnemonic(char *str);


//Returns the condition for a given string
Condition string_to_condition(char *str);


//Returns opcode for a given mnemonic
Opcode mnemonic_to_opcode(Mnemonic mnemonic);


//Returns register address (number) given by the string
Reg_address string_to_reg_address(char *str);


//Returns the shift for a given shift string
Shift string_to_shift(char *str);

//Parses the shift and adds it to the given instruction
void parse_shift(String_array *tokens, Instruction *instruction);


//Parses operand and adds it to the given instruction
void parse_operand(String_array *tokens, Instruction *instruction);


//Returns the immediate value given by the string
Word parse_immediate_value(char *str);

#endif
