#ifndef CPU_H
#define CPU_H

#include "types.h"
#include "memory.h"

// Processor status flags
// |N|V|-|B|D|I|Z|C|
#define CARRY_FLAG 0x01        // 00000001
#define ZERO_FLAG 0x02         // 00000010
#define IRQ_DISABLE_FLAG 0x04  // 00000100
#define DECIMAL_MODE_FLAG 0x08 // 00001000
#define BREAK_FLAG 0x10        // 00010000
#define UNUSED_FLAG 0x20       // 00100000
#define OVERFLOW_FLAG 0x40     // 01000000
#define NEGATIVE_FLAG 0x80     // 10000000

// Opcodes
// LDA - Load accumulator with memory
#define OP_LDA_IM 0xA9 // Immediate addressing mode
#define OP_LDA_ZP 0xA5 // Zero page addressing mode
#define OP_LDA_ZPX 0xB5 // Zero page X addressing mode

typedef struct {
  word PC; // Program counter
  byte SP; // Stack pointer

  byte A; // Accumulator
  byte X, Y; // index X, and Y registers

  byte PS; // Processor status flags

} CPU;

void reset(CPU *cpu, Memory *memory);
byte fetchByte(CPU *cpu, Memory *memory, uint *cycles);
void execute(CPU *cpu, Memory *memory, uint *cycles);

#endif