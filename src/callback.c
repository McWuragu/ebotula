/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/


#include"dbaccess.h"
#include"irc.h"
#include"extract.h"
#include"utilities.h"

#include"callback.h"

/* destroxCallbackItem
* 
* Description: Destroys Dataelement und sub-item of it
*
* Parameter:   CallbackItem_t
*              -Pointer to CallbackItem_t
*
* Return:      void
*
*/
void destroyCallbackItem(CallbackItem_t *data)
{
      /** destroy all sub-element of CallbackIten_t **/
        free(data->nickname);
        free(data->data);
        free(data);
        return;
}


void ModeResetCb(char *pNetmask,void* data){
	DEBUG("ResetMode");
}

void SetBanCb(char *pNetmask,void * data){
	DEBUG("SetBan");
}

void GetBanCb(char *pNetmask,void * data){
	DEBUG("GetBan");
}

void KickCb(char *pNetmask, void *data) {
    DEBUG("Kick user")
}

