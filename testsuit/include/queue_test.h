/*
 * queue_test.h
 *
 *  Created on: 17.12.2023
 *      Author: slaube
 */
#include "testsuit.h"
#include "queue.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

#ifndef TESTSUIT_INCLUDE_QUEUE_TEST_H_
#define TESTSUIT_INCLUDE_QUEUE_TEST_H_

int init_queue(void);
int clean_queue(void);

void test_initQueue(void);
void test_isemptyQueue(void);
void test_isemptyQueue_not(void);
void test_isemptyQueue_after_oush_pop(void);
void test_isfullQueue(void);

#define NUMBER_OF_QUEUE_TESTS	5

static strTestDesc_t pstrQueueTestSet[NUMBER_OF_QUEUE_TESTS]= {
		{test_initQueue,	"initQueue(): testing the initalisation"},
		{test_isemptyQueue, "isemptyQueue(): testing  of empty queue"},
		{test_isemptyQueue_not, "isemptyQueue(): testing  of none empty queue"},
		{test_isemptyQueue_after_oush_pop,	"isemptyQueue(): testing of none empty queue afterpush and pop"},
		{test_isfullQueue,		"isfullQueue(): testing  of a not empty queue"},
};

#endif /* TESTSUIT_INCLUDE_QUEUE_TEST_H_ */
