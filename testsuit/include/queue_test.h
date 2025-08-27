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
void test_isemptyQueue_after_push_pop(void);
void test_isemptyQueue_after_push_delete(void);
void test_isfullQueue(void);
void test_popQueue_after_push_not_null(void);
void test_popQueue_blocking(void);
void test_popQueue_multiple_push_pop(void);
void test_flushQueue_flush_empty_queue(void);
void test_flushQueue_flush_not_empty_queue(void);
void test_pushQueue(void);
void test_deleteQueue(void);
void test_getNextitrQueue(void);
void test_fifo_order(void);

#define NUMBER_OF_QUEUE_TESTS	15

static strTestDesc_t pstrQueueTestSet[NUMBER_OF_QUEUE_TESTS]= {
		{test_initQueue,						"initQueue(): testing the initalisation"},
		{test_isemptyQueue, 					"isemptyQueue(): testing  of empty queue"},
		{test_isemptyQueue_not, 				"isemptyQueue(): testing  of none empty queue"},
		{test_isemptyQueue_after_push_pop,		"isemptyQueue(): testing of empty queue after push and pop"},
		{test_isemptyQueue_after_push_delete,	"isemptyQueue(): testing of empty queue after push and delete"},
		{test_isfullQueue,						"isfullQueue(): testing  of a not empty queue"},
		{test_pushQueue,						"pushQueue(): testing of  pushing of the queue"},
		{test_popQueue_after_push_not_null,		"popQueue(): testing of popping a queue after push"},
		{test_popQueue_blocking,				"popQueue(): testing of blocking pop operation"},
		{test_popQueue_multiple_push_pop,		"popQueue(): testing of  multiple popping of a  queue"},
		{test_flushQueue_flush_empty_queue,		"flushQueue(): testing of flushing  of a empty queue"},
		{test_flushQueue_flush_not_empty_queue,	"flushQueue(): testing of flushing  of a not empty queue"},
		{test_deleteQueue,						"deleteQueue(): testing of deleting of a non empty queue"},
		{test_getNextitrQueue,					"getnextitrQueue(): testing  of getting the  nex iterator of the Queue"},
		{test_fifo_order,						"general test: validate the  fifo order"}
};

#endif /* TESTSUIT_INCLUDE_QUEUE_TEST_H_ */
