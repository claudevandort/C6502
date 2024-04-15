#ifndef CPU_H
#define CPU_H

#include "types.h"
#include "memory.h"

// Processor status flags
#define CARRY_FLAG 0x01
#define ZERO_FLAG 0x02
#define IRQ_DISABLE_FLAG 0x04
#define DECIMAL_MODE_FLAG 0x08
#define BREAK_FLAG 0x10
#define UNUSED_FLAG 0x20
#define OVERFLOW_FLAG 0x40
#define NEGATIVE_FLAG 0x80

// Opcodes
#define OP_LDA_IM 0xA9

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