/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef _CALLBACK_H
#define _CALLBACK_H

typedef struct CallbackStruct {
	char *nickname;
	void (*CallbackFkt)(char *pNick,void *data);
    void *data;
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

void ModeResetCb(char *pNetmask,void *data);
void SetBanCb(char *pNetmask,void *data);
void GetBanCb(char *pNetmask,void *data);
void KickCb(char *pNetmask, void *data);

#endif

