#include "CUnit/Basic.h"
#include "test_truth.h"
#include "test_memory.h"
#include "test_cpu.h"

int main() {
  CU_initialize_registry();

  run_truth_tests();
  run_memory_tests();
  run_cpu_tests();

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}