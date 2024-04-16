#include "CUnit/Basic.h"

void test_truth() {
    CU_ASSERT(0);
}

void run_truth_tests() {
    CU_pSuite suite = CU_add_suite("Truth Tests", NULL, NULL);
    CU_add_test(suite, "Truth", test_truth);
}