#ifndef ASSEMBLER_SYMBOLTABLE_H
#define ASSEMBLER_SYMBOLTABLE_H
#include <stdint.h>

typedef uint32_t Address;

/* Struct to represent an entry in the symbol table */
typedef struct {
    char* symbol;
    Address address;
} Symbol;

/* Struct to represent the symbol table container */
typedef struct{
    int size;
    Symbol* table;
} SymbolTable;


SymbolTable create_symboltable(char* source);

Address lookup_symbol(SymbolTable* table, char* symbol);

void free_symboltable(SymbolTable *table);


#endif 