#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "parser.h"
#include "symboltable.h"
#include "functions.h"

// Tokenises data processing instruction into Token struct
void tokenise_dataprocessing(char* str, Token *token);

// Tokenises data transfer instruction into Token struct
void tokenise_datatransfer(char* str, Token *token);

// Tokenises multiply instruction into Token struct
void tokenise_multiply(char* str, Token *token);

// Tokenises branch instruction into Token struct
void tokenise_branch(char* str, Token *token, SymbolTable *symboltable);

// Tokenises assembly code into array of Tokens
TokenArray *tokenise(StringArray *code, SymbolTable *symboltable);

