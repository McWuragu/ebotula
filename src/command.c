/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * #############################################################
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "ircbot.h"
#include "extract.h"
#include "utilities.h"
#include "messages.h"
#include "dbaccess.h"
#include "irc.h"
#include "irchelp.h"
#include "account.h"
#include "callbacklist.h"
#include "callback.h"
#include "command.h"


/* #########################################################################
   Bot comand: !help <command>
   ######################################################################### */
void help(MsgItem_t *pMsg) {
    char *pParameter;
    char *pTmp;
    char *pMsgStr;
    char *pMsgPart;
    unsigned int i,j,iLength;


    pParameter=getParameters(pMsg->pRawLine);


    /* check for parameters */
    if (!pParameter) {
        DEBUG("Commands list\n");

        /* Header of help message */
        for (i=0;pIrcHelp[0][i][0]!=EOM;i++) {
            /* look for the end  of msg */
            privmsg(pMsg->pCallingNick,pIrcHelp[0][i]);
        }

        for (i=CMD_OTHERS;i<CMDCOUNT;i++) {
            /* checking  for allow commands */
            if (pMsg->UserLevel < LoggedLevel && i>=CMD_LOGGED) {
                continue;
            } else if (pMsg->UserLevel < MasterLevel && i >= CMD_MASTER) {
                continue;
            }

            /* calculat the length of buffer */
            pMsgStr=(char *)calloc(HELP_TAB+1+strlen((char*)pIrcHelp[CmdIdToHelpId(i)][0]),sizeof(char));

            /* build string */
            strcpy(pMsgStr,CmdList[i]);
            iLength=HELP_TAB-strlen(CmdList[i]);
            /* fill */
            for (j=0;j<iLength;j++) {
                strcat(pMsgStr," ");
            }
            strcat(pMsgStr,(char*)pIrcHelp[CmdIdToHelpId(i)][0]);
            /* send notice */
            privmsg(pMsg->pCallingNick,pMsgStr);
        }
        /* the tail */
        privmsg(pMsg->pCallingNick,getMsgString(INFO_HELP_END));
    } else {
        DEBUG("Spezial help text for a command\n");

        /* cut the first word */
        strtok(pParameter," ");

        /* checking  of a leading '!' */
        if (pParameter[0]!='!') {
            pTmp=(char*)malloc((strlen(pParameter)+2)*sizeof(char));
            sprintf(pTmp,"!%s",pParameter);
            free(pParameter);
            pParameter=pTmp;
        }

        DEBUG("Looking for the information about \"%s\"\n",pParameter);

        /* Help for a command */
        for (i=CMD_OTHERS+1;i<CMDCOUNT;i++) {
            if (!strcmp((char*)CmdList[i],&pParameter[1])) {
                DEBUG("Command found %d\n",i);
                int nCmdHelpID=CmdIdToHelpId(i);

                /* the headi for help */
                pMsgPart=getMsgString(INFO_HELP_FOR);
                pTmp=(char*)malloc((strlen(pMsgPart)+strlen((char *)CmdList[i])+3)*sizeof(char));
                sprintf(pTmp,"%s %s:",pMsgPart,pParameter);
                privmsg(pMsg->pCallingNick,pTmp);

                /* print  the  help text */
                for (j=1;pIrcHelp[nCmdHelpID][j][0]!=EOM;j++) {
                    privmsg(pMsg->pCallingNick,(char*)
                           pIrcHelp[nCmdHelpID][j]);
                }

                /* syntax from the command */
                privmsg(pMsg->pCallingNick,pIrcSyntax[0][0]);
                for (j=0;pIrcSyntax[nCmdHelpID][j][0]!=EOM;j++) {
                    privmsg(pMsg->pCallingNick,(char*)pIrcSyntax[nCmdHelpID][j]);
                }
                privmsg(pMsg->pCallingNick,getMsgString(INFO_HELP_END));
                return;
            }
        }
        privmsg(pMsg->pCallingNick,getMsgString(ERR_NOT_COMMAND));
    }
}
/* #########################################################################
   Bot comand: !hello
   ######################################################################### */
void hello(MsgItem_t *pMsg) {
    DEBUG("Try to create an new account for %s\n",pMsg->pCallingNick);

    if (pMsg->UserLevel>LoggedLevel) {
        notice(pMsg->pCallingNick,getMsgString(ERR_ALREADY_LOGON));
        return;
    }

    StrToLower(pMsg->pCallingNick);


    if (add_db(USER_DB,pMsg->pCallingNick,"")) {
        
        /* autoidentify after create an new account */
        log_on(pMsg->pNetmask,pMsg->pCallingNick);

        notice(pMsg->pCallingNick,getMsgString(OK_HELLO));
        notice(pMsg->pCallingNick,getMsgString(OK_HELLO2));
        notice(pMsg->pCallingNick,getMsgString(OK_IDENT));
        
    } else {
	    notice(pMsg->pCallingNick,getMsgString(ERR_NICK_EXIST));
	} 

}
/* #########################################################################
   Bot comand: !pass <password>
   ######################################################################### */
void password(MsgItem_t *pMsg) {
    char *pLogin;
    char *pPasswd;

    
    if ((pLogin=get_db(NICKTOUSER_DB,pMsg->pNetmask))) {

	    DEBUG("Check the  password for the account %s\n",pLogin);

	    /* get  the  login name */
	    if (strlen(pLogin)) {
        	pPasswd=getParameters(pMsg->pRawLine);

	        /* parse the  password  form  parameter list */
	        if (!pPasswd) {
        	    notice(pMsg->pCallingNick,getMsgString(INFO_NOT_PASS));
	        }

        	/* set password */
	        replace_db(USER_DB,pLogin,pPasswd);
        	notice(pMsg->pCallingNick,getMsgString(OK_PASSWD));        
	    }
    }
}
/* #########################################################################
   Bot comand: !logoff
   ######################################################################### */
void logoff(MsgItem_t *pMsg,int nRemoveMode) {
    char *pLogin;
	char *pKey;
	char *pMode;

	QueueData *pChannel;
    PQueue pChannelQueue;

    int i, nLength;

	/* check login status */
   	if ((pLogin=get_db(NICKTOUSER_DB,pMsg->pNetmask))) {
	    log_out(pLogin);
   
		/* only by manuel logoff then remove  the modes */
		if (nRemoveMode) {	
			/* get channel list */
			pChannelQueue=list_db(CHANNEL_DB);
			nLength=strlen(pLogin);
			/* remove the mod  for  this account */
			while (isfullQueue(pChannelQueue)){
                pChannel=popQueue(pChannelQueue);
				pKey=(char *)malloc((pChannel->t_size+nLength)*sizeof(char));
				sprintf(pKey,"%s%s",pLogin,(char *)pChannel->data);
				
				/* look for the  access rights and  remove this */
				if ((pMode=get_db(ACCESS_DB,pKey))){
					pMode[0]='-';
					mode(pChannel->data,pMode,pLogin);	
					free(pMode);
				} else if (pMsg->UserLevel==MasterLevel){
                    mode(pChannel->data,"-o",pLogin);
                }
				free(pKey);
				free(pChannel);	
			}

			/* delete the queue */
            deleteQueue(pChannelQueue);
			notice(pMsg->pCallingNick,getMsgString(OK_LOGOFF));
		}
	}
}
/* #########################################################################
   Bot comand: !ident login <password>
   ######################################################################### */
void ident(MsgItem_t *pMsg) {
    char *pLogin;
    char *pPasswd;
    char *pPos;
    char *pParameter;
    PQueue pChannelQueue;
	QueueData *pChannel;
    char *pKey;
    char *pMod;
    boolean isMaster;

    int i,login_len;


    DEBUG("try to identify %s\n",pMsg->pCallingNick);

    
    if (!exist_db(NICKTOUSER_DB,pMsg->pNetmask)) {
        pParameter=getParameters(pMsg->pRawLine);
    
        /* no parameter found */
        if (pParameter) {
            /* parse the password */
            if ((pPos=strstr(pParameter," "))==NULL) {
                /* no Passwd found */
                /* try empty pass */
                notice(pMsg->pCallingNick,getMsgString(INFO_NOT_PASS));
                pPasswd="";
            } else {
                pPasswd=(char *)malloc(strlen(pPos)*sizeof(char));
                strcpy(pPasswd,&pPos[1]);
            }
        
        
            /* parse the login */
            strtok(pParameter," ");
            pLogin=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
            strcpy(pLogin,pParameter);
            StrToLower(pLogin);
        
            DEBUG("Look for the account %s\n",pLogin);
        
            /* check the account */
            if (check_db(USER_DB,pLogin,pPasswd)) {
                DEBUG("User %s found\n",pLogin);
                log_on(pMsg->pNetmask,pLogin);
                notice(pMsg->pCallingNick,getMsgString(OK_IDENT));
        
                isMaster=exist_db(ACCESS_DB,pLogin);
        
                /* set the mod  for  this account */
                pChannelQueue=list_db(CHANNEL_DB);
                login_len=strlen(pLogin);
        
                DEBUG("Reconstruct the access rights for the channels\n");
                while (isfullQueue(pChannelQueue)) {
                    pChannel=popQueue(pChannelQueue);

                    if (isMaster) {
                        mode((char*)pChannel->data,"+o",pMsg->pCallingNick);
                    } else {
                        pKey=(char*)malloc((pChannel->t_size+login_len+1)*sizeof(char));
                        sprintf(pKey,"%s%s",pLogin,(char*)pChannel->data);
                        if ((pMod=get_db(ACCESS_DB,pKey))) {
   	                        mode((char*)pChannel->data,pMod,pMsg->pCallingNick);
      	                    free(pMod);
          	            }
                       	free(pKey);
                    }
					free(pChannel);
                }
				deleteQueue(pChannelQueue);
            } else {
                notice(pMsg->pCallingNick,getMsgString(ERR_NOT_ACCOUNT));
            }    
        } else {
            notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        }
    } else {
        notice(pMsg->pCallingNick,getMsgString(ERR_ALREADY_LOGON));
    }
}
/* #########################################################################
   Bot comand: !addchannel #channel
   ######################################################################### */
void addChannel(MsgItem_t *pMsg) {
    char *pCmdChannel;
    char *channelmod;

   

    if (!pMsg->pAccessChannel){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    if ((pCmdChannel=getChannel(pMsg->pRawLine))) {
         if (!strcmp(pMsg->pAccessChannel,pCmdChannel)) {
            notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
            return;
        }
    }

    DEBUG("Join and  try to add the channnel %s\n",pMsg->pAccessChannel);

    /* checking of channel exist */
    if (exist_db(CHANNEL_DB,pMsg->pAccessChannel)) {
        notice(pMsg->pCallingNick,getMsgString(ERR_ADDCHANNEL_ALREADY));
    } else {
        /* add channel */
        channelmod=(char *)malloc(3*sizeof(char));
        strcpy(channelmod,"\t\t");
        add_db(CHANNEL_DB,pMsg->pAccessChannel,channelmod);
        notice(pMsg->pCallingNick,getMsgString(OK_ADDCHANNEL));
    }

    /* join the channel */
    join(pMsg->pAccessChannel);
    notice(pMsg->pCallingNick,getMsgString(OK_JOIN));

}
/* #########################################################################
   Bot comand: !rmchannel <#channel>
   ######################################################################### */
void rmChannel(MsgItem_t *pMsg){
    
    /* check channel parameter */
    if (!pMsg->pAccessChannel) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }
    
    DEBUG("Part and  try to remove the channnel %s\n",pMsg->pAccessChannel);


    /* checking of channel exists */
    if (!del_db(CHANNEL_DB,pMsg->pAccessChannel)) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNEL));
    } else {
        notice(pMsg->pCallingNick,getMsgString(OK_RMCHANNEL));
    }

    // TODO: remove old access  rights for this channel

    /* part the channel */
    part(pMsg->pAccessChannel);
    notice(pMsg->pCallingNick,getMsgString(OK_PART));

}
/* #########################################################################
   Bot comand: !join #channel
   ######################################################################### */
void joinChannel(MsgItem_t *pMsg) {
    char *pCmdChannel;

    

    if (!pMsg->pAccessChannel){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    if ((pCmdChannel=getChannel(pMsg->pRawLine))) {
        /* compare the current channel and  the channel for joining */
        if (!(strcmp(pMsg->pAccessChannel,pCmdChannel))) {
            notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
            return;
        }
    }

    DEBUG("Join the channel %s\n",pMsg->pAccessChannel);
    /* join the channel */
    join(pMsg->pAccessChannel);
    notice(pMsg->pCallingNick,getMsgString(OK_JOIN));

}
/* #########################################################################
   Bot comand: !part <#channel>
   ######################################################################### */
void partChannel(MsgItem_t *pMsg) {

    if (!pMsg->pAccessChannel){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
    } else {
        DEBUG("Part the channel %s\n",pMsg->pAccessChannel);

        /* part the channel */
        part(pMsg->pAccessChannel);
        notice(pMsg->pCallingNick,getMsgString(OK_PART));
    }
}
/* #########################################################################
   Bot comand: !die
   ######################################################################### */
void die(MsgItem_t *pMsg) {
    notice(pMsg->pCallingNick,getMsgString(OK_DIE));
    stopParser(0);
}
/* #########################################################################
   Bot comand: !restart
   ######################################################################### */
void restart(MsgItem_t *pMsg) {
    extern boolean again;
    notice(pMsg->pCallingNick,getMsgString(OK_RESTART));
    again=true;
    stopParser(0);
}
/* #########################################################################
   Bot comand: !nick nickname
   ######################################################################### */
void setNick(MsgItem_t *pMsg){
    char *pParameter;

    pParameter=getParameters(pMsg->pRawLine);

    /* read parameters */
    if (!pParameter) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
    } else if (!NickStringCheck(pParameter)) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NICK_INVALID));
    } else {
        nick(pParameter);
        notice(pMsg->pCallingNick,getMsgString(OK_NICK_SET));
    }

}
/* #########################################################################
   Bot comand: !chanlist
   ######################################################################### */
void chanlist(MsgItem_t *pMsg){
    char *buffer;
    char *pMode;
    char *pMsgStr;
    char *pMsgPart;
	char *pChannelSet;
    int i=0,buffer_size=0;
    ChannelData_t *pChannelData;
	PQueue pChannelQueue;
	QueueData *pChannel;

    DEBUG("Build channel list...\n");


    privmsg(pMsg->pCallingNick,getMsgString(INFO_CHANNELLIST_BEGIN));

    /* get  the channel list form the DB */
    pChannelQueue=list_db(CHANNEL_DB);


    while (isfullQueue(pChannelQueue)) {
        pChannel=popQueue(pChannelQueue);

		if ((pChannelSet=get_db(CHANNEL_DB,(char*)pChannel->data))) {
	        pChannelData=StrToChannelData(pChannelSet);
    	    pMode=ChannelModeToStr(pChannelData->pModes);

        	DEBUG("...for channel %s\n",(char*)pChannel->data);
	        privmsg(pMsg->pCallingNick,(char*)pChannel->data);

            pMsgPart=getMsgString(INFO_CHANNELLIST_MODE);
    	    pMsgStr=(char*)malloc((strlen(pMsgPart)+strlen(pMode)+2)*sizeof(char));
        	sprintf(pMsgStr,"%s %s",pMsgPart,pMode);
	        privmsg(pMsg->pCallingNick,pMsgStr);
    	    free(pMsgStr);

        	if (pChannelData->pTopic) {
                pMsgPart=getMsgString(INFO_CHANNELLIST_TOPIC);
            	pMsgStr=(char*)malloc((strlen(pMsgPart)+strlen(pChannelData->pTopic)+2)*sizeof(char));
	            sprintf(pMsgStr,"%s %s",pMsgPart,pChannelData->pTopic);
    	        privmsg(pMsg->pCallingNick,pMsgStr);
        	    free(pMsgStr);
	        }

    	    if (pChannelData->pGreeting) {
                pMsgPart=getMsgString(INFO_CHANNELLIST_GREET);
        	    pMsgStr=(char*)malloc((strlen(pMsgPart)+strlen(pChannelData->pGreeting)+2)*sizeof(char));
            	sprintf(pMsgStr,"%s %s",pMsgPart,pChannelData->pGreeting);
	            privmsg(pMsg->pCallingNick,pMsgStr);
    	        free(pMsgStr);
        	}


	        free(pMode);
    	    free(pChannelData);
			free(pChannelSet);
		}
		free(pChannel);
    }
	deleteQueue(pChannelQueue);
    privmsg(pMsg->pCallingNick,getMsgString(INFO_CHANNELLIST_END));
}
/* #########################################################################
   Bot comand: !version
   ######################################################################### */
