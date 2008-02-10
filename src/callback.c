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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>                                                                                
#include "dbaccess.h"
#include "irc.h"
#include "extract.h"
#include "utilities.h"
#include "messages.h"
#include "type.h"
#include "callback.h"
#include "account.h"
/* ############################################################################# */
void destroyCallbackItem(CallbackItem_t *data)
{
      /** destroy all sub-element of CallbackIten_t **/
    free(data->nickname);
        free(data->data);
        free(data);
        return;
}
/* ############################################################################# */
void ModeResetCb(char *pNetmask,void* data){
    extern ConfigSetup_t sSetup;
    char *pChannel=NULL;
    char *pOldMode=NULL;
    char *pAccessKey=NULL;
    size_t nAccessKeySize=0;
    char *pLogin=NULL;
    char *pNick=NULL;
    char *pMod=NULL;

	logger(LOG_DEBUG,_("Callback routine for the reset of the modes"));

    /* split the  input datums */
    pChannel=getFirstPart((char*)data,&pOldMode);
    pNick=getNickname(pNetmask);

    if (pNick) {
        if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
            logger(LOG_DEBUG,_("Reset the account modes for the identified account %s"),pLogin);
    
            /* built the  key for the access_db */
			nAccessKeySize = (strlen(pChannel)+strlen(pLogin)+1)*sizeof(char);
			pAccessKey=(char*)malloc(nAccessKeySize);
            snprintf(pAccessKey,nAccessKeySize,"%s%s",pLogin,pChannel);
    
    
            /* read user rights */
            if ((pMod=get_db(ACCESS_DB,pAccessKey))) {
                /* normal user */
                if (pMod[1]==pChannel[1]) {
                    /* check the  right status of mode and  set return */
                    if (pChannel[0]=='-') {
                        pOldMode[0]='+';
                        mode(pChannel,pOldMode,pNick);
                    }
                } else {
                    /* reset the mode for identify user */
                    pOldMode[0]='-';
                    mode(pChannel,pOldMode,pNick);
                }
                free(pMod);
            }  else if ((pMod=get_db(ACCESS_DB,pLogin))) {
                logger(LOG_DEBUG,_("Reset the account modes of the master %s"),pLogin);
                /* master mod reset */
                if (strcmp(pOldMode,"+o")!=0){
                    /* reset all mod other mod until op */
                    pOldMode[0]=(pOldMode[0]=='-')?'+':'-';
                    mode(pChannel,pOldMode,pNick);
                }
                free(pMod);
            }
            free(pAccessKey);
            free(pLogin);
        } else if (pOldMode[0]=='+' && strcasecmp(pNick,sSetup.pBotname)) {
            logger(LOG_DEBUG,_("Reset the account mode for user %s"),pNick);
            /* reset the mode for not identify user */
            pOldMode[0]='-';
            mode(pChannel,pOldMode,pNick);
        }

	free(pNick);
    }
    free(pChannel);
    free(pOldMode);

}
/* ############################################################################# */
void SetBanCb(char *pNetmask,void * data){
    boolean doBan;
	char *pRest=NULL;
    char *pBanmask;
    /*char *pAccessKey;
    char *pMod=NULL;*/
    char *pChannel;
    char *pLogin;
    char *pDestLogin;
    char *pCmdNick;
    

    logger(LOG_DEBUG,_("Callback routine for user banning"));

    /* get user information */
    pLogin=getFirstPart((char*)data,&pRest);
    pChannel=getFirstPart(pRest,NULL);
    free(pRest);

    pBanmask=getBanmask(pNetmask);
    pDestLogin=get_db(NICKTOUSER_DB,pNetmask);
    pCmdNick=getNickname(get_db(USERTONICK_DB,pLogin));

    
    if (pCmdNick) {
        doBan=checkUserLevel(pCmdNick,pDestLogin,pChannel);
    
        /* try to ban */
        if (doBan) {
            ban(pChannel,pBanmask);
            notice(pCmdNick,_("The user is banned by the bot."));
        } else {
            notice(pCmdNick,_("Couldn't ban this user"));
        }
        free(pCmdNick);
    }
    free(pBanmask);
    free(pDestLogin);
    free(pLogin);
    free(pChannel);
    
}

/* #############################################################################  */
void KickCb(char *pNetmask, void *data) {
    boolean doKick;
    /*char *pAccessKey;*/
    char *pReason=NULL;
    char *pNick;
/*    char *pMod=NULL;*/
    char *pLogin;
    char *pCmdLogin;
    char *pChannel;
    char *pCmdNick;
    char *pCmdNetmask;
    
    logger(LOG_DEBUG,_("Callback routine for user kicking"));


    /* split Datums */
    pCmdLogin=getFirstPart((char*)data,&pReason);
    pChannel=getFirstPart(pReason,&pReason);

    /* get user information */
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
                notice(pCmdNick,_("The user is kicked by the bot"));
                free(pNick);
            }
        } else {
            notice(pCmdNick,_("Couldn't kick this user"));
        }

        free(pCmdNick);
    }
    free(pChannel);
    free(pCmdLogin);
    free(pCmdNetmask);
    free(pReason);
    free(pLogin);
}

