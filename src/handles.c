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
#include "utilities.h"
#include "dbaccess.h"
#include "messages.h"
#include "irc.h"
#include "type.h"
#include "handles.h"


// #########################################################################
// Event handler: NICK
// #########################################################################
void hNickChange(char *pLine) {
    extern pthread_mutex_t account_mutex;
    char *pNetmask;
    char *pLogin;
    char *pNewNetmask;

    pNetmask=getNetmask(pLine);
    
    pNewNetmask=strstr(pLine," :");
    pNewNetmask+=2;
        strtok(pNewNetmask,"\r");

    pthread_mutex_lock(&account_mutex);
    if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {

	    // replace  the netmask
    	if (del_db(NICKTOUSER_DB,pNetmask)) {
        	add_db(NICKTOUSER_DB,pNewNetmask,pLogin);
	        replace_db(USERTONICK_DB,pLogin,pNewNetmask);
    	}

    	free(pNetmask);

    	DEBUG("Changethe netmask \"%s\" to \"%s\"",pNetmask,pNewNetmask);
	}
    pthread_mutex_unlock(&account_mutex);

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

    DEBUG("Look for OP right for %s",pSearchStr);
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
    extern pthread_mutex_t account_mutex;
    char *pLogin;
    char *pNick;
    char *pKey;
    char *pChannel;
    char *pMod;

    
    pNick=getNickname(pLine);

    if (strcmp(pNick,sSetup.botname)) {
        DEBUG("Set the mod for Account %s with nickname %s",pLogin,pNick);
            
        pthread_mutex_lock(&account_mutex);
        if ((pLogin=get_db(NICKTOUSER_DB,getNetmask(pLine)))) {

	        if (strlen(pLogin)) {
    	        pChannel=getAccessChannel(pLine);

        	    // build key for access.dbf
            	pKey=(char *)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char));
	            sprintf(pKey,"%s%s",pLogin,pChannel);

    	        // read  the  mod
        	    if ((pMod=get_db(ACCESS_DB,pKey))) {
	        	    // set the mod  for this nick
    	        	if (strlen(pMod)) {
        	        	mode(pChannel,pMod,pNick);
	            	    free(pMod);
		            } else if (exist_db(ACCESS_DB,pLogin)) {
    		            mode(pChannel,"+o",pNick);
        		    }
				}
            	free(pChannel);
	            free(pKey);
			}

        }
        pthread_mutex_unlock(&account_mutex);
    }
    free(pNick);
}
// #########################################################################
// Event handler: MODE
// Action: reset the mod  for the user if this chang not by bot self
// #########################################################################
void hResetModes(char *pLine) {
    extern ConfigSetup_t sSetup;
    char *pPos;
    char *pChannel;
    char *pMod;
    char *pNick;

    pChannel=getAccessChannel(pLine);

    // set pPos of the position of changed mode
    pPos=strstr(pLine,"MODE");
    pPos=strchr(pPos,' ');
    pPos++;
    pPos=strchr(pPos,' ');
    pPos++;

    if (!strstr(pLine,sSetup.botname)) {
        DEBUG("Reset the Modes");


        pPos[0]=(pPos[0]=='+')?'-':'+';

        // reset
        mode(pChannel,pPos,NULL);
        free(pChannel);
    } else if (strcmp(getNickname(pLine),sSetup.botname)) {
        // mode set for the bot from other user of operator
        // then initiallize this  channel
        if (pPos[1]=='o') {
            privmsg(pChannel,MSG_INIT_CHANNEL);
            channelInit(pChannel);

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
        	free(pChannelSet);
		}	
        free(pChannel);
    }
}
// #########################################################################
// Event handler: MODE
// Action: initialize the channel after  become operator access
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
