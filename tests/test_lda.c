#include "CUnit/Basic.h"
#include "../src/6502.h"
#include "setup_tests.h"

void test_lda_immediate_positive() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDA_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x7F
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.A, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_lda_immediate_zero() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDA_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x00
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.A, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_lda_immediate_negative() {
  CPU cpu;
  Memory memory;
  immediateParams params = {
    .opcode = OP_LDA_IM, .cycles = 2,
    .startingAddress = 0x0100, .testValue = 0x80
  };

  setup_immediate(&cpu, &memory, &params);
  execute(&cpu, &memory, &params.cycles);

  CU_ASSERT_EQUAL(cpu.A, params.testValue);
  CU_ASSERT_EQUAL(params.cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, params.startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void test_lda_zero_page_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  word startingAddress = 0x0100; // Example zero page address
  byte zeroPageAddress = 0x20;
  byte testValue = 0x7F; // Test with a positive value
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDA from zero page
  writeByte(&memory, startingAddress, OP_LDA_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT_EQUAL(cpu.A, testValue); // Check if A register is loaded correctly
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG); // Zero flag should not be set
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG); // Negative flag should not be set
}

void test_lda_zero_page_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  word startingAddress = 0x0100; // Example zero page address
  byte zeroPageAddress = 0x20;
  byte testValue = 0x00; // Test with a zero value
  cpu.PC = startingAddress;
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDA from zero page
  writeByte(&memory, startingAddress, OP_LDA_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT_EQUAL(cpu.A, testValue); // Check if A register is loaded correctly
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG); // Zero flag should be set
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG); // Negative flag should not be set
}

void test_lda_zero_page_negative() {
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
  writeByte(&memory, startingAddress, OP_LDA_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT_EQUAL(cpu.A, testValue); // Check if A register is loaded correctly
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG); // Zero flag should not be set
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG); // Negative flag should be set
}

void test_lda_zpx_positive() {
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

  writeByte(&memory, startingAddress, OP_LDA_ZPX);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
}

void test_lda_zpx_zero() {
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

  writeByte(&memory, startingAddress, OP_LDA_ZPX);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_zpx_negative() {
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

  writeByte(&memory, startingAddress, OP_LDA_ZPX);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_zpx_x_is_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte zeroPageAddress = 0x20;
  byte xIndex = 0x00;
  byte testValue = 0x7F;
  byte xIndexedZeroPageAddress = (zeroPageAddress + xIndex) % 256;

  cpu.PC = startingAddress;
  cpu.X = xIndex;
  writeByte(&memory, xIndexedZeroPageAddress, testValue);

  writeByte(&memory, startingAddress, OP_LDA_ZPX);
  writeByte(&memory, startingAddress + 0x01, zeroPageAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x78;

  cpu.PC = startingAddress;
  writeByte(&memory, testAddress, testValue);
  
  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);
  
  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);  // Assuming testValue is not negative
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);      // Assuming testValue is not zero
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x00;

  cpu.PC = startingAddress;
  writeByte(&memory, testAddress, testValue);
  
  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x1234;
  byte testValue = 0x80;

  cpu.PC = startingAddress;
  writeByte(&memory, testAddress, testValue);
  
  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABS);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_x_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x78;
  byte xIndex = 0x01;
  word xIndexedAddress = testAddress + xIndex;

  cpu.PC = startingAddress; // Arbitrary position
  cpu.X = xIndex;

  writeByte(&memory, xIndexedAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABSX);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_x_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x00;
  byte xIndex = 0x01;
  word xIndexedAddress = testAddress + xIndex;

  cpu.PC = startingAddress; // Arbitrary position
  cpu.X = xIndex;

  writeByte(&memory, xIndexedAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABSX);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_x_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x80;
  byte xIndex = 0x01;
  word xIndexedAddress = testAddress + xIndex;

  cpu.PC = startingAddress; // Arbitrary position
  cpu.X = xIndex;

  writeByte(&memory, xIndexedAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABSX);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_x_page_cross() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0010;
  word testAddress = 0x00FF;
  byte testValue = 0x42;
  byte xIndex = 0x01;
  word xIndexedAddress = testAddress + xIndex;

  cpu.PC = startingAddress; // Arbitrary position
  cpu.X = xIndex;

  writeByte(&memory, xIndexedAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABSX);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 5;  // Assuming an additional cycle for page crossing
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x03);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_y_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x78;
  byte yIndex = 0x01;
  word yIndexedAddress = testAddress + yIndex;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;

  writeByte(&memory, yIndexedAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABSY);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_y_zero() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x00;
  byte yIndex = 0x01;
  word yIndexedAddress = testAddress + yIndex;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;

  writeByte(&memory, yIndexedAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABSY);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void test_lda_abs_y_negative() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word startingAddress = 0x0100;
  word testAddress = 0x0010;
  byte testValue = 0x80;
  byte yIndex = 0x01;
  word yIndexedAddress = testAddress + yIndex;

  cpu.PC = startingAddress;
  cpu.Y = yIndex;

  writeByte(&memory, yIndexedAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, startingAddress, OP_LDA_ABSY);
  writeWord(&memory, startingAddress + 0x01, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_EQUAL(cycles, 0);
}

void run_lda_tests() {
  CU_pSuite suite = CU_add_suite("LDA tests", 0, 0);

  CU_add_test(suite, "Immediate mode with a positive value", test_lda_immediate_positive);
  CU_add_test(suite, "Immediate mode with a zero value", test_lda_immediate_zero);
  CU_add_test(suite, "Immediate mode with a negative value", test_lda_immediate_negative);

  CU_add_test(suite, "Zero Page mode with a positive value", test_lda_zero_page_positive);
  CU_add_test(suite, "Zero Page mode with a zero value", test_lda_zero_page_zero);
  CU_add_test(suite, "Zero Page mode with a negative value", test_lda_zero_page_negative);

  CU_add_test(suite, "Zero Page X-indexed with Positive Value", test_lda_zpx_positive);
  CU_add_test(suite, "Zero Page X-indexed with Zero", test_lda_zpx_zero);
  CU_add_test(suite, "Zero Page X-indexed with Negative Value", test_lda_zpx_negative);
  CU_add_test(suite, "Zero Page X-indexed with X=0", test_lda_zpx_x_is_zero);

  CU_add_test(suite, "Absolute mode with positive value", test_lda_abs_positive);
  CU_add_test(suite, "Absolute mode with zero", test_lda_abs_zero);
  CU_add_test(suite, "Absolute mode with negative value", test_lda_abs_negative);

  CU_add_test(suite, "Absolute X-indexed with positive value", test_lda_abs_x_positive);
  CU_add_test(suite, "Absolute X-indexed with zero", test_lda_abs_x_zero);
  CU_add_test(suite, "Absolute X-indexed with negative value", test_lda_abs_x_negative);
  CU_add_test(suite, "Absolute X-indexed with page cross", test_lda_abs_x_page_cross);

  CU_add_test(suite, "Absolute Y-indexed with positive value", test_lda_abs_y_positive);
  CU_add_test(suite, "Absolute Y-indexed with zero", test_lda_abs_y_zero);
  CU_add_test(suite, "Absolute Y-indexed with negative value", test_lda_abs_y_negative);
}