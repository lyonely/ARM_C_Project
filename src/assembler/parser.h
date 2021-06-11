#ifndef PARSER_H
#define PARSER_H
#include "datatypes.h"


//Returns operation for a given string
Operation string_to_operation(char *str);


//Returns the condition for a given string
Condition string_to_condition(char *str);


//Returns opcode for a given operation 
Opcode mnemonic_to_opcode(Operation operation);


//Returns register address (number) given by the string
RegAddress string_to_reg_address(char *str);


//Returns the shift for a given shift string
Shift string_to_shift(char *str);

//Parses the shift and adds it to the given instruction
void parse_shift_data_processing(StringArray *tokens, DataProcessingInstruction *instruction);

void parse_shift_data_transfer(StringArray *tokens, DataTransferInstruction *instruction);

//Parses operand and adds it to the given instruction
void parse_operand_data_processing(StringArray *tokens, DataProcessingInstruction *instruction);

void parse_operand_data_transfer(StringArray *tokens, DataTransferInstruction *instruction);

//Returns the immediate value given by the string
Word parse_immediate_value(char *str);

#endif

