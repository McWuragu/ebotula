/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#include"dbaccess.h"
#include"irc.h"
#include"extract.h"
#include"utilities.h"
#include"messages.h"
#include"type.h"
#include"callback.h"
                                                                                
// #############################################################################
void destroyCallbackItem(CallbackItem_t *data)
{
      /** destroy all sub-element of CallbackIten_t **/
        free(data->nickname);
        free(data->data);
        free(data);
        return;
}
// #############################################################################
void ModeResetCb(char *pNetmask,void* data){
    extern ConfigSetup_t sSetup;
    char **ppDataPart;
    char *pAccessKey;
    char *pLogin;
    char *pNick;
    char *pMod;

	DEBUG("Callback for ResetMode");

    // split the  input datums
    ppDataPart=splitString((char*)data,2);
    pNick=getNickname(pNetmask);

    if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
        DEBUG("Reset the mode for the identified User %s",pLogin);

        // built the  key for the access_db
        pAccessKey=(char*)malloc((strlen(ppDataPart[0])+strlen(pLogin)+1)*sizeof(char));
        sprintf(pAccessKey,"%s%s",pLogin,ppDataPart[0]);


        // read user rights
        if ((pMod=get_db(ACCESS_DB,pAccessKey))) {
            // normal user
            if (pMod[1]==ppDataPart[0][1]) {
                // check the  right status of mode and  set return
                if (ppDataPart[0][0]=='-') {
                    ppDataPart[1][0]='+';
                    mode(ppDataPart[0],ppDataPart[1],pNick);
                }
            } else {
                // reset the mode for identify user
                ppDataPart[1][0]='-';
                mode(ppDataPart[0],ppDataPart[1],pNick);
            }

        }  else if ((get_db(ACCESS_DB,pLogin))) {
            DEBUG("Reset the master");
            // master mod reset
            if (strcmp(ppDataPart[1],"+o")!=0){
                // reset all mod other mod until op
                ppDataPart[1][0]=(ppDataPart[1][0]=='-')?'+':'-';
                mode(ppDataPart[0],ppDataPart[1],pNick);
            }
        }

    } else if (ppDataPart[1][0]=='+' && strcmp(pNick,sSetup.botname)) {
        DEBUG("Reset the mode for %s",pNick);
        // reset the mode for not identify user
        ppDataPart[1][0]='-';
        mode(ppDataPart[0],ppDataPart[1],pNick);
    }
}
// #############################################################################
void SetBanCb(char *pNetmask,void * data){
    boolean doBan;
	char **pDataVec;
    char *pBanmask;
    char *pAccessKey;
    char *pMod=NULL;
    char *pNick;
    char *pLogin;
    char *pCmdNick;
    

    DEBUG("Callback for user banning");

    /* get user information */
    pDataVec=splitString((char*)data,3);
    pBanmask=getBanmask(pNetmask);
    pNick=getNickname(pNetmask);
    pLogin=get_db(NICKTOUSER_DB,pNetmask);
    pCmdNick=getNickname(get_db(USERTONICK_DB,pDataVec[0]));

    doBan=checkUserLevel(pCmdNick,pLogin,pDataVec[1]);

    /* try to ban */
    if (doBan) {
        ban(pDataVec[1],pBanmask);
        notice(pCmdNick,getMsgString(OK_BAN));
    } else {
        notice(pCmdNick,getMsgString(ERR_NOT_BAN));
    }

}

// #############################################################################    
void KickCb(char *pNetmask, void *data) {
    boolean doKick;
    char **ppDataPart;
    char *pAccessKey;
    char *pReason;
    char *pNick;
    char *pMod=NULL;
    char *pLogin;
    char *pCmdNick;
    
    DEBUG("Callback for user kicking");


    /* split Datums */
    ppDataPart=splitString((char*)data,3);

    /* get user information */
    pNick=getNickname(pNetmask);
    pLogin=get_db(NICKTOUSER_DB,pNetmask);
    pCmdNick=getNickname(get_db(USERTONICK_DB,ppDataPart[0]));

    /* extract reason */
    pReason=(char*)ppDataPart[2];
   
    doKick=checkUserLevel(pCmdNick,pLogin,ppDataPart[1]);

    /* try to kick */
    if (doKick) {
        kick(ppDataPart[1],pNick,pReason);
        notice(pCmdNick,getMsgString(OK_KICK));
    } else {
        notice(pCmdNick,getMsgString(ERR_NOT_KICK));
    } 
}

