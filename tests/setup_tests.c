#include "../src/6502.h"
#include "setup_tests.h"

void setup_immediate(CPU *cpu, Memory *memory, immediateParams *params) {
  reset(cpu, memory);

  cpu->PC = params->startingAddress;
  writeByte(memory, params->startingAddress, params->opcode);
  writeByte(memory, params->startingAddress + 0x01, params->testValue);
  params->cycles = 2;
}