/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
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
    char *pChannel;
    char *pOldMode=NULL;
    char *pAccessKey;
    char *pLogin;
    char *pNick;
    char *pMod;
    char *pTmpBotName;

	logger(LOG_DEBUG,gettext("Callback routine for the reset of the modes"));

    // split the  input datums
    pChannel=getFirstPart((char*)data,&pOldMode);
    pNick=getNickname(pNetmask);

    if (pNick) {
        StrToLower(pNetmask);
        
        /* make  a bot name with small letters */
        pTmpBotName=(char*)malloc((strlen(sSetup.pBotname)+1)*sizeof(char));
        strcpy(pTmpBotName,sSetup.pBotname);
        StrToLower(pTmpBotName);

        if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
            logger(LOG_DEBUG,gettext("Reset the account modes for the identified account %s"),pLogin);
    
            // built the  key for the access_db
            pAccessKey=(char*)malloc((strlen(pChannel)+strlen(pLogin)+1)*sizeof(char));
            sprintf(pAccessKey,"%s%s",pLogin,pChannel);
    
    
            // read user rights
            if ((pMod=get_db(ACCESS_DB,pAccessKey))) {
                // normal user
                if (pMod[1]==pChannel[1]) {
                    // check the  right status of mode and  set return
                    if (pChannel[0]=='-') {
                        pOldMode[0]='+';
                        mode(pChannel,pOldMode,pNick);
                    }
                } else {
                    // reset the mode for identify user
                    pOldMode[0]='-';
                    mode(pChannel,pOldMode,pNick);
                }
                free(pMod);
            }  else if ((pMod=get_db(ACCESS_DB,pLogin))) {
                logger(LOG_DEBUG,gettext("Reset the account modes of the master %s"),pLogin);
                // master mod reset
                if (strcmp(pOldMode,"+o")!=0){
                    // reset all mod other mod until op
                    pOldMode[0]=(pOldMode[0]=='-')?'+':'-';
                    mode(pChannel,pOldMode,pNick);
                }
                free(pMod);
            }
            free(pAccessKey);
            free(pLogin);
        } else if (pOldMode[0]=='+' && strcmp(pNick,pTmpBotName)) {
            logger(LOG_DEBUG,gettext("Reset the account mode for user %s"),pNick);
            // reset the mode for not identify user
            pOldMode[0]='-';
            mode(pChannel,pOldMode,pNick);
        }

        free(pTmpBotName);
        free(pNick);
    }
    
    free(pChannel);
    free(pOldMode);

}
// #############################################################################
void SetBanCb(char *pNetmask,void * data){
    boolean doBan;
	char *pRest=NULL;
    char *pBanmask;
    char *pAccessKey;
    char *pMod=NULL;
    char *pChannel;
    char *pLogin;
    char *pDestLogin;
    char *pCmdNick;
    

    logger(LOG_DEBUG,gettext("Callback routine for user banning"));

    /* get user information */
    pLogin=getFirstPart((char*)data,&pRest);
    pChannel=getFirstPart(pRest,NULL);
    free(pRest);

    StrToLower(pNetmask);
    pBanmask=getBanmask(pNetmask);
    pDestLogin=get_db(NICKTOUSER_DB,pNetmask);
    pCmdNick=getNickname(get_db(USERTONICK_DB,pLogin));

    
    if (pCmdNick) {
        doBan=checkUserLevel(pCmdNick,pDestLogin,pChannel);
    
        /* try to ban */
        if (doBan) {
            ban(pChannel,pBanmask);
            notice(pCmdNick,gettext("The user is banned by the bot."));
        } else {
            notice(pCmdNick,gettext("Couldn't ban this user"));
        }
        free(pCmdNick);
    }
    free(pBanmask);
    free(pDestLogin);
    free(pLogin);
    free(pChannel);
    
}

// #############################################################################    
void KickCb(char *pNetmask, void *data) {
    boolean doKick;
    char *pAccessKey;
    char *pReason=NULL;
    char *pNick;
    char *pMod=NULL;
    char *pLogin;
    char *pCmdLogin;
    char *pChannel;
    char *pCmdNick;
    char *pCmdNetmask;
    
    logger(LOG_DEBUG,gettext("Callback routine for user kicking"));


    /* split Datums */
    pCmdLogin=getFirstPart((char*)data,&pReason);
    pChannel=getFirstPart(pReason,&pReason);

    /* get user information */
    StrToLower(pNetmask);
    pLogin=get_db(NICKTOUSER_DB,pNetmask);
    pCmdNetmask=get_db(USERTONICK_DB,pCmdLogin);
    pCmdNick=getNickname(pCmdNetmask);

    if (pCmdNick) {
        doKick=checkUserLevel(pCmdNick,pLogin,pChannel);
    
        /* try to kick */
        if (doKick) {
            pNick=getNickname(pNetmask);
            if (pNick) {
                kick(pChannel,pNick,pReason);
                notice(pCmdNick,gettext("The user is kicked by the bot"));
                free(pNick);
            }
        } else {
            notice(pCmdNick,gettext("Couldn't kick this user"));
        }

        free(pCmdNick);
    }
    free(pChannel);
    free(pCmdLogin);
    free(pCmdNetmask);
    free(pReason);
    free(pLogin);
}

