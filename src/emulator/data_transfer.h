#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include <stdio.h>
#include <stdint.h>
#include "functions.h"
#include "datatypes.h"
#include "shifter.h"
#define PC_REG (15)

void single_data_transfer(Instruction instr, struct Registers *registers, Byte* memory); 

#endif

