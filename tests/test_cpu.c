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

void test_fetch_word() {
  reset(&cpu, &memory);
  cpu.PC = 0x1000; // Setting an arbitrary start position
  writeWord(&memory, 0x1000, 0xABCD); // Write a known value

  uint cycles = 2;
  word result = fetchWord(&cpu, &memory, &cycles);

  CU_ASSERT(result == 0xABCD);
  CU_ASSERT(cpu.PC == 0x1002);
  CU_ASSERT(cycles == 0);
}

void test_lda_immediate_positive() {
  reset(&cpu, &memory);

  cpu.PC = 0x2000; // Arbitrary position
  byte testValue = 0x7F;
  writeByte(&memory, cpu.PC, OP_LDA_IM); // Opcode for LDA Immediate
  writeByte(&memory, cpu.PC + 1, testValue); // Positive test value that does not set the negative flag

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_immediate_zero() {
  reset(&cpu, &memory);
  
  cpu.PC = 0x2000; // Arbitrary position
  byte testValue = 0x00;
  writeByte(&memory, 0x2000, OP_LDA_IM); // Opcode for LDA Immediate
  writeByte(&memory, 0x2001, testValue); // Zero value

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & ZERO_FLAG);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_immediate_negative() {
  reset(&cpu, &memory);

  cpu.PC = 0x2000; // Arbitrary position
  byte testValue = 0x80;
  writeByte(&memory, 0x2000, OP_LDA_IM); // Opcode for LDA Immediate
  writeByte(&memory, 0x2001, testValue); // Negative test value (128, which sets the negative flag in 8-bit)

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_zero_page_positive() {
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  byte testValue = 0x7F; // Test with a positive value
  word zeroPageAddress = 0x20; // Example zero page address
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDA from zero page
  writeByte(&memory, cpu.PC, OP_LDA_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, cpu.PC + 1, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT(cpu.A == testValue); // Check if A register is loaded correctly
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG)); // Negative flag should not be set
  CU_ASSERT(!(cpu.PS & ZERO_FLAG)); // Zero flag should not be set
  CU_ASSERT(cycles == 0);
}

void test_lda_zero_page_zero() {
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  byte testValue = 0x00; // Test with a zero value
  word zeroPageAddress = 0x20; // Example zero page address
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDA from zero page
  writeByte(&memory, cpu.PC, OP_LDA_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, cpu.PC + 1, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT(cpu.A == testValue); // Check if A register is loaded correctly
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG)); // Negative flag should not be set
  CU_ASSERT(cpu.PS & ZERO_FLAG); // Zero flag should be set
  CU_ASSERT(cycles == 0);
}

void test_lda_zero_page_negative() {
  reset(&cpu, &memory);

  // Set a test value in zero page memory
  byte testValue = 0x80; // Test with a negative value
  word zeroPageAddress = 0x20; // Example zero page address
  writeByte(&memory, zeroPageAddress, testValue);

  // Set up CPU to perform LDA from zero page
  writeByte(&memory, cpu.PC, OP_LDA_ZP); // Opcode for LDA Zero Page
  writeByte(&memory, cpu.PC + 1, zeroPageAddress); // Zero page address to load from

  uint cycles = 3; // Cycles needed for zero page addressing mode
  execute(&cpu, &memory, &cycles);

  // Asserts
  CU_ASSERT(cpu.A == testValue); // Check if A register is loaded correctly
  CU_ASSERT(cpu.PS & NEGATIVE_FLAG); // Negative flag should be set
  CU_ASSERT(!(cpu.PS & ZERO_FLAG)); // Zero flag should not be set
  CU_ASSERT(cycles == 0);
}

