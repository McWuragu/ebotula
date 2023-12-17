#include "utilities_test.h"

ConfigSetup_t sSetup;

int init_utilities(void) {
	return 0;
}

int clean_utilities(void) {
	return 0;
}


void testTrim_empty_string(void) {
	char pTestStr[] = "";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"");
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

void testTrim_multi_leading_space(void) {
	char pTestStr[] = "     hallo";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo");
}

void testTrim_newline (void) {
	char pTestStr[] = "hallo\n\r";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo");
}

void testTrim_sentence(void) {
	char pTestStr[] = " hallo mein Freund, wie  geht es dir?\n\r";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo mein Freund, wie geht es dir?");
}

void testStrToLower_UpToLow(void) {
	char pTestStr[]="HALLO";
	char *pCheckStr;
	
	pCheckStr=StrToLower(pTestStr);
	
	CU_ASSERT_STRING_EQUAL(pCheckStr,"hallo");
}

void testStrToLower_LowToLow(void) {
	char pTestStr[]="hallo";
	char *pCheckStr;
	
	pCheckStr=StrToLower(pTestStr);
	
	CU_ASSERT_STRING_EQUAL(pCheckStr,"hallo");
}

void testclearspace_NOP(void) {

	char pTestStr[]="Teststring";
	clearspace(pTestStr);

	CU_ASSERT_STRING_EQUAL(pTestStr,"Teststring");
}

void testclearspace_space_only(void) {

	char pTestStr[]="      ";
	clearspace(pTestStr);

	CU_ASSERT_STRING_EQUAL(pTestStr,"");
}

void testclearspace_space_only_and_quotes(void) {

	char pTestStr[]=" \"  \"   ";
	clearspace(pTestStr);

	CU_ASSERT_STRING_EQUAL(pTestStr,"  ");
}

void testclearspace_space_at_the_begin_and_end(void) {

	char pTestStr[]=" Teststring ";
	clearspace(pTestStr);

	CU_ASSERT_STRING_EQUAL(pTestStr,"Teststring");
}

void testclearspace_space_between(void) {

	char pTestStr[]="Hallo Teststring";
	clearspace(pTestStr);

	CU_ASSERT_STRING_EQUAL(pTestStr,"HalloTeststring");
}

void testclearspace_multi_space_between(void) {

	char pTestStr[]="  Hallo  Teststring  ";
	clearspace(pTestStr);
	
	CU_ASSERT_STRING_EQUAL(pTestStr,"HalloTeststring");
}

void testclearspace_space_quote_between(void) {

	char pTestStr[]="\"Hallo Teststring\"";
	clearspace(pTestStr);

	
	CU_ASSERT_STRING_EQUAL(pTestStr,"Hallo Teststring");
}

void testclearspace_space_quotes_mix(void) {

	char pTestStr[]="\"Hallo Teststring \" more Teststring  and more";
	clearspace(pTestStr);

	
	CU_ASSERT_STRING_EQUAL(pTestStr,"Hallo Teststring moreTeststringandmore");
}
