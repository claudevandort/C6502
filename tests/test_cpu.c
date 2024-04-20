#include "CUnit/Basic.h"
#include "../src/cpu.h"
#include "../src/memory.h"

Memory memory;
CPU cpu;

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

void test_lda_immediate_positive() {
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte testValue = 0x7F;
  cpu.PC = startingAddress; // Arbitrary position
  writeByte(&memory, startingAddress, OP_LDA_IM); // Opcode for LDA Immediate
  writeByte(&memory, startingAddress + 0x01, testValue); // Positive test value that does not set the negative flag

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_lda_immediate_zero() {
  reset(&cpu, &memory);
  
  word startingAddress = 0x0100;
  byte testValue = 0x00;
  cpu.PC = startingAddress; // Arbitrary position
  writeByte(&memory, startingAddress, OP_LDA_IM); // Opcode for LDA Immediate
  writeByte(&memory, startingAddress + 0x01, testValue); // Zero value

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_TRUE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_FALSE(cpu.PS & NEGATIVE_FLAG);
}

void test_lda_immediate_negative() {
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte testValue = 0x80;
  cpu.PC = startingAddress; // Arbitrary position
  writeByte(&memory, startingAddress, OP_LDA_IM); // Opcode for LDA Immediate
  writeByte(&memory, startingAddress + 0x01, testValue); // Negative test value (128, which sets the negative flag in 8-bit)

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.A, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void test_lda_zero_page_positive() {
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

void run_cpu_tests() {
  CU_pSuite suite = CU_add_suite("CPU tests", 0, 0);
  CU_pSuite suite_op = CU_add_suite("Opcode tests", 0, 0);

  CU_add_test(suite, "CPU reset", test_cpu_reset);
  CU_add_test(suite, "Fetch byte", test_fetch_byte);
  CU_add_test(suite, "Fetch word", test_fetch_word);

  CU_add_test(suite_op, "LDA Immediate mode with a positive value", test_lda_immediate_positive);
  CU_add_test(suite_op, "LDA Immediate mode with a zero value", test_lda_immediate_zero);
  CU_add_test(suite_op, "LDA Immediate mode with a negative value", test_lda_immediate_negative);

  CU_add_test(suite_op, "LDA Zero Page mode with a positive value", test_lda_zero_page_positive);
  CU_add_test(suite_op, "LDA Zero Page mode with a zero value", test_lda_zero_page_zero);
  CU_add_test(suite_op, "LDA Zero Page mode with a negative value", test_lda_zero_page_negative);

  CU_add_test(suite_op, "LDA Zero Page X-indexed with Positive Value", test_lda_zpx_positive);
  CU_add_test(suite_op, "LDA Zero Page X-indexed with Zero", test_lda_zpx_zero);
  CU_add_test(suite_op, "LDA Zero Page X-indexed with Negative Value", test_lda_zpx_negative);
  CU_add_test(suite_op, "LDA Zero Page X-indexed with X=0", test_lda_zpx_x_is_zero);

  CU_add_test(suite_op, "LDA Absolute mode with positive value", test_lda_abs_positive);
  CU_add_test(suite_op, "LDA Absolute mode with zero", test_lda_abs_zero);
  CU_add_test(suite_op, "LDA Absolute mode with negative value", test_lda_abs_negative);

  CU_add_test(suite_op, "LDA Absolute X-indexed with positive value", test_lda_abs_x_positive);
  CU_add_test(suite_op, "LDA Absolute X-indexed with zero", test_lda_abs_x_zero);
  CU_add_test(suite_op, "LDA Absolute X-indexed with negative value", test_lda_abs_x_negative);
  CU_add_test(suite_op, "LDA Absolute X-indexed with page cross", test_lda_abs_x_page_cross);

  CU_add_test(suite_op, "LDA Absolute Y-indexed with positive value", test_lda_abs_y_positive);
  CU_add_test(suite_op, "LDA Absolute Y-indexed with zero", test_lda_abs_y_zero);
  CU_add_test(suite_op, "LDA Absolute Y-indexed with negative value", test_lda_abs_y_negative);
}