/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#ifndef _CALLBACK_LIST_H
#define _CALLBACK_LIST_H
#include <stdlib.h>
#include "callback.h"

/** 
 * Setting up Callback_List-Base-Item to hold all Elements 
 */
typedef struct tag_Callback_DList_Item {
	CallbackItem_t *data;
	struct tag_Callback_DList_Item *prev;
	struct tag_Callback_DList_Item *next;
} CallbackDListItem;

/** 
 * Setting up Callback_List Base to hold head, tail, size and match keys 
 */
typedef struct tag_Callback_DList {
	int size;
	void (*destroy)(CallbackItem_t *data);
	CallbackDListItem *head;
	CallbackDListItem *tail;
	pthread_mutex_t callbacklist_mutex;
}CallbackDList;

/* -----------------------Functions  --------------------------------------- */

/**
 * initing List
 * 
 * @param list    a pointer to Callback list
 * @param destroy
 */
void init_extended_CallbackDList(CallbackDList *list ,void (*destroy)(CallbackItem_t *data));

/**
 * destroying List
 *
 * @param list a pointer to Callback list
 */
void destroyCallbackDList(CallbackDList *list);

/**
 * inserting a new item after element with data into list
 *
 * @param list a pointer to Callback list
 * @param element a pointer of element after that the data should be put
 * @param data the data for the next element
 * 	
 * @return 0 if OK
 */
int insert_next_CallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t *data);

/**
 * inserting a new item before element with data into list
 *
 * @param list a pointer to Callback list
 * @param element a pointer of element before that the data should be put
 * @param data the data for the next element
 * 	
 * @return 0 if OK
 */
int insert_prev_CallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t *data);

/**
 * remove element from list and put Data into data
 * 
 * @param list    a pointer to Callback list
 * @param element element to remove
 * @param data    the data of the element that was removed
 *                
 * @return 0 if OK
 */
int removeCallbackDList(CallbackDList *list, CallbackDListItem *element, CallbackItem_t **data);

/**
 * searching the the Nickname from given element
 * 
 * @param list     a pointer to Callback list
 * @param element  element to search from
 * @param nickname the Nickname we are searching for
 * @return returns Null for error or elment were nickname is
 */
CallbackDListItem * searchNicknameFromCallbackDList(CallbackDList *list, CallbackDListItem *element,char *nickname);


/**** some Makros ****/
#define getSizeCallbackDList(list)		((list)->size)
#define getHeadCallbackDList(list)		((list)->head)
#define getTailCallbackDList(list)		((list)->tail)
#define isheadCallbackDList(element)		((element)->prev==NULL ? 1 : 0)
#define istailCallbackDList(element)		((element)->next==NULL ? 1 : 0)
#define getDataCallbackDList(element)		((element)->data)
#define getNextCallbackDList(element)		((element)->next)
#define getPrevCallbackDList(element)		((element)->prev)
#define pushCallbackDList(list,data)		(insert_next_CallbackDList(list,getTailCallbackDList(list),data))
#define popCallbackDList(list,data)		(removeCallbackDList(list,getHeadCallbackDList(list),data))
/*new init */
#define initCallbackDList(list)			(init_extended_CallbackDList(list,destroyCallbackItem))
/* searching */
#define searchNicknameFromHeadCallbackDList(list,nickname)	(searchNicknameFromCallbackDList(list,getHeadCallbackDList(list),nickname))
#define searchNicknameFromTailCallbackDList(list,nickname)	(searchNicknameFromCallbackDList(list,getTailCallbackDList(list),nickname))
#endif
