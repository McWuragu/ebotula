#ifndef _CALLBACK_H
#define _CALLBACK_H

typedef struct CallbackStruct {
	char *nickname;
	void *CallbackFkt;
	unsigned int ttl;
} CallbackItem_t;

/* destroxCallbackItem
 *
 * Description:	Destroys Dataelement und sub-item of it
 * 
 * Parameter: 	CallbackItem_t
 * 		-Pointer to CallbackItem_t
 *
 * Return:	void
 *
 */
void destroyCallbackItem(CallbackItem_t *data);

void ModeResetCb(char *pLine);
void SetBanCb(char *pLine);
void GetBanCb(char *pLine);

#endif

