
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
        free(data);
        return;
}


void ModeResetCb(char *pLine){
	DEBUG("ResetMode");
}

void SetBanCb(char *pLine){
	DEBUG("SetBan");
}

void GetBanCb(char *pLine){
	DEBUG("GetBan");
}

