#include "testsuit.h"
#include "utilities.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#ifndef UTILITIES_TEST_H_
#define UTILITIES_TEST_H_

int init_utilities(void);
int clean_utilities(void); 

void testTrim_no_space(void);
void testTrim_leading_space(void);
void testTrim_newline(void);
void testTrim_double_space(void);
void testTrim_sentence(void);
void testStrToLower_UpToLow(void);
void testStrToLower_LowToLow(void);

#define NUMBER_OF_UTILITIES_TESTS	7

/* collection of all test für the utilities function */
static strTestDesc_t pstrUtilitiesTestSet[NUMBER_OF_UTILITIES_TESTS]= { 
	{testTrim_no_space,		"Trim(): null spaces"}, 
	{testTrim_leading_space,"Trim(): leading space"}, 
	{testTrim_newline,		"Trim(): newline and carage return"}, 
	{testTrim_double_space,	"Trim(): double spaces"},
	{testTrim_sentence,		"Trim(): sentence"},
	{testStrToLower_UpToLow,"StrToLower(): full upper to lower"},
	{testStrToLower_LowToLow,"StrToLower(): full lower to lower"},
};

#endif
