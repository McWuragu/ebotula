/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
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

    pSearchStr=(char *) malloc((strlen(sSetup.botname)+2)*sizeof(char));
    sprintf(pSearchStr,"@%s",sSetup.botname);

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
    char *pMod;

    
    pNick=getNickname(pLine);

    if (strcmp(pNick,sSetup.botname)) {
        if ((pLogin=get_db(NICKTOUSER_DB,getNetmask(pLine)))) {
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
    }
    free(pNick);
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
    char *pMode;
    char **ppLinePart;
    extern CallbackDList CallbackList;
    CallbackItem_t *Callback;
    
    

    //splitt the string
    ppLinePart=splitString(pLine,6);
    pChannel=(char*)ppLinePart[2];
    pMode=(char*)ppLinePart[3];

    // extract  the nick
    pNick=(char*)ppLinePart[4];
    
    if (strcmp(getNickname(ppLinePart[0]),sSetup.botname)!=0) {
        if (pMode[1]=='o' || pMode[1]=='v') {
          
            // check of bot new mods or  other user
            if (!strcmp(pNick,sSetup.botname)) {
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
    } else if (strcmp(getNickname(ppLinePart[0]),sSetup.botname)!=0) {
        DEBUG("Bot get new mods\n");
        // mode set for the bot from other user of operator
        // then initiallize this  channel
        if (strcmp(pMode,"+o")==0) {
            channelInit(pChannel);
        } else {
            privmsg(pChannel,getMsgString(INFO_NEED_OP));
        }
    }
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

    if (strcmp(getNickname(pLine),sSetup.botname)) {

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
}
// #########################################################################
// Event handler: KICK
// Action: rejoin the channel fater  kick
// #########################################################################
void hRejoinAfterKick(char *pLine){
    char ** pArgv;
    extern ConfigSetup_t sSetup;

    pArgv=splitString(pLine,5);
                                               
    if (!strcmp(sSetup.botname,pArgv[3])) {
        join(pArgv[2]);

        DEBUG("Rejoin the  channel %s\n",pArgv[2]);
    }
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
    ChannelData_t *pChannelData;
    char *pMode,*pChannelSet;

      
    if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
        pChannelData=StrToChannelData(pChannelSet);

        // set Topic
        if (pChannelData->pTopic) {
            topic(pChannel,pChannelData->pTopic);
        }

        // set Modes
        pMode=ChannelModeToStr(pChannelData->pModes);
        if (strlen(pMode)) {
            mode(pChannel,pMode,NULL);
        }

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
    char **ppLinePart;
    char *pNick;

    ppLinePart=splitString(pLine,7);

    /* identify nick name only small charakter */
    pNick=(char*)malloc((strlen(ppLinePart[3])+1)*sizeof(char*));
    strcpy(pNick,ppLinePart[3]);
    StrToLower(pNick);

    /*
     * this  loop stop if no item more found  in the  queue
     * Any  call of  this  callback handler execute one callback item
     */
    do {
        /** lock for the Callback item for the nick **/
        if ((pCallbackItemReturn=searchNicknameFromCallbackDList(&CallbackList,CallbackList.head,pNick))) {
            /* 
             * remove  entrie and read the  callback datum
             * if  this item not more in the  queue then look for  the next
             */
            if (!removeCallbackDList(&CallbackList,pCallbackItemReturn,&CB_Data)) {
                // built netmask
                pNetmask=(char*)malloc((strlen(ppLinePart[3])+strlen(ppLinePart[4])+strlen(ppLinePart[5])+3)*sizeof(char));
                sprintf(pNetmask,"%s!%s@%s",ppLinePart[3],ppLinePart[4],ppLinePart[5]);
        
                // execute the callback
                DEBUG("Callback\n");
                CB_Data->CallbackFkt(pNetmask,CB_Data->data);
        
                /* destroy  callback item */
                destroyCallbackItem(CB_Data);
    
                break;
            }
        } else {
            /* no item found in the callback queue */
            break;
        }
      
    } while ( true );
}
