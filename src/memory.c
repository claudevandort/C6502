#include "memory.h"

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