#ifndef C6502_H
#define C6502_H

/*
 * TYPES
 * Data types used in the 6502
 */

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int uint;


/*
 * MEMORY
 *
 * Holds an array of bytes that represents the 64KB of memory the 6502 has.
 * Also includes functions for basic memory manipulation operations.
 */

#define MEMORY_SIZE 1024 * 64 // 64KB of memory

typedef struct {
  byte data[MEMORY_SIZE];
} Memory;

void initMemory(Memory *memory);
byte readByte(Memory *memory, word address);
void writeByte(Memory *memory, word address, byte value);
word readWord(Memory *memory, word address);
void writeWord(Memory *memory, word address, word value);


/*
 * CPU
 *
 * Holds the state of the 6502 CPU.
 * Also includes functions for basic CPU operations, and opcodes implementation.
 */

// Processor status flags
// |N|V|-|B|D|I|Z|C|
#define CARRY_FLAG        0x01 // 00000001
#define ZERO_FLAG         0x02 // 00000010
#define IRQ_DISABLE_FLAG  0x04 // 00000100
#define DECIMAL_MODE_FLAG 0x08 // 00001000
#define BREAK_FLAG        0x10 // 00010000
#define UNUSED_FLAG       0x20 // 00100000
#define OVERFLOW_FLAG     0x40 // 01000000
#define NEGATIVE_FLAG     0x80 // 10000000

typedef struct {
  word PC; // Program counter
  byte SP; // Stack pointer

  byte A; // Accumulator
  byte X, Y; // index X, and Y registers

  byte PS; // Processor status flags

} CPU;

typedef void (*instructionHandler)(CPU *cpu, Memory *memory, uint *cycles);

void initInstructions();
void reset(CPU *cpu, Memory *memory);
byte CPUreadByte(const Memory *memory, const word address, uint *cycles);
byte fetchByte(CPU *cpu, const Memory *memory, uint *cycles);
word fetchWord(CPU *cpu, const Memory *memory, uint *cycles);
void execute(CPU *cpu, Memory *memory, uint *cycles);

// Opcodes
// LDA - Load accumulator with memory
#define OP_LDA_IM   0xA9 // Immediate addressing mode
#define OP_LDA_ZP   0xA5 // Zero page addressing mode
#define OP_LDA_ZPX  0xB5 // Zero page X-indexed addressing mode
#define OP_LDA_ABS  0xAD // Absolute addressing mode
#define OP_LDA_ABSX 0xBD // Absolute X-indexed addressing mode
#define OP_LDA_ABSY 0xB9 // Absolute Y-indexed addressing mode
// #define OP_LDA_INDX 0xA1 // Indirect X-indexed addressing mode
// #define OP_LDA_INDY 0xB1 // Indirect Y-indexed addressing mode

// LDX - Load index X with memory
#define OP_LDX_IM   0xA2 // Immediate addressing mode
#define OP_LDX_ZP   0xA6 // Zero page addressing mode
#define OP_LDX_ZPY  0xB6 // Zero page Y-indexed addressing mode
#define OP_LDX_ABS  0xAE // Absolute addressing mode
#define OP_LDX_ABSY 0xBE // Absolute Y-indexed addressing mode

// LDY - Load index Y with memory
#define OP_LDY_IM   0xA0 // Immediate addressing mode
#define OP_LDY_ZP   0xA4 // Zero page addressing mode
#define OP_LDY_ZPX  0xB4 // Zero page X-indexed addressing mode
#define OP_LDY_ABS  0xAC // Absolute addressing mode
#define OP_LDY_ABSX 0xBC // Absolute X-indexed addressing mode

void LDA_IM(CPU *cpu, Memory *memory, uint *cycles);
void LDA_ZP(CPU *cpu, Memory *memory, uint *cycles);
void LDA_ZPX(CPU *cpu, Memory *memory, uint *cycles);
void LDA_ABS(CPU *cpu, Memory *memory, uint *cycles);
void LDA_ABSX(CPU *cpu, Memory *memory, uint *cycles);
void LDA_ABSY(CPU *cpu, Memory *memory, uint *cycles);

void LDX_IM(CPU *cpu, Memory *memory, uint *cycles);
void LDX_ZP(CPU *cpu, Memory *memory, uint *cycles);
void LDX_ZPY(CPU *cpu, Memory *memory, uint *cycles);
void LDX_ABS(CPU *cpu, Memory *memory, uint *cycles);
void LDX_ABSY(CPU *cpu, Memory *memory, uint *cycles);

void LDY_IM(CPU *cpu, Memory *memory, uint *cycles);
void LDY_ZP(CPU *cpu, Memory *memory, uint *cycles);
void LDY_ZPX(CPU *cpu, Memory *memory, uint *cycles);
void LDY_ABS(CPU *cpu, Memory *memory, uint *cycles);
void LDY_ABSX(CPU *cpu, Memory *memory, uint *cycles);

#endif