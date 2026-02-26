/* #############################################################
 *
 *  This file is a part of ebotula testsuit.
 *
 *  Coypright (C)2023-2024 Steffen Laube <Laube.Steffen@gmx.de>
 *
 * #############################################################
 */

#include <stdlib.h>
#include <string.h>
#include "callbacklist_test.h"

static CallbackDList sList;
static void destroy_item(CallbackItem_t *data);

static void reset_list(void) {
    destroyCallbackDList(&sList);
    init_extended_CallbackDList(&sList, destroy_item);
}

static CallbackItem_t *make_item(const char *nick) {
    CallbackItem_t *item = (CallbackItem_t *)malloc(sizeof(CallbackItem_t));
    if (!item) {
        return NULL;
    }

    item->nickname = (char *)malloc(strlen(nick) + 1);
    if (!item->nickname) {
        free(item);
        return NULL;
    }

    strcpy(item->nickname, nick);
    item->CallbackFkt = NULL;
    item->data = NULL;

    return item;
}

static void destroy_item(CallbackItem_t *data) {
    if (data) {
        free(data->nickname);
        free(data);
    }
}

int init_callbacklist(void) {
    init_extended_CallbackDList(&sList, destroy_item);
    return 0;
}

int clean_callbacklist(void) {
    destroyCallbackDList(&sList);
    return 0;
}

void test_callbacklist_init(void) {
    reset_list();
    CU_ASSERT_EQUAL(getSizeCallbackDList(&sList), 0);
    CU_ASSERT_PTR_NULL(getHeadCallbackDList(&sList));
    CU_ASSERT_PTR_NULL(getTailCallbackDList(&sList));
}

void test_callbacklist_push_and_search_head(void) {
    reset_list();
    CallbackItem_t *a = make_item("Alice");
    CallbackItem_t *b = make_item("Bob");

    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(b);

    CU_ASSERT_EQUAL(pushCallbackDList(&sList, a), 0);
    CU_ASSERT_EQUAL(pushCallbackDList(&sList, b), 0);
    CU_ASSERT_EQUAL(getSizeCallbackDList(&sList), 2);

    CallbackDListItem *found = searchNicknameFromHeadCallbackDList(&sList, "bob");
    CU_ASSERT_PTR_NOT_NULL(found);
    if (found) {
        CU_ASSERT_PTR_EQUAL(found, getTailCallbackDList(&sList));
        CU_ASSERT_STRING_EQUAL(getDataCallbackDList(found)->nickname, "Bob");
    }
}

void test_callbacklist_insert_prev_before_tail(void) {
    reset_list();
    CallbackItem_t *a = make_item("Alpha");
    CallbackItem_t *c = make_item("Charlie");
    CallbackItem_t *b = make_item("Beta");

    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);
    CU_ASSERT_PTR_NOT_NULL_FATAL(b);

    CU_ASSERT_EQUAL(pushCallbackDList(&sList, a), 0);
    CU_ASSERT_EQUAL(pushCallbackDList(&sList, c), 0);

    CU_ASSERT_EQUAL(insert_prev_CallbackDList(&sList, getTailCallbackDList(&sList), b), 0);
    CU_ASSERT_EQUAL(getSizeCallbackDList(&sList), 3);

    CU_ASSERT_PTR_NOT_NULL(getHeadCallbackDList(&sList));
    CU_ASSERT_PTR_NOT_NULL(getTailCallbackDList(&sList));
    CU_ASSERT_STRING_EQUAL(getDataCallbackDList(getHeadCallbackDList(&sList))->nickname, "Alpha");
    CU_ASSERT_STRING_EQUAL(getDataCallbackDList(getTailCallbackDList(&sList))->nickname, "Charlie");

    CallbackDListItem *mid = getNextCallbackDList(getHeadCallbackDList(&sList));
    CU_ASSERT_PTR_NOT_NULL(mid);
    if (mid) {
        CU_ASSERT_STRING_EQUAL(getDataCallbackDList(mid)->nickname, "Beta");
    }
}

void test_callbacklist_remove_tail(void) {
    reset_list();
    CallbackItem_t *a = make_item("One");
    CallbackItem_t *b = make_item("Two");
    CallbackItem_t *removed = NULL;

    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(b);

    CU_ASSERT_EQUAL(pushCallbackDList(&sList, a), 0);
    CU_ASSERT_EQUAL(pushCallbackDList(&sList, b), 0);

    CU_ASSERT_EQUAL(removeCallbackDList(&sList, getTailCallbackDList(&sList), &removed), 0);
    CU_ASSERT_PTR_NOT_NULL(removed);
    CU_ASSERT_STRING_EQUAL(removed->nickname, "Two");
    CU_ASSERT_EQUAL(getSizeCallbackDList(&sList), 1);
    CU_ASSERT_PTR_EQUAL(getHeadCallbackDList(&sList), getTailCallbackDList(&sList));

    destroy_item(removed);
}

void test_callbacklist_search_not_found(void) {
    reset_list();
    CallbackItem_t *a = make_item("Alice");

    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_EQUAL(pushCallbackDList(&sList, a), 0);

    CU_ASSERT_PTR_NULL(searchNicknameFromHeadCallbackDList(&sList, "Charlie"));
}

void test_callbacklist_search_from_tail_case_insensitive(void) {
    reset_list();
    CallbackItem_t *a = make_item("Alpha");
    CallbackItem_t *b = make_item("Beta");
    CallbackItem_t *c = make_item("Gamma");

    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(b);
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    CU_ASSERT_EQUAL(pushCallbackDList(&sList, a), 0);
    CU_ASSERT_EQUAL(pushCallbackDList(&sList, b), 0);
    CU_ASSERT_EQUAL(pushCallbackDList(&sList, c), 0);

    CallbackDListItem *found = searchNicknameFromTailCallbackDList(&sList, "beta");
    CU_ASSERT_PTR_NOT_NULL(found);
    if (found) {
        CU_ASSERT_STRING_EQUAL(getDataCallbackDList(found)->nickname, "Beta");
    }
}

void test_callbacklist_remove_head_updates_links(void) {
    reset_list();
    CallbackItem_t *a = make_item("First");
    CallbackItem_t *b = make_item("Second");
    CallbackItem_t *removed = NULL;

    CU_ASSERT_PTR_NOT_NULL_FATAL(a);
    CU_ASSERT_PTR_NOT_NULL_FATAL(b);

    CU_ASSERT_EQUAL(pushCallbackDList(&sList, a), 0);
    CU_ASSERT_EQUAL(pushCallbackDList(&sList, b), 0);

    CU_ASSERT_EQUAL(removeCallbackDList(&sList, getHeadCallbackDList(&sList), &removed), 0);
    CU_ASSERT_PTR_NOT_NULL(removed);
    CU_ASSERT_STRING_EQUAL(removed->nickname, "First");
    CU_ASSERT_EQUAL(getSizeCallbackDList(&sList), 1);

    CU_ASSERT_PTR_NOT_NULL(getHeadCallbackDList(&sList));
    CU_ASSERT_PTR_EQUAL(getHeadCallbackDList(&sList), getTailCallbackDList(&sList));
    CU_ASSERT_STRING_EQUAL(getDataCallbackDList(getHeadCallbackDList(&sList))->nickname, "Second");

    destroy_item(removed);
}
