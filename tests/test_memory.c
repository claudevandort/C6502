#include "CUnit/Basic.h"
#include "../src/6502.h"

// Test for memory initialization
void test_memory_initialization() {
  Memory memory;
  initMemory(&memory);
  for (int i = 0; i < MEMORY_SIZE; i++) {
      CU_ASSERT(memory.data[i] == 0x00);
  }
}

// Test for byte read/write
void test_byte_read_write() {
  Memory memory;
  initMemory(&memory);
  writeByte(&memory, 0x1234, 0xAB);
  CU_ASSERT(readByte(&memory, 0x1234) == 0xAB);
}

// Test for word read/write
void test_word_read_write() {
  Memory memory;
  initMemory(&memory);
  writeWord(&memory, 0x2345, 0xBEEF);
  CU_ASSERT(readWord(&memory, 0x2345) == 0xBEEF);
}

void run_memory_tests() {
  CU_pSuite suite = CU_add_suite("Memory tests", 0, 0);

  CU_add_test(suite, "Memory initialization", test_memory_initialization);
  CU_add_test(suite, "Byte read/write", test_byte_read_write);
  CU_add_test(suite, "Word read/write", test_word_read_write);
}
