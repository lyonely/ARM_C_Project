#ifndef ASSEMBLER_SYMBOLTABLE_H
#define ASSEMBLER_SYMBOLTABLE_H
#include <stdint.h>
#include "datatypes.h"

/* Creates a symbol table given the source code as a string */
SymbolTable create_symboltable(char** source);

/* Returns address of a label using a symbol table */
Address lookup_symbol(SymbolTable* table, char* symbol);

/* Frees memory of a symbol table, including memory used by its entries */
void free_symboltable(SymbolTable *table);


#endif

