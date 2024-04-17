#include "cpu.h"
#include "memory.h"

/*
 * Basic CPU functions
*/

void reset(CPU *cpu, Memory *memory) {
  cpu->PC = 0xFFFC;
  cpu->SP = 0x01;
  cpu->A = cpu->X = cpu->Y = 0;
  cpu->PS = UNUSED_FLAG | IRQ_DISABLE_FLAG;
  initMemory(memory);
  initInstructions();
}

byte CPUreadByte(Memory *memory, word address, uint *cycles) {
  (*cycles)--;
  return memory->data[address];
}

byte fetchByte(CPU *cpu, Memory *memory, uint *cycles) {
  byte data = CPUreadByte(memory, cpu->PC, cycles);
  cpu->PC++;
  return data;
}

/*
 * LDA instruction
*/

// Set the processor status flags
void LDA_setPS(CPU *cpu) {
  cpu->PS = (cpu->A == 0) ? (cpu->PS | ZERO_FLAG) : (cpu->PS & ~ZERO_FLAG);
  cpu->PS = (cpu->A & 0x80) ? (cpu->PS | NEGATIVE_FLAG) : (cpu->PS & ~NEGATIVE_FLAG);
}

// LDA immediate addressing mode
void LDA_IM(CPU *cpu, Memory *memory, uint *cycles) {
  byte data = fetchByte(cpu, memory, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

// LDA zero page addressing mode
void LDA_ZP(CPU *cpu, Memory *memory, uint *cycles) {
  byte address = fetchByte(cpu, memory, cycles);
  byte data = CPUreadByte(memory, address, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

// LDA zero page X addressing mode
void LDA_ZPX(CPU *cpu, Memory *memory, uint *cycles) {
  byte address = fetchByte(cpu, memory, cycles);
  address = (address + cpu->X) % 256;
  byte data = CPUreadByte(memory, address, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

instructionHandler instructions[256];

void initInstructions() {
  instructions[OP_LDA_IM] = LDA_IM;
  instructions[OP_LDA_ZP] = LDA_ZP;
  instructions[OP_LDA_ZPX] = LDA_ZPX;
}

void execute(CPU *cpu, Memory *memory, uint *cycles) {
  while(*cycles > 0) {
    byte opcode = fetchByte(cpu, memory, cycles);
    instructionHandler handler = instructions[opcode];
    if(handler) {
      handler(cpu, memory, cycles);
    }
  }
}
