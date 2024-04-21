#include "CUnit/Basic.h"
#include "../src/6502.h"
#include "test_truth.h"
#include "test_memory.h"
#include "test_cpu.h"
#include "test_lda.h"

int main() {
  CU_initialize_registry();

  run_truth_tests();
  run_memory_tests();
  run_cpu_tests();
  run_lda_tests();

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();

  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();

  return (failures == 0) ? 0 : -1;
}