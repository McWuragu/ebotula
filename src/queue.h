/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

/* Defining Queue defaults */
/* This Ring list -> Fifo */
#ifndef __H_QUEUE
#define __H_QUEUE

#include<pthread.h>

/** internal error defines */
typedef enum {
    QUEUE_SUCCESS=0,
    QUEUE_ERROR=1, 
    QUEUE_MEMORY_ALLOC_ERROR=2,
    QUEUE_NO_ELEMENT_LEFT=3,
    QUEUE_NULL_POINTER_AS_IN_PARAMETER=4,
} QueueExeStatus;

/* ##################### types ######################### */
/**
 * Setting QueueType 
 * Sentinel ist the beginning and the and of the queue 
 */
typedef enum {QUEUENONE=0,QUEUESENTINEL=1} QueueType;

/**
 * Setting up data elements size must be the first element 
 */
typedef struct tag_QueueData
{
	long t_size;
	void * data;
}QueueData;

/**
 *Queue elements - this the base structure of all Queue elements
 */
typedef struct tag_Queue
{
    /** Next and prev element */
	struct tag_Queue *next,*prev;
    /** pointer to the sentinel; To jump from every elment to the sentinel */
	struct tag_Queue *sentinel; 
    /** pointer to the iterator element; You can only jump from the sentinal 
     * to this element 
     * default iterator points at sentinel */
	struct tag_Queue *iterator;
	/** Type of Queue */
    QueueType queuetypeT;
    /** Data Elements */
    QueueData *queuedataData;
    /** number of items */
	unsigned long long longCount;
	
    /** Thread control */
    pthread_mutex_t *queue_mutex;  /* global access mutex */

    /** thread condition */
    pthread_cond_t *StopThreadCond;
    
}Queue,*PQueue;


/**
 * initialization and creating a empty queue
 *		  	   	
 * @return  pointer to queue	
 */
PQueue initQueue();

/**							
 * pushing 1 element to queue
 *			
 * @param pqueueIn pointer to queue	
 * @param queuedataElement pointer to the datum	 		
 * @return 0 if Success		
 */
QueueExeStatus pushQueue(PQueue pqueueIn, QueueData queuedataElement);

/**
 * popping one element from Queue until Queue is empty
 *							
 * @param pqueueIn pointer to queue	
 * @return pointer of the datum	 		
 */
QueueData * popQueue(PQueue pqueueIn);

/**							
 * checking the fill status of the queue
 *			
 * @Param pqueueIn pointer to queue	
 * @Return true if emtpy
 */
int isemptyQueue(PQueue pqueueIn);

/**							
 * checking the fill status of the queue
 *			
 * @Param pqueueIn pointer to queue	
 * @Return true if full
 */
int isfullQueue(PQueue pqueueIn);

/**
 * popping until queue is empty and deleting queue
 *							
 * @param pqueueIn pointer to queue	
 * @return 0 if success		
 */
QueueExeStatus deleteQueue(PQueue pqueueIn);

/**
 * flushing queue until queue is empty
 *   			
 * @param pqueueIn pointer to queue	
 * @return 0 if Success		
 */
QueueExeStatus flushQueue(PQueue pqueueIn);
/**
 * getting next parameter until sentinel
 *
 * @param pqueueIn pointer to queue
 * @return pointer of the datum	 		
 */
 QueueData * getNextitrQueue(PQueue pqueueIn);

#define SetIterToFirstQueue(_PQueue)     ((_PQueue)->Sentinel->Iterator = (_PQueue)->Sentinel)
#endif
