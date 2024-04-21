#include "CUnit/Basic.h"
#include "../src/6502.h"

void test_ldx_immediate_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte testValue = 0x7F;
  cpu.PC = startingAddress; // Arbitrary position
  writeByte(&memory, startingAddress, OP_LDX_IM); // Opcode for LDX Immediate
  writeByte(&memory, startingAddress + 0x01, testValue); // Positive test value that does not set the negative flag

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldx_immediate_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte testValue = 0x00;
  cpu.PC = startingAddress; // Arbitrary position
  writeByte(&memory, startingAddress, OP_LDX_IM); // Opcode for LDX Immediate
  writeByte(&memory, startingAddress + 0x01, testValue); // Zero test value that sets the zero flag

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldx_immediate_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte testValue = 0x80;
  cpu.PC = startingAddress; // Arbitrary position
  writeByte(&memory, startingAddress, OP_LDX_IM); // Opcode for LDX Immediate
  writeByte(&memory, startingAddress + 0x01, testValue); // Negative test value that sets the negative flag

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldx_zero_page_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  word startingAddress = 0x0100; // Example zero page address
  byte zeroPageAddress = 0x20;
  byte testValue = 0x7F; // Test with a positive value
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDX from zero page
  writeByte(&memory, startingAddress, OP_LDX_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress); // Zero page address to load from

  uint cycles = 3;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldx_zero_page_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  word startingAddress = 0x0100; // Example zero page address
  byte zeroPageAddress = 0x20;
  byte testValue = 0x00; // Test with a zero value
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDX from zero page
  writeByte(&memory, startingAddress, OP_LDX_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT_EQUAL(cpu.X, testValue); // Check if A register is loaded correctly
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG); // Zero flag should not be set
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG); // Negative flag should not be set
}

void test_ldx_zero_page_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  word startingAddress = 0x0100; // Example zero page address
  byte zeroPageAddress = 0x20;
  byte testValue = 0x80; // Test with a negative value
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDA from zero page
  writeByte(&memory, startingAddress, OP_LDX_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT_EQUAL(cpu.X, testValue); // Check if A register is loaded correctly
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG); // Zero flag should not be set
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG); // Negative flag should be set
}

void run_ldx_tests() {
  CU_pSuite suite = CU_add_suite("LDX tests", NULL, NULL);

  CU_add_test(suite, "Immediate mode with a positive value", test_ldx_immediate_positive);
  CU_add_test(suite, "Immediate mode with a zero value", test_ldx_immediate_zero);
  CU_add_test(suite, "Immediate mode with a negative value", test_ldx_immediate_negative);

  CU_add_test(suite, "Zero page mode with a positive value", test_ldx_zero_page_positive);
  CU_add_test(suite, "Zero page mode with a zero value", test_ldx_zero_page_zero);
  CU_add_test(suite, "Zero page mode with a negative value", test_ldx_zero_page_negative);
}