void version(MsgItem_t *pMsg) {
    char pMsgStr[256];
    // creat Versions String
    sprintf(pMsgStr,VERSIONSTR);
    strcat(pMsgStr,"\r\n");
    notice(pMsg->pCallingNick,pMsgStr);
}
/* #########################################################################
   Bot comand: !greeting <#channel> <text>
   ######################################################################### */
void setGreeting(MsgItem_t *pMsg) {
    char *pChannelSet;
    char *pMode;

    ChannelData_t *pChannelData;

    if (!pMsg->pAccessChannel){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    DEBUG("Greeting seting for %s\n",pMsg->pAccessChannel);


    /* check of  existenz of the channel */
    if ((pChannelSet=get_db(CHANNEL_DB,pMsg->pAccessChannel))) {
	    pChannelData=StrToChannelData(pChannelSet);
    	free(pChannelSet);
	    
		/* remove old greeting */
		if (pChannelData->pGreeting) {
    	    free(pChannelData->pGreeting);
	    }
	    pChannelData->pGreeting=getParameters(pMsg->pRawLine);

    
		pChannelSet=ChannelDataToStr(pChannelData);
	    replace_db(CHANNEL_DB,pMsg->pAccessChannel,pChannelSet);


    	/* message */
	    if (!pChannelData->pGreeting) {
     	   notice(pMsg->pCallingNick,getMsgString(OK_RM_GREETING));
	    } else {
    	    notice(pMsg->pCallingNick,getMsgString(OK_SET_GREETING));
	    }
	} else {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNEL));
	}
}
/* #########################################################################
   Bot comand: !topic <#channel> <topic>
   ######################################################################### */
void setTopic(MsgItem_t *pMsg) {
    char *pChannelSet;


    ChannelData_t *pChannelData;

    
    if (!pMsg->pAccessChannel) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    DEBUG("Topic seting for %s\n",pMsg->pAccessChannel);
    

    if ((pChannelSet=get_db(CHANNEL_DB,pMsg->pAccessChannel))) {

	    pChannelData=StrToChannelData(pChannelSet);
    	free(pChannelSet);

		/* remove old topic */
        if (pChannelData->pTopic) {
            free(pChannelData->pTopic);
        }
        pChannelData->pTopic=getParameters(pMsg->pRawLine);

	    pChannelSet=ChannelDataToStr(pChannelData);
    	replace_db(CHANNEL_DB,pMsg->pAccessChannel,pChannelSet);


	    /* message */
    	if (!pChannelData->pTopic) {
	        notice(pMsg->pCallingNick,getMsgString(OK_RM_TOPIC));
    	} else {
        	notice(pMsg->pCallingNick,getMsgString(OK_SET_TOPIC));
    	}

	    topic(pMsg->pAccessChannel,pChannelData->pTopic);
	} else {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNEL));
	}
}
/* #########################################################################
   Bot comand: !viewgreet <#channel>
   ######################################################################### */
void greeting(MsgItem_t *pMsg) {
    extern ConfigSetup_t sSetup;
    char *pChannelSet;
    char *greeting;

    /* only greeting  send  to other user */
    if (strcmp(pMsg->pCallingNick,sSetup.botname)) {
        
        if (!pMsg->pAccessChannel){
            return;
        }

        DEBUG("Greeting for %s\n",pMsg->pAccessChannel);

        if ((pChannelSet=get_db(CHANNEL_DB,pMsg->pAccessChannel))) {
    	    if ((greeting=getGreeting(pChannelSet))) {
            	notice(pMsg->pCallingNick,greeting);
        	}
		}
    }
}
/* #########################################################################
   Bot comand: !say <#channel> text
   ######################################################################### */
void say(MsgItem_t *pMsg) {
    char *pParameter;

    
    if (!pMsg->pAccessChannel) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    pParameter=getParameters(pMsg->pRawLine);
    if (!pParameter) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }

    privmsg(pMsg->pAccessChannel,pParameter);
}
/* #########################################################################
   Bot comand: !allsay text
   ######################################################################### */
void allsay(MsgItem_t *pMsg) {
    char *pMsgStr;
    PQueue pChannelQueue;
	QueueData *pChannel;
    

    if (!(pMsgStr=getParameters(pMsg->pRawLine))) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }

	/* get the channel list */
    pChannelQueue=list_db(CHANNEL_DB);

	/* send privmsg to all channels */
    while (isfullQueue(pChannelQueue)) {
        pChannel=popQueue(pChannelQueue);
        privmsg((char*)pChannel->data,pMsgStr);
		free(pChannel);
    }
	deleteQueue(pChannelQueue);
}

/* #########################################################################
   Bot comand: !ban <#channel> nick
   ######################################################################### */
