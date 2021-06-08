#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS_PER_LINE (511)

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

  /* TEXT FILE HAS BEEN READ INTO ASSEMBLY_CODE ARRAY */

  free(assembly_code);
  return 0;
}

