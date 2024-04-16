#include "cpu.h"
#include "memory.h"

void reset(CPU *cpu, Memory *memory) {
  cpu->PC = 0xFFFC;
  cpu->SP = 0x01;
  cpu->A = cpu->X = cpu->Y = 0;
  cpu->PS = UNUSED_FLAG | IRQ_DISABLE_FLAG;
  initMemory(memory);
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

void setProcessorStatusLDA(CPU *cpu) {
  cpu->PS = (cpu->A == 0) ? (cpu->PS | ZERO_FLAG) : (cpu->PS & ~ZERO_FLAG);
  cpu->PS = (cpu->A & 0x80) ? (cpu->PS | NEGATIVE_FLAG) : (cpu->PS & ~NEGATIVE_FLAG);
}

void execute(CPU *cpu, Memory *memory, uint *cycles) {
  while(*cycles > 0) {
    byte opcode = fetchByte(cpu, memory, cycles);
    switch(opcode) {
      case OP_LDA_IM: {
        byte data = fetchByte(cpu, memory, cycles); 
        cpu->A = data;
        setProcessorStatusLDA(cpu);
        break;
      }
      case OP_LDA_ZP: {
        byte address = fetchByte(cpu, memory, cycles);
        byte data = CPUreadByte(memory, address, cycles);
        cpu->A = data;
        setProcessorStatusLDA(cpu);
        break;
      }
    }
  }
}