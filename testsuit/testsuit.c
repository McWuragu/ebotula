#include "include/testsuit.h"

int main () {
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();
   	/* Run all tests using the CUnit Basic interface */
   	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();	
	

	CU_cleanup_registry();
	return CU_get_error();
}
