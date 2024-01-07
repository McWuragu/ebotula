/* #############################################################
 *
 *  This file is a part of ebotula testsuit.
 *
 *  Coypright (C)2023-2024 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * #############################################################
 */

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
