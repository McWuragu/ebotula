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

	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	CU_ASSERT_PTR_NOT_NULL(popQueue(Queue));

	CU_ASSERT_TRUE(isemptyQueue(Queue));
}

void test_isemptyQueue_after_push_delete(void) {
	PQueue Queue;
	Queue = initQueue();

	QueueData testItem;

	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	CU_ASSERT_EQUAL(deleteQueue(Queue), QUEUE_SUCCESS);

	CU_ASSERT_TRUE(isemptyQueue(Queue));
}
void test_isfullQueue(void) {
	PQueue Queue;
	Queue = initQueue();

	QueueData testItem;

	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);

	CU_ASSERT_TRUE(isfullQueue(Queue));

}

void test_popQueue_after_push_not_null(void) {
	PQueue Queue;
	Queue = initQueue();

	QueueData testItem;

	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	
	CU_ASSERT_PTR_NOT_NULL(popQueue(Queue));
}


void test_flushQueue_flush_empty_queue(void) {
	PQueue Queue;
	Queue = initQueue();

	CU_ASSERT_EQUAL(flushQueue(Queue), QUEUE_SUCCESS);

	CU_ASSERT_TRUE(isemptyQueue(Queue));
}

void test_flushQueue_flush_not_empty_queue(void) {
	PQueue Queue;
	QueueData testItem;

	Queue = initQueue();

	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);
	CU_ASSERT_EQUAL(pushQueue(Queue,testItem), QUEUE_SUCCESS);

	CU_ASSERT_EQUAL(flushQueue(Queue), QUEUE_SUCCESS);

	CU_ASSERT_TRUE(isemptyQueue(Queue));
}


