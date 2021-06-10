#include <stdio.h>
#include <stdlib.h>
#include "assemble.h"
#include "datatypes.h"

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  
  if (fp == NULL) {
    perror("Error opening file.\n");
    exit(EXIT_FAILURE);
  }
  
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  rewind(fp);  

  char **assembly_code = calloc(size, MAX_CHARS_PER_LINE + 1); // +1 for \0 char
  if (assembly_code == NULL) {
    perror("Error allocating memory.\n");
    exit(EXIT_FAILURE);
  }

  size_t result = fread(assembly_code, MAX_CHARS_PER_LINE + 1, size, fp);
  if (result != size) {
    perror("Error reading file.\n");
    exit(EXIT_FAILURE);
  }
  
  fclose(fp);
  
  StringArray *source = malloc(sizeof(StringArray));

  source->array = assembly_code;
  source->size = size;
  /* TEXT FILE HAS BEEN READ INTO SOURCE STRINGARRAY */

  // assemble(source);

  free(assembly_code);
  free(source);
  return 0;
}

