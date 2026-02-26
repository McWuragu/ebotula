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

#include <stdlib.h>
#include <string.h>
#include "extract_test.h"

int init_extract(void) {
    return 0;
}

int clean_extract(void) {
    return 0;
}

static void assert_access_channel(const char *line, const char *expected) {
    char *res = getAccessChannel(line);
    if (expected == NULL) {
        CU_ASSERT_PTR_NULL(res);
    } else {
        CU_ASSERT_PTR_NOT_NULL(res);
        if (res) {
            CU_ASSERT_STRING_EQUAL(res, expected);
        }
    }
    free(res);
}

void test_getNetmask_valid(void) {
    const char line[] = ":nick!user@host PRIVMSG #chan :hello";
    char *res = getNetmask(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, "nick!user@host");
    free(res);
}

void test_getNetmask_no_mask(void) {
    const char line[] = "PING :irc.example.org";
    char *res = getNetmask(line);

    CU_ASSERT_PTR_NULL(res);
}

void test_getNickname_valid(void) {
    const char line[] = ":nick!user@host PRIVMSG #chan :hello";
    char *res = getNickname(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, "nick");
    free(res);
}

void test_getCommand_valid(void) {
    const char line[] = ":server 332 #chan :Topic text";
    char *res = getCommand(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, ":server 332 #chan ");
    free(res);
}

void test_getArgument_valid(void) {
    const char line[] = ":nick!user@host PRIVMSG :!say hallo welt";
    char *res = getArgument(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, "hallo welt");
    free(res);
}

void test_getChannel_valid(void) {
    const char line[] = ":nick!user@host PRIVMSG #chan :!help";
    char *res = getChannel(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, "#chan");
    free(res);
}

void test_getAccessChannel_from_parameter(void) {
    const char line[] = ":nick!user@host PRIVMSG :!say #chan hello";
    assert_access_channel(line, "#chan");
}

void test_getTopic_getGreeting_getChannelMode(void) {
    const char set[] = "+nt key\tTopic text\tGreeting text";
    char *mode = getChannelMode(set);
    char *topic = getTopic(set);
    char *greeting = getGreeting(set);

    CU_ASSERT_PTR_NOT_NULL(mode);
    CU_ASSERT_PTR_NOT_NULL(topic);
    CU_ASSERT_PTR_NOT_NULL(greeting);
    CU_ASSERT_STRING_EQUAL(mode, "+nt key");
    CU_ASSERT_STRING_EQUAL(topic, "Topic text");
    CU_ASSERT_STRING_EQUAL(greeting, "Greeting text");

    free(mode);
    free(topic);
    free(greeting);
}

void test_getParameters_no_channel(void) {
    const char line[] = ":nick!user@host PRIVMSG :!help foo bar";
    char *res = getParameters(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, "foo bar");
    free(res);
}

void test_getParameters_with_channel(void) {
    const char line[] = ":nick!user@host PRIVMSG :!say #chan foo bar";
    char *res = getParameters(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, "foo bar");
    free(res);
}

void test_getFirstPart_split(void) {
    const char line[] = "one two three";
    char *rest = NULL;
    char *first = getFirstPart(line, &rest);

    CU_ASSERT_PTR_NOT_NULL(first);
    CU_ASSERT_PTR_NOT_NULL(rest);
    CU_ASSERT_STRING_EQUAL(first, "one");
    CU_ASSERT_STRING_EQUAL(rest, "two three");

    free(first);
    free(rest);
}

void test_getBanmask_valid(void) {
    const char line[] = ":nick!user@sub.host.tld PRIVMSG #chan :hello";
    char *res = getBanmask(line);

    CU_ASSERT_PTR_NOT_NULL(res);
    CU_ASSERT_STRING_EQUAL(res, "*!user@*.host.tld");
    free(res);
}

void test_getAnswerMode_notice(void) {
    const char line[] = ":nick!user@host PRIVMSG #chan :!help";
    CU_ASSERT_EQUAL(getAnswerMode(line), NoticeMode);
}

void test_getAnswerMode_privmsg(void) {
    const char line[] = ":nick!user@host PRIVMSG :!help";
    CU_ASSERT_EQUAL(getAnswerMode(line), PrvMsgMode);
}

void test_getArgument_without_parameter(void) {
    const char line[] = ":nick!user@host PRIVMSG :!help";
    char *res = getArgument(line);

    CU_ASSERT_PTR_NULL(res);
}

void test_getParameters_only_channel(void) {
    const char line[] = ":nick!user@host PRIVMSG :!say #chan";
    char *res = getParameters(line);

    CU_ASSERT_PTR_NULL(res);
}

void test_getFirstPart_without_delimiter(void) {
    const char line[] = "singleword";
    char *rest = NULL;
    char *first = getFirstPart(line, &rest);

    CU_ASSERT_PTR_NOT_NULL(first);
    CU_ASSERT_PTR_NULL(rest);
    CU_ASSERT_STRING_EQUAL(first, "singleword");

    free(first);
}

void test_getAccessChannel_invalid_parameter_channel(void) {
    const char line[] = ":nick!user@host PRIVMSG :!say !chan hello";
    assert_access_channel(line, NULL);
}


void test_getChannel_without_channel(void) {
    const char line[] = ":server NOTICE nick :message";
    char *res = getChannel(line);

    CU_ASSERT_PTR_NULL(res);
}

void test_getParameters_null_input(void) {
    CU_ASSERT_PTR_NULL(getParameters(NULL));
}

void test_getAccessChannel_from_direct_text(void) {
    const char line[] = ":nick!user@host PRIVMSG :#mychan hello";
    assert_access_channel(line, "#mychan");
}
