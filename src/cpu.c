#include "cpu.h"
#include "memory.h"

void reset(CPU *cpu, Memory *memory) {
    cpu->PC = 0xFFFC;
    cpu->SP = 0x01;
    cpu->A = cpu->X = cpu->Y = 0;
    cpu->PS = UNUSED_FLAG | IRQ_DISABLE_FLAG;
    initMemory(memory);
}

byte fetchByte(CPU *cpu, Memory *memory, uint *cycles) {
    byte data = memory->data[cpu->PC];
    cpu->PC++;
    (*cycles)--;
    return data;
}

void execute(CPU *cpu, Memory *memory, uint *cycles) {
  while(*cycles > 0) {
    byte opcode = fetchByte(cpu, memory, cycles);
    switch(opcode) {
      case OP_LDA_IM:
        cpu->A = fetchByte(cpu, memory, cycles);
        cpu->PS = (cpu->A == 0) ? (cpu->PS | ZERO_FLAG) : (cpu->PS & ~ZERO_FLAG);
        cpu->PS = (cpu->A & 0x80) ? (cpu->PS | NEGATIVE_FLAG) : (cpu->PS & ~NEGATIVE_FLAG);
        break;
    }
  }
}