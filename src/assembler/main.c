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
    perror("Error opening file in main\n");
    exit(EXIT_FAILURE);
  }
  
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp) + 1;
  rewind(fp);  
  
  StringArray *source = malloc(sizeof(StringArray));
  source->array = calloc(size, sizeof(char*));
  if (!source->array) {
    perror("Error allocating memory for source->array in main\n");
    exit(EXIT_FAILURE);
  }

  source->size = 0;
  char *line = malloc(MAX_CHARS_PER_LINE + 1);

  while (fgets(line, MAX_CHARS_PER_LINE + 1, fp)) {
    source->array[source->size] = malloc(MAX_CHARS_PER_LINE + 1);
    if (!source->array[source->size]) {
      perror("Error allocating memory for source->array[size] in main\n");
      exit(EXIT_FAILURE);
    }
    // Remove newline character
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    strcpy(source->array[source->size], line);
    source->size++;
  }

  fclose(fp);
  free(line);
   
  assemble(source, filename);

  free(source);
  return 0;
}

