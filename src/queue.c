/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2023 Steffen Laube <Laube.Steffen@gmx.de>
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

/** Queue functions		 **/
/** This Ring list -> Fifo 	**/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utilities.h"
#include "type.h"
#include "queue.h"

/* ############################################################################# */
PQueue initQueue()
{
	PQueue pqueueInit;
	
	/** Alocationg memory for base Structure **/
	if (!(pqueueInit=(PQueue)malloc(sizeof(Queue))))
	{
		/** Error not enough memory **/
		logger(LOG_ERR,_("Couldn't allocate memory!"));
        return NULL;
	}
	
	/** creating Sentinel pointer **/
	pqueueInit->sentinel=pqueueInit;
	pqueueInit->queuetypeT=(QueueType)QUEUESENTINEL;
	/* iterator not set because Queue is empty */
	pqueueInit->iterator=pqueueInit;	
	/** creating rest of pointer **/
	pqueueInit->next=pqueueInit;
	pqueueInit->prev=pqueueInit;
	pqueueInit->queuedataData=NULL;
	pqueueInit->longCount=0;
	
	/** create and init the  mutex */
	pqueueInit->queue_mutex=(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(pqueueInit->queue_mutex,NULL);	
	
    /** create and init Threadcontrolmutex */
	pqueueInit->StopThreadCond=(pthread_cond_t *)malloc(sizeof(pthread_cond_t));
	pthread_cond_init(pqueueInit->StopThreadCond,NULL);

	return	pqueueInit;
}

/* ############################################################################# */
QueueExeStatus pushQueue(PQueue pqueueIn, QueueData queuedataElement)
{
	/** lokal auto vars **/
	PQueue pqueueNew;/** new Element **/
	
	/*int intStatus;*/
	
	
    pthread_mutex_lock(pqueueIn->sentinel->queue_mutex);	
	/** che�k if Queue not initialized **/
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
		logger(LOG_ERR,_("Couldn't allocate memory!"));
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
		logger(LOG_ERR,_("Couldn't allocate memory!"));
        pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
		return QUEUE_MEMORY_ALLOC_ERROR;

	}

	/** allocate the memory and copy data */
	pqueueNew->queuedataData->t_size=queuedataElement.t_size;
	pqueueNew->queuedataData->data=malloc(queuedataElement.t_size);	
	
    memcpy(pqueueNew->queuedataData->data,queuedataElement.data,queuedataElement.t_size);
    
    /* increament  the counter for the reading  threads*/
    pthread_cond_signal(pqueueIn->sentinel->StopThreadCond);

    pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
	return QUEUE_SUCCESS;
}
/* ############################################################################# */
QueueData * popQueue(PQueue pqueueIn)
{
	PQueue pqueueWork;
	PQueue pqueueOld;
	QueueData *queuedataElement;
	pqueueWork=pqueueIn;
	
    
    /* protect the queue access */
	pthread_mutex_lock(pqueueIn->sentinel->queue_mutex);	
	
    /* condition handling */
    if (pqueueIn->sentinel->longCount<=0) {
        pthread_cond_wait(pqueueIn->sentinel->StopThreadCond,pqueueIn->sentinel->queue_mutex);
    }

    /* check for valid Queue */
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

			
            pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
			return (QueueData *)queuedataElement;
		}	
	} else {
		logger(LOG_DEBUG,_("Queue is empty"));
	}

	pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
	return (QueueData *)NULL;
}
/* ############################################################################# */
int isemptyQueue(PQueue pqueueIn)
{
    boolean ret=true;

    pthread_mutex_lock(pqueueIn->sentinel->queue_mutex);	
	if (pqueueIn) {	
		ret=(pqueueIn)->sentinel->longCount?false:true;
	}
    pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
	return ret;
}
/* ############################################################################# */
int isfullQueue(PQueue pqueueIn)
{
	return !isemptyQueue(pqueueIn);
}
/* ############################################################################# */
QueueExeStatus deleteQueue(PQueue pqueueIn)
{
    QueueExeStatus err;

    err=flushQueue(pqueueIn);
	if (err) return err;

	
    /* destroy all mutex and  conditions of the  queue */
	pthread_mutex_destroy(pqueueIn->sentinel->queue_mutex);
    pthread_cond_destroy(pqueueIn->sentinel->StopThreadCond);
    
    free(pqueueIn->sentinel->queue_mutex);
    free(pqueueIn->sentinel->StopThreadCond);

    /* cleanup the queue */
	free(pqueueIn);
	return QUEUE_SUCCESS;
}
/* ############################################################################# */
QueueExeStatus flushQueue(PQueue pqueueIn) {
	QueueData *pTmp;
    QueueExeStatus err;
	
    
    /** do it while something is in the queue **/
	if (pqueueIn!=NULL)
	{
		while(isfullQueue(pqueueIn))
		{
			/** remove one  item and destroy it */
			if((pTmp=popQueue(pqueueIn))){
                free(pTmp->data);
				free(pTmp);
			}
		}
        
        /* wake up all sleepers */
        pthread_cond_broadcast(pqueueIn->sentinel->StopThreadCond);
        err=QUEUE_SUCCESS;
    } else {
        err=QUEUE_NULL_POINTER_AS_IN_PARAMETER;
    }

    return err;
}
/* ############################################################################# */
QueueData * getNextitrQueue(PQueue pqueueIn) {
    PQueue pqueueSentinel;
    QueueData* pQueueData=NULL;
    
    if (!pqueueIn) {return NULL;}
    
    /* protect the queue access */
    pthread_mutex_lock(pqueueIn->sentinel->queue_mutex);	
	
    pqueueSentinel=(PQueue )pqueueIn->sentinel;
    
    /* check for valid Queue */
    if (pqueueSentinel->longCount!=0) {
	if (pqueueSentinel!=(PQueue)pqueueSentinel->iterator->next){
		/** poping **/
		pqueueSentinel->iterator=(PQueue)pqueueSentinel->iterator->next;
		pQueueData=(QueueData *)pqueueSentinel->iterator->queuedataData;	
	}	
    } else {
	logger(LOG_DEBUG,_("Queue is empty"));
    }

    pthread_mutex_unlock(pqueueIn->sentinel->queue_mutex);	
    return pQueueData;
}
