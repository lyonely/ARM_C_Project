#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
// assuming memory capacity is 16 bytes (<< shiftsleft)
#define MEMORY_CAPACITY 1<<15

int main(int argc, char* argv[]) {
        // check that the binary file is the sole argument
        if (argc != 2){
                printf(“ERROR; expected format of arguments: %s filename\n, argv[0])
                exit(EXIT_FAILURE);
        }

        FILE *file;
        // rb is the mode for reading binary files
        File = fopen(argv[1], “rb”)

        // check that the file is valid
        if (file == NULL) {
                printf(“File is invalid\n”);
                exit(EXIT_FAILURE);
        }

        // allocate 1 byte to 16 elements, stores pointer in *memory
        Byte *memory = calloc(MEMORY_CAPACITY, 1);

        // read each binary and store in memory, return filesize
        readBinary(file, memory);

        // allocate memory for registers
        Registers *regs = calloc(1, sizeof(Registers));

        fclose(file);
    
        /* TODO: pipeline here (to be completed last)

        do{
          fetchData =
          Regs -> pc += 4;
          // Decode Instruction
          // Execute Instruction
        } while (!halt);

        // upon termination
        print_registers(regs);
        // print content of non-zero memory locations
        free(regs);
        exit(EXIT_SUCCESS);
        */

}
