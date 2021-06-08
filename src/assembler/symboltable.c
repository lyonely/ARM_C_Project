#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "symboltable.h"

SymbolTable create_symboltable(char* source){
    if(source == NULL){
        perror("SourceFile is NULL, create_symboltable failed");
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for a symbol table */
    SymbolTable *table = malloc(sizeof(SymbolTable));
    if(table == NULL){
        perror("Memory couldn't be allocated for symbol table. Create_symboltable failed.");
        exit(EXIT_FAILURE);
    }
    
    table -> size = 0;



}

Address lookup_symbol(SymbolTable* table, char* symbol) {
        if(table == NULL){
            perror("SymbolTable is NULL, lookup_symbol failed");
            exit(EXIT_FAILURE);
        }

        int size = table -> size;
        for ( int i = 0; i < size; i++) {
            if(!strcmp(table -> table[i].symbol, symbol)){
                return table -> table[i].address;
            }
        }

        perror("Label not found in symbol table. Lookup_symbol failed");
        exit(EXIT_FAILURE);
}


/* Frees a symbol table, including memory used for every entry */
void free_symboltable(SymbolTable *table){
    if(table == NULL){
        perror("SymbolTable is NULL, free_symboltable failed");
        exit(EXIT_FAILURE);
    }

    int size = table -> size;
    for(int i = 0; i < size; i ++){
        free(table -> table[i].symbol);
    }

    free(table);
}




