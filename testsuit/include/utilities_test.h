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
#include "utilities.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#ifndef UTILITIES_TEST_H_
#define UTILITIES_TEST_H_

int init_utilities(void);
int clean_utilities(void); 

void testTrim_empty_string(void);
void testTrim_no_space(void);
void testTrim_leading_space(void);
void testTrim_multi_leading_space(void);
void testTrim_newline(void);
void testTrim_only_whitespace(void);
void testTrim_idempotent(void);
void testTrim_sentence(void);

void testStrToLower_empty(void);
void testStrToLower_UpToLow(void);
void testStrToLower_LowToLow(void);
void testStrToLower_ascii_basic(void);
void testStrToLower_punctuation_digits_unchanged(void);
void testStrToLower_specials_and_umlauts(void);
void testStrToLower_idempotent(void) ;

void testclearspace_NOP(void);
void testclearspace_space_at_the_begin_and_end(void);
void testclearspace_space_between(void);
void testclearspace_multi_space_between(void);
void testclearspace_space_quote_between(void);
void testclearspace_space_quotes_mix(void);
void testclearspace_space_only(void);
void testclearspace_space_only_and_quotes(void);

void testNickStringCheck_valid(void);
void testNickStringCheck_invalid(void);

void testChannelStringCheck_valid(void);
void testChannelStringCheck_invalid(void);

void testChannelDataToStr(void);
void testStrToChannelData(void);

#define NUMBER_OF_UTILITIES_TESTS	29

/* collection of all test für the utilities function */
static strTestDesc_t pstrUtilitiesTestSet[NUMBER_OF_UTILITIES_TESTS]= { 
	{testTrim_empty_string,						"Trim(): empty string"},
	{testTrim_no_space,							"Trim(): null spaces"}, 
	{testTrim_leading_space,					"Trim(): leading space"},
	{testTrim_multi_leading_space,				"Trim(): multi leading space"},
	{testTrim_newline,							"Trim(): newline and carage return"}, 
	{testTrim_only_whitespace, 					"Trim(): only whitespace"},
	{testTrim_idempotent,						"Trim(): idempotent"},
	{testTrim_sentence,							"Trim(): sentence"},
	{testStrToLower_empty,						"StrToLower(): empty string"},
	{testStrToLower_UpToLow,					"StrToLower(): full upper to lower"},
	{testStrToLower_LowToLow,					"StrToLower(): full lower to lower"},
	{testStrToLower_ascii_basic,				"StrToLower(): ascii basic string"},
	{testStrToLower_punctuation_digits_unchanged,"strToLower(): punctation digitis"},
	{testStrToLower_specials_and_umlauts,		"StrToLower(): special and umlauts"},
	{testStrToLower_idempotent,					"StrToLower(): idempotent"},
	{testclearspace_NOP,						"clearspace(): no spaces and nop"},
	{testclearspace_space_at_the_begin_and_end,	"clearspace(): Space at the begin and end"},
	{testclearspace_space_between,				"clearspace(): Space between"},
	{testclearspace_multi_space_between,		"clearspace(): Mutlispace between"},
	{testclearspace_space_quote_between,		"clearspace(): quoted spaces"},
	{testclearspace_space_quotes_mix,			"clearspace(): mx of quotes and none quotes"},
	{testclearspace_space_only,					"clearspace(): string with only spaces"},
	{testclearspace_space_only_and_quotes,		"clearspace(): Space with and without quotes"},
	{testNickStringCheck_valid,					"NickStringCheck(): valid nick string"},
	{testNickStringCheck_invalid,				"NickStringCheck(): invalid nick string"},
	{testChannelStringCheck_valid,				"ChannelStringCheck(): check validate channel string"},
	{testChannelStringCheck_invalid,			"ChannelStringCheck(): check invalidate channel string"},
	{testChannelDataToStr,						"ChannelDataToStr(): convert data of a channel to a string"},
	{testStrToChannelData,						"StrToChannelData(): convert string to  channel data"},
};

#endif