void banuser(MsgItem_t *pMsg) {
    extern CallbackDList CallbackList;
    extern ConfigSetup_t sSetup;
    CallbackItem_t *Callback;
    char *Data;

    char **pArgv;
    char *pParameter;
    char *pToBanNick;
    char *pLogin;

    
    if (!pMsg->pAccessChannel){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    if (!(pParameter=getParameters(pMsg->pRawLine))) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }

    pArgv=splitString(pParameter,2);
    pToBanNick=pArgv[0];

    if (!pToBanNick) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }


    if (strcmp(pToBanNick,sSetup.botname)==0) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOTSELF_BAN));
        return;
    }

    // read the login from the database
    pLogin=get_db(NICKTOUSER_DB,pMsg->pNetmask);

    Data=(char*)malloc((strlen(pLogin)+strlen(pMsg->pAccessChannel)+2)*sizeof(char));
    sprintf(Data,"%s %s",pLogin,pMsg->pAccessChannel);

    Callback=(CallbackItem_t*)malloc(sizeof(CallbackItem_t));

    StrToLower(pToBanNick);

    // fill callback item
    Callback->nickname=pToBanNick;
    Callback->CallbackFkt=SetBanCb;
    Callback->data=Data;

    insert_next_CallbackDList(&CallbackList,CallbackList.tail,Callback);
    whois(pToBanNick);
}

/* #########################################################################
   Bot comand: !deban <#channel> banmask
   ######################################################################### */
void debanuser(MsgItem_t *pMsg) {
    char *pParameter;
    char **pArgv;

    
    if (!pMsg->pAccessChannel){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    /* extract parameters */
    if (!(pParameter=getParameters(pMsg->pRawLine))){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }

    pArgv=splitString(pParameter,2);
    

    // reset the ban
    if (pArgv) {
        deban(pMsg->pAccessChannel,pArgv[0]);
        notice(pMsg->pCallingNick,getMsgString(OK_DEBAN));
    }
    return;
}

/* #########################################################################
   Bot comand: !kick <#channel> nick reason
   ######################################################################### */
void kickuser(MsgItem_t *pMsg) {
    extern ConfigSetup_t sSetup;
    extern CallbackDList CallbackList;
    CallbackItem_t *Callback;
    char *pKicknick;
    char *pReason;
    char *pParameter;
    char **pArgv;
    char *pPos;
    char *pLogin;
    char *pData;

    if (!pMsg->pAccessChannel) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }

    /* get parameters*/
    if (!(pParameter=getParameters(pMsg->pRawLine))){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }

    pArgv=splitString(pParameter,2);
    
    /* parse nick */
    pKicknick=pArgv[0];
    if (!pKicknick) {return;}
    
    if (strcmp(pKicknick,sSetup.botname)==0) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOTSELF_KICK));
        return;
    }


    /* parse reason */
    pReason=pArgv[1];
    if (!pReason) {
        /* empty reason */
        pReason=getMsgString(INFO_DEFAULT_REASON);
    }

    /* read  the  login name of the  kicking user */
    if (!(pLogin=get_db(NICKTOUSER_DB,pMsg->pNetmask))) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_KICK));
        return;
    }

    /* built data for the callback */
    pData=(char*)malloc((strlen(pLogin)+strlen(pMsg->pAccessChannel)+strlen(pReason)+5)*sizeof(char));
    sprintf(pData,"%s %s %s",pLogin,pMsg->pAccessChannel,pReason);
    
                          
    /* built  and add to callback list */
    StrToLower(pKicknick);
    Callback=(CallbackItem_t*)malloc(sizeof(CallbackItem_t));
    Callback->CallbackFkt=KickCb;
    Callback->nickname=pKicknick;
    Callback->data=pData;

    insert_next_CallbackDList(&CallbackList,CallbackList.tail,Callback);

    whois(pKicknick);
}
/* #########################################################################
   Bot comand: !usermode [#channel] <login> <mod>
   ######################################################################### */
void usermode(MsgItem_t *pMsg){
    char *pPos;
    char *accesslogin;
    char *pLogin;
    char *usernick;
    char *pNetmask;
    char *pKey;
    char *oldmod;
    char *pParameter;
    char mod[3];

    if(!pMsg->pAccessChannel){
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }
    
    if ((accesslogin=get_db(NICKTOUSER_DB,pMsg->pNetmask))) {

	    /* check the channel */
    	if (!(exist_db(CHANNEL_DB,pMsg->pAccessChannel))) {
        	notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNEL));
    	    return;
	    }
                   
    	/* get parameters */
	    if (!(pParameter=getParameters(pMsg->pRawLine))) {
            notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
            return;
        }

    	/* user mod */
	    DEBUG("Modify user mod\n");

    	/* look for the space and separat the login for the user which want modify */
	    if (!(pPos=strchr(pParameter,' '))) {
            notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        	return;
    	}

	    /* set the end mark for login string and  place the pointer of the substr */
    	*pPos='\0';
	    pPos++;

    	/* extract the  login */
	    pLogin=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
    	strcpy(pLogin,pParameter);
	    StrToLower(pLogin);


    	/* check login in the user db */
	    if (!(exist_db(USER_DB,pLogin))) {
	        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_ACCOUNT));
        	return;
    	} else if (!strcmp(pLogin,accesslogin)) {
        	notice(pMsg->pCallingNick,getMsgString(ERR_NOT_SELF));
    	    return;
	    }

    	/* build the key  for access rights  db */
	    pKey=(char*)malloc((strlen(pLogin)+strlen(pMsg->pAccessChannel)+1)*sizeof(char));
	    sprintf(pKey,"%s%s",pLogin,pMsg->pAccessChannel);

    
	    /* get the  old mod for  a  user from the  access db */
    	if (!(oldmod=get_db(ACCESS_DB,pKey))) {
        	/* get the old mod for user as master */
	        if ((oldmod=get_db(ACCESS_DB,pLogin))) {
		   	    /* only a master  can modify  a other master */
    		    if (strchr(oldmod,'m') && pMsg->UserLevel != MasterLevel) {
            		notice(pMsg->pCallingNick,getMsgString(ERR_NOT_MASTER));
	        	    return;
    		    }
		
			}

	    }

    	DEBUG("Modify account %s in channel %s\n",pLogin,pMsg->pAccessChannel);

	    /* check for add or remove  mod */
    	if (pPos[0]!='+' && pPos[0]!='-') {
    	    return;
	    } else {
    	    mod[0]=pPos[0];
	    }

    	/* checke the mod */
	   	switch (pPos[1]) {
	    case 'v':
    	case 'o':
	        mod[1]=pPos[1];
        	break;
    	case 'm':
	        if (pMsg->UserLevel != MasterLevel) {
        	    notice(pMsg->pCallingNick,getMsgString(ERR_NOT_MASTER));
    	    } else {
	            mod[1]=pPos[1];
        	}
    	    break;
	    default:
        	notice(pMsg->pCallingNick,getMsgString(ERR_UNKNOWN_MODS));
    	    return;
	    }

    	/* set the end mark */
	    mod[2]='\0';

	    DEBUG("Found mod %s\n",mod);

		/* look for old master rights */
		if (mod[1]!='m' && oldmod) {
	    	/* check for old or new master */
		    if (oldmod[1]=='m') {
	      	    /* remove master account */
    		    del_db(ACCESS_DB,pLogin);
		    } 
		}
				
		/* set the new key and remove old rights for new master */
		if (mod[1]=='m')  {
			if (oldmod) {
				if (oldmod[1]!='m') {
			        /* remove old access right */
    	    	   	rmAccessRights(pLogin);
				}
			}

            /* replace the  old key by the login as new key */
	   	    free(pKey);
    	   	pKey=(char *)malloc((strlen(pLogin)+1)*sizeof(char));
        	strcpy(pKey,pLogin);
    	}
	    
		/* add or remove the mod */
   		if (mod[0]=='+') {
        	DEBUG("Set new mode\n");

	        if (!(add_db(ACCESS_DB,pKey,mod))) {
                replace_db(ACCESS_DB,pKey,mod);
    	    }
	  	} else {
    	DEBUG("Remove new mode\n");
	        /* remove mod */
        	del_db(ACCESS_DB,pKey);

    	}


	    /* identify the  login and set the rights */
		if ((pNetmask=get_db(USERTONICK_DB,pLogin))) {
    	    	DEBUG("Modify the current mode\n");
				usernick=getNickname(pNetmask);

		        if (mod[0]=='-') {
    	        	/* remove mods */
        		    mode(pMsg->pAccessChannel,mod,usernick);
	    	    } else {
					if (oldmod) {
			        	/* remove old mods */
    	    			if (strchr(oldmod,'v')) {
    		    			/* remove -o */
		    	        	mode(pMsg->pAccessChannel,"-v",usernick);
    	        		} else if (strlen(oldmod)) {
        		    	   /* remove -v */
	    	    	       mode(pMsg->pAccessChannel,"-o",usernick);
		            	}
					}
	           	
					/* set the new mods */
    	   	    	if (mod[1]=='v') {
    	            	mode(pMsg->pAccessChannel,"+v",usernick);
		       	    } else {
           		    mode(pMsg->pAccessChannel,"+o",usernick);
	       	    }
    	    }
		}
	    notice(pMsg->pCallingNick,getMsgString(OK_USERMODE));
	} 
}
/* #########################################################################
   Bot comand: !chanmode [#channel] <mod>
   ######################################################################### */
void chanmode(MsgItem_t *pMsg) {
    char *pChannelSet;
    char *pParameters;

    int i;

    ChannelMode_t *pNewMode;
    ChannelData_t *pChannelData;


    if (!pMsg->pAccessChannel) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
        return;
    }
    
    if(!(pParameters=getParameters(pMsg->pRawLine))) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }


    /* read the old channel parameters */
    
    if (pChannelSet=get_db(CHANNEL_DB,pMsg->pAccessChannel)) {
        pChannelData=StrToChannelData(pChannelSet);

	    /* read the new channel parameters */
	    pNewMode=StrToChannelMode(pParameters);
    	DEBUG("Found the new channel modes \"%s\"\n",ChannelModeToStr(pNewMode));

	    DEBUG("Build the new modes for the channel %s\n",pMsg->pAccessChannel);
    	/* build the new channel parameters */
	    for (i=1;i<MAX_MODES;i++) {
    	    if (pNewMode->pModeStr[MOD_TYPE]=='+') {
        	    /* add  the new mode in the pChannelData */
            	if (pNewMode->pModeStr[i]!=' ') {
                	/* set the new flag */
	                pChannelData->pModes->pModeStr[i]=pNewMode->pModeStr[i];
	
    	            /* set keyword and limit */
        	        if (pNewMode->pModeStr[i]=='k') {
            	        free(pChannelData->pModes->pKeyword);
                	    pChannelData->pModes->pKeyword=(char*)malloc((strlen(pNewMode->pKeyword)+1)*sizeof(char));
                    	strcpy(pChannelData->pModes->pKeyword,pNewMode->pKeyword);
	                } else if (pNewMode->pModeStr[i]=='l') {
    	                free(pChannelData->pModes->pLimit);
        	            pChannelData->pModes->pLimit=(char*)malloc((strlen(pNewMode->pLimit)+1)*sizeof(char));
            	        strcpy(pChannelData->pModes->pLimit,pNewMode->pLimit);
                	}
	            }

    	    } else if (pNewMode->pModeStr[MOD_TYPE]=='-') {
        	    /* remove  flags from the pChanneldata */
            	if (pNewMode->pModeStr[i]!=' ') {

	                /* remove the  mode flag */
    	            pChannelData->pModes->pModeStr[i]=' ';

        	        /* remove keyword and limit */
            	    if (pChannelData->pModes->pModeStr[i]=='k') {
                	    pChannelData->pModes->pKeyword[0]='\0';
	                } else if (pChannelData->pModes->pModeStr[i]=='l') {
    	                pChannelData->pModes->pLimit[0]='\0';
        	        }
            	}

	        }
    	}


	    /* check the exit  of channell modes */
    	/* set or remove the leading plus */
	    if (strpbrk(pChannelData->pModes->pModeStr,CHANNEL_MODES)) {
    	    pChannelData->pModes->pModeStr[0]='+';
	    } else {
    	    pChannelData->pModes->pModeStr[0]=' ';
	    }

    	DEBUG("Write the new modes channel \"%s\"\n",ChannelModeToStr(pChannelData->pModes))
	    /* set the new mode in database */
    	free(pChannelSet);
	    pChannelSet=ChannelDataToStr(pChannelData);
    	replace_db(CHANNEL_DB,pMsg->pAccessChannel,pChannelSet);


	    DEBUG("Set the new channel modes\n")
    	/* set the mods */
	    mode(pMsg->pAccessChannel,ChannelModeToStr(pNewMode),NULL);
    } else {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNEL));
    }
}
/* #########################################################################
   Bot comand: !rmuser <login>
   ######################################################################### */
void rmuser(MsgItem_t *pMsg) {
    char *pLogin;
    char *pNetmask;
    char *rmnick;
    PQueue pChannelQueue;
	QueueData *pChannel;
	
    if (!(pLogin=getParameters(pMsg->pRawLine))) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }

	StrToLower(pLogin);
    
    /* extract the first parameter */
    strtok(pLogin," ");

    if (exist_db(USER_DB,pLogin)) {
        /* remove the account */
        rmAccount(pLogin);
    
        /* remove the mod  for  this account */
        if ((pNetmask=get_db(USERTONICK_DB,pLogin))) {
			rmnick=getNickname(pNetmask);
        	
			pChannelQueue=list_db(CHANNEL_DB);
            while (isfullQueue(pChannelQueue)) {
                pChannel=popQueue(pChannelQueue);
                mode((char*)pChannel->data,"-o",rmnick);
                mode((char*)pChannel->data,"-v",rmnick);
				free(pChannel);
            }
			deleteQueue(pChannelQueue);
        }
        notice(pMsg->pCallingNick,getMsgString(OK_RMUSER));        
    	DEBUG("Remove %s from the user list\n",pLogin);
    } else {
    	notice(pMsg->pCallingNick,getMsgString(ERR_NOT_ACCOUNT));
    }
}
/* #########################################################################
   Bot comand: !userlist [#channel]
   ######################################################################### */
