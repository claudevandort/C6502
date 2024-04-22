#ifndef SETUP_TESTS_H
#define SETUP_TESTS_H

typedef struct {
  word startingAddress;
  byte testValue;
  byte opcode;
  uint cycles;
} immediateParams;

void setup_immediate(CPU *cpu, Memory *memory, immediateParams *params);

#endif