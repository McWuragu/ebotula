/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

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

    		DEBUG("Change the netmask \"%s\" to \"%s\"",pNetmask,pNewNetmask);
	    	
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

    pChannel=getAccessChannel(pLine);

    // extrakt Namelist
    pPos=strchr(&pLine[1],':');
    pNickList=(char*)malloc((strlen(pPos)+1)*sizeof(char));
    strcpy(pNickList,pPos);

    pSearchStr=(char *) malloc((strlen(sSetup.botname)+2)*sizeof(char));
    sprintf(pSearchStr,"@%s",sSetup.botname);

    if (!strstr(pNickList,pSearchStr)) {
        privmsg(pChannel,MSG_NEED_OP);
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
    	    DEBUG("Set the mod for Account %s with nickname %s",pLogin,pNick);

            pChannel=getAccessChannel(pLine);

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
    
    

    //splitthe string
    ppLinePart=splitString(pLine);
    pChannel=(char*)ppLinePart[2];
    pMode=(char*)ppLinePart[3];

    if (!strstr(ppLinePart[0],sSetup.botname)) {
        if (pMode[1]=='o' || pMode[1]=='v') {
            // add callback for reset the modes for a user    
            DEBUG("Added Callback for Mode Reset");
           
            // extract  the nick
            pNick=(char*)ppLinePart[4];
            
            // built the data for callback
            pData=(char*)malloc((strlen(pChannel)+strlen(pMode)+1)*sizeof(char));
            sprintf(pData,"%s %s",pChannel,pMode);
            
            // build  the  element
            Callback=(CallbackItem_t*)malloc(sizeof(CallbackItem_t));
            Callback->nickname=pNick;
            Callback->CallbackFkt=ModeResetCb;
            Callback->data=pData;
            
            // put  the  element  in the  callback list  before tail
            //insert_prev_CallbackDList(&CallbackList,&CallbackList.tail,&Callback);
	insert_prev_CallbackDList(&CallbackList,CallbackList.tail,Callback);

            // send the who
            whois(pNick);


        } else {
            // reset channel
            pPos=strstr(pLine,pMode);
            pPos[0]=(pPos[0]=='-')?'+':'-';
            mode(pChannel,pPos,NULL);
            DEBUG("Reset the modes from the channel %s",pChannel);
        }
    } else if (strcmp(getNickname(ppLinePart[0]),sSetup.botname)) {
        // mode set for the bot from other user of operator
        // then initiallize this  channel
        if (strcmp(pMode,"+o")) {
            privmsg(pChannel,MSG_INIT_CHANNEL);
            channelInit(pChannel);
            DEBUG("Initialize the channel %s", pChannel);

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
        pChannel=getAccessChannel(pLine);


        if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
        	if ((pTopic=getTopic(pChannelSet))) {
            	// reset the topic
	            topic(pChannel,pTopic);
    	        free(pTopic);
        	} else {
	            topic(pChannel,"");
    	    }

            DEBUG("Reset the topic in the channel %s",pChannel);
        	free(pChannelSet);
		}	
        free(pChannel);
    }
}
// #########################################################################
// Event handler: MODE
// Action: initialize the channel after  get operator access
// #########################################################################
void hInitAfterOp(char *pLine) {
    char *pChannel;

    pChannel=getAccessChannel(pLine);

    channelInit(pChannel);

    privmsg(pChannel,MSG_INIT_CHANNEL);
}

// #########################################################################
// Event helper: Channel intializalisation
// #########################################################################
static void channelInit(char *pChannel) {
    ChannelData_t *pChannelData;
    char *pMode,*pChannelSet;

    // check of  existenz of the channel
    if (exist_db(CHANNEL_DB,pChannel)) {
        
        
        if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
	        pChannelData=StrToChannelData(pChannelSet);

    	    // set Topic
        	if (pChannelData->pTopic) {
	            topic(pChannel,pChannelData->pTopic);
    	    }

        	// set Modes
	        if ((pMode=ChannelModeToStr(pChannelData->pModes))) {
    	        mode(pChannel,pMode,NULL);
        	}
		}
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

    ppLinePart=splitString(pLine);

    /** lock for the Callback item for the nick **/
    //    CB_Data=searchNicknameFromCallbackDList(&CallbackList,CallbackList.head,ppLinePart[3]);
	/** remove was removed from searchNicknameFromCallbackDList 
	 * Remove all by hand 						**/
    pCallbackItemReturn=searchNicknameFromCallbackDList(&CallbackList,CallbackList.head,ppLinePart[3]);
	 
    // remove  entrie and read the  callback datum
    if (removeCallbackDList(&CallbackList,pCallbackItemReturn,CB_Data)) {
        // built netmask
        pNetmask=(char*)malloc((strlen(ppLinePart[3])+strlen(ppLinePart[4])+strlen(ppLinePart[5])+3)*sizeof(char));
        sprintf(pNetmask,"%s!%s@%s",ppLinePart[3],ppLinePart[4],ppLinePart[5]);

        // execute the callback
        DEBUG("Callback");
        CB_Data->CallbackFkt(pNetmask,CB_Data->data);

        // destroy  callback item
        destroyCallbackItem(CB_Data);
    }
  
}
