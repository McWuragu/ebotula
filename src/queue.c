/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

/** Queue functions		 **/
/** This Ring list -> Fifo 	**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utilities.h"
#include "type.h"
#include "queue.h"

/**							**
 ** Function: 		initQueue			**
 ** Parameters: 	PQueue *pqueueInit  	   	**
 **				Pointer to Queue	**
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Desctiption:		initing Queue,		**
 **				creating Sentinel	**
 **							**/
PQueue initQueue()
{
	PQueue pqueueInit;
	
	/** Alocationg memory for base Structure **/
	if (!(pqueueInit=(PQueue)malloc(sizeof(Queue))))
	{
		/** Error not enough memory **/
		DEBUG("initQueue() - Can't allocate memory!");
		return NULL;
	}
	
	/** creating Sentinel pointer **/
	pqueueInit->sentinel=pqueueInit;
	pqueueInit->queuetypeT=(QueueType)QUEUESENTINEL;
	
	/** creating rest of pointer **/
	pqueueInit->next=pqueueInit;
	pqueueInit->prev=pqueueInit;
	pqueueInit->queuedataData=NULL;
	pqueueInit->longCount=0;
	
	/** create and init the  mutex */
	pqueueInit->queue_mutex=(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(pqueueInit->queue_mutex,NULL);	
	/** create and init Threadcontrolmutex */
	pqueueInit->ElementsInQueueForThreads=0;
	pqueueInit->StopThreadCond=(pthread_cond_t *)malloc(sizeof(pthread_cond_t));
	pthread_cond_init(pqueueInit->StopThreadCond,NULL);

	return	pqueueInit;
}

/**							**
 ** Function: 		pushQueue			**
 ** Parameters: 	PQueue *pqueueIn  	   	**
 **				Pointer to Queue	**
 **			QueueData queuedataElement	** 		
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Description:	pushing 1 element to Queue 	**
 **							**/
int pushQueue(PQueue pqueueIn, QueueData queuedataElement)
{
	/** lokal auto vars **/
	PQueue pqueueNew;/** new Element **/
	
	int intStatus;
	
	
	pthread_mutex_lock(pqueueIn->sentinel->queue_mutex);	
	while(pqueueIn->sentinel->ElementsInQueueForThreads>= MAX_ELEMENTS_IN_QUEUE_FOR_THREADS )
		pthread_cond_wait(pqueueIn->sentinel->queue_mutex,pqueueIn->sentinel->StopThreadCond);
	/** cheçk if Queue not initialized **/
	if (!pqueueIn)
	{
		/** creating Queue and check for Status **/
		;
		if (!(pqueueIn=initQueue()))
		{
			pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
			return QUEUE_MEMORY_ALLOC_ERROR;
		}
	}
	
	/** creating elements for Queue **/
	if ((pqueueNew=(PQueue)malloc(sizeof(Queue)))==NULL)
	{
		/** Error not enough memory **/
		DEBUG("pushQueue() - Can't allocate memory!");
			pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
		return QUEUE_MEMORY_ALLOC_ERROR;
	}
	/** assembling Queue **/
	pqueueNew->sentinel=pqueueIn->sentinel;
	pqueueNew->next=pqueueIn->sentinel->next;
	pqueueNew->queue_mutex=NULL;
	pqueueNew->StopThreadCond=NULL;

	if (pqueueIn->sentinel->longCount==0)
	{
		pqueueIn->sentinel->prev=(PQueue)pqueueNew;
	}
	else
	{
		pqueueIn->sentinel->next->prev=(PQueue)pqueueNew;
		
	}

	pqueueNew->prev=pqueueIn->sentinel;
	pqueueIn->sentinel->next=pqueueNew;
	pqueueNew->queuetypeT=(QueueType)QUEUENONE;
	pqueueIn->sentinel->longCount++;

	/** Adding Data **/
	if(( pqueueNew->queuedataData=(QueueData*) malloc(sizeof(QueueData)))==NULL)
	{
		/** Error not enough memory **/
		DEBUG("pushQueue() - Can't allocate memory!");
		pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
		return QUEUE_MEMORY_ALLOC_ERROR;

	}

	/** allocate the memmory and copy data */
	pqueueNew->queuedataData->t_size=queuedataElement.t_size;
	pqueueNew->queuedataData->data=malloc(sizeof(QueueData));	
	(pqueueIn->sentinel->ElementsInQueueForThreads)++;
	memcpy(pqueueNew->queuedataData->data,queuedataElement.data,sizeof(QueueData));
	pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
	return QUEUE_SUCCESS;
}
/**							**
 ** Function: 		popQueue			**
 ** Parameters: 	PQueue *pqueueIn  	   	**
 **				Pointer to Queue	**
 ** Return:		QueueData *queuedataElement	** 		
 **				0 if Success		**
 ** Description:	poping 1 element from Queue 	**
 **			until Queue is empty		**
 **							**/
QueueData * popQueue(PQueue pqueueIn)
{
	PQueue pqueueWork;
	PQueue pqueueOld;
	QueueData *queuedataElement;
	pqueueWork=pqueueIn;
	
	pthread_mutex_lock(pqueueIn->sentinel->queue_mutex);	
	// THIS IS A VERY BAD MISTAKE
	// Because this is a Deat-End for Threads
//	while(pqueueIn->sentinel->ElementsInQueueForThreads<=0 )
//		pthread_cond_wait(pqueueIn->sentinel->queue_mutex,pqueueIn->sentinel->StopThreadCond);

	/** check for valid Queue **/
	if (pqueueWork)
	{
		pqueueWork=(PQueue )pqueueWork->sentinel;
		if (pqueueWork->sentinel->prev!=(PQueue)pqueueWork->sentinel)
		{
			/** poping **/
			pqueueOld=(PQueue)pqueueWork->prev;
			queuedataElement=(QueueData *)pqueueOld->queuedataData;
			pqueueWork->prev=(PQueue )pqueueOld->prev;
			pqueueWork->prev->next=(PQueue )pqueueWork->sentinel;
			pqueueWork->sentinel->longCount--;
			free(pqueueOld);

			(pqueueIn->sentinel->ElementsInQueueForThreads)--;
			pthread_cond_signal(pqueueIn->sentinel->StopThreadCond);
			pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
			return (QueueData *)queuedataElement;
		}	
	} else {
		DEBUG("ERROR: popQueue() - empty Queue!");
	}

	pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
	return (QueueData *)NULL;
}
/**							**
 ** Function:		isemptyQueue			**
 ** Parameters:		PQueue *pqueueIn		**
 **				Pointer to Queue	**
 ** Return:		int BOOLEAN			**
 **				true if empty		**
 **							**/
int isemptyQueue(PQueue pqueueIn)
{
	if (pqueueIn) {	
		pthread_mutex_lock(pqueueIn->sentinel->queue_mutex);	
		if ((pqueueIn)->sentinel->longCount){
			/** SHOULD be not empty **/
			pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
			return false;
		}

		pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
	}
	return true;
}
/**							**
 ** Function:		isfullQueue			**
 ** Parameters:		PQueue *pqueueIn		**
 **				Pointer to Queue	**
 ** Return:		int BOOLEAN			**
 **				true if full 		**
 **							**/
int isfullQueue(PQueue pqueueIn)
{
	return !isemptyQueue(pqueueIn);
}
/**							**
 ** Function: 		deleteQueue			**
 ** Parameters: 	PQueue *pqueueIn  	   	**
 **				Pointer to Queue	**
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Description:	deleting Queue 			**
 **			until Queue is empty		**
 **							**/
int deleteQueue(PQueue pqueueIn)
{
	QueueData *pTmp;
	
	/** do it while something is in the queue **/
	if (pqueueIn!=NULL)
	{
		while(isfullQueue(pqueueIn))
		{
			/** remove one  item and destroy it */
			if((pTmp=popQueue(pqueueIn))){
				free(pTmp);
			}
		}
	}
	else return QUEUE_NULL_POINTER_AS_IN_PARAMETER;
	
	pthread_mutex_destroy(pqueueIn->sentinel->queue_mutex);
	free(pqueueIn->sentinel->queue_mutex);
	free(pqueueIn);
	return QUEUE_SUCCESS;
}

