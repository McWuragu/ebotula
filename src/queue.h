/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

/** Defining Queue defaults **/
/** This Ring list -> Fifo **/
#ifndef __H_QUEUE
#define __H_QUEUE

#include<pthread.h>

/** Internal Error defines **/
typedef enum {
    QUEUE_SUCCESS=0,
    QUEUE_ERROR=1, 
    QUEUE_MEMORY_ALLOC_ERROR=2,
    QUEUE_NO_ELEMENT_LEFT=3,
    QUEUE_NULL_POINTER_AS_IN_PARAMETER=4,
} QueueExeStatus;

/** types **/
/** Setting QueueType **/
/** Sentinel ist the beginning and the and of the queue **/
typedef enum {QUEUENONE=0,QUEUESENTINEL=1} QueueType;

/** Setting up data elements size must be the first element **/
typedef struct tag_QueueData
{
	long t_size;
	void * data;
}QueueData;
/** Queue elements - this the base structure of all Queue elements **/
typedef struct tag_Queue
{
	struct tag_Queue *next,*prev; /** Next and prev element **/
	struct tag_Queue *sentinel; /** pointer to the sentinel; To jump from every elment to the sentinel **/
	QueueType queuetypeT; /** Type of Queue **/
	QueueData *queuedataData; /**Data Elements **/
	
	unsigned long long longCount;
	
    /* Thread control */
    pthread_mutex_t *queue_mutex;  /** global access mutex */

    /* thread condition */
	unsigned long long ElementsInQueueForThreads;
	pthread_cond_t *StopThreadCond;
    
}Queue,*PQueue;


/**							**
 ** Function: 		initQueue			**
 ** Parameters: 	void		  	   	**
 ** Return:		PQueue	Pointer to Queue	**
 ** Desctiption:		initing Queue,		**
 **				creating Sentinel	**
 **							**/
PQueue initQueue();

/**							**
 ** Function: 		pushQueue			**
 ** Parameters: 	PQueue pqueueIn  	   	**
 **				Pointer to Queue	**
 **			QueueData queuedataElement	** 		
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Description:	pushing 1 element to Queue 	**
 **						 	**/
QueueExeStatus pushQueue(PQueue pqueueIn, QueueData queuedataElement);

/**							**
** Function: 		popQueue			**
** Parameters: 	PQueue pqueueIn  	   		**
**				Pointer to Queue	**
** Return:		QueueData *queuedataElement	** 		
** Description:	poping 1 element from Queue 		**
**			until Queue is empty		**
**							**/
QueueData * popQueue(PQueue pqueueIn);
/**							**
** Function: 		isemptyQueue			**
** Parameters: 	PQueue pqueueIn	  	   	**
**				Pointer to Queue	**
** Return:		int BOOLEAN			**
**				True if emtpy		**
**							**/
int isemptyQueue(PQueue pqueueIn);
/**							**
** Function: 		isfullQueue			**
** Parameters: 	PQueue pqueueIn	  	   	**
**				Pointer to Queue	**
** Return:		int BOOLEAN			**
**				True if full		**
**							**/
int isfullQueue(PQueue pqueueIn);
/**							**
 ** Function: 		deleteQueue			**
 ** Parameters: 	PQueue pqueueIn  	   	**
 **				Pointer to Queue	**
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Description:	deleting Queue 			**
 **			until Queue is empty		**
 **							**/
QueueExeStatus deleteQueue(PQueue pqueueIn);

/**							**
 ** Function: 		flushQueue			**
 ** Parameters: 	PQueue pqueueIn  	   	**
 **				Pointer to Queue	**
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Description:	flushing Queue 			**
 **			until Queue is empty		**
 **							**/
QueueExeStatus flushQueue(PQueue pqueueIn);
#endif
