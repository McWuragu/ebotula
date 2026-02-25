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
#include "extract.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#ifndef TESTSUIT_INCLUDE_EXTRACT_TEST_H_
#define TESTSUIT_INCLUDE_EXTRACT_TEST_H_

int init_extract(void);
int clean_extract(void);

void test_getNetmask_valid(void);
void test_getNetmask_no_mask(void);
void test_getNickname_valid(void);
void test_getCommand_valid(void);
void test_getArgument_valid(void);
void test_getChannel_valid(void);
void test_getAccessChannel_from_parameter(void);
void test_getTopic_getGreeting_getChannelMode(void);
void test_getParameters_no_channel(void);
void test_getParameters_with_channel(void);
void test_getFirstPart_split(void);
void test_getBanmask_valid(void);
void test_getAnswerMode_notice(void);
void test_getAnswerMode_privmsg(void);
void test_getArgument_without_parameter(void);
void test_getParameters_only_channel(void);
void test_getFirstPart_without_delimiter(void);
void test_getAccessChannel_invalid_parameter_channel(void);

#define NUMBER_OF_EXTRACT_TESTS 18

static strTestDesc_t pstrExtractTestSet[NUMBER_OF_EXTRACT_TESTS] = {
    {test_getNetmask_valid,                     "getNetmask(): valid netmask"},
    {test_getNetmask_no_mask,                   "getNetmask(): no netmask in line"},
    {test_getNickname_valid,                    "getNickname(): valid nickname"},
    {test_getCommand_valid,                     "getCommand(): preamble extraction"},
    {test_getArgument_valid,                    "getArgument(): argument extraction"},
    {test_getChannel_valid,                     "getChannel(): channel extraction"},
    {test_getAccessChannel_from_parameter,      "getAccessChannel(): channel from parameter"},
    {test_getTopic_getGreeting_getChannelMode,  "channel-set parsing: mode/topic/greeting"},
    {test_getParameters_no_channel,             "getParameters(): direct parameters"},
    {test_getParameters_with_channel,           "getParameters(): parameters after channel"},
    {test_getFirstPart_split,                   "getFirstPart(): split token and rest"},
    {test_getBanmask_valid,                     "getBanmask(): build banmask"},
    {test_getAnswerMode_notice,                 "getAnswerMode(): notice mode for channel"},
    {test_getAnswerMode_privmsg,                "getAnswerMode(): private message mode"},
    {test_getArgument_without_parameter,         "getArgument(): command without argument"},
    {test_getParameters_only_channel,            "getParameters(): only channel parameter"},
    {test_getFirstPart_without_delimiter,        "getFirstPart(): line without space delimiter"},
    {test_getAccessChannel_invalid_parameter_channel, "getAccessChannel(): invalid channel in parameter"}
};

#endif /* TESTSUIT_INCLUDE_EXTRACT_TEST_H_ */
