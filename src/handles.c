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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "extract.h"
#include "callback.h"
#include "callbacklist.h"
#include "utilities.h"
#include "dbaccess.h"
#include "messages.h"
#include "irc.h"
#include "type.h"
#include "account.h"
#include "handles.h"

/* #########################################################################
   Event handler: PING
  #########################################################################*/
void hPong(MsgItem_t *pMsg){
    pong(NULL);
}

/* #########################################################################
   Event handler: NICK
  #########################################################################*/
void hNickChange(MsgItem_t *pMsg) {
    char *pLogin;
    char *pNewNetmask;
    char *pNewNick;
	char *pNetmaskCopy;
    char *pTmp;



	if (pMsg->pNetmask) {
        if ((pLogin=get_db(NICKTOUSER_DB,pMsg->pNetmask))) {
    		/* get the new nickname */
    		pNewNick=strrchr(pMsg->pRawLine,' ');
    		
    		if (strlen(pNewNick)>2) {
    			pNewNick+=2;
    
    			pNetmaskCopy=(char*)malloc((strlen(pMsg->pNetmask)+1)*sizeof(char));
    			strcpy(pNetmaskCopy,pMsg->pNetmask);
    			
    			/* cut out the secondary part of the netmask */
    			pTmp=strchr(pNetmaskCopy,'!');
    			strtok(pTmp," ");
    
    			/* build new netmask	*/
    			pNewNetmask=(char*)malloc((strlen(pNewNick)+strlen(pTmp)+1)*sizeof(char));
    			sprintf(pNewNetmask,"%s%s",pNewNick,pTmp);

    			/* reset Login Status*/
    			log_out(pLogin);
    			log_on(pNewNetmask,pLogin);
    
        		logger(LOG_DEBUG,_("Change the netmask \"%s\" to \"%s\""),pMsg->pNetmask,pNewNetmask);
    	    	
    			free(pNetmaskCopy);
    	    	free(pNewNetmask);
    			free(pLogin);
    		}
    
    	}
    }

}
/* ######################################################################### 
    Event handler: JOIN
    Action: Request OP
   ######################################################################### */
void hBotNeedOp(MsgItem_t *pMsg){
    extern ConfigSetup_t sSetup;
    char *pChannel;
    char *pNickList;
    char *pPos;
    char *pSearchStr;

    
    pChannel=pMsg->pAccessChannel;
    if (!pChannel) return;

    /* extrakt Namelist */
    pPos=strchr(&(pMsg->pRawLine)[1],':');
    pNickList=(char*)malloc((strlen(pPos)+1)*sizeof(char));
    strcpy(pNickList,pPos);

    pSearchStr=(char *) malloc((strlen(sSetup.pBotname)+2)*sizeof(char));
    sprintf(pSearchStr,"@%s",sSetup.pBotname);

    if (!strstr(pNickList,pSearchStr)) {
        action(pChannel,_("needs operator access rights."));
    } else {
        channelInit(pChannel);
    }


    free(pSearchStr);
    free(pNickList);
}
/* ######################################################################### 
   Event handler: JOIN
   Action: set the mod  for the user
   ######################################################################### */
void hSetModUser(MsgItem_t *pMsg) {
    extern ConfigSetup_t sSetup;
    char *pLogin;
    char *pNick;
    char *pKey;
    char *pChannel;
    char *pNetmask;
    char *pMod;

    
    pNick=pMsg->pCallingNick;

    if (pNick) {
        if (strcasecmp(pNick,sSetup.pBotname)) {
            pNetmask=pMsg->pNetmask;
            if (pNetmask) {
                
                if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
            	    logger(LOG_DEBUG,_("Set the account modes for the account %s with nickname %s"),pLogin,pNick);
        
                    if ((pChannel=pMsg->pAccessChannel)) {
                    	/* build key for access.dbf */
                        pKey=(char *)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char));
            	        sprintf(pKey,"%s%s",pLogin,pChannel);
               	        
            			/* read  the  mod */
                   	    if ((pMod=get_db(ACCESS_DB,pKey))) {
                    	    /* set the mod  for this nick */
                   	       	mode(pChannel,pMod,pNick);
                            free(pMod);
            	        } else if (exist_db(ACCESS_DB,pLogin)) {
               		        mode(pChannel,"+o",pNick);
                   		}
            
                        free(pKey);
                    }
                    free (pLogin);
                }
            }
        }
    }
}
/* #########################################################################
   Event handler: MODE
   Action:  added the callback item in the list for the user if this change
            not by bot self
   ######################################################################### */
void hResetModes(MsgItem_t *pMsg) {
    extern ConfigSetup_t sSetup;
    char *pChannel;
    char *pData;
    char *pNick;
    char *pNetmask;
    char *pAccessNick;
    char *pMode;
    char *pRest=NULL;
    char cModeChr;
    extern CallbackDList CallbackList;
    CallbackItem_t *Callback;
    boolean bTryToSet;
    unsigned int i;
    
    

    /*split the string*/
    pNetmask=getFirstPart(pMsg->pRawLine,&pRest);
    /* extract  the nick */
    pAccessNick=getNickname(pNetmask);
    free(pNetmask);
    
    rmFirstPart(pRest,&pRest);
    pChannel=getFirstPart(pRest,&pRest);
    pMode=getFirstPart(pRest,&pRest);

    if (pAccessNick) {
        if (strcasecmp(pAccessNick,sSetup.pBotname)!=0) {
            if (pMode) {
                unsigned int nSizeMode = strlen(pMode);
                for (i=0;i<nSizeMode;i++) {

                    /* check of set or remove command */
                    if (pMode[i]=='+') {
                        bTryToSet=true;
                        cModeChr='+';
                        continue;
                    } else if (pMode[i]=='-') {
                        bTryToSet=false;
                        cModeChr='-';
                        continue;
                    }

                    /* user modes*/
                    if (pMode[i]=='o' || pMode[i]=='v') {
                        pNick=getFirstPart(pRest,&pRest);
                        if (!pNick) {
                            logger(LOG_CRIT,_("Invalid server message \"%s\""),pMsg->pRawLine);
                            break;
                        }
                        /* check of bot new mods or  other user */
                        if (strcasecmp(pNick,sSetup.pBotname)==0) {
                            logger(LOG_INFO,_("Bot get a new account mode"));
                            /* mode set for the bot from other user of operator */
                            /* then initiallize this  channel */
                            if (bTryToSet && pMode[i]=='o') {
                                channelInit(pChannel);
                            } else {
                                action(pChannel,_("needs operator access rights."));
                            }
                        } else {
                            /* add callback for reset the modes for a user    */
                            logger(LOG_DEBUG,_("Added callback routine for the reset of the account modes"));
                            
                            /* built the data for callback */
                            pData=(char*)malloc((strlen(pChannel)+3)*sizeof(char));
                            sprintf(pData,"%s %c%c",pChannel,cModeChr,pMode[i]);
                            
                            /* build  the  element*/
                            Callback=(CallbackItem_t*)malloc(sizeof(CallbackItem_t));
                            Callback->nickname=(char*)malloc((strlen(pNick)+1)*sizeof(char));
                            strcpy(Callback->nickname,pNick);
                            /* Callback->nickname; *HÄÄÄ?? */
                            Callback->CallbackFkt=ModeResetCb;
                            Callback->data=pData;
                            
                            /* put  the  element  in the  callback list  before tail */
                            insert_next_CallbackDList(&CallbackList,CallbackList.tail,Callback);
                
                            /* send the who*/
                            whois(pNick);
                        }

                        free(pNick);
                    } else if (pMode[i]=='b') {
                        /* banning */
                        logger(LOG_INFO,_("Ban reset not implemented jet"));
                    } else if (pMode[i]== 't' || pMode[i]== 'n') {
                        /* channel modes   */
                    } else {
                        /* unknown Mode */
                        logger(LOG_DEBUG,"Ignore mode switch \"%c%c\"",cModeChr,pMode[i]);
                    }

                }
            
            } else {
                logger(LOG_CRIT,_("No mode entries found in \"%s\""),pMsg->pRawLine);
            }  
        }     
        free(pAccessNick);
    }
    if (pRest) { free(pRest);}
    free(pChannel);
    free(pMode);

}
/* #########################################################################
   Event handler: TOPIC
   Action: reset the top  for the chanel if this change not by bot self
   ######################################################################### */
void hResetTopic(MsgItem_t *pMsg){
    extern ConfigSetup_t sSetup;
    char *pChannelSet;
    char *pTopic;


    if (pMsg->pCallingNick) {
        if (strcasecmp(pMsg->pCallingNick,sSetup.pBotname)) {
    
            /* get the  right topic for this channel*/
            if (!(pMsg->pAccessChannel)) return;    
    
            if ((pChannelSet=get_db(CHANNEL_DB,pMsg->pAccessChannel))) {
            	if ((pTopic=getTopic(pChannelSet))) {
                	/* reset the topic */
    	            topic(pMsg->pAccessChannel,pTopic);
        	        free(pTopic);
            	} else {
    	            topic(pMsg->pAccessChannel,"");
        	    }
    
                logger(LOG_DEBUG,_("Reset the topic in the channel %s"),pMsg->pAccessChannel);
            	free(pChannelSet);
    		}	
        }
    }
}
/* #########################################################################
   Event handler: KICK
   Action: rejoin the channel after a kick
   #########################################################################*/
