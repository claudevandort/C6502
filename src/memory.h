#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

#define MEMORY_SIZE 1024 * 64 // 64KB of memory

typedef struct {
  byte data[MEMORY_SIZE];
} Memory;

void initMemory(Memory *memory);
byte readByte(Memory *memory, word address);
void writeByte(Memory *memory, word address, byte value);
word readWord(Memory *memory, word address);
void writeWord(Memory *memory, word address, word value);

#endif