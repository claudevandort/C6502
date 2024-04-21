#include "CUnit/Basic.h"
#include "../src/6502.h"

void test_cpu_reset() {
  reset(&cpu, &memory);

  CU_ASSERT_EQUAL(cpu.PC, 0xFFFC);
  CU_ASSERT_EQUAL(cpu.SP, 0x01);
  CU_ASSERT_EQUAL(cpu.A, 0);
  CU_ASSERT_EQUAL(cpu.X, 0);
  CU_ASSERT_EQUAL(cpu.Y, 0);
  CU_ASSERT_EQUAL(cpu.PS, (UNUSED_FLAG | IRQ_DISABLE_FLAG));
}

void test_fetch_byte() {
  reset(&cpu, &memory);

  word testAddress = 0x1000;
  byte testValue = 0xAB;
  cpu.PC = testAddress; // Setting an arbitrary start position
  writeByte(&memory, testAddress, testValue); // Write a known value

  uint cycles = 1;
  byte result = fetchByte(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(result, testValue);
  CU_ASSERT_EQUAL(cpu.PC, testAddress + 0x01);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_fetch_word() {
  reset(&cpu, &memory);

  word testAddress = 0x1000;
  word testValue = 0xABCD;
  cpu.PC = testAddress; // Setting an arbitrary start position
  writeWord(&memory, testAddress, testValue); // Write a known value

  uint cycles = 2;
  word result = fetchWord(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(result, testValue);
  CU_ASSERT_EQUAL(cpu.PC, testAddress + 0x02);
  CU_ASSERT_EQUAL(cycles, 0);
}

void run_cpu_tests() {
  CU_pSuite suite = CU_add_suite("CPU tests", 0, 0);

  CU_add_test(suite, "CPU reset", test_cpu_reset);
  CU_add_test(suite, "Fetch byte", test_fetch_byte);
  CU_add_test(suite, "Fetch word", test_fetch_word);
}