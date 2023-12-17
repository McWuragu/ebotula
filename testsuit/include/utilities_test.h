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
void testclearspace_NOP(void);
void testclearspace_space_at_the_begin_and_end(void);
void testclearspace_space_between(void);
void testclearspace_multi_space_between(void);
void testclearspace_space_quote_between(void);
void testclearspace_space_quotes_mix(void);
void testclearspace_space_only(void);
void testclearspace_space_only_and_quotes(void);

#define NUMBER_OF_UTILITIES_TESTS	15

/* collection of all test für the utilities function */
static strTestDesc_t pstrUtilitiesTestSet[NUMBER_OF_UTILITIES_TESTS]= { 
	{testTrim_no_space,		"Trim(): null spaces"}, 
	{testTrim_leading_space,"Trim(): leading space"}, 
	{testTrim_newline,		"Trim(): newline and carage return"}, 
	{testTrim_double_space,	"Trim(): double spaces"},
	{testTrim_sentence,		"Trim(): sentence"},
	{testStrToLower_UpToLow,	"StrToLower(): full upper to lower"},
	{testStrToLower_LowToLow,	"StrToLower(): full lower to lower"},
	{testclearspace_NOP,						"clearspace(): no spaces and nop"},
	{testclearspace_space_at_the_begin_and_end,	"clearspace(): Space at the begin and end"},
	{testclearspace_space_between,				"clearspace(): Space between"},
	{testclearspace_multi_space_between,		"clearspace(): Mutlispace between"},
	{testclearspace_space_quote_between,		"clearspace(): quoted spaces"},
	{testclearspace_space_quotes_mix,			"clearspace(): mx of quotes and none quotes"},
	{testclearspace_space_only,					"clearspace(): string with only spaces"},
	{testclearspace_space_only_and_quotes,		"clearspace(): Space wirh and without quotes"},
};

#endif
