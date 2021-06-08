#include <stdio.h>
#include "datatypes.h"

// Writes an array of instructions into a binary file
void write_to_file(Instruction *instructions, int num_instructions) {
  FILE *fp = fopen("../../out/program.bin", "wb");
  fwrite(instructions, sizeof(Instruction), num_instructions, fp);
  fclose(fp);
}

