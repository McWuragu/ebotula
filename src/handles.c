/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
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


// #########################################################################
// Event handler: NICK
// #########################################################################
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
    		// get the new nickname
    		pNewNick=strrchr(pLine,' ');
    		
    		if (strlen(pNewNick)>2) {
    			pNewNick+=2;
    
    			pNetmaskCopy=(char*)malloc((strlen(pNetmask)+1)*sizeof(char));
    			strcpy(pNetmaskCopy,pNetmask);
    			
    			// cut out the secondary part of the netmask
    			pTmp=strchr(pNetmaskCopy,'!');
    			strtok(pTmp," ");
    
    			// build new netmask	
    			pNewNetmask=(char*)malloc((strlen(pNewNick)+strlen(pTmp)+1)*sizeof(char));
    			sprintf(pNewNetmask,"%s%s",pNewNick,pTmp);

    			// reset Login Status
    			log_out(pLogin);
    			log_on(pNewNetmask,pLogin);
    
        		DEBUG("Change the netmask \"%s\" to \"%s\"\n",pNetmask,pNewNetmask);
    	    	
    			free(pNetmaskCopy);
    	    	free(pNewNetmask);
    			free(pLogin);
    		}
    
    	}
        free(pNetmask);
    }

}
// #########################################################################
// Event handler: JOIN
// Action: Request OP
// #########################################################################
void hBotNeedOp(char *pLine){
    extern ConfigSetup_t sSetup;
    char *pChannel;
    char *pNickList;
    char *pPos;
    char *pSearchStr;

    

    if (!(pChannel=getAccessChannel(pLine)))
        return;

    // extrakt Namelist
    pPos=strchr(&pLine[1],':');
    pNickList=(char*)malloc((strlen(pPos)+1)*sizeof(char));
    strcpy(pNickList,pPos);

    pSearchStr=(char *) malloc((strlen(sSetup.pBotname)+2)*sizeof(char));
    sprintf(pSearchStr,"@%s",sSetup.pBotname);

    if (!strstr(pNickList,pSearchStr)) {
        privmsg(pChannel,getMsgString(INFO_NEED_OP));
    } else {
        channelInit(pChannel);
    }


    free(pSearchStr);
    free(pNickList);
    free(pChannel);
}
// #########################################################################
// Event handler: JOIN
// Action: set the mod  for the user
// #########################################################################
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
            	    DEBUG("Set the mod for Account %s with nickname %s\n",pLogin,pNick);
        
                    if (!(pChannel=getAccessChannel(pLine)))
                        return;
        
                	// build key for access.dbf
                    pKey=(char *)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char));
        	        sprintf(pKey,"%s%s",pLogin,pChannel);
           	        
        			// read  the  mod
               	    if ((pMod=get_db(ACCESS_DB,pKey))) {
                	    // set the mod  for this nick
               	       	mode(pChannel,pMod,pNick);
                        free(pMod);
        	        } else if (exist_db(ACCESS_DB,pLogin)) {
           		        mode(pChannel,"+o",pNick);
               		}
        
                   	free(pChannel);
                    free(pKey);
        
                }
                free(pNetmask);
            }
        }
        free(pTmpBotName);
        free(pNick);
    }
}
// #########################################################################
// Event handler: MODE
// Action:  added the callback item in the list for the user if this change
//          not by bot self
// #########################################################################
void hResetModes(char *pLine) {
    extern ConfigSetup_t sSetup;
    char *pPos;
    char *pChannel;
    char *pData;
    char *pNick;
    char *pNetmask;
    char *pAccessNick;
    char *pMode;
    char *pRest=NULL;
    extern CallbackDList CallbackList;
    CallbackItem_t *Callback;
    
    

    //splitt the string
    pNetmask=getFirstPart(pLine,&pRest);
    rmFirstPart(pRest,&pRest);
    
    pChannel=getFirstPart(pRest,&pRest);
    pMode=getFirstPart(pRest,&pRest);
    pNick=getFirstPart(pRest,&pRest);;
    free(pRest);
    
    // extract  the nick
    pAccessNick=getNickname(pNetmask);
    free(pNetmask);

    if (pAccessNick) {
        char *pTmpBotName;
        pTmpBotName=(char*)malloc((strlen(sSetup.pBotname)+1)*sizeof(char));
        strcpy(pTmpBotName,sSetup.pBotname);
        StrToLower(pTmpBotName);

        if (strcmp(pAccessNick,pTmpBotName)!=0) {
            if (pMode[1]=='o' || pMode[1]=='v') {
              
                // check of bot new mods or  other user
                if (!strcmp(pNick,pTmpBotName)) {
                    DEBUG("Bot get new mods\n");
                // mode set for the bot from other user of operator
                    // then initiallize this  channel
                    if (strcmp(pMode,"+o")==0) {
                        channelInit(pChannel);
                    } else {
                        privmsg(pChannel,getMsgString(INFO_NEED_OP));
                    }
                } else {
                    // add callback for reset the modes for a user    
                    DEBUG("Added Callback for Mode Reset\n");
                    
                    // built the data for callback
                    pData=(char*)malloc((strlen(pChannel)+strlen(pMode)+1)*sizeof(char));
                    sprintf(pData,"%s %s",pChannel,pMode);
                    
                    // build  the  element
                    StrToLower(pNick);
                    Callback=(CallbackItem_t*)malloc(sizeof(CallbackItem_t));
                    Callback->nickname=pNick;
                    Callback->CallbackFkt=ModeResetCb;
                    Callback->data=pData;
                    
                    // put  the  element  in the  callback list  before tail
                    insert_prev_CallbackDList(&CallbackList,CallbackList.tail,Callback);
        
                    // send the who
                    whois(pNick);
                }
            } else if (pMode[1]=='b') {
                DEBUG("Ban reset not implemented jet\n");
            } else {
                // reset other mods
                pPos=strstr(pLine,pMode);
                pPos[0]=(pPos[0]=='-')?'+':'-';
                mode(pChannel,pPos,NULL);
                DEBUG("Reset the modes from the channel %s",pChannel);
            }
        } else if (strcmp(pAccessNick,pTmpBotName)!=0) {
            DEBUG("Bot get new mods\n");
            // mode set for the bot from other user of operator
            // then initiallize this  channel
            if (strcmp(pMode,"+o")==0) {
                channelInit(pChannel);
            } else {
                privmsg(pChannel,getMsgString(INFO_NEED_OP));
            }
        }
        free(pTmpBotName);
        free(pAccessNick);
    } else {
        free(pNick);
    }
    free(pChannel);
    free(pMode);

}
// #########################################################################
// Event handler: TOPIC
// Action: reset the top  for the chanel if this change not by bot self
// #########################################################################
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
    
            // get the  right topic for this channel
            if (!(pChannel=getAccessChannel(pLine)))
                return;
    
    
            if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
            	if ((pTopic=getTopic(pChannelSet))) {
                	// reset the topic
    	            topic(pChannel,pTopic);
        	        free(pTopic);
            	} else {
    	            topic(pChannel,"");
        	    }
    
                DEBUG("Reset the topic in the channel %s\n",pChannel);
            	free(pChannelSet);
    		}	
            free(pChannel);
        }
        free(pNick);
        free(pTmpBotName);
    }
}
// #########################################################################
// Event handler: KICK
// Action: rejoin the channel fater  kick
// #########################################################################
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

    if (!strcmp(pTmpBotName,pNick)) {
        join(pChannel);

        DEBUG("Rejoin the  channel %s\n",pChannel);
    }
    free(pTmpBotName);
    free(pChannel);
    free(pNick);

}

