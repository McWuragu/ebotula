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

#include "utilities_test.h"
#include <stdlib.h>

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

void testTrim_only_whitespace(void){
    char pTestStr[] = " \t \n\r  ";
    trim(pTestStr);
    CU_ASSERT_STRING_EQUAL(pTestStr, "");
}
void testTrim_idempotent(void){
    char pTestStr[] = "  hallo  ";
    trim(pTestStr); 
	trim(pTestStr);
    CU_ASSERT_STRING_EQUAL(pTestStr, "hallo");
}

void testTrim_sentence(void) {
	char pTestStr[] = " hallo mein Freund, wie  geht es dir?\n\r";

	trim(pTestStr);
	CU_ASSERT_STRING_EQUAL(pTestStr,"hallo mein Freund, wie geht es dir?");
}

void testStrToLower_empty(void) {
    const char in[] = "";
    char* r = StrToLower(in);
    CU_ASSERT_PTR_NOT_NULL(r);
    CU_ASSERT_STRING_EQUAL(r, "");
    CU_ASSERT_PTR_NOT_EQUAL(r, in);
    free(r);
}

void testStrToLower_UpToLow(void) {
	char pTestStr[]="HALLO";
	char *pCheckStr;
	
	pCheckStr=StrToLower(pTestStr);
	
	CU_ASSERT_PTR_NOT_NULL(pCheckStr);
	CU_ASSERT_STRING_EQUAL(pCheckStr,"hallo");
	free(pCheckStr);
}

void testStrToLower_LowToLow(void) {
	char pTestStr[]="hallo";
	char *pCheckStr;
	
	pCheckStr=StrToLower(pTestStr);
	
	CU_ASSERT_PTR_NOT_NULL(pCheckStr);
	CU_ASSERT_STRING_EQUAL(pCheckStr,"hallo");
	free(pCheckStr);
}


void testStrToLower_ascii_basic(void) {
    const char in[] = "HALLO_abc-XYZ123";
    char* r = StrToLower(in);
    CU_ASSERT_PTR_NOT_NULL(r);
    CU_ASSERT_STRING_EQUAL(r, "hallo_abc-xyz123");
    free(r);
}

void testStrToLower_punctuation_digits_unchanged(void) {
    const char in[] = "A!B?C#1_2-3";
    char* r = StrToLower(in);
    CU_ASSERT_STRING_EQUAL(r, "a!b?c#1_2-3");
    free(r);
}

void testStrToLower_specials_and_umlauts(void){
    char pTestStr[] = "ÄÖÜß_ABC-xyz123";
    char* pCheckStr = StrToLower(pTestStr);
	
	CU_ASSERT_PTR_NOT_NULL(pCheckStr);
    CU_ASSERT_PTR_NOT_EQUAL(pCheckStr, pTestStr);        
    CU_ASSERT_STRING_EQUAL(pCheckStr, "ÄÖÜß_abc-xyz123"); // nur ASCII runter, Umlaute bleiben
	free(pCheckStr);
}

void testStrToLower_idempotent(void) {
    const char in[] = "MiXeD_123";
    char* r1 = StrToLower(in);
    char* r2 = StrToLower(r1);
    CU_ASSERT_STRING_EQUAL(r1, r2);
    free(r1);
    free(r2);
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

void testNickStringCheck_valid(void) {
    char nickName[] = "test_nick";

    CU_ASSERT_TRUE(NickStringCheck(nickName));
}

void testNickStringCheck_invalid(void) {
    char invalidNickName[] = "test_nick!";

    CU_ASSERT_FALSE(NickStringCheck(invalidNickName));
}

void testChannelStringCheck_valid(void) {
    char channelName[] = "#test_channel";

    CU_ASSERT_TRUE(ChannelStringCheck(channelName));
}

void testChannelStringCheck_invalid(void) {
    char invalidChannelName[] = "!test_channel";

    CU_ASSERT_FALSE(ChannelStringCheck(invalidChannelName));
}

void testChannelDataToStr(void) {
    ChannelData_t channelData;
    memset(&channelData, 0, sizeof(ChannelData_t));

    strcpy(channelData.sModes.pModeStr,"+k tn       ");
    channelData.sModes.pKeyword="test";
    channelData.pTopic = "Test Topic";
    channelData.pGreeting = "Welcome to the channel!";

    char *result = ChannelDataToStr(&channelData);

    CU_ASSERT_STRING_EQUAL(result, "+ktn test\tTest Topic\tWelcome to the channel!");

    free(result);
}


void testStrToChannelData(void) {
    char channelSet[] = "+ntk test\tTest Topic\tWelcome to the channel!";
    ChannelData_t channelData;

    memset(&channelData, 0, sizeof(ChannelData_t));

    StrToChannelData(channelSet, &channelData);

    CU_ASSERT_STRING_EQUAL(channelData.sModes.pModeStr, "+k tn       ");
    CU_ASSERT_STRING_EQUAL(channelData.sModes.pKeyword, "test");
    CU_ASSERT_STRING_EQUAL(channelData.pTopic, "Test Topic");
    CU_ASSERT_STRING_EQUAL(channelData.pGreeting, "Welcome to the channel!");
}
