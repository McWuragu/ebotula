/** Queue functions		 **/
/** This Ring list -> Fifo 	**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"
#include "utilities.h"

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
	/** cheçk if Queue not initialized **/
	if (pqueueWork==NULL)
	{
		/** creating Queue and check for Status **/
		intStatus=initQueue(&pqueueWork);
		if (!intStatus==QUEUE_SUCCESS)
		{
			return intStatus;
		}
	}
	
	/** creating elements for Queue **/
	if ((pqueueNew=(PQueue)malloc(sizeof(PQueue)))==NULL)
	{
		/** Error not enough memory **/
		DEBUG(" FATAL: pushQueue() - Can't allocate memory!\n");
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
		return QUEUE_MEMORY_ALLOC_ERROR;

	}
	memcpy(pqueueNew->queuedataData,(QueueData *)&queuedataElement,1*sizeof(QueueData*));
	return QUEUE_SUCCESS;
}
/**							**
 ** Function: 		popQueue			**
 ** Parameters: 	PQueue *pqueueIn  	   	**
 **				Pointer to Queue	**
 **			QueueData *queuedataElement	** 		
 ** Return:		int STATUS			**
 **				0 if Success		**
 ** Description:	poping 1 element from Queue 	**
 **			until Queue is empty		**
 **							**/
int popQueue(PQueue *pqueueIn, QueueData *queuedataElement)
{
	PQueue pqueueWork,pqueueOld;
	
	pqueueWork=(PQueue)*pqueueIn;
	/** check for valid Queue **/
	if (pqueueWork==NULL)
	{
		DEBUG(" ERROR: popQueue() - empty Queue!\n");
		return QUEUE_NULL_POINTER_AS_IN_PARAMETER;
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
	} 
	else
	{
		/** nothing to pop empty Queue **/
		queuedataElement=NULL;
	}
	return QUEUE_SUCCESS;
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
	if ((*pqueueIn)->longCount==0)
	{
		/** SHOULD be empty **/
		return True;
	}
	return False;
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
	QueueData data;	
	pqueueWork=(PQueue)*pqueueIn;
	/** do it while something is in the queue **/
	if (pqueueWork!=NULL)
	{
		while(isfullQueue(&pqueueWork))
		{
			if (popQueue(&pqueueWork,&data))
			{
				return QUEUE_NULL_POINTER_AS_IN_PARAMETER;

			}
		}
	}
	else return QUEUE_NULL_POINTER_AS_IN_PARAMETER;
	free(pqueueWork);
	pqueueWork=(PQueue)NULL;
	*pqueueIn=(PQueue )pqueueWork;
	return QUEUE_SUCCESS;
}

