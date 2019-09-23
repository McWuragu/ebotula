#include "utilities_test.h"

ConfigSetup_t sSetup;

int init_utilities(void) {
	return 0;
}

int clean_utilities(void) {
	return 0;
}


void testTrim_no_space(void) {
	char pTestStr[] = "hallo";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo");
}

void testTrim_leading_space(void) {
	char pTestStr[] = " hallo";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo");
}

void testTrim_double_space(void) {
	char pTestStr[] = "  hallo";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo");
}

void testTrim_newline (void) {
	char pTestStr[] = "hallo\n\r";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo");
}

