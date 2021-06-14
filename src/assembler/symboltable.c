#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "symboltable.h"
#include "datatypes.h"
#include "functions.h"

/* Helper function to check if given line is a label */
int is_label(char* line);

/* Creates a symbol table given the source code as an array of strings */
SymbolTable* create_symboltable(StringArray* source){
    if(source->array == NULL){
        perror("SourceFile is NULL, create_symboltable failed");
        exit(EXIT_FAILURE);
    }

    int source_num_lines = source->size; 

    /* Allocates memory for a symbol table */
    /* Assumes every 2 lines is a label */
    SymbolTable *symbol_table = malloc(sizeof(SymbolTable));
    if(symbol_table == NULL){
        perror("Memory couldn't be allocated for symbol table. Create_symboltable failed.");
        exit(EXIT_FAILURE);
    }

    /* Allocates enough memory for all symbols */
    /* Initially assumes every other line is a label */
    symbol_table->table = malloc((MAX_CHARS_PER_LINE + 1) * source_num_lines / 2);
    if(symbol_table->table == NULL){
        perror("Memory couldn't be allocated for symbol table. Create_symboltable failed.");
        exit(EXIT_FAILURE);
    }
    
    symbol_table->size = 0;

    Address address = 0;
    int current_table_size = 0;
    char* line;

    // Allocate memory for new array (StringArray) to store only instructions
    char ** array_no_labels = malloc(source_num_lines);
    if(array_no_labels == NULL){
        perror("Memory couldn't be allocated for array (source without labels). Create_symboltable failed.");
        exit(EXIT_FAILURE);
    }
    int no_labels_size = 0;
    int skip = 0;

    // stores symbols (labels) in the symbol table;
    // adds line to source_no_labels if it isnt a label
    for(int i = 0; i < source_num_lines; i ++){
       line = source->array[i];

       if(is_label(line)){
            // line is a label, remove the last ':' character
            line[strlen(line) - 1] = '\0';
            skip = 1;
        } else {
            // if line is not address corresponding to a label, add to source_without_labels
            if(skip == 0){
                array_no_labels[no_labels_size] = line;
                no_labels_size ++;
            } else {
                skip = 0;
            }
            
            // increment address;
            address += sizeof(line);
        }

        Symbol new_symbol;
        new_symbol.address = address;
        new_symbol.symbol = line;

        symbol_table -> table[current_table_size] = new_symbol;
        current_table_size++;
   }

    /* Reassign source array to array without labels, only instructions */
    delete_string_array(source);
    source -> array = array_no_labels;
    source -> size = no_labels_size;

    /* Reallocate memory to match actual number of symbols stored */
    symbol_table -> table = realloc(symbol_table -> table, current_table_size * sizeof(Symbol));
    symbol_table -> size = current_table_size;
    return symbol_table;
}

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

