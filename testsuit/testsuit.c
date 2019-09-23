#include "testsuit.h"
#include "utilities_test.h"


int main () {
	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

   	/* add a suite to the registry */
	pSuite = CU_add_suite("Utilites", init_utilities, clean_utilities);
	if (NULL == pSuite) {
      		CU_cleanup_registry();
      		return CU_get_error();
   	}		


  	if (NULL == CU_add_test(pSuite, "test of trim() with no space", testTrim_no_space)){
      		CU_cleanup_registry();
      		return CU_get_error();
   	}	

  	if (NULL == CU_add_test(pSuite, "test of trim() with leading space", testTrim_leading_space)){
      		CU_cleanup_registry();
      		return CU_get_error();
   	}	

  	if (NULL == CU_add_test(pSuite, "test of trim() with double space", testTrim_double_space)){
      		CU_cleanup_registry();
      		return CU_get_error();
   	}	

  	if (NULL == CU_add_test(pSuite, "test of trim() with newline return", testTrim_newline)){
      		CU_cleanup_registry();
      		return CU_get_error();
   	}	

   	/* Run all tests using the CUnit Basic interface */
   	CU_basic_set_mode(CU_BRM_NORMAL);
	CU_basic_run_tests();	

	CU_cleanup_registry();
	return CU_get_error();
}
