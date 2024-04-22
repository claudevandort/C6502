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
  instructions[OP_LDX_ZP] = LDX_ZP;
  instructions[OP_LDX_ZPY] = LDX_ZPY;
  instructions[OP_LDX_ABS] = LDX_ABS;
  instructions[OP_LDX_ABSY] = LDX_ABSY;

  instructions[OP_LDY_IM] = LDY_IM;
  instructions[OP_LDY_ZP] = LDY_ZP;
  instructions[OP_LDY_ZPX] = LDY_ZPX;
  instructions[OP_LDY_ABS] = LDY_ABS;
  instructions[OP_LDY_ABSX] = LDY_ABSX;
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

void setPS(CPU *cpu, byte *target, byte flags) {
  if(flags & ZERO_FLAG) {
    cpu->PS = (*target == 0) ? (cpu->PS | ZERO_FLAG) : (cpu->PS & ~ZERO_FLAG);
  }
  if(flags & NEGATIVE_FLAG) {
    cpu->PS = (*target & 0x80) ? (cpu->PS | NEGATIVE_FLAG) : (cpu->PS & ~NEGATIVE_FLAG);
  }
}

/*
 * Addressing modes
 */

/*
 * Immediate addressing mode
 * Assenbly: OP #$nn
 * Bytes: 2
 * Cycles: 2
 */
void ADDR_IM(CPU *cpu, Memory *memory, byte *target, uint *cycles) {
  byte data = fetchByte(cpu, memory, cycles);
  *target = data;
  
  setPS(cpu, target, ZERO_FLAG | NEGATIVE_FLAG);
}

/*
 * Zero page addressing mode
 * Assembly: OP $nn
 * Bytes: 2
 * Cycles: 3
 */
void ADDR_ZP(CPU *cpu, Memory *memory, byte *target, uint *cycles) {
  byte address = fetchByte(cpu, memory, cycles);
  byte data = CPUreadByte(memory, address, cycles);
  *target = data;

  setPS(cpu, target, ZERO_FLAG | NEGATIVE_FLAG);
}

/*
 * Zero page X-indexed addressing mode
 * Assembly: OP $nn,X
 * Bytes: 2
 * Cycles: 4
 */
void ADDR_ZPX(CPU *cpu, Memory *memory, byte *target, uint *cycles) {
  byte address = fetchByte(cpu, memory, cycles);
  address = (address + cpu->X) % 256;
  (*cycles)--;
  byte data = CPUreadByte(memory, address, cycles);
  *target = data;

  setPS(cpu, target, ZERO_FLAG | NEGATIVE_FLAG);
}

/*
 * Zero page Y-indexed addressing mode
 * Assembly: OP $nn,Y
 * Bytes: 2
 * Cycles: 4
 */
void ADDR_ZPY(CPU *cpu, Memory *memory, byte *target, uint *cycles) {
  byte address = fetchByte(cpu, memory, cycles);
  address = (address + cpu->Y) % 256;
  (*cycles)--;
  byte data = CPUreadByte(memory, address, cycles);
  *target = data;

  setPS(cpu, target, ZERO_FLAG | NEGATIVE_FLAG);
}

/*
 * Absolute addressing mode
 * Assembly: OP $nnnn
 * Bytes: 3
 * Cycles: 4
 */
void ADDR_ABS(CPU *cpu, Memory *memory, byte *target, uint *cycles) {
  word address = fetchWord(cpu, memory, cycles);
  byte data = CPUreadByte(memory, address, cycles);
  *target = data;

  setPS(cpu, target, ZERO_FLAG | NEGATIVE_FLAG);
}

/*
 * Absolute X-indexed addressing mode
 * Assembly: OP $nnnn,X
 * Bytes: 3
 * Cycles: 4-5
 */
void ADDR_ABSX(CPU *cpu, Memory *memory, byte *target, uint *cycles) {
  word address = fetchWord(cpu, memory, cycles);
  address += cpu->X;
  byte high = (address >> 8) & 0xFF;
  if (high != 0x00)
    (*cycles)--;
  byte data = CPUreadByte(memory, address, cycles);
  *target = data;

  setPS(cpu, target, ZERO_FLAG | NEGATIVE_FLAG);
}

