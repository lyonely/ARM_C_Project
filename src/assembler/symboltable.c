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

    /* Allocates memory for a symbol table */
    SymbolTable *table = calloc(strlen(source)/2, sizeof(Symbol));
    if(table == NULL){
        perror("Memory couldn't be allocated for symbol table. Create_symboltable failed.");
        exit(EXIT_FAILURE);
    }
    
    table -> size = 0;

    Address address = 0;
    int tableIndex = 0;

    char* currentLine;

}

/* Checks if given line is preceded by a label */
int is_label(char* line){
    char* colon = strchr(line, ':');
    if(colon != NULL){
        return 1;
    } 

    return 0;
}

/* Returns next line of a source code. */
/* Returns NULL if no more instructions to process */
char* next_line(char* source){
    char* line = strsep(&source, '\n');

    if(line == NULL) {
        returns NULL;
    } 

    /* removes whitespace from line */
    while(isspace(*line)) {
        line ++;
    }

    /* If line is just an empty string after whitespace removal,
       continue reading */
    while(strcmp(line, "") == 0) {

    }

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




