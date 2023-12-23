/*
 * queue_test.c
 *
 *  Created on: 17.12.2023
 *      Author: slaube
 */

#include "queue_test.h"

int init_queue(void) {
	return 0;
}

int clean_queue(void) {
	return 0;
}


void test_initQueue(void) {
	CU_ASSERT_PTR_NOT_NULL(initQueue());
}

void test_isemptyQueue(void) {
	PQueue Queue;
	Queue = initQueue();

	CU_ASSERT_TRUE(isemptyQueue(Queue));
}

void test_isemptyQueue_not(void) {
	PQueue Queue;
	Queue = initQueue();

	QueueData testItem;

	pushQueue(Queue,testItem);

	CU_ASSERT_FALSE(isemptyQueue(Queue));
}

void test_isemptyQueue_after_push_pop(void) {
	PQueue Queue;
	Queue = initQueue();

	QueueData testItem;

	pushQueue(Queue,testItem);
	popQueue(Queue);

	CU_ASSERT_TRUE(isemptyQueue(Queue));
}

void test_isemptyQueue_after_push_delete(void) {
	PQueue Queue;
	Queue = initQueue();

	QueueData testItem;

	pushQueue(Queue,testItem);
	pushQueue(Queue,testItem);
	deleteQueue(Queue);

	CU_ASSERT_TRUE(isemptyQueue(Queue));
}
void test_isfullQueue(void) {
	PQueue Queue;
	Queue = initQueue();

	QueueData testItem;

	pushQueue(Queue,testItem);

	CU_ASSERT_TRUE(isfullQueue(Queue));
}
