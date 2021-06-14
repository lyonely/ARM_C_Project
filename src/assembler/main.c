#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assemble.h"
#include "datatypes.h"
#include "functions.h"

int main(int argc, char **argv) {
  FILE *fp = fopen(argv[1], "r");
  char *filename = argv[2];

  if (fp == NULL) {
    perror("Error opening file.\n");
    exit(EXIT_FAILURE);
  }
  
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp) + 1;
  rewind(fp);  
  
  StringArray source;
  source.array = malloc(sizeof(char*) * size);
  source.size = 0;
 
  char *line = malloc(MAX_CHARS_PER_LINE + 1);

  while (fgets(line, MAX_CHARS_PER_LINE + 1, fp)) {
    source.array[size] = malloc(MAX_CHARS_PER_LINE + 1);
    strcpy(source.array[size], line);
    source.size++;
  }

  fclose(fp);
  free(line);
  source.array = realloc(source.array, source.size);
   
  assemble(&source, filename);

  delete_string_array(&source);
  return 0;
}

