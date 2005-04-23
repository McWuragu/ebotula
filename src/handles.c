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
   Event handler: NICK
  #########################################################################*/
void hNickChange(char *pLine) {
    char *pNetmask;
    char *pLogin;
    char *pNewNetmask;
    char *pNewNick;
	char *pNetmaskCopy;
    char *pTmp;

    pNetmask=getNetmask(pLine);

	if (pNetmask) {
        if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
    		/* get the new nickname */
    		pNewNick=strrchr(pLine,' ');
    		
    		if (strlen(pNewNick)>2) {
    			pNewNick+=2;
    
    			pNetmaskCopy=(char*)malloc((strlen(pNetmask)+1)*sizeof(char));
    			strcpy(pNetmaskCopy,pNetmask);
    			
    			/* cut out the secondary part of the netmask */
    			pTmp=strchr(pNetmaskCopy,'!');
    			strtok(pTmp," ");
    
    			/* build new netmask	*/
    			pNewNetmask=(char*)malloc((strlen(pNewNick)+strlen(pTmp)+1)*sizeof(char));
    			sprintf(pNewNetmask,"%s%s",pNewNick,pTmp);

    			/* reset Login Status*/
    			log_out(pLogin);
    			log_on(pNewNetmask,pLogin);
    
        		logger(LOG_DEBUG,_("Change the netmask \"%s\" to \"%s\""),pNetmask,pNewNetmask);
    	    	
    			free(pNetmaskCopy);
    	    	free(pNewNetmask);
    			free(pLogin);
    		}
    
    	}
        free(pNetmask);
    }

}
/* ######################################################################### 
    Event handler: JOIN
    Action: Request OP
   ######################################################################### */
void hBotNeedOp(char *pLine){
    extern ConfigSetup_t sSetup;
    char *pChannel;
    char *pNickList;
    char *pPos;
    char *pSearchStr;

    

    if (!(pChannel=getAccessChannel(pLine)))
        return;

    /* extrakt Namelist */
    pPos=strchr(&pLine[1],':');
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
    free(pChannel);
}
/* ######################################################################### 
   Event handler: JOIN
   Action: set the mod  for the user
   ######################################################################### */
void hSetModUser(char *pLine) {
    extern ConfigSetup_t sSetup;
    char *pLogin;
    char *pNick;
    char *pKey;
    char *pChannel;
    char *pNetmask;
    char *pMod;

    
    pNick=getNickname(pLine);

    if (pNick) {
        char *pTmpBotName;
        pTmpBotName=(char*)malloc((strlen(sSetup.pBotname)+1)*sizeof(char));
        strcpy(pTmpBotName,sSetup.pBotname);
        StrToLower(pTmpBotName);

        if (strcmp(pNick,pTmpBotName)) {
            pNetmask=getNetmask(pLine);
            if (pNetmask) {
                
                if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
            	    logger(LOG_DEBUG,_("Set the account modes for the account %s with nickname %s"),pLogin,pNick);
        
                    if ((pChannel=getAccessChannel(pLine))) {
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
            
                       	free(pChannel);
                        free(pKey);
                    }
                    free (pLogin);
                }
                free(pNetmask);
            }
        }
        free(pTmpBotName);
        free(pNick);
    }
}
/* #########################################################################
   Event handler: MODE
   Action:  added the callback item in the list for the user if this change
            not by bot self
   ######################################################################### */
void hResetModes(char *pLine) {
    extern ConfigSetup_t sSetup;
    char *pPos;
    char *pModeFirst=NULL;
    char *pModeSec=NULL;
    char *pModeCurr;
    char *pChannel;
    char *pChannelStr;
    char *pData;
    char *pNick;
    char *pNetmask;
    char *pAccessNick;
    char *pMode;
    char *pRest=NULL;
    ChannelData_t ChannelSet;
    extern CallbackDList CallbackList;
    CallbackItem_t *Callback;
    boolean bSet,bContains;
    unsigned int nStrSize,i;
    
    

    /*splitt the string*/
    pNetmask=getFirstPart(pLine,&pRest);
    rmFirstPart(pRest,&pRest);
  
    pChannel=getFirstPart(pRest,&pRest);
    pMode=getFirstPart(pRest,&pRest);
    
    /* extract  the nick */
    pAccessNick=getNickname(pNetmask);
    free(pNetmask);

    if (pAccessNick) {
        char *pTmpBotName;
        pTmpBotName=(char*)malloc((strlen(sSetup.pBotname)+1)*sizeof(char));
        strcpy(pTmpBotName,sSetup.pBotname);
        StrToLower(pTmpBotName);


        if (strcmp(pAccessNick,pTmpBotName)!=0) {
            
//            /* splitt the mode in set and remove */
//            pPos=strchr(pMode,pMode[0]=='+'?'-':'+');
//            
//            
//            nStrSize=(pPos)?pPos-pMode:strlen(pMode);
//            pModeFirst=(char*)malloc((nStrSize+1)+sizeof(char));
//
//            /* copy the first part */
//            strncpy(pModeFirst,pMode,nStrSize);
//            pModeFirst[nStrSize]='\0';
//
//            /* copy the second part if the exist */ 
//            if (pPos) {
//                pModeSec=(char*)malloc((strlen(pPos)+1)*sizeof(char));
//                strcpy(pModeSec,pPos);
//            }
//
//            /* set the  first part of the mode string to the current mode */
//            pModeCurr=pModeFirst;
//
//            for (i=0;i<2;i++) {
//                for (pPos=pModeCurr;*pPos!='\0';pPos++) {
//                    /* user */
//                    if (pPos=='o' || pPos=='v') {
//                        /* check of bot new mods or  other user */
//                        if (strcmp(pNick,pTmpBotName)==0) {
//                            logger(LOG_DEBUG,_("Bot get a new account mode"));
//                            /* mode set for the bot from other user of operator */
//                            /* then initiallize this  channel */
//                            if (bSet) {
//                                channelInit(pChannel);
//                            } else {
//                                action(pChannel,_("needs operator access rights."));
//                            }
//                        } else {
//                            /* add callback for reset the modes for a user    */
//                            logger(LOG_DEBUG,_("Added callback routine for the reset of the account modes"));
//                            
//                            /* built the data for callback */
//                            pData=(char*)malloc((strlen(pChannel)+3)*sizeof(char));
//                            sprintf(pData,"%s %c%c",pChannel,pModeCurr[0],*pPos);
//                            
//                            /* build  the  element*/
//                            Callback=(CallbackItem_t*)malloc(sizeof(CallbackItem_t));
//                            Callback->nickname=(char*)malloc((strlen(pNick)+1)*sizeof(char));
//                            strcpy(Callback->nickname,pNick);
//                            Callback->CallbackFkt=ModeResetCb;
//                            Callback->data=pData;
//                            
//                            /* put  the  element  in the  callback list  before tail */
//                            insert_prev_CallbackDList(&CallbackList,CallbackList.tail,Callback);
//                
//                            /* send the who*/
//                            whois(pNick);
//                        }
//                    } else if (pPos=='b') {
//                        logger(LOG_INFO,_("Ban reset not implemented jet"));
//                    } else {
//                        if ((pChannelStr=get_db(CHANNEL_DB,pChannel))) {
//                            StrToChannelData(pChannelStr,&ChannelSet);
//                               free(pChannelStr);
//                        }
//                    }
//                }
//                
//                /* switch to the second part */
//                if (pModeSec) {pModeCurr=pModeSec;} 
//            }
        }     
        free(pTmpBotName);
        free(pAccessNick);
    }
    free(pChannel);
    free(pMode);
    free(pNick);

}
/* #########################################################################
   Event handler: TOPIC
   Action: reset the top  for the chanel if this change not by bot self
   ######################################################################### */
void hResetTopic(char *pLine){
    extern ConfigSetup_t sSetup;
    char *pChannel;
    char *pChannelSet;
    char *pTopic;
    char *pNick;
    char *pTmpBotName;

    pNick=getNickname(pLine);

    if (pNick) {
        /* make  a bot name with small letters */
        pTmpBotName=(char*)malloc((strlen(sSetup.pBotname)+1)*sizeof(char));
        strcpy(pTmpBotName,sSetup.pBotname);
        StrToLower(pTmpBotName);

        if (strcmp(pNick,pTmpBotName)) {
    
            /* get the  right topic for this channel*/
            if (!(pChannel=getAccessChannel(pLine)))
                return;
    
    
            if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
            	if ((pTopic=getTopic(pChannelSet))) {
                	/* reset the topic */
    	            topic(pChannel,pTopic);
        	        free(pTopic);
            	} else {
    	            topic(pChannel,"");
        	    }
    
                logger(LOG_DEBUG,_("Reset the topic in the channel %s"),pChannel);
            	free(pChannelSet);
    		}	
            free(pChannel);
        }
        free(pNick);
        free(pTmpBotName);
    }
}
/* #########################################################################
   Event handler: KICK
   Action: rejoin the channel fater  kick
   #########################################################################*/
void hRejoinAfterKick(char *pLine){
    char *pRest=NULL;
    char *pChannel;
    char *pNick;
    char *pTmpBotName;
    extern ConfigSetup_t sSetup;

    rmFirstPart(pLine,&pRest);
    rmFirstPart(pRest,&pRest);
    
    pChannel=getFirstPart(pRest,&pRest);
    pNick=getFirstPart(pRest,NULL);
    free(pRest);
                                               
    /* make  a bot name with small letters */
    pTmpBotName=(char*)malloc((strlen(sSetup.pBotname)+1)*sizeof(char));
    strcpy(pTmpBotName,sSetup.pBotname);
    
    StrToLower(pTmpBotName);
    StrToLower(pNick);

    if (!strcmp(pTmpBotName,pNick)) {
        join(pChannel);

        logger(LOG_DEBUG,_("Rejoin the channel %s"),pChannel);
    }
    free(pTmpBotName);
    free(pChannel);
    free(pNick);

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
void hCallback(char *pLine) {
    extern CallbackDList CallbackList;
    CallbackItem_t *CB_Data;
    CallbackDListItem *pCallbackItemReturn;
    char *pNetmask;
    char *pRest=NULL;
    char *pNick;
    char *pLogin;
    char *pDomain;

    rmFirstPart(pLine,&pRest); /* 0 */
    rmFirstPart(pRest,&pRest); /* 1 */
    rmFirstPart(pRest,&pRest); /* 2 */
    
    pNick=getFirstPart(pRest,&pRest);
    pLogin=getFirstPart(pRest,&pRest);
    pDomain=getFirstPart(pRest,NULL);
    free(pRest);
    StrToLower(pNick);

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
void hWhoisFailed(char *pLine) {
    extern CallbackDList CallbackList;
    CallbackItem_t *CB_Data;
    CallbackDListItem *pCallbackItemReturn;
    char *pRest=NULL;
    char *pNick;

    rmFirstPart(pLine,&pRest); /* 0 */
    rmFirstPart(pRest,&pRest); /* 1 */
    rmFirstPart(pRest,&pRest); /* 2 */
    
    pNick=getFirstPart(pRest,NULL);
    free(pRest);
    StrToLower(pNick);

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

