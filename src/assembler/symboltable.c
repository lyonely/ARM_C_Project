#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symboltable.h"
#include "datatypes.h"

/*
char* next_line(char* source){
    char* line = strsep(&source, '\n');

    if(line == NULL) {
        return NULL;
    } 

    while(isspace(*line)) {
        line++;
    }

    if(strcmp(line, "") == 0) {
        return strsep(&source, '\n');
    }

    return line;
}
*/

SymbolTable create_symboltable(char** source){
    if(source == NULL){
        perror("SourceFile is NULL, create_symboltable failed");
        exit(EXIT_FAILURE);
    }

    /* Allocates memory for a symbol table */
    SymbolTable *symbol_table = malloc(sizeof(SymbolTable));
    if(symbol_table == NULL){
        perror("Memory couldn't be allocated for symbol table. Create_symboltable failed.");
        exit(EXIT_FAILURE);
    }

    /* Allocates enough memory for all symbols */
    /* Initially assumes there will be at most strlen(source) / 2 symbols */
    symbol_table->table = malloc(sizeof(strlen(source) / 2) * sizeof(Symbol));
    
    symbol_table->size = 0;

    Address address = 0;
    int current_table_size = 0;
    char* line;

    while(line = next_line(source) != NULL){
        if(is_label(line)){
            // line is a label, remove the last ':' character
            line[strlen(line) - 1] = '\0';
        } else {
            // line is not a label, increment address;
            address += sizeof(line);
        }

        Symbol new_symbol;
        new_symbol.address = address;
        new_symbol.symbol = line;

        symbol_table -> table[current_table_size] = new_symbol;
        current_table_size++;
    }

    /* Reallocate memory to match actual number of symbols stored */
    symbol_table -> table = realloc(symbol_table -> table, current_table_size * sizeof(Symbol));
    symbol_table -> size = current_table_size;
    return *symbol_table;
}

/* Checks if given line is a label */
int is_label(char* line){
    char* colon = strchr(line, ':');
    if(colon != NULL){
        return 1;
    } 
    return 0;
}

/* Returns address of a label using a symbol table */
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

