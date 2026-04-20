/* #############################################################
 *
 *  This file is a part of ebotula testsuit.
 *
 *  Coypright (C)2023-2024 Steffen Laube <Laube.Steffen@gmx.de>
 *
 * #############################################################
 */

#include "testsuit.h"
#include "configparser.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#ifndef TESTSUIT_INCLUDE_CONFIGPARSER_TEST_H_
#define TESTSUIT_INCLUDE_CONFIGPARSER_TEST_H_

int init_configparser(void);
int clean_configparser(void);

void test_key_int_checker_valid_boundary(void);
void test_key_int_checker_below_min(void);
void test_key_server_checker_invalid(void);
void test_key_nick_checker_valid(void);
void test_key_nick_checker_invalid(void);
void test_ConfigFileParser_valid_config(void);
void test_ConfigFileParser_duplicate_key_uses_first_value(void);

#define NUMBER_OF_CONFIGPARSER_TESTS 7

static strTestDesc_t pstrConfigParserTestSet[NUMBER_OF_CONFIGPARSER_TESTS] = {
    {test_key_int_checker_valid_boundary,                "key_int_checker(): accept value on min boundary"},
    {test_key_int_checker_below_min,                     "key_int_checker(): reject value below min"},
    {test_key_server_checker_invalid,                    "key_server_checker(): reject invalid hostname"},
    {test_key_nick_checker_valid,                        "key_nick_checker(): accept valid botname"},
    {test_key_nick_checker_invalid,                      "key_nick_checker(): reject invalid botname"},
    {test_ConfigFileParser_valid_config,                 "ConfigFileParser(): parse valid configuration"},
    {test_ConfigFileParser_duplicate_key_uses_first_value, "ConfigFileParser(): duplicate key keeps first value"}
};

#endif /* TESTSUIT_INCLUDE_CONFIGPARSER_TEST_H_ */
