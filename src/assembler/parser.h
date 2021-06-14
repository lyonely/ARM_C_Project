#ifndef PARSER_H
#define PARSER_H

#include "datatypes.h"

// Parses data processing instruction into Token struct
void tokenise_dataprocessing(char *str, Token *token);

// Parses data transfer instruction into Token struct
void tokenise_datatransfer(char *str, Token *token);

// Parses multiply instruction into Token struct
void tokenise_multiply(char *str, Token *token);

// Parses branch instruction into Token struct
void tokenise_branch(char *str, Token *token, SymbolTable *symboltable);

// Parses assembly code into array of Tokens
int tokenise(char *line, Address address, SymbolTable *symboltable, Token *token);

#endif

