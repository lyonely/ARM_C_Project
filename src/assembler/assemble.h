#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include "datatypes.h"

// Builds data processing instruction word from token
void build_datap_instr(Token *token, Instruction *i);

// Builds single data transfer instruction word from token
void build_sdt_instr(Token *token, Instruction *i);

// Builds multiply instruction word from token
void build_multiply_instr(Token *token, Instruction *i);

// Builds branch instruction word from token
void build_branch_instr(Token *token, Instruction *i);

// Two-pass assembly implementation
void assemble(StringArray *source);

#endif

