#include "CUnit/Basic.h"
#include "../src/cpu.h"
#include "../src/memory.h"

Memory memory;
CPU cpu;

void test_cpu_reset() {
  reset(&cpu, &memory);

  CU_ASSERT(cpu.PC == 0xFFFC);
  CU_ASSERT(cpu.SP == 0x01);
  CU_ASSERT(cpu.A == 0);
  CU_ASSERT(cpu.X == 0);
  CU_ASSERT(cpu.Y == 0);
  CU_ASSERT(cpu.PS == (UNUSED_FLAG | IRQ_DISABLE_FLAG));
}

void test_fetch_byte() {
  reset(&cpu, &memory);
  cpu.PC = 0x1000; // Setting an arbitrary start position
  writeByte(&memory, 0x1000, 0xAB); // Write a known value

  uint cycles = 1;
  byte result = fetchByte(&cpu, &memory, &cycles);

  CU_ASSERT(result == 0xAB);
  CU_ASSERT(cpu.PC == 0x1001);
  CU_ASSERT(cycles == 0);
}

void test_lda_immediate(void) {
  reset(&cpu, &memory);
  cpu.PC = 0x2000; // Arbitrary position
  writeByte(&memory, 0x2000, OP_LDA_IM); // Opcode for LDA Immediate
  writeByte(&memory, 0x2001, 0x80); // Test value to load

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == 0x80);
  CU_ASSERT(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
}

void run_cpu_tests() {
    CU_pSuite suite = CU_add_suite("CPU tests", 0, 0);
    CU_pSuite suite_op = CU_add_suite("Opcode tests", 0, 0);

    CU_add_test(suite, "CPU reset", test_cpu_reset);
    CU_add_test(suite, "Fetch byte", test_fetch_byte);
    CU_add_test(suite_op, "LDA Immediate mode", test_lda_immediate);
}