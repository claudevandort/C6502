#include "CUnit/Basic.h"
#include "../src/6502.h"
#include "setup_tests.h"

void test_ldx_immediate_positive() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDX_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x7F
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.X, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldx_immediate_zero() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDX_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x00
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.X, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldx_immediate_negative() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDX_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x80
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.X, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
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

void test_ldx_zpy_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte yIndex = 0x01;
  byte testValue = 0x7F;
  byte yIndexedZeroPageAddress = (zeroPageAddress + yIndex) % 256;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;
  writeByte(&memory, yIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ZPY);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
}

void test_ldx_zpy_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte yIndex = 0x01;
  byte testValue = 0x00;
  byte yIndexedZeroPageAddress = (zeroPageAddress + yIndex) % 256;

  cpu.PC = startingAddress;
  cpu.X = yIndex;
  writeByte(&memory, yIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ZPY);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_zpy_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte yIndex = 0x01;
  byte testValue = 0x80;
  byte yIndexedZeroPageAddress = (zeroPageAddress + yIndex) % 256;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;
  writeByte(&memory, yIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ZPY);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_zpy_y_is_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte xIndex = 0x00;
  byte testValue = 0x7F;
  byte yIndexedZeroPageAddress = (zeroPageAddress + xIndex) % 256;

  cpu.PC = startingAddress;
  cpu.X = xIndex;
  writeByte(&memory, yIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ZPY);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_abs_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x7F;

  cpu.PC = startingAddress;
  writeByte(&memory, testAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_abs_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x00;

  cpu.PC = startingAddress;
  writeByte(&memory, testAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_abs_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x80;

  cpu.PC = startingAddress;
  writeByte(&memory, testAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_abs_y_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x78;
  byte yIndex = 0x01;
  word yIndexedAddress = testAddress + yIndex;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;

  writeByte(&memory, yIndexedAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ABSY);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_abs_y_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x00;
  byte yIndex = 0x01;
  word yIndexedAddress = testAddress + yIndex;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;

  writeByte(&memory, yIndexedAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ABSY);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_ldx_abs_y_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x80;
  byte yIndex = 0x01;
  word yIndexedAddress = testAddress + yIndex;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;

  writeByte(&memory, yIndexedAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDX_ABSY);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void run_ldx_tests() {
  CU_pSuite suite = CU_add_suite("LDX tests", NULL, NULL);

  CU_add_test(suite, "Immediate mode with a positive value", test_ldx_immediate_positive);
  CU_add_test(suite, "Immediate mode with a zero value", test_ldx_immediate_zero);
  CU_add_test(suite, "Immediate mode with a negative value", test_ldx_immediate_negative);

  CU_add_test(suite, "Zero page mode with a positive value", test_ldx_zero_page_positive);
  CU_add_test(suite, "Zero page mode with a zero value", test_ldx_zero_page_zero);
  CU_add_test(suite, "Zero page mode with a negative value", test_ldx_zero_page_negative);

  CU_add_test(suite, "Zero page, Y-indexed mode with a positive value", test_ldx_zpy_positive);
  CU_add_test(suite, "Zero page, Y-indexed mode with a zero value", test_ldx_zpy_zero);
  CU_add_test(suite, "Zero page, Y-indexed mode with a negative value", test_ldx_zpy_negative);
  CU_add_test(suite, "Zero page, Y-indexed mode with Y=0", test_ldx_zpy_y_is_zero);

  CU_add_test(suite, "Absolute mode with a positive value", test_ldx_abs_positive);
  CU_add_test(suite, "Absolute mode with a zero value", test_ldx_abs_zero);
  CU_add_test(suite, "Absolute mode with a negative value", test_ldx_abs_negative);

  CU_add_test(suite, "Absolute, Y-indexed mode with a positive value", test_ldx_abs_y_positive);
  CU_add_test(suite, "Absolute, Y-indexed mode with a zero value", test_ldx_abs_y_zero);
  CU_add_test(suite, "Absolute, Y-indexed mode with a negative value", test_ldx_abs_y_negative);
}