// #########################################################################
// Event handler: MODE
// Action: initialize the channel after  get operator access
// #########################################################################

void hInitAfterOp(char *pLine) {
    char *pChannel;
                   
    if (!(pChannel=getAccessChannel(pLine)))
        return;

    channelInit(pChannel);
}

// #########################################################################
// Event helper: Channel intializalisation
// #########################################################################
static void channelInit(char *pChannel) {
    ChannelData_t sChannelData;
    char *pMode,*pChannelSet;

      
    if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
        StrToChannelData(pChannelSet,&sChannelData);
        free(pChannelSet);
                                             
        // set Topic
        if (sChannelData.pTopic) {
            topic(pChannel,sChannelData.pTopic);
        }

        // set Modes
        pMode=ChannelModeToStr(&(sChannelData.sModes));
        if (strlen(pMode)) {
            mode(pChannel,pMode,NULL);
        }
        free(pMode);

        free(sChannelData.pGreeting);
        free(sChannelData.pTopic);
        free (sChannelData.sModes.pKeyword);
        free (sChannelData.sModes.pLimit);
	
        privmsg(pChannel,getMsgString(INFO_INIT_CHANNEL));
        DEBUG("Initialize the channel %s\n", pChannel);
    }
}

// #########################################################################
// Event handler: 352
// Action: call the callback function for a nickname
// #########################################################################
void hCallback(char *pLine) {
    extern CallbackDList CallbackList;
    CallbackItem_t *CB_Data;
    CallbackDListItem *pCallbackItem;
    CallbackDListItem *pCallbackItemReturn;
    char *pNetmask;
    char *pRest=NULL;
    char *pNick;
    char *pLogin;
    char *pDomain;

    rmFirstPart(pLine,&pRest); // 0
    rmFirstPart(pRest,&pRest); // 1
    rmFirstPart(pRest,&pRest); // 2
    
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
            // built netmask
            pNetmask=(char*)malloc((strlen(pNick)+strlen(pLogin)+strlen(pDomain)+3)*sizeof(char));
            sprintf(pNetmask,"%s!%s@%s",pNick,pLogin,pDomain);
    
            // execute the callback
            DEBUG("Callback\n");
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

// #########################################################################
// Event handler: 401
// Action: handle a whois failed
// #########################################################################
void hWhoisFailed(char *pLine) {
    extern CallbackDList CallbackList;
    CallbackItem_t *CB_Data;
    CallbackDListItem *pCallbackItemReturn;
    char *pRest=NULL;
    char *pNick;

    rmFirstPart(pLine,&pRest); // 0
    rmFirstPart(pRest,&pRest); // 1
    rmFirstPart(pRest,&pRest); // 2
    
    pNick=getFirstPart(pRest,NULL);
    free(pRest);
    StrToLower(pNick);

    DEBUG("Callback Zombie %s\n",pNick);

   /** lock for the Callback item for the nick **/
    if ((pCallbackItemReturn=searchNicknameFromCallbackDList(&CallbackList,CallbackList.head,pNick))) {
        /* 
         * remove  entrie and read the  callback datum
         * if  this item not more in the  queue then look for  the next
         */
        if (!removeCallbackDList(&CallbackList,pCallbackItemReturn,&CB_Data)) {
            /* destroy  callback item */
            destroyCallbackItem(CB_Data);
            DEBUG("Callback Zombie removed\n");
        }
    }
    free(pNick);
}

