/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

/* callbacklist.c - handling and functions  to handle the callback list
 */
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include "callback.h"
#include "callbacklist.h"
#include "utilities.h"
#include "type.h"

/************************************************************************************************/
/************************************************************************************************/
/************************ Functions *************************************************************/
/************************************************************************************************/
/************************************************************************************************/
/* init_extendedCallbackDList
 *
 * Description: initing List
 *
 * Parameter: 	CallbackDList *list
 * 		- Pointer to Callback list
 * 		void (*destroy)(CallbackItem_t *data)
 * 		- funkction pointer to function which destroys the data
 *
 * Return:	void
 *
 */
void init_extended_CallbackDList(CallbackDList *list, void (*destroy)(CallbackItem_t *data))
{
	list->size=0;
	list->destroy=destroy;
	list->head=NULL;
	list->tail=NULL;
	
	return;
}

/* destroyCallbackDList
 *
 * Description: destroying List
 *
 * Parameter: 	CallbackDList *list
 * 		- Pointer to Callback list
 *
 * Return:	void
 *
 */
void destroyCallbackDList(CallbackDList *list)
{
	CallbackItem_t *data;

	/** delete elements **/
	while(getSizeCallbackDList(list)>0)
	{
		if (removeCallbackDList(list,getTailCallbackDList(list),(CallbackItem_t **)&data) == 0 && list->destroy != NULL)
		{
			/** destroy element Data **/
			list->destroy(data);	
		}
	}

	/** no opertions are allowed 
	 * but we deleting the CallbackDList */
	memset(list,0,sizeof(CallbackDList));

	return;	
}

/* insert_next_CallbackDList
 *
 * Description: inserting a new item after element with data into list
 *
 * Parameter: 	CallbackDList *list
 * 		- Pointer to Callback list
 * 		CallbackDListItem *element
 * 		- Pointer of element after that the data should be put
 * 		CallbackItem_t *data
 * 		- Data for the next element
 * 	
 * Return:	int Success
 * 		- returns 0 if OK
 *
 */
int insert_next_CallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t *data)
{
	CallbackDListItem *new_element;
	
	/** Don't allow a NULL-element while list is not empty **/
	if (element==NULL && getSizeCallbackDList(list)!=0)
		return -1;
	
	/** get memory for a new item **/
	if ((new_element=(CallbackDListItem *) malloc(sizeof(CallbackDListItem)))==NULL)
		return -1;
	
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

	return 0;
}

/* insert_prev_CallbackDList
 *
 * Description: inserting a new item before element with data into list
 *
 * Parameter: 	CallbackDList *list
 * 		- Pointer to Callback list
 * 		CallbackDListItem *element
 * 		- Pointer of element before that the data should be put
 * 		CallbackItem_t *data
 * 		- Data for the next element
 * 	
 * Return:	int Success
 * 		- returns 0 if OK
 *
 */
int insert_prev_CallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t *data)
{
	CallbackDListItem *new_element;
		
	/** Don't allow a NULL-element while list is not empty **/
	if (element==NULL && getSizeCallbackDList(list)!=0)
		return -1;
	
	/** get memory for a new item **/
	if ((new_element=(CallbackDListItem *) malloc(sizeof(CallbackDListItem)))==NULL)
		return -1;
	
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

	return 0;

}

/* renoveCallbackDList
 *
 * Description: remove element from list and put Data into data
 *
 * Parameter: 	CallbackDList *list
 * 		- Pointer to Callback list
 * 		CallbackDListItem *element
 * 		- element to remove
 * 		CallbackItem_t *data
 * 		- Data of the element that was removed
 * 	
 * Return:	int Success
 * 		- returns 0 if OK
 *
 */
int removeCallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t *data)
{
	/** Don't delete a NULL-element from an empty list **/
	if (element == NULL || getSizeCallbackDList(list)==0)
		return -1;
	/** delete an element from list **/
	data=element->data;
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
	return 0;
}

/* searchNicknameFromCallbackDList
 *
 * Description: searching the the Nickname from given element
 *
 * Parameter: 	CallbackDList *list
 * 		- Pointer to Callback list
 * 		CallbackDListItem *element
 * 		- element to search from
 * 		char *nickname
 * 		- the Nickname we are searching for
 * 		 	
 * Return:	CallbackItem_t *
 * 		- returns Null for error or elment were nickname is
 *
 */
CallbackItem_t * searchNicknameFromCallbackDList(CallbackDList *list, CallbackDListItem *element,char *nickname)
{
	CallbackDListItem *temp;
    CallbackItem_t *data;
	
	/** check for empty list **/
	if (list==NULL)
		return NULL;
	if (getHeadCallbackDList(list)==NULL || getTailCallbackDList(list)==NULL || getSizeCallbackDList(list)==0)
	{
		return NULL;
	}
	/** check for empty start element **/
	if (element==NULL)
		return NULL;
	/** checking for element, what is it head, tail or something else **/
	temp=element;
	if (isheadCallbackDList(element))
	{
		while(!strcmp(temp->data->nickname,nickname) && temp->next!=NULL)
		{
			temp=temp->next;
		}
		
	}
	else if (istailCallbackDList(element))
	{
		while(!strcmp(temp->data->nickname,nickname) && temp->prev!=NULL)
		{
			temp=temp->prev;
		}
	}
	else
	{
		while(!strcmp(temp->data->nickname,nickname) && temp->next!=NULL)
		{
			temp=temp->next;
		}
	}
	if (!strcmp(temp->data->nickname,nickname)) {
		removeCallbackDList(list,temp,data);
        return data;
    }
	else
		return NULL;
}