void test_lda_zpx_positive() {
  reset(&cpu, &memory);

  cpu.PC = 0x0100;
  cpu.X = 0x01;
  byte address = 0xFE;
  byte testValue = 0x7F;  // A positive value that doesn't set the negative flag
  writeByte(&memory, (address + cpu.X) % 256, testValue);

  writeByte(&memory, cpu.PC, OP_LDA_ZPX);
  writeByte(&memory, cpu.PC + 1, address);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_zpx_zero() {
  reset(&cpu, &memory);

  cpu.X = 0x01;
  byte address = 0xFE;
  byte testValue = 0x00;  // Zero value
  writeByte(&memory, (address + cpu.X) % 256, testValue);

  writeByte(&memory, cpu.PC, OP_LDA_ZPX);
  writeByte(&memory, cpu.PC + 1, address);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & ZERO_FLAG);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_zpx_negative() {
  reset(&cpu, &memory);

  cpu.X = 0x01;
  byte address = 0xFE;
  byte testValue = 0x80;  // Negative value
  writeByte(&memory, (address + cpu.X) % 256, testValue);

  writeByte(&memory, cpu.PC, OP_LDA_ZPX);
  writeByte(&memory, cpu.PC + 1, address);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_zpx_x_is_zero() {
  reset(&cpu, &memory);

  cpu.X = 0x00;  // X register is zero
  byte address = 0xFE;
  byte testValue = 0x7F;  // A positive value
  writeByte(&memory, address, testValue);  // Directly use address without offset

  writeByte(&memory, cpu.PC, OP_LDA_ZPX);
  writeByte(&memory, cpu.PC + 1, address);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_positive() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word testAddress = 0x1234;
  byte testValue = 0x78;
  writeByte(&memory, testAddress, testValue);
  
  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABS);
  writeWord(&memory, cpu.PC + 1, testAddress);
  //writeByte(&memory, cpu.PC + 1, testAddress & 0xFF);       // Low byte of the address
  //writeByte(&memory, cpu.PC + 2, (testAddress >> 8) & 0xFF); // High byte of the address
  
  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));  // Assuming testValue is not negative
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));      // Assuming testValue is not zero
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_zero() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word testAddress = 0x1234;
  byte testValue = 0x00;
  writeByte(&memory, testAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABS);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & ZERO_FLAG);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_negative() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word testAddress = 0x1234;
  byte testValue = 0x80;
  writeByte(&memory, testAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABS);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_x_positive() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  cpu.PC = 0x0100; // Arbitrary position
  cpu.X = 0x01;
  word testAddress = 0x0010;
  byte testValue = 0x78;
  writeByte(&memory, (testAddress + cpu.X), testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABSX);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_x_zero() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word testAddress = 0x0010;
  byte testValue = 0x00;
  writeByte(&memory, testAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABSX);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & ZERO_FLAG);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_x_negative() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word testAddress = 0x0010;
  byte testValue = 0x80;
  writeByte(&memory, testAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABSX);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_x_page_cross() {
  reset(&cpu, &memory);

  cpu.PC = 0x0110;
  cpu.X = 0x01;
  word baseAddress = 0x00FF;  // Near end of page, will cross with X added
  byte testValue = 0x42;
  writeByte(&memory, baseAddress + cpu.X, testValue);

  writeByte(&memory, cpu.PC, OP_LDA_ABSX);
  writeByte(&memory, cpu.PC + 1, baseAddress & 0xFF);
  writeByte(&memory, cpu.PC + 2, (baseAddress >> 8) & 0xFF);

  uint cycles = 5;  // Assuming an additional cycle for page crossing
  execute(&cpu, &memory, &cycles);

  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_y_positive() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  cpu.PC = 0x0100; // Arbitrary position
  cpu.Y = 0x01;
  word testAddress = 0x0010;
  byte testValue = 0x78;
  writeByte(&memory, (testAddress + cpu.Y), testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABSY);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_y_zero() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word testAddress = 0x0010;
  byte testValue = 0x00;
  writeByte(&memory, testAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABSY);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & ZERO_FLAG);
  CU_ASSERT(!(cpu.PS & NEGATIVE_FLAG));
  CU_ASSERT(cycles == 0);
}

void test_lda_abs_y_negative() {
  reset(&cpu, &memory);

  // Set up memory and CPU for test
  word testAddress = 0x0010;
  byte testValue = 0x80;
  writeByte(&memory, testAddress, testValue);

  // Write the opcode and address to the program counter location
  writeByte(&memory, cpu.PC, OP_LDA_ABSY);
  writeWord(&memory, cpu.PC + 1, testAddress);

  uint cycles = 4;
  execute(&cpu, &memory, &cycles);

  // Assertions to ensure correct execution
  CU_ASSERT(cpu.A == testValue);
  CU_ASSERT(cpu.PS & NEGATIVE_FLAG);
  CU_ASSERT(!(cpu.PS & ZERO_FLAG));
  CU_ASSERT(cycles == 0);
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