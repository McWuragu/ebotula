/** Queue functions		 **/
/** This Ring list -> Fifo 	**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"
#include "utilities.h"
#include "type.h"

/**							**
 ** Function: 		initQueue			**
 ** Parameters: 	PQueue *pqueueInit  	   	**
 **				Pointer to Queue	**
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Desctiption:		initing Queue,		**
 **				creating Sentinel	**
 **							**/
int initQueue(PQueue *pqueueInit)
{
	/** Initing Queue **/
	PQueue pqueueWork;
	
	/** Alocationg memory for base Structure **/
	if ((pqueueWork=(PQueue) malloc(sizeof(PQueue)))==NULL)
	{
		/** Error not enough memory **/
		DEBUG(" initQueue() - Can't allocate memory!\n");
		return QUEUE_MEMORY_ALLOC_ERROR;
	}
	
	/** creating Sentinel pointer **/
	(pqueueWork)->sentinel=(pqueueWork);
	(pqueueWork)->queuetypeT=(QueueType)QUEUESENTINEL;
	
	/** creating rest of pointer **/
	(pqueueWork)->next=(PQueue)(pqueueWork);
	(pqueueWork)->prev=(PQueue)(pqueueWork);
	(pqueueWork)->queuedataData=NULL;
	(pqueueWork)->longCount=0;
	
	/** create and init the miutex */
	(pqueueWork)->sentinel->QueueMutex=(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));	
	pthread_mutex_init((pqueueWork)->sentinel->QueueMutex,NULL);	
	
	/** Success - I've did it **/
	*pqueueInit=(PQueue)pqueueWork;


	return	QUEUE_SUCCESS;
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
int pushQueue(PQueue *pqueueIn, QueueData queuedataElement)
{
	/** lokal auto vars **/
	PQueue pqueueWork;
	PQueue pqueueNew;/** new Element **/
	
	int intStatus;
	pqueueWork=*pqueueIn;
	
	pthread_mutex_lock(pqueueWork->sentinel->QueueMutex);
	
	/** cheçk if Queue not initialized **/
	if (pqueueWork==NULL)
	{
		/** creating Queue and check for Status **/
		intStatus=initQueue(&pqueueWork);
		if (!intStatus==QUEUE_SUCCESS)
		{
			pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
			return intStatus;
		}
	}
	
	/** creating elements for Queue **/
	if ((pqueueNew=(PQueue)malloc(sizeof(PQueue)))==NULL)
	{
		/** Error not enough memory **/
		DEBUG(" FATAL: pushQueue() - Can't allocate memory!\n");
		pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
		return QUEUE_MEMORY_ALLOC_ERROR;
	}
	/** assembling Queue **/
	pqueueNew->sentinel=(PQueue)pqueueWork->sentinel;
	pqueueNew->next=(PQueue)pqueueWork->sentinel->next;
	if (pqueueWork->sentinel->longCount==0)
	{
		pqueueWork->sentinel->prev=(PQueue)pqueueNew;
	}
	else
	{
		pqueueWork->sentinel->next->prev=(PQueue)pqueueNew;
		
	}

	pqueueNew->prev=(PQueue)pqueueWork->sentinel;
	pqueueWork->sentinel->next=(PQueue)pqueueNew;
	pqueueNew->queuetypeT=(QueueType)QUEUENONE;
	pqueueWork->sentinel->longCount++;
	/** Adding Data **/
	if(( pqueueNew->queuedataData=(QueueData*) malloc(sizeof(QueueData*)))==NULL)
	{
		/** Error not enough memory **/
		DEBUG(" FATAL: pushQueue() - Can't allocate memory!\n");
		pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
		return QUEUE_MEMORY_ALLOC_ERROR;

	}
	memcpy(pqueueNew->queuedataData,(QueueData *)&queuedataElement,1*sizeof(QueueData*));
	pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
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
QueueData * popQueue(PQueue *pqueueIn)
{
	PQueue pqueueWork,pqueueOld;
	QueueData *queuedataElement;
	pqueueWork=(PQueue)*pqueueIn;
	/** check for valid Queue **/
	
	pthread_mutex_lock(pqueueWork->sentinel->QueueMutex);
	
	if (pqueueWork==NULL)
	{
		DEBUG(" ERROR: popQueue() - empty Queue!\n");
		pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
		return NULL;
	}
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
		pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
		return (QueueData *)queuedataElement;
	} 
	else
	{
		/** nothing to pop empty Queue **/
		pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
		return (QueueData *)NULL;
	}
	pthread_mutex_unlock(pqueueWork->sentinel->QueueMutex);
	return (QueueData *)NULL;
}
/**							**
 ** Function:		isemptyQueue			**
 ** Parameters:		PQueue *pqueueIn		**
 **				Pointer to Queue	**
 ** Return:		int BOOLEAN			**
 **				true if empty		**
 **							**/
int isemptyQueue(PQueue *pqueueIn)
{
	if (*pqueueIn!=NULL)
	{	
		/** Error not enough memory **/
		DEBUG(" FATAL: pushQueue() - Can't allocate memory!\n");
		return QUEUE_NULL_POINTER_AS_IN_PARAMETER;
	}
	if ((*pqueueIn)->sentinel->longCount==0)
	{
		/** SHOULD be empty **/
		return true;
	}
	return false;
}
/**							**
 ** Function:		isfullQueue			**
 ** Parameters:		PQueue *pqueueIn		**
 **				Pointer to Queue	**
 ** Return:		int BOOLEAN			**
 **				true if full 		**
 **							**/
int isfullQueue(PQueue *pqueueIn)
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
int deleteQueue(PQueue *pqueueIn)
{
	PQueue pqueueWork;
	
	pqueueWork=(PQueue)*pqueueIn;
	
	
	/** do it while something is in the queue **/
	if (pqueueWork!=NULL)
	{
		while(isfullQueue(&pqueueWork))
		{
			if (popQueue(&pqueueWork)!=NULL)
			{
				return QUEUE_NULL_POINTER_AS_IN_PARAMETER;

			}
		}
	}
	else return QUEUE_NULL_POINTER_AS_IN_PARAMETER;
	
	pthread_mutex_destroy(pqueueWork->sentinel->QueueMutex);	

	free(pqueueWork);
	pqueueWork=(PQueue)NULL;
	*pqueueIn=(PQueue )pqueueWork;
	return QUEUE_SUCCESS;
}

