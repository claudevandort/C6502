#include "CUnit/Basic.h"
#include "../src/6502.h"
#include "setup_tests.h"

void test_ldy_immediate_positive() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDY_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x7F
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.Y, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_immediate_zero() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDY_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x00
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.Y, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_immediate_negative() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDY_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x80
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.Y, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_zero_page_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte testValue = 0x7F;
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ZP);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 3;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_zero_page_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte testValue = 0x00;
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ZP);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 3;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_zero_page_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte testValue = 0x80;
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ZP);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 3;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_zpx_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte xIndex = 0x01;
  byte testValue = 0x7F;
  byte xIndexedZeroPageAddress = (zeroPageAddress + xIndex) % 256;

  cpu.PC = startingAddress;
  cpu.X = xIndex;
  writeByte(&memory, xIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ZPX);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_zpx_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte xIndex = 0x01;
  byte testValue = 0x00;
  byte xIndexedZeroPageAddress = (zeroPageAddress + xIndex) % 256;

  cpu.PC = startingAddress;
  cpu.X = xIndex;
  writeByte(&memory, xIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ZPX);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_zpx_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte xIndex = 0x01;
  byte testValue = 0x80;
  byte xIndexedZeroPageAddress = (zeroPageAddress + xIndex) % 256;

  cpu.PC = startingAddress;
  cpu.X = xIndex;
  writeByte(&memory, xIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ZPX);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_abs_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x7F;

  cpu.PC = startingAddress;
  writeWord(&memory, testAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_abs_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x00;

  cpu.PC = startingAddress;
  writeWord(&memory, testAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_abs_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x80;

  cpu.PC = startingAddress;
  writeWord(&memory, testAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_absx_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x7F;
  byte xIndex = 0x01;
  word xIndexedAddress = (testAddress + xIndex);

  cpu.PC = startingAddress;
  cpu.X = xIndex;

  writeWord(&memory, xIndexedAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ABSX);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_absx_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x00;
  byte xIndex = 0x01;
  word xIndexedAddress = (testAddress + xIndex);

  cpu.PC = startingAddress;
  cpu.X = xIndex;

  writeWord(&memory, xIndexedAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ABSX);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_ldy_absx_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x80;
  byte xIndex = 0x01;
  word xIndexedAddress = (testAddress + xIndex);

  cpu.PC = startingAddress;
  cpu.X = xIndex;

  writeWord(&memory, xIndexedAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDY_ABSX);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.Y, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void run_ldy_tests() {
  CU_pSuite suite = CU_add_suite("LDY tests", NULL, NULL);

  CU_add_test(suite, "Immediate mode with a positive value", test_ldy_immediate_positive);
  CU_add_test(suite, "Immediate mode with a zero value", test_ldy_immediate_zero);
  CU_add_test(suite, "Immediate mode with a negative value", test_ldy_immediate_negative);

  CU_add_test(suite, "Zero page mode with a positive value", test_ldy_zero_page_positive);
  CU_add_test(suite, "Zero page mode with a zero value", test_ldy_zero_page_zero);
  CU_add_test(suite, "Zero page mode with a negative value", test_ldy_zero_page_negative);

  CU_add_test(suite, "Zero page X mode with a positive value", test_ldy_zpx_positive);
  CU_add_test(suite, "Zero page X mode with a zero value", test_ldy_zpx_zero);
  CU_add_test(suite, "Zero page X mode with a negative value", test_ldy_zpx_negative);

  CU_add_test(suite, "Absolute mode with a positive value", test_ldy_abs_positive);
  CU_add_test(suite, "Absolute mode with a zero value", test_ldy_abs_zero);
  CU_add_test(suite, "Absolute mode with a negative value", test_ldy_abs_negative);

  CU_add_test(suite, "Absolute X mode with a positive value", test_ldy_absx_positive);
  CU_add_test(suite, "Absolute X mode with a zero value", test_ldy_absx_zero);
  CU_add_test(suite, "Absolute X mode with a negative value", test_ldy_absx_negative);
}