/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
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

/* callbacklist.c - handling and functions  to handle the callback list
 */
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "callback.h"
#include "callbacklist.h"
#include "utilities.h"
#include "type.h"

/*
 *  init_extendedCallbackDList
 */
void init_extended_CallbackDList(CallbackDList *list, void (*destroy)(CallbackItem_t *data))
{
	list->size=0;
	list->destroy=destroy;
	list->head=NULL;
	list->tail=NULL;
	  /** create and init the  mutex */
        list->callbacklist_mutex=(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	 pthread_mutex_init(list->callbacklist_mutex,NULL);
			
	return;
}

/* 
 * destroyCallbackDList
 */
void destroyCallbackDList(CallbackDList *list)
{
	CallbackItem_t *data;

	/** delete elements **/
	while(getSizeCallbackDList(list)>0)
	{
		if (removeCallbackDList(list,getTailCallbackDList(list),&data) == 0 && list->destroy != NULL)
		{
			/** destroy element Data **/
			list->destroy(data);	
		}
	}

	/** no opertions are allowed 
	 * but we deleting the CallbackDList */
	pthread_mutex_destroy(list->callbacklist_mutex);
	free(list->callbacklist_mutex);
		    
	memset(list,0,sizeof(CallbackDList));

	return;	
}

/*
 * insert_next_CallbackDList
 */
int insert_next_CallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t *data)
{
	CallbackDListItem *new_element;

	  pthread_mutex_lock(list->callbacklist_mutex);
	  /** Don't allow a NULL-element while list is not empty **/
	if (element==NULL && getSizeCallbackDList(list)!=0)
	{
		pthread_mutex_unlock(list->callbacklist_mutex);
		return -1;
	}
	
	/** get memory for a new item **/
	if ((new_element=(CallbackDListItem *) malloc(sizeof(CallbackDListItem)))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
		pthread_mutex_unlock(list->callbacklist_mutex);
		return -1;
	}
	
	/** insert the new item **/
	new_element->data=(CallbackItem_t *) data;
	if (getSizeCallbackDList(list)==0)
	{
		/** insert something in an empty list **/
		list->head=new_element;
		list->head->prev=NULL;
		list->head->next=NULL;
		list->tail=new_element;
	}
	else {
		/** insert an item in a not empty list **/
		new_element->next=element->next;
		new_element->prev=element;
		if (element->next==NULL)
		{
			list->tail=new_element;
		}
		else
		{
			element->next->prev=new_element;
		}
		element->next=new_element;
	}
	list->size++;
	pthread_mutex_unlock(list->callbacklist_mutex);
			    
	return 0;
}

/*
 * insert_prev_CallbackDList
 */
int insert_prev_CallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t *data)
{
	CallbackDListItem *new_element;
	  
	pthread_mutex_lock(list->callbacklist_mutex);
	/** Don't allow a NULL-element while list is not empty **/
	if (element==NULL && getSizeCallbackDList(list)!=0)
	{
		pthread_mutex_unlock(list->callbacklist_mutex);
		return -1;
	}
	
	/** get memory for a new item **/
	if ((new_element=(CallbackDListItem *) malloc(sizeof(CallbackDListItem)))==NULL)
	{
	   	logger(LOG_ERR,_("Couldn't allocate memory!"));
		pthread_mutex_unlock(list->callbacklist_mutex);
		return -1;
	}
	
	/** insert the new item **/
	new_element->data=(CallbackItem_t *) data;
	if (getSizeCallbackDList(list)==0)
	{
		/** insert something in an empty list **/
		list->head=new_element;
		list->head->prev=NULL;
		list->head->next=NULL;
		list->tail=new_element;
	}
	else {
		/** insert an item in a not empty list **/
		new_element->next=element;
		new_element->prev=element->prev;
		if (element->next==NULL)
		{
			list->tail=new_element;
		}
		else
		{
			element->prev->next=new_element;
		}
		element->prev=new_element;
	}
	list->size++;
	
	pthread_mutex_unlock(list->callbacklist_mutex);
	return 0;

}

/*
 * renoveCallbackDList
 */
int removeCallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t **data)
{
	 pthread_mutex_lock(list->callbacklist_mutex);
	/** Don't delete a NULL-element from an empty list **/
	if (element == NULL || getSizeCallbackDList(list)==0)
	{
		logger(LOG_DEBUG,_("List is empty!"));
	 	pthread_mutex_unlock(list->callbacklist_mutex);
		return -1;
	}
	/** delete an element from list **/
	*data=element->data;
	if (element == list->head)
	{
		/** deleteing from the head **/
		list->head=element->next;
		if (list->head == NULL)
		{
			list->tail=NULL;
		}
		else 
		{
			element->next->prev=NULL;
		}
	}
	else {
		/** delete from inner list **/
		element->prev->next=element->next;
		if (element->next == NULL)
		{
			list->tail=element->prev;
		}
		else {
			element->next->prev=element->prev;
		}
	}
	/** freeing memory from element **/
	free(element);
	/** Resize size of list by on **/
	list->size--;
	
	pthread_mutex_unlock(list->callbacklist_mutex);
	return 0;
}

/*
 * searchNicknameFromCallbackDList
 */
CallbackDListItem * searchNicknameFromCallbackDList(CallbackDList *list, CallbackDListItem *element,char *nickname)
{
	CallbackDListItem *temp;
    /*CallbackItem_t *data;*/
	
	 pthread_mutex_lock(list->callbacklist_mutex);
    	/** check for empty list **/
	if (list==NULL)
	{
	 	pthread_mutex_unlock(list->callbacklist_mutex);
		return NULL;
	}
	if (getHeadCallbackDList(list)==NULL || getTailCallbackDList(list)==NULL || getSizeCallbackDList(list)==0)
	{
		logger(LOG_DEBUG,_("Callback list empty list."));
	 	pthread_mutex_unlock(list->callbacklist_mutex);
		return NULL;
	}
	/** check for empty start element **/
	if (element==NULL)
	{
	 	pthread_mutex_unlock(list->callbacklist_mutex);
		return NULL;
	}
	/** checking for element, what is it head, tail or something else **/
	temp=element;
	if (isheadCallbackDList(element))
	{
		while(strcasecmp(temp->data->nickname,nickname)!=0 && temp->next!=NULL)
		{
			temp=temp->next;
		}
		
	}
	else if (istailCallbackDList(element))
	{
		while(strcasecmp(temp->data->nickname,nickname)!=0)
		{
            if (temp->prev) {
                temp=temp->prev;
            }else {
                break;
            }
		}
	}
	else
	{
		while(strcasecmp(temp->data->nickname,nickname)!=0)
		{
            if (temp->next) {
                temp=temp->next;
            }else {
                break;
            }
		}
	}
	
    
    if (strcasecmp(temp->data->nickname,nickname)==0) {
        pthread_mutex_unlock(list->callbacklist_mutex);
        return temp;
    }
	else
	{
		logger(LOG_DEBUG,_("User \"%s\" not found in the callback list!"), nickname);
	 	pthread_mutex_unlock(list->callbacklist_mutex);
		return NULL;
	}
}
