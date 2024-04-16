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
}

void run_cpu_tests() {
    CU_pSuite suite = CU_add_suite("CPU tests", 0, 0);
    CU_pSuite suite_op = CU_add_suite("Opcode tests", 0, 0);

    CU_add_test(suite, "CPU reset", test_cpu_reset);
    CU_add_test(suite, "Fetch byte", test_fetch_byte);
    CU_add_test(suite_op, "LDA Immediate mode with a positive value", test_lda_immediate_positive);
    CU_add_test(suite_op, "LDA Immediate mode with a zero value", test_lda_immediate_zero);
    CU_add_test(suite_op, "LDA Immediate mode with a negative value", test_lda_immediate_negative);
    CU_add_test(suite_op, "LDA Zero Page mode with a positive value", test_lda_zero_page_positive);
    CU_add_test(suite_op, "LDA Zero Page mode with a zero value", test_lda_zero_page_zero);
    CU_add_test(suite_op, "LDA Zero Page mode with a negative value", test_lda_zero_page_negative);
}