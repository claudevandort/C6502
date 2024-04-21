#include "6502.h"

/*
 * Basic Memory functions
 */

void initMemory(Memory* memory) {
  for(int i = 0; i < MEMORY_SIZE; i++) {
    memory->data[i] = 0x00;
  }
}

byte readByte(Memory* memory, word address) {
  return memory->data[address];
}

void writeByte(Memory* memory, word address, byte value) {
  memory->data[address] = value;
}

word readWord(Memory* memory, word address) {
  word low = memory->data[address];
  word high = memory->data[address + 1];
  return low | (high << 8);
}

void writeWord(Memory* memory, word address, word value) {
  memory->data[address] = value & 0xFF;
  memory->data[address + 1] = (value >> 8) & 0xFF;
}

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

byte CPUreadByte(const Memory *memory, const word address, uint *cycles) {
  (*cycles)--;
  return memory->data[address];
}

byte fetchByte(CPU *cpu, const Memory *memory, uint *cycles) {
  byte data = CPUreadByte(memory, cpu->PC, cycles);
  cpu->PC++;
  return data;
}

word fetchWord(CPU *cpu, const Memory *memory, uint *cycles) {
  byte low = fetchByte(cpu, memory, cycles);
  byte high = fetchByte(cpu, memory, cycles);
  word data = (high << 8) | low;
  return data;
}

instructionHandler instructions[256];

void initInstructions() {
  instructions[OP_LDA_IM] = LDA_IM;
  instructions[OP_LDA_ZP] = LDA_ZP;
  instructions[OP_LDA_ZPX] = LDA_ZPX;
  instructions[OP_LDA_ABS] = LDA_ABS;
  instructions[OP_LDA_ABSX] = LDA_ABSX;
  instructions[OP_LDA_ABSY] = LDA_ABSY;

  instructions[OP_LDX_IM] = LDX_IM;
}

void execute(CPU *cpu, Memory *memory, uint *cycles) {
  while(*cycles > 0) {
    byte opcode = fetchByte(cpu, memory, cycles);
    instructionHandler handler = instructions[opcode];
    handler(cpu, memory, cycles);
  }
}

/*
 * Opcodes implementation
 */

/*
 * LDA instruction
 */

// Set the processor status flags
void LDA_setPS(CPU *cpu) {
  cpu->PS = (cpu->A == 0) ? (cpu->PS | ZERO_FLAG) : (cpu->PS & ~ZERO_FLAG);
  cpu->PS = (cpu->A & 0x80) ? (cpu->PS | NEGATIVE_FLAG) : (cpu->PS & ~NEGATIVE_FLAG);
}

// LDA immediate addressing mode
// Assembly: LDA #$nn
// Opcode: 0xA9
// Bytes: 2
// Cycles: 2
void LDA_IM(CPU *cpu, Memory *memory, uint *cycles) {
  byte data = fetchByte(cpu, memory, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

// LDA zero page addressing mode
// Assembly: LDA $nn
// Opcode: 0xA5
// Bytes: 2
// Cycles: 3
void LDA_ZP(CPU *cpu, Memory *memory, uint *cycles) {
  byte address = fetchByte(cpu, memory, cycles);
  byte data = CPUreadByte(memory, address, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

// LDA zero page X-indexed addressing mode
// Assembly: LDA $nn,X
// Opcode: 0xB5
// Bytes: 2
// Cycles: 4
void LDA_ZPX(CPU *cpu, Memory *memory, uint *cycles) {
  byte address = fetchByte(cpu, memory, cycles);
  address = (address + cpu->X) % 256;
  (*cycles)--;
  byte data = CPUreadByte(memory, address, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

// LDA absolute addressing mode
// Assembly: LDA $nnnn
// Opcode: 0xAD
// Bytes: 3
// Cycles: 4
void LDA_ABS(CPU *cpu, Memory *memory, uint *cycles) {
  word address = fetchWord(cpu, memory, cycles);
  byte data = CPUreadByte(memory, address, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

// LDA absolute X-indexed addressing mode
// Assembly: LDA $nnnn,X
// Opcode: 0xBD
// Bytes: 3
// Cycles: 4-5
void LDA_ABSX(CPU *cpu, Memory *memory, uint *cycles) {
  word address = fetchWord(cpu, memory, cycles);
  address += cpu->X;
  byte high = (address >> 8) & 0xFF;
  if (high != 0x00)
    (*cycles)--;
  byte data = CPUreadByte(memory, address, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

// LDA absolute Y-indexed addressing mode
// Assembly: LDA $nnnn,Y
// Opcode: 0xB9
// Bytes: 3
// Cycles: 4-5
void LDA_ABSY(CPU *cpu, Memory *memory, uint *cycles) {
  word address = fetchWord(cpu, memory, cycles);
  address += cpu->Y;
  byte high = (address >> 8) & 0xFF;
  if (high != 0x00)
      (*cycles)--;
  byte data = CPUreadByte(memory, address, cycles);
  cpu->A = data;
  LDA_setPS(cpu);
}

/*
 * LDX instruction
 */

// Set the processor status flags
void LDX_setPS(CPU *cpu) {
  cpu->PS = (cpu->X == 0) ? (cpu->PS | ZERO_FLAG) : (cpu->PS & ~ZERO_FLAG);
  cpu->PS = (cpu->X & 0x80) ? (cpu->PS | NEGATIVE_FLAG) : (cpu->PS & ~NEGATIVE_FLAG);
}

// LDX immediate addressing mode
// Assembly: LDX #$nn
// Opcode: 0xA2
// Bytes: 2
// Cycles: 2
void LDX_IM(CPU *cpu, Memory *memory, uint *cycles) {
  byte data = fetchByte(cpu, memory, cycles);
  cpu->X = data;
  LDX_setPS(cpu);
}
