/* #############################################################
 *
 *  This file is a part of ebotula testsuit.
 *
 *  Coypright (C)2023-2024 Steffen Laube <Laube.Steffen@gmx.de>
 *
 * #############################################################
 */

#include "testsuit.h"
#include <pthread.h>
#include "callbacklist.h"
#include "callback.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#ifndef TESTSUIT_INCLUDE_CALLBACKLIST_TEST_H_
#define TESTSUIT_INCLUDE_CALLBACKLIST_TEST_H_

int init_callbacklist(void);
int clean_callbacklist(void);

void test_callbacklist_init(void);
void test_callbacklist_push_and_search_head(void);
void test_callbacklist_insert_prev_before_tail(void);
void test_callbacklist_remove_tail(void);
void test_callbacklist_search_not_found(void);
void test_callbacklist_search_from_tail_case_insensitive(void);
void test_callbacklist_remove_head_updates_links(void);

#define NUMBER_OF_CALLBACKLIST_TESTS 7

static strTestDesc_t pstrCallbackListTestSet[NUMBER_OF_CALLBACKLIST_TESTS] = {
    {test_callbacklist_init,                         "CallbackList: init state"},
    {test_callbacklist_push_and_search_head,         "CallbackList: push and search from head"},
    {test_callbacklist_insert_prev_before_tail,      "CallbackList: insert prev before tail"},
    {test_callbacklist_remove_tail,                  "CallbackList: remove tail returns data"},
    {test_callbacklist_search_not_found,             "CallbackList: search returns null when missing"},
    {test_callbacklist_search_from_tail_case_insensitive, "CallbackList: search from tail case insensitive"},
    {test_callbacklist_remove_head_updates_links,    "CallbackList: remove head keeps links consistent"}
};

#endif /* TESTSUIT_INCLUDE_CALLBACKLIST_TEST_H_ */
