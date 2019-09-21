#include "utilities_test.h"

ConfigSetup_t sSetup;

int init_utilities(void) {
	return 0;
}

int clean_utilities(void) {
	return 0;
}

void testTrim(void) {
	char pTestStr[] = "hallo";
	trim(pTestStr);
	
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo");
}