void hRejoinAfterKick(MsgItem_t *pMsg){
    char *pRest=NULL;
    char *pNick;
    extern ConfigSetup_t sSetup;

    rmFirstPart(pMsg->pRawLine,&pRest);
    rmFirstPart(pRest,&pRest);
    rmFirstPart(pRest,&pRest);
    pNick=getFirstPart(pRest,&pRest);
    
    if (pRest) {free(pRest);}

    if (pNick) {
        if (strcasecmp(pNick,sSetup.pBotname)==0) {
            join(pMsg->pAccessChannel);
            logger(LOG_DEBUG,_("Rejoin the channel %s"),pMsg->pAccessChannel);
        }
        free(pNick);
    }
}

/* #########################################################################
   Event handler: MODE
   Action: initialize the channel after  get operator access
   #########################################################################*/

void hInitAfterOp(char *pLine) {
    char *pChannel;
                   
    if (!(pChannel=getAccessChannel(pLine)))
        return;

    channelInit(pChannel);
}

/* #########################################################################
   Event helper: Channel intializalisation
   #########################################################################*/
static void channelInit(char *pChannel) {
    ChannelData_t sChannelData;
    char *pMode,*pChannelSet;

      
    if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
        StrToChannelData(pChannelSet,&sChannelData);
        free(pChannelSet);
                                             
        /* set Topic */
        if (sChannelData.pTopic) {
            topic(pChannel,sChannelData.pTopic);
        }

        /* set Modes */
        pMode=ChannelModeToStr(&(sChannelData.sModes));
        if (strlen(pMode)) {
            mode(pChannel,pMode,NULL);
        }
        free(pMode);

        free(sChannelData.pGreeting);
        free(sChannelData.pTopic);
        free (sChannelData.sModes.pKeyword);
        free (sChannelData.sModes.pLimit);
	
        action(pChannel,_("has initialized this channnel."));
        logger(LOG_INFO,_("Initialize the channel %s"), pChannel);
    }
}

/* #########################################################################
   Event handler: 352
   Action: call the callback function for a nickname
   #########################################################################*/
void hCallback(MsgItem_t *pMsg) {
    extern CallbackDList CallbackList;
    CallbackItem_t *CB_Data;
    CallbackDListItem *pCallbackItemReturn;
    char *pNetmask;
    char *pRest=NULL;
    char *pNick;
    char *pLogin;
    char *pDomain;

    rmFirstPart(pMsg->pRawLine,&pRest); /* 0 */
    rmFirstPart(pRest,&pRest); /* 1 */
    rmFirstPart(pRest,&pRest); /* 2 */
    
    pNick=getFirstPart(pRest,&pRest);
    pLogin=getFirstPart(pRest,&pRest);
    pDomain=getFirstPart(pRest,NULL);
    free(pRest);

   /** lock for the Callback item for the nick **/
    if ((pCallbackItemReturn=searchNicknameFromCallbackDList(&CallbackList,CallbackList.head,pNick))) {
        /* 
         * remove  entrie and read the  callback datum
         * if  this item not more in the  queue then look for  the next
         */
        if (!removeCallbackDList(&CallbackList,pCallbackItemReturn,&CB_Data)) {
            /* built netmask */
            pNetmask=(char*)malloc((strlen(pNick)+strlen(pLogin)+strlen(pDomain)+3)*sizeof(char));
            sprintf(pNetmask,"%s!%s@%s",pNick,pLogin,pDomain);
    
            /* execute the callback */
            CB_Data->CallbackFkt(pNetmask,CB_Data->data);
    
            /* destroy  callback item */
            destroyCallbackItem(CB_Data);

            free(pNetmask);
        }                                          
    }
    free(pNick);
    free(pLogin);
    free(pDomain);
}

/* #########################################################################
   Event handler: 401
   Action: handle a whois failed
   #########################################################################*/
void hWhoisFailed(MsgItem_t *pMsg) {
    extern CallbackDList CallbackList;
    CallbackItem_t *CB_Data;
    CallbackDListItem *pCallbackItemReturn;
    char *pRest=NULL;
    char *pNick;

    rmFirstPart(pMsg->pRawLine,&pRest); /* 0 */
    rmFirstPart(pRest,&pRest); /* 1 */
    rmFirstPart(pRest,&pRest); /* 2 */
    
    pNick=getFirstPart(pRest,NULL);
    free(pRest);

    logger(LOG_DEBUG,_("Look for a callback zombie routine of the user %s"),pNick);

   /** lock for the Callback item for the nick **/
    if ((pCallbackItemReturn=searchNicknameFromCallbackDList(&CallbackList,CallbackList.head,pNick))) {
        /* 
         * remove  entrie and read the  callback datum
         * if  this item not more in the  queue then look for  the next
         */
        if (!removeCallbackDList(&CallbackList,pCallbackItemReturn,&CB_Data)) {
            /* destroy  callback item */
            destroyCallbackItem(CB_Data);
            logger(LOG_DEBUG,_("The callback zombie routine of the user %s is removed"),pNick);
        }
    }
    free(pNick);
}