/*
 * Absolute Y-indexed addressing mode
 * Assembly: OP $nnnn,Y
 * Bytes: 3
 * Cycles: 4-5
 */
void ADDR_ABSY(CPU *cpu, Memory *memory, byte *target, uint *cycles) {
  word address = fetchWord(cpu, memory, cycles);
  address += cpu->Y;
  byte high = (address >> 8) & 0xFF;
  if (high != 0x00)
    (*cycles)--;
  byte data = CPUreadByte(memory, address, cycles);
  *target = data;

  setPS(cpu, target, ZERO_FLAG | NEGATIVE_FLAG);
}

/*
 * LDA instruction
 */

// LDA immediate addressing mode
// Assembly: LDA #$nn
// Opcode: 0xA9
void LDA_IM(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_IM(cpu, memory, &cpu->A, cycles);
}

// LDA zero page addressing mode
// Assembly: LDA $nn
// Opcode: 0xA5
void LDA_ZP(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ZP(cpu, memory, &cpu->A, cycles);
}

// LDA zero page X-indexed addressing mode
// Assembly: LDA $nn,X
// Opcode: 0xB5
void LDA_ZPX(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ZPX(cpu, memory, &cpu->A, cycles);
}

// LDA absolute addressing mode
// Assembly: LDA $nnnn
// Opcode: 0xAD
void LDA_ABS(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ABS(cpu, memory, &cpu->A, cycles);
}

// LDA absolute X-indexed addressing mode
// Assembly: LDA $nnnn,X
// Opcode: 0xBD
void LDA_ABSX(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ABSX(cpu, memory, &cpu->A, cycles);
}

// LDA absolute Y-indexed addressing mode
// Assembly: LDA $nnnn,Y
// Opcode: 0xB9
void LDA_ABSY(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ABSY(cpu, memory, &cpu->A, cycles);
}

/*
 * LDX instruction
 */

// LDX immediate addressing mode
// Assembly: LDX #$nn
// Opcode: 0xA2
void LDX_IM(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_IM(cpu, memory, &cpu->X, cycles);
}

// LDX zero page addressing mode
// Assembly: LDX $nn
// Opcode: 0xA6
void LDX_ZP(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ZP(cpu, memory, &cpu->X, cycles);
}

// LDX zero page Y-indexed addressing mode
// Assembly: LDX $nn,Y
// Opcode: 0xB6
void LDX_ZPY(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ZPY(cpu, memory, &cpu->X, cycles);
}

// LDX absolute addressing mode
// Assembly: LDX $nnnn
// Opcode: 0xAE
void LDX_ABS(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ABS(cpu, memory, &cpu->X, cycles);
}

// LDX absolute Y-indexed addressing mode
// Assembly: LDX $nnnn,Y
// Opcode: 0xBE
void LDX_ABSY(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ABSY(cpu, memory, &cpu->X, cycles);
}

/*
 * LDY instruction
 */

// LDY immediate addressing mode
// Assembly: LDY #$nn
// Opcode: 0xA0
void LDY_IM(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_IM(cpu, memory, &cpu->Y, cycles);
}

// LDY zero page addressing mode
// Assembly: LDY $nn
// Opcode: 0xA4
void LDY_ZP(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ZP(cpu, memory, &cpu->Y, cycles);
}

// LDY zero page X-indexed addressing mode
// Assembly: LDY $nn,X
// Opcode: 0xB4
void LDY_ZPX(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ZPX(cpu, memory, &cpu->Y, cycles);
}

// LDY absolute addressing mode
// Assembly: LDY $nnnn
// Opcode: 0xAC
void LDY_ABS(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ABS(cpu, memory, &cpu->Y, cycles);
}

// LDY absolute X-indexed addressing mode
// Assembly: LDY $nnnn,X
// Opcode: 0xBC
void LDY_ABSX(CPU *cpu, Memory *memory, uint *cycles) {
  ADDR_ABSX(cpu, memory, &cpu->Y, cycles);
}
