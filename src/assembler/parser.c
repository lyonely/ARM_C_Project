#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Operation string_to_operation(char *str) {
  if (!strcmp(str, "add")) {
    return ADD;
  }
  if (!strcmp(str, "sub")) {
    return SUB;
  }
  if (!strcmp(str, "rsb")) {
    return RSB;
  }
  if (!strcmp(str, "and")) {
    return AND;
  }
  if (!strcmp(str, "eor")) {
    return EOR;
  }
  if (!strcmp(str, "orr")) {
    return ORR;
  }
  if (!strcmp(str, "mov")) {
    return MOV;
  }
  if (!strcmp(str, "tst")) {
    return TST;
  }
  if (!strcmp(str, "teq")) {
    return TEQ;
  }
  if (!strcmp(str, "cmp")) {
    return CMP;
  }
  if (!strcmp(str, "mul")) {
    return MUL;
  }
  if (!strcmp(str, "mla")) {
    return MLA;
  }
  if (!strcmp(str, "ldr")) {
    return LDR;
  }
  if (!strcmp(str, "str")) {
    return STR;
  }
  if (!strcmp(str, "beq")) {
    return BEQ;
  }
  if (!strcmp(str, "bne")) {
    return BNE;
  }
  if (!strcmp(str, "bge")) {
    return BGE;
  }
  if (!strcmp(str, "blt")) {
    return BLT;
  }
  if (!strcmp(str, "bgt")) {
    return BGT;
  }
  if (!strcmp(str, "ble")) {
    return BLE;
  }
  if (!strcmp(str, "b")) {
    return B;
  }
  if (!strcmp(str, "lsl")) {
    return LSL;
  }
  if (!strcmp(str, "andeq")) {
    return ANDEQ;
  }

  fprintf(stderr, "No such mnemonic found.\n");
  exit(EXIT_FAILURE);
}

Condition string_to_condition(char *str) {
  if (!strcmp(str, "eq")) {
    return EQ;
  }
  if (!strcmp(str, "ne")) {
    return NE;
  }
  if (!strcmp(str, "ge")) {
    return GE;
  }
  if (!strcmp(str, "lt")) {
    return LT;
  }
  if (!strcmp(str, "gt")) {
    return GT;
  }
  if (!strcmp(str, "le")) {
    return LE;
  }
  if (!strcmp(str, "al") || 0 == strlen(str)) {
    return AL;
  }
  fprintf(stderr, "No such condition found.\n");
  exit(EXIT_FAILURE);
}
