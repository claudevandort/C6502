#include<stdio.h>
#include "cpu.h"
#include "memory.h"

int main(const int argc, const char* argv[]) {
  CPU cpu;
  Memory memory;

  reset(&cpu, &memory);
  writeByte(&memory, 0xBEBD, OP_LDA_IM);
  writeByte(&memory, 0xBEBE, 0x69);
  cpu.PC = 0xBEBD;
  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  printf("CPU A: %x\n", cpu.A);

	return 0;
}