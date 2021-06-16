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
    perror("SourceFile is NULL in create_symboltable\n");
    exit(EXIT_FAILURE);
  }

  int source_num_lines = source->size; 

  /* Allocates memory for a symbol table */
  /* Assumes every 2 lines is a label */
  SymbolTable *symbol_table = malloc(sizeof(SymbolTable));
  if(symbol_table == NULL){
    perror("Memory allocation for symboltable failed in create_symboltable\n");
    exit(EXIT_FAILURE);
  }

  /* Allocates enough memory for all symbols */
  /* Initially assumes every other line is a label */
  symbol_table->table = malloc((MAX_CHARS_PER_LINE + 1) * source_num_lines / 2);
  if(symbol_table->table == NULL){
    perror("Memory allocation for symboltable->table failed in create_symboltable\n");
    exit(EXIT_FAILURE);
  }
  
  symbol_table->size = 0;

  Address address = 0;
  int current_table_size = 0;
  char* line;

  // Allocate memory for new array (StringArray) to store only instructions
  char** array_no_labels = malloc(source_num_lines * sizeof(char*));
  if(array_no_labels == NULL) {
    perror("Memory allocation for array_no_labels failed in create_symboltable\n");
    exit(EXIT_FAILURE);
  }

  int no_labels_size = 0;
  Symbol symbol;
  int line_is_addr = 0; // tracks if next line is stored in symboltable

  // stores symbols (labels) in the symbol table;
  // adds line to source_no_labels if it isnt a label
  for(int i = 0; i < source_num_lines; i++){
    line = source->array[i];

    if(is_label(line)){
      // line is a label, remove the last ':' character
      line[strlen(line) - 1] = '\0';
      symbol.symbol = line;
      line_is_addr = 1;
    } else {
      if (line_is_addr) {
        symbol.address = address;
        symbol_table->table[current_table_size] = symbol;
        printf("Symbol %s added to symboltable index %d: symbol - %s, address - %x\n",
            symbol.symbol, current_table_size, 
            symbol_table->table[current_table_size].symbol, 
            symbol_table->table[current_table_size].address);
        current_table_size++;
        line_is_addr = 0;
      }

      array_no_labels[no_labels_size] = line;
      no_labels_size++;

      address += 4;
    }
  }
    
  /* Reassign source array to array without labels, only instructions */
  source->size = no_labels_size;
  memcpy(source->array, array_no_labels, no_labels_size * sizeof(char*));
    
  free(array_no_labels);
    
  symbol_table->size = current_table_size;
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
int get_symbol_address(SymbolTable* table, char* symbol, Address *address) {
  if(table == NULL){
    perror("SymbolTable is NULL, lookup_symbol failed");
    exit(EXIT_FAILURE);
  }

  int size = table->size;
  for (int i = 0; i < size; i++) {
    if(!strcmp(table->table[i].symbol, symbol)){
      *address = table->table[i].address;
      return 1;
    }
  }
  return 0;
}

/* Frees a symbol table, including memory used for every entry */
void free_symboltable(SymbolTable *symboltable){
  if (symboltable == NULL) {
    perror("SymbolTable is NULL, free_symboltable failed");
    exit(EXIT_FAILURE);
  }

  int size = symboltable -> size;
  for(int i = 0; i < size; i ++){
    free(symboltable->table[i].symbol);
  }
  free(symboltable->table);
  free(symboltable);
}

