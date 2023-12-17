#include "testsuit.h"
#include "utilities_test.h"
#include "queue_test.h"


int main () {
	CU_pSuite pSuite = NULL;
	unsigned int nIndex=0;

	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

   	/* add a suite to the registry */
	pSuite = CU_add_suite("Utilites", init_utilities, clean_utilities);
	if (NULL == pSuite) {
      		CU_cleanup_registry();
      		return CU_get_error();
   	}		



	for (nIndex=0; nIndex<NUMBER_OF_UTILITIES_TESTS; nIndex++ ) {
	  	if (NULL == CU_add_test(pSuite, pstrUtilitiesTestSet[nIndex].pDesc, pstrUtilitiesTestSet[nIndex].TestFkt)){
    	  		CU_cleanup_registry();
      			return CU_get_error();
	   	}		
	}


   	/* add a suite to the registry */
	pSuite = CU_add_suite("Queue", init_queue, clean_queue);
	if (NULL == pSuite) {
      		CU_cleanup_registry();
      		return CU_get_error();
   	}

	for (nIndex=0; nIndex<NUMBER_OF_QUEUE_TESTS; nIndex++ ) {
	  	if (NULL == CU_add_test(pSuite, pstrQueueTestSet[nIndex].pDesc, pstrQueueTestSet[nIndex].TestFkt)){
    	  		CU_cleanup_registry();
      			return CU_get_error();
	   	}
	}

   	/* Run all tests using the CUnit Basic interface */
   	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();	

	CU_cleanup_registry();
	return CU_get_error();
}