void userlist(MsgItem_t *pMsg){
    char *pLogin;
    char *pArgv;
    char *pKey;
    PQueue pLoginQueue;
	QueueData *pLoginItem;
    PQueue pChannelQueue;
	QueueData *pChannel;
    char *pMod;
    char *pMsgStr;

    unsigned int i,j,k,iChanLen,iLoginLen;

    pArgv=getArgument(pMsg->pRawLine);

    /* read the list of Logins */
    pLoginQueue=list_db(USER_DB);


    /* build the  login list  for output */
    if (pMsg->UserLevel==MasterLevel && !pArgv) {
        /* Bot masters */
        privmsg(pMsg->pCallingNick,getMsgString(INFO_USERLIST_BEGIN));
        
        DEBUG("Genrate the Userlist for a master\n");
        pChannelQueue=list_db(CHANNEL_DB);
		
        /* get the kist of all channels */
        while (isfullQueue(pLoginQueue)) {
            pLoginItem=popQueue(pLoginQueue);
            iLoginLen=pLoginItem->t_size;

            /* check for master or normal user */
            if (exist_db(ACCESS_DB,(char*)pLoginItem->data)) {
                /* user is master */
                pMsgStr=(char*)malloc((USERLIST_TAB+strlen("Master   Status:")+5)*sizeof(char));
                strcpy(pMsgStr,(char*)pLoginItem->data);

                /* fill */
                for (j=0;j<(USERLIST_TAB-iLoginLen);j++) {
                    strcat(pMsgStr," ");
                }

                strcat(pMsgStr,"Master   Status: ");

                /* insert  online status */
                /* set  online Status */
                if (exist_db(USERTONICK_DB,(char*)pLoginItem->data)) {
                    strcat(pMsgStr,"ON ");
                } else {
                    strcat(pMsgStr,"OFF");
                }

                privmsg(pMsg->pCallingNick,pMsgStr);
                free(pMsgStr);
            } else {
                /* normal user */
                SetIterToFirstQueue(pChannelQueue);

                while (pChannel=getNextitrQueue(pChannelQueue)) {
                iChanLen=pChannel->t_size;
    
    		    /* build the key  for  access.dbf */
    			pKey=(char*)malloc((iChanLen+1+iLoginLen)*sizeof(char));
    			sprintf(pKey,"%s%s",(char*)pLoginItem->data,(char*)pChannel->data);
    
                pMsgStr=(char*)malloc((USERLIST_TAB+strlen("Status:")+14)*sizeof(char));
                strcpy(pMsgStr,(char*)pLoginItem->data);
    
                /* fill */
                for (j=0;j<(USERLIST_TAB-iLoginLen);j++) {
                    strcat(pMsgStr," ");
                }
    
    
    			if ((pMod=get_db(ACCESS_DB,pKey))) {
    				/* set access rights */       
    				if (pMod[1]=='o') {
    					strcat(pMsgStr,"Owner of ");
                        strcat(pMsgStr,(char*)pChannel->data);
    				} else if (pMod[1]=='v') {
    					strcat(pMsgStr,"Friend from ");
                        strcat(pMsgStr,(char*)pChannel->data);
    				} else {
    					free(pMsgStr);
    					break;
    				}
    
    			} else {
                    strcat(pMsgStr,"Other ");
                }
    
                /* set  online Status */
                strcat(pMsgStr,"   Status: ");
                if (exist_db(USERTONICK_DB,(char*)pLoginItem->data)) {
                    strcat(pMsgStr,"ON ");
                } else {
                    strcat(pMsgStr,"OFF");
                }
                /* send notice out */
                privmsg(pMsg->pCallingNick,pMsgStr);
                free(pMsgStr);
    			free(pKey);
    		}
	    }
	    free(pLoginItem);    
	}
	flushQueue(pChannelQueue);
	deleteQueue(pChannelQueue);
        privmsg(pMsg->pCallingNick,getMsgString(INFO_USERLIST_END));
    } else {
        
        DEBUG("Genrate the Userlist for a owner\n");

        if (!pMsg->pAccessChannel) {
            notice(pMsg->pCallingNick,getMsgString(ERR_NOT_CHANNELOPT));
            return;
        }
        
        privmsg(pMsg->pCallingNick,getMsgString(INFO_USERLIST_BEGIN));
        
        /* look for  rights of  user for  the channel */
		iChanLen=strlen(pMsg->pAccessChannel);

		 while (isfullQueue(pLoginQueue)) {
            pLoginItem=popQueue(pLoginQueue);
			iLoginLen=pLoginItem->t_size;

			/* build the key  for  access.dbf */
			pKey=(char*)malloc((iChanLen+1+iLoginLen)*sizeof(char));
			sprintf(pKey,"%s%s",(char*)pLoginItem->data,pMsg->pAccessChannel);

            pMsgStr=(char*)malloc((USERLIST_TAB+strlen("Status:")+14)*sizeof(char));
            strcpy(pMsgStr,(char*)pLoginItem->data);

            /* fill */
            for (j=0;j<(USERLIST_TAB-iLoginLen);j++) {
                strcat(pMsgStr," ");
            }


			if ((pMod=get_db(ACCESS_DB,pKey))) {
				/* set access rights */
				if (pMod[1]=='o') {
					strcat(pMsgStr,"Owner ");
				} else if (pMod[1]=='v') {
					strcat(pMsgStr,"Friend");
				} else {
					free(pMsgStr);
					break;
				}

    			
                /* set  online Status */
                strcat(pMsgStr,"   Status: ");
                if (exist_db(USERTONICK_DB,(char*)pLoginItem->data)) {
                    strcat(pMsgStr,"ON ");
                } else {
                    strcat(pMsgStr,"OFF");
                }
                /* send notice out */
                privmsg(pMsg->pCallingNick,pMsgStr);
                free(pMsgStr);
            }
			free(pKey);

        }
        privmsg(pMsg->pCallingNick,getMsgString(INFO_USERLIST_END));
    }
	deleteQueue(pLoginQueue);
}

/* #########################################################################
   Bot comand: !invite <#channel> [Nick]
   ######################################################################### */
void inviteuser(MsgItem_t *pMsg){
    char *pParameter;
    char **pParts;
    char *pInviteNick;

    
    // extract and select the nick name  for inviting
    if (!(pParameter=getArgument(pMsg->pRawLine))) {
        notice(pMsg->pCallingNick,getMsgString(ERR_NOT_PARAMETER));
        return;
    }
    
    // extract the parameters
    pParts=splitString(pParameter,3);

    // select the nickname
    if (pParts[1]== NULL) {
        pInviteNick=pMsg->pCallingNick;
    } else {
            if (pMsg->UserLevel==FriendLevel) {
                pInviteNick=pMsg->pCallingNick;
            } else {
                pInviteNick=pParts[1];
            }
    }

    // invite
    invite(pMsg->pAccessChannel,pInviteNick);
}
/* #########################################################################
   Bot comand: \001PING <ID>\001
   ######################################################################### */
void ctcpping(MsgItem_t *pMsg) {
    char *pPing;
    char pPong[64];
   if ((pPing=strstr(pMsg->pRawLine," :\001"))==NULL) {
 	/* ignoring errors */
        return;
    }
   pPing+=2;
   sprintf(pPong,"%s",pPing);
   notice(pMsg->pCallingNick,pPong);
}
/* #########################################################################
   Bot comand: \001VERSION\001
   ######################################################################### */
void ctcpversion(MsgItem_t *pMsg) {
    char pMsgStr[300];
    char pTemp[256];
    /* creat Versions String */
    sprintf(pTemp,VERSIONSTR);
    sprintf(pMsgStr,"\001VERSION %s\001",pTemp);
    notice(pMsg->pCallingNick,pMsgStr);
    return;
}
/* #########################################################################
   Bot comand: \001TIME\001
   ######################################################################### */
void ctcptime(MsgItem_t *pMsg) {
	char pMsgStr[64];
	time_t t;
	struct tm *pTm;
	time(&t);
	/* UTC-TIME */
	/* pTm=gmtime(&t);*/
	/* localtime */
	pTm=localtime(&t);
	strftime(pMsgStr,63,"\001TIME %a %b %d %H:%M:%S\001",pTm);
	notice(pMsg->pCallingNick,pMsgStr);
	return;
}

