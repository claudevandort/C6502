#include "CUnit/Basic.h"
#include "../src/6502.h"

void test_ldx_immediate_positive() {
  CPU cpu;
  Memory memory;
  reset(&cpu, &memory);

  word startingAddress = 0x0100;
  byte testValue = 0x7F;
  cpu.PC = startingAddress; // Arbitrary position
  writeByte(&memory, startingAddress, OP_LDX_IM); // Opcode for LDA Immediate
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
  writeByte(&memory, startingAddress, OP_LDX_IM); // Opcode for LDA Immediate
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
  writeByte(&memory, startingAddress, OP_LDX_IM); // Opcode for LDA Immediate
  writeByte(&memory, startingAddress + 0x01, testValue); // Negative test value that sets the negative flag

  uint cycles = 2;
  execute(&cpu, &memory, &cycles);

  CU_ASSERT_EQUAL(cpu.X, testValue);
  CU_ASSERT_EQUAL(cycles, 0);
  CU_ASSERT_EQUAL(cpu.PC, startingAddress + 0x02);
  CU_ASSERT_FALSE(cpu.PS & ZERO_FLAG);
  CU_ASSERT_TRUE(cpu.PS & NEGATIVE_FLAG);
}

void run_ldx_tests() {
  CU_pSuite suite = CU_add_suite("LDX tests", NULL, NULL);

  CU_add_test(suite, "Immediate mode with a positive value", test_ldx_immediate_positive);
  CU_add_test(suite, "Immediate mode with a zero value", test_ldx_immediate_zero);
  CU_add_test(suite, "Immediate mode with a negative value", test_ldx_immediate_negative);
}
