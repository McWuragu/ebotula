/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

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
#include "command.h"


// #########################################################################
// Bot comand: !help <command>
// #########################################################################
void help(char *pLine) {
    char *pNetmask;
    char *pNick;
    char *pParameter;
    char *pTmp;
    char *pMsgStr;
    unsigned int i,j,iLength;
    boolean bIsLogin=false,bIsMaster=false;

    pNetmask=getNetmask(pLine);
    pNick=getNickname(pLine);

    pParameter=getParameters(pLine);


    // check for parameters
    if (!strlen(pParameter)) {
        DEBUG("Default information");

        // Header of help message

        for (i=0;pIrcHelp[0][i]!=NULL;i++) {
            // look for the end  of msg
            notice(pNick,pIrcHelp[0][i]);
        }

        // checking  login and  master status
        if ((bIsLogin=exist_db(NICKTOUSER_DB,pNetmask))) {
            bIsMaster=exist_db(ACCESS_DB,get_db(NICKTOUSER_DB,pNetmask));
        }


        for (i=CMD_OTHERS;i<CMDCOUNT;i++) {
            // checking  for allow commands
            if (!bIsLogin && i>=CMD_LOGGED) {
                continue;
            } else if (!bIsMaster && i >= CMD_MASTER) {
                continue;
            }

            // calculat the length of buffer
            pMsgStr=(char *)calloc(HELP_TAB+1+strlen((char*)pIrcHelp[CmdIdToHelpId(i)][0]),sizeof(char));

            // build string
            strcpy(pMsgStr,CmdList[i]);
            iLength=HELP_TAB-strlen(CmdList[i]);
            // fill
            for (j=0;j<iLength;j++) {
                strcat(pMsgStr," ");
            }
            strcat(pMsgStr,(char*)pIrcHelp[CmdIdToHelpId(i)][0]);
            // send notice
            notice(pNick,pMsgStr);
        }
        // the tail
        notice(pNick,MSG_HELP_END);
    } else {
        DEBUG("Spezial information for a command");

        // cut the first word
        strtok(pParameter," ");

        // checking  of a leading '!'
        if (pParameter[0]!='!') {
            pTmp=(char*)malloc((strlen(pParameter)+2)*sizeof(char));
            sprintf(pTmp,"!%s",pParameter);
            free(pParameter);
            pParameter=pTmp;
        }

        DEBUG("Looking for information about \"%s\"",pParameter);

        // Help for a command
        for (i=CMD_OTHERS+1;i<CMDCOUNT;i++) {
            if (!strcmp((char*)CmdList[i],&pParameter[1])) {
                DEBUG("Command found %d",i);

                // the headi for help
                pTmp=(char*)malloc((strlen(MSG_HELP_FOR)+strlen((char *)CmdList[i])+3)*sizeof(char));
                sprintf(pTmp,"%s %s:",MSG_HELP_FOR,pParameter);
                notice(pNick,pTmp);

                // print  the  help text
                for (j=1;pIrcHelp[CmdIdToHelpId(i)][j]!=NULL;j++) {
                    notice(pNick,(char*)
                           pIrcHelp[CmdIdToHelpId(i)][j]);
                }

                // syntax from the command
                notice(pNick,pIrcSyntax[0][0]);
                for (j=0;pIrcSyntax[CmdIdToHelpId(i)][j]!=NULL;j++) {
                    notice(pNick,(char*)pIrcSyntax[CmdIdToHelpId(i)][j]);
                }
                notice(pNick,MSG_HELP_END);
                return;
            }
        }
        notice(pNick,MSG_NOT_COMMAND);
    }
}
// #########################################################################
// Bot comand: !hello
// #########################################################################
void hello(char *pLine) {
    extern pthread_mutex_t account_mutex;
    char *pNetmask;
    char *pNick;
    char *pLogin;


    pNetmask=getNetmask(pLine);
    pNick=getNickname(pLine);
    pLogin=(char*)malloc((strlen(pNick)+1)*sizeof(char));

    DEBUG("Try to create an new account for %s",pNick);

    strcpy(pLogin,pNick);

    StrToLower(pLogin);

    pthread_mutex_lock(&account_mutex);
    if (add_db(USER_DB,pLogin,"")) {
        
        // autoidentify after create an new account
        pNetmask=getNetmask(pLine);
        log_on(pNetmask,pLogin);

        notice(pNick,MSG_HELLO);
        notice(pNick,MSG_HELLO2);
        notice(pNick,MSG_IDENT_OK);
        
    }
    notice(pNick,MSG_NICK_EXIST);
    
    pthread_mutex_unlock(&account_mutex);

}
// #########################################################################
// Bot comand: !pass <password>
// #########################################################################
void password(char *pLine) {
    extern pthread_mutex_t account_mutex;
    char *pLogin;
    char *pPasswd;
    char *pNick;
    char *pNetmask;

    pNetmask=getNetmask(pLine);
    pNick=getNickname(pLine);
    
    pthread_mutex_lock(&account_mutex);
    if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {

	    DEBUG("Check the  password for the account %s",pLogin);

	    // get  the  login name
	    if (strlen(pLogin)) {
        	pPasswd=getParameters(pLine);

	        // parse the  password  form  parameter list
	        if (!strlen(pPasswd)) {
        	    notice(pNick,MSG_NOT_PASS);
	        }

        	// set password
	        replace_db(USER_DB,pLogin,pPasswd);
        	notice(pNick,MSG_PASSWD);        
	    }
    }
    pthread_mutex_lock(&account_mutex);
}
// #########################################################################
// Bot comand: !logoff
// #########################################################################
void logoff(char *pLine) {
    extern pthread_mutex_t account_mutex;
    char *pLogin;
    char *pNick;
    char *pNetmask;
    char **ppChannels;

    int i;

    pNetmask=getNetmask(pLine);
    pNick=getNickname(pLine);

    pthread_mutex_lock(&account_mutex);
   	if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
	    log_out(pLogin);

    	// remove the mod  for  this account
	    ppChannels=list_db(CHANNEL_DB);
    	for (i=0;ppChannels[i]!=NULL;i++) {
        	mode(ppChannels[i],"-o",pNick);
	        mode(ppChannels[i],"-v",pNick);
    	}
	}
    pthread_mutex_unlock(&account_mutex);
    notice(pNick,MSG_LOGOFF);
}
// #########################################################################
// Bot comand: !ident login <password>
// #########################################################################
void ident(char *pLine) {
    extern pthread_mutex_t account_mutex;
    char *pLogin;
    char *pPasswd;
    char *pPos;
    char *pNick;
    char *pNetmask;
    char *pParameter;
    char **ppChannels;
    char *pKey;
    char *pMod;
    boolean isMaster;

    int i,login_len;

    pNetmask=getNetmask(pLine);
    pNick=getNickname(pLine);

    DEBUG("try to identify %s",pNick);

    pthread_mutex_lock(&account_mutex);
    
    if (!exist_db(NICKTOUSER_DB,pNetmask)) {
        pParameter=getParameters(pLine);
    
        // no parameter found
        if (strlen(pParameter)) {
            // parse the password
            if ((pPos=strstr(pParameter," "))==NULL) {
                // no Passwd found
                // try empty pass
                notice(pNick,MSG_NOT_PASS);
                pPasswd="";
            } else {
                pPasswd=(char *)malloc(strlen(pPos)*sizeof(char));
                strcpy(pPasswd,&pPos[1]);
            }
        
        
            // parse the login
            strtok(pParameter," ");
            pLogin=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
            strcpy(pLogin,pParameter);
            StrToLower(pLogin);
        
            DEBUG("Look for the account %s",pLogin);
        
            // check the account
            if (check_db(USER_DB,pLogin,pPasswd)) {
                log_on(pNetmask,pLogin);
                notice(pNick,MSG_IDENT_OK);
        
                isMaster=exist_db(ACCESS_DB,pLogin);
        
                // set the mod  for  this account
                ppChannels=list_db(CHANNEL_DB);
                login_len=strlen(pLogin);
        
                for (i=0;ppChannels[i]!=NULL;i++) {
                    if (isMaster) {
                        mode(ppChannels[i],"+o",pNick);
                    } else {
                        pKey=(char*)malloc((strlen(ppChannels[i])+login_len+1)*sizeof(char));
                        sprintf(pKey,"%s%s",pLogin,ppChannels[i]);
                        if ((pMod=get_db(ACCESS_DB,pKey))) {
   	                        mode(ppChannels[i],pMod,pNick);
      	                    free(pMod);
          	            }
                       	free(pKey);
                    }
                }
            } else {
                notice(pNick,MSG_NOT_ACCOUNT);
            }    
        } else {
            notice(pNick,MSG_IDENT_ERR);
        }
    } else {
        notice(pNick,MSG_ALREADY_LOGON);
    }
    pthread_mutex_unlock(&account_mutex);
}
// #########################################################################
// Bot comand: !addchannel #channel
// #########################################################################
void addChannel(char *pLine) {
    char *pChannel;
    char *pNick;
    char *channelmod;

    pNick=getNickname(pLine);

    pChannel=getAccessChannel(pLine);


    if (!strcmp(pChannel,getChannel(pLine))) {
        notice(pNick,MSG_NOT_CHANNELOPT);
        notice(pNick,MSG_ADDCHANNEL_ERR);
        return;
    }

    DEBUG("Join and  try to add the channnel %s",pChannel);

    // checking of channel exist
    if (exist_db(CHANNEL_DB,pChannel)) {
        notice(pNick,MSG_ADDCHANNEL_ALREADY);
    } else {
        // add channel
        channelmod=(char *)malloc(3*sizeof(char));
        strcpy(channelmod,"\t\t");
        add_db(CHANNEL_DB,pChannel,channelmod);
        notice(pNick,MSG_ADDCHANNEL_OK);
    }

    // join the channel
    join(pChannel);
    notice(pNick,MSG_JOIN_OK);

}
// #########################################################################
// Bot comand: !rmchannel <#channel>
// #########################################################################
void rmChannel(char *pLine){
    char *pChannel;
    char *pNick;

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);

    DEBUG("Part and  try to remove the channnel %s",pChannel);


    // checking of channel exists
    if (!del_db(CHANNEL_DB,pChannel)) {
        notice(pNick,MSG_NOT_CHANNEL);
    } else {
        notice(pNick,MSG_RMCHANNEL_OK);
    }


    // part the channel
    part(pChannel);
    notice(pNick,MSG_PART_OK);

}
// #########################################################################
// Bot comand: !join #channel
// #########################################################################
void joinChannel(char *pLine) {
    char *pChannel;
    char *pNick;

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);

    // compare the current channel and  the channel for joining
    if (!(strcmp(pChannel,getChannel(pLine)))) {
        notice(pNick,MSG_NOT_CHANNELOPT);
        notice(pNick,MSG_JOIN_ERR);
        return;
    }

    DEBUG("Join the channel %s",pChannel);
    // join the channel
    join(pChannel);
    notice(pNick,MSG_JOIN_OK);

}
// #########################################################################
// Bot comand: !part <#channel>
// #########################################################################
void partChannel(char *pLine) {
    char *pNick;
    char *pChannel;

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);

    DEBUG("Part the channel %s",pChannel);

    // part the channel
    part(pChannel);
    notice(pNick,MSG_PART_OK);
}
// #########################################################################
// Bot comand: !die
// #########################################################################
void die(char *pLine) {
    notice(getNickname(pLine),MSG_DIE_OK);
    stopParser(0);
}
// #########################################################################
// Bot comand: !restart
// #########################################################################
void restart(char *pLine) {
    extern boolean again;
    notice(getNickname(pLine),MSG_RESTART_OK);
    again=true;
    stopParser(0);
}
// #########################################################################
// Bot comand: !nick nickname
// #########################################################################
void setNick(char *pLine){
    char *pParameter;
    char *pNick;

    pNick=getNickname(pLine);
    pParameter=getParameters(pLine);

    // read parameters
    if (!strlen(pParameter)) {
        notice(pNick,MSG_NICK_ERR);
        return;
    } else if (!NickStringCheck(pParameter)) {
        notice(pNick,MSG_NICK_INVALID);
        return;
    }

    notice(pNick,MSG_NICK_SET);
    nick(pParameter);

}
// #########################################################################
// Bot comand: !chanlist
// #########################################################################
void chanlist(char *pLine){
    char *pNick,*buffer;
    char **ppChannels;
    char *pMode;
    char *pMsgStr;
	char *pChannelSet;
    int i=0,buffer_size=0;
    ChannelData_t *pChannelData;

    DEBUG("Build channel list...");

    pNick=getNickname(pLine);

    notice(pNick,MSG_CHANNELLIST_BEGIN);

    // get  the channel list form the DB
    ppChannels=list_db(CHANNEL_DB);


    for (i=0;ppChannels[i]!=NULL;i++) {
		if ((pChannelSet=get_db(CHANNEL_DB,ppChannels[i]))) {
	        pChannelData=StrToChannelData(pChannelSet);
    	    pMode=ChannelModeToStr(pChannelData->pModes);

        	DEBUG("...for channel %s",ppChannels[i]);
	        notice(pNick,ppChannels[i]);

    	    pMsgStr=(char*)malloc((strlen(MSG_CHANNELLIST_MODE)+strlen(pMode)+2)*sizeof(char));
        	sprintf(pMsgStr,"%s %s",MSG_CHANNELLIST_MODE,pMode);
	        notice(pNick,pMsgStr);
    	    free(pMsgStr);

        	if (pChannelData->pTopic) {
            	pMsgStr=(char*)malloc((strlen(MSG_CHANNELLIST_TOPIC)+strlen(pChannelData->pTopic)+2)*sizeof(char));
	            sprintf(pMsgStr,"%s %s",MSG_CHANNELLIST_TOPIC,pChannelData->pTopic);
    	        notice(pNick,pMsgStr);
        	    free(pMsgStr);
	        }

    	    if (pChannelData->pGreeting) {
        	    pMsgStr=(char*)malloc((strlen(MSG_CHANNELLIST_GREAT)+strlen(pChannelData->pGreeting)+2)*sizeof(char));
            	sprintf(pMsgStr,"%s %s",MSG_CHANNELLIST_GREAT,pChannelData->pGreeting);
	            notice(pNick,pMsgStr);
    	        free(pMsgStr);
        	}


	        free(pMode);
    	    free(pChannelData);
			free(pChannelSet);
		}
    }

    notice(pNick,MSG_CHANNELLIST_END);
}
// #########################################################################
// Bot comand: !version
// #########################################################################
void version(char *pLine) {
    char pMsgStr[256];
    // creat Versions String
    sprintf(pMsgStr,VERSIONSTR);
    strcat(pMsgStr,"\r\n");
    notice(getNickname(pLine),pMsgStr);
}
// #########################################################################
// Bot comand: !greeting <#channel> <text>
// #########################################################################
void setGreeting(char *pLine) {
    char *pChannel;
    char *pChannelSet;
    char *pMode;
    char *pNick;

    ChannelData_t *pChannelData;

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);

    DEBUG("Greeting seting for %s",pChannel);


    // check of  existenz of the channel
    if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
	    pChannelData=StrToChannelData(pChannelSet);
    	free(pChannelSet);
	    
		// remive old greeting
		if (pChannelData->pGreeting) {
    	    free(pChannelData->pGreeting);
	    }
	    pChannelData->pGreeting=getParameters(pLine);

    
		pChannelSet=ChannelDataToStr(pChannelData);
	    replace_db(CHANNEL_DB,pChannel,pChannelSet);


    	// message
	    if (pChannelData->pGreeting[0]=='\0') {
     	   notice(pNick,MSG_RM_GREATING);
	    } else {
    	    notice(pNick,MSG_SET_GREATING);
	    }
	} else {
        notice(pNick,MSG_NOT_CHANNEL);
	}
}
// #########################################################################
// Bot comand: !topic <#channel> <topic>
// #########################################################################
void setTopic(char *pLine) {
    char *pChannel;
    char *pChannelSet;
    char *pNick;


    ChannelData_t *pChannelData;

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);

    DEBUG("Topic seting for %s",pChannel);
    

    if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {

	    pChannelData=StrToChannelData(pChannelSet);
    	free(pChannelSet);

		// remove old topic
        if (pChannelData->pTopic) {
            free(pChannelData->pTopic);
        }
        pChannelData->pTopic=getParameters(pLine);

	    pChannelSet=ChannelDataToStr(pChannelData);
    	replace_db(CHANNEL_DB,pChannel,pChannelSet);


	    // message
    	if (pChannelData->pTopic[0]=='\0') {
	        notice(pNick,MSG_RM_TOPIC);
    	} else {
        	notice(pNick,MSG_SET_TOPIC);
    	}

	    topic(pChannel,pChannelData->pTopic);
	} else {
        notice(pNick,MSG_NOT_CHANNEL);
	}
}
// #########################################################################
// Bot comand: !viewgreet <#channel>
// #########################################################################
void greeting(char *pLine) {
    extern ConfigSetup_t sSetup;
    char *pChannel;
    char *pChannelSet;
    char *greeting;
    char *pNick;

    pNick=getNickname(pLine);

    // only greeting  send  to other user
    if (strcmp(pNick,sSetup.botname)) {
        pChannel=getAccessChannel(pLine);

        DEBUG("Greeting for %s",pChannel);

        if ((pChannelSet=get_db(CHANNEL_DB,pChannel))) {
    	    if ((greeting=getGreeting(pChannelSet))) {
            	notice(pNick,greeting);
        	}
		}
    }
}
// #########################################################################
// Bot comand: !say <#channel> text
// #########################################################################
void say(char *pLine) {
    char *pChannel;
    char *pParameter;
    char *pNick;

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);

    pParameter=getParameters(pLine);
    if (!strlen(pParameter)) {
        notice(pNick,MSG_SAY_ERR);
        return;
    }

    privmsg(pChannel,pParameter);
}
// #########################################################################
// Bot comand: !allsay text
// #########################################################################
void allsay(char *pline) {
    char *pMsgStr;
    char **ppChannels;
    int i;
    pMsgStr=getParameters(pline);

    ppChannels=list_db(CHANNEL_DB);

    for (i=0;ppChannels[i]!=NULL;i++) {
        privmsg(ppChannels[i],pMsgStr);
    }
}
// #########################################################################
// Bot comand: !kick <#channel> nick reason
// #########################################################################
void kickuser(char *pLine) {
    char *pNick;
    char *kicknick;
    char *pChannel;
    char *reason;
    char *pParameter;
    char *pPos;

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);

    // get parameters
    pParameter=getParameters(pLine);
    if (!strlen(pParameter)) {
        notice(pNick,MSG_KICK_ERR);
        return;
    }

    // parse reason
    if (!(pPos=strchr(pParameter,' '))) {
        // empty reason
        reason="";
    } else {
        // set null byte  for  nick
        *pPos='\0';
        // over jump the  space
        pPos++;
        reason=(char*)malloc((strlen(pPos)+1)*sizeof(char));
        strcpy(reason,pPos);

    }

    // parse nick
    kicknick=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
    strcpy(kicknick,pParameter);

    kick(pChannel,kicknick,reason);
    notice(pNick,MSG_KICK_OK);
}
// #########################################################################
// Bot comand: !usermode [#channel] <login> <mod>
// #########################################################################
void usermode(char *pLine){
    extern pthread_mutex_t account_mutex;
    char *pChannel;
    char *pNick;
    char *pParameter;
    char *pPos;
    char *accesslogin;
    char *pLogin;
    char *usernick;
    char *pKey;
    char *oldmod;
    char mod[3];

    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);
    
    if ((accesslogin=get_db(NICKTOUSER_DB,getNetmask(pLine)))) {
    	pthread_mutex_lock(&account_mutex);

	    // check the channel
    	if (!(exist_db(CHANNEL_DB,pChannel))) {
	        pthread_mutex_unlock(&account_mutex);
        	notice(pNick,MSG_NOT_CHANNEL);
    	    return;
	    }

    	// get parameters
	    pParameter=getParameters(pLine);

    	// user mod
	    DEBUG("Modify user mod");

    	// look for the space and separat the login for the user which want modify
	    if (!(pPos=strchr(pParameter,' '))) {
    	    pthread_mutex_unlock(&account_mutex);
	        notice(pNick,MSG_USERMODE_ERR);
        	return;
    	}

	    // set the end mark for login string and  place the pointer of the substr
    	*pPos='\0';
	    pPos++;

    	// extract the  login
	    pLogin=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
    	strcpy(pLogin,pParameter);
	    StrToLower(pLogin);


    	// check login in the user db
	    if (!(exist_db(USER_DB,pLogin))) {
    	    pthread_mutex_unlock(&account_mutex);
	        notice(pNick,MSG_NOT_ACCOUNT);
        	return;
    	} else if (!strcmp(pLogin,accesslogin)) {
	        pthread_mutex_unlock(&account_mutex);
        	notice(pNick,MSG_NOT_SELF);
    	    return;
	    }

    	// build the key  for access rights  db
	    pKey=(char*)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char));
	    sprintf(pKey,"%s%s",pLogin,pChannel);

    
	    // get the  old mod for  a  user from the  access db
    	if (!(oldmod=get_db(ACCESS_DB,pKey))) {
        	// get the old mod for user as master
	        if ((oldmod=get_db(ACCESS_DB,pLogin))) {
		   	    // only a master  can modify  a other master
    		    if (strchr(oldmod,'m') && !exist_db(ACCESS_DB,accesslogin)) {
	        	    pthread_mutex_unlock(&account_mutex);
            		notice(pNick,MSG_NOT_MASTER);
	        	    return;
    		    }
		
			}

	    }

    	DEBUG("Modify account %s in channel %s",pLogin,pChannel);

	    //check for add or remove  mod
    	if (pPos[0]!='+' && pPos[0]!='-') {
	        notice(pNick,MSG_USERMODE_ERR);
        	pthread_mutex_unlock(&account_mutex);
    	    return;
	    } else {
    	    mod[0]=pPos[0];
	    }

    	// checke the mod
	   	switch (pPos[1]) {
	    case 'v':
    	case 'o':
	        mod[1]=pPos[1];
        	break;
    	case 'm':
	        if (!exist_db(ACCESS_DB,accesslogin)) {
        	    notice(pNick,MSG_NOT_MASTER);
    	    } else {
	            mod[1]=pPos[1];
        	}
    	    break;
	    default:
        	notice(pNick,MSG_UNKNOWN_MODS);
    	    return;
	    }

    	// set the end mark
	    mod[2]='\0';

    	// set end mark for the  mode line
	    DEBUG("Found mod %s",mod);

    	// check for old or new master
	    if (strchr(oldmod,'m') || (mod[1]=='m')) {
    	    DEBUG("Modify a master account");

        	// remove old  access rights if make a user to bot master or
	        // remove  old  master access rights if a master put of a other access level
    	    if (mod[1]=='m' && !strchr(oldmod,'m')) {
        	    // remove old access right
            	rmAccessRights(pLogin);

	            // replace the  old key by the login as new key
    	        free(pKey);
        	    pKey=(char *)malloc((strlen(pLogin)+1)*sizeof(char));
            	strcpy(pKey,pLogin);

	        } else if (mod[1]!='m' && strchr(oldmod,'m')) {
        	    // remove master account
    	        del_db(ACCESS_DB,pLogin);
	        }
    	}
	    
		// add or remove the mod
   		if (mod[0]=='+') {
        DEBUG("Set new mode");

        if (!(add_db(ACCESS_DB,pKey,mod))) {
            replace_db(ACCESS_DB,pKey,mod);
        }
	    } else {
    	    DEBUG("Remove new mode");
	        // remove mod
        	del_db(ACCESS_DB,pKey);

    	}


	    // identify the  login and set the rights
    	usernick=getNickname(get_db(USERTONICK_DB,pLogin));	
	    if (strlen(usernick) && oldmod[1]!= mod[1]) {
    	    DEBUG("Modify the current mode");
	        if (mod[0]=='-') {
            	// remove mods
        	    mode(pChannel,mod,usernick);
    	    } else {
	            // remove old mods
        	    if (strchr(oldmod,'v')) {
    	            // remove -o
	                mode(pChannel,"-v",usernick);
            	} else if (strlen(oldmod)) {
        	        // remove -v
    	            mode(pChannel,"-o",usernick);
	            }

            	// set the new mods
        	    if (mod[1]=='v') {
    	            mode(pChannel,"+v",usernick);
	            } else {
            	    mode(pChannel,"+o",usernick);
        	    }
    	    }
	    }
		pthread_mutex_unlock(&account_mutex);
	    notice(pNick,MSG_USERMODE_OK);
	} 
}
// #########################################################################
// Bot comand: !chanmode [#channel] <mod>
// #########################################################################
void chanmode(char *pLine) {
    char *pNick;
    char *pChannel;
    char *pChannelSet;
    char *pParameters;

    int i;

    ChannelMode_t *pNewMode;
    ChannelData_t *pChannelData;


    pNick=getNickname(pLine);
    pChannel=getAccessChannel(pLine);
    pParameters=getParameters(pLine);


    // read the old channel parameters
    
    if (pChannelSet=get_db(CHANNEL_DB,pChannel)) {
        pChannelData=StrToChannelData(pChannelSet);

	    // read the new channel parameters
	    pNewMode=StrToChannelMode(getParameters(pLine));
    	DEBUG("Found the new channel modes \"%s\"",ChannelModeToStr(pNewMode));

	    DEBUG("Build the new modes for the channel %s",pChannel);
    	// build the new channel parameters
	    for (i=1;i<MAX_MODES;i++) {
    	    if (pNewMode->pModeStr[MOD_TYPE]=='+') {
        	    // add  the new mode in the pChannelData
            	if (pNewMode->pModeStr[i]!=' ') {
                	// set the new flag
	                pChannelData->pModes->pModeStr[i]=pNewMode->pModeStr[i];
	
    	            // set keyword and limit
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
        	    // remove  flags from the pChanneldata
            	if (pNewMode->pModeStr[i]!=' ') {

	                // remove the  mode flag
    	            pChannelData->pModes->pModeStr[i]=' ';

        	        // remove keyword and limit
            	    if (pChannelData->pModes->pModeStr[i]=='k') {
                	    pChannelData->pModes->pKeyword[0]='\0';
	                } else if (pChannelData->pModes->pModeStr[i]=='l') {
    	                pChannelData->pModes->pLimit[0]='\0';
        	        }
            	}

	        }
    	}


	    // check the exit  of channell modes
    	// set or remove the leading plus
	    if (strpbrk(pChannelData->pModes->pModeStr,CHANNEL_MODES)) {
    	    pChannelData->pModes->pModeStr[0]='+';
	    } else {
    	    pChannelData->pModes->pModeStr[0]=' ';
	    }

    	DEBUG("Write the new modes channel \"%s\"",ChannelModeToStr(pChannelData->pModes))
	    // set the new mode in database
    	free(pChannelSet);
	    pChannelSet=ChannelDataToStr(pChannelData);
    	replace_db(CHANNEL_DB,pChannel,pChannelSet);


	    DEBUG("Set the new channel modes")
    	// set the mods
	    mode(pChannel,ChannelModeToStr(pNewMode),NULL);
    } else {
        notice(pNick,MSG_NOT_CHANNEL);
    }
}
// #########################################################################
// Bot comand: !rmuser <login>
// #########################################################################
void rmuser(char *pLine) {
    extern pthread_mutex_t account_mutex;
    char *pLogin;
    char *pNick;
    char *rmnick;
    char **ppChannels;
    int i;

    pNick=getNickname(pLine);
    pLogin=getParameters(pLine);
    
    pthread_mutex_lock(&account_mutex);


    DEBUG("Remove %s from the user list",pLogin);
    // extract the first parameter
    strtok(pLogin," ");

    if (exist_db(USER_DB,pLogin)) {
        // remove the account
        rmAccount(pLogin);
    
        // remove the mod  for  this account
        ppChannels=list_db(CHANNEL_DB);
        if ((rmnick=getNickname(get_db(USERTONICK_DB,pLogin)))) {
            for (i=0;ppChannels[i]!=NULL;i++) {
                mode(ppChannels[i],"-o",rmnick);
                mode(ppChannels[i],"-v",rmnick);
            }
        }
        notice(pNick,MSG_RMUSER_OK);        
    }
    pthread_mutex_lock(&account_mutex);
    notice(pNick,MSG_NOT_ACCOUNT);
}
// #########################################################################
// Bot comand: !userlist [#channel]
// #########################################################################
void userlist(char *pLine){
    
    char *pNick;
    char *pLogin;
    char *pAccessChannel;
    char *pArgv;
    char *pKey;
    char **ppLogins;
    char **ppChannels;
    char *pMod;
    char *pMsgStr;

    unsigned int i,j,k,iChanLen,iLoginLen;

    pLogin=get_db(NICKTOUSER_DB,getNetmask(pLine));
    pNick=getNickname(pLine);
    pAccessChannel=getAccessChannel(pLine);
    pArgv=getArgument(pLine);

    // read the list of Logins
    ppLogins=list_db(USER_DB);

    notice(pNick,MSG_USERLIST_BEGIN);

    // build the  login list  for output
    if (exist_db(ACCESS_DB,pLogin) && !strlen(pArgv)) {
        DEBUG("Genrate the Userlist for a master");
        // Bot masters

        // get the kist of all channels
        ppChannels=list_db(CHANNEL_DB);
        for (i=0;ppLogins[i]!=NULL;i++) {
            iLoginLen=strlen(ppLogins[i]);

            // check for master or normal user
            if (exist_db(ACCESS_DB,ppLogins[i])) {
                // user is master
                pMsgStr=(char*)malloc((USERLIST_TAB+strlen("Master   Status:")+5)*sizeof(char));
                strcpy(pMsgStr,ppLogins[i]);

                // fill
                for (j=0;j<(USERLIST_TAB-iLoginLen);j++) {
                    strcat(pMsgStr," ");
                }

                strcat(pMsgStr,"Master   Status: ");

                // insert  online status
                // set  online Status
                if (exist_db(USERTONICK_DB,ppLogins[i])) {
                    strcat(pMsgStr,"ON ");
                } else {
                    strcat(pMsgStr,"OFF");
                }

                notice(pNick,pMsgStr);
                free(pMsgStr);
            } else {
                // normal user
                if (ppChannels) {
                    for (j=0;ppChannels[j]!=NULL;j++) {
                        iChanLen=strlen(ppChannels[j]);

                        // build key for access.dbf
                        pKey=(char*)malloc((iLoginLen+iChanLen+1)*sizeof(char));
                        sprintf(pKey,"%s%s",ppLogins[i],ppChannels[j]);

                        if ((pMod=get_db(ACCESS_DB,pKey))) {
                            pMsgStr=(char*)malloc((USERLIST_TAB+iChanLen+strlen("Status:")+16)*sizeof(char));
                            strcpy(pMsgStr,ppLogins[i]);

                            // fill
                            for (k=0;k<(USERLIST_TAB-iLoginLen);k++) {
                                strcat(pMsgStr," ");
                            }

                            strcat(pMsgStr,ppChannels[j]);

                            // set access rights
                            if (pMod[1]=='o') {
                                strcat(pMsgStr,"->Owner ");
                            } else if (pMod[1]=='v') {
                                strcat(pMsgStr,"->Friend");
                            } else {
                                free(pMsgStr);
                                break;
                            }

                            // set  online Status
                            strcat(pMsgStr,"   Status: ");
                            if (exist_db(USERTONICK_DB,ppLogins[i])) {
                                strcat(pMsgStr,"ON ");
                            } else {
                                strcat(pMsgStr,"OFF");
                            }

                            // send notice out
                            notice(pNick,pMsgStr);
                            free(pMsgStr);
                        }
                        free(pKey);
                    }
                }
            }
        }
    } else {
        DEBUG("Genrate the Userlist for a owner");

        // look for  rights of  user for  the channel
        if (ppLogins) {
            iChanLen=strlen(pAccessChannel);

            for (i=0;ppLogins[i]!=NULL;i++) {
                iLoginLen=strlen(ppLogins[i]);

                // build the key  for  access.dbf
                pKey=(char*)malloc((iChanLen+1+iLoginLen)*sizeof(char));
                sprintf(pKey,"%s%s",ppLogins[i],pAccessChannel);

                if ((pMod=get_db(ACCESS_DB,pKey))) {
                    pMsgStr=(char*)malloc((USERLIST_TAB+strlen("Status:")+14)*sizeof(char));
                    strcpy(pMsgStr,ppLogins[i]);

                    // fill
                    for (j=0;j<(USERLIST_TAB-iLoginLen);j++) {
                        strcat(pMsgStr," ");
                    }

                    // set access rights
                    if (pMod[1]=='o') {
                        strcat(pMsgStr,"Owner ");
                    } else if (pMod[1]=='v') {
                        strcat(pMsgStr,"Friend");
                    } else {
                        free(pMsgStr);
                        break;
                    }

                    // set  online Status
                    strcat(pMsgStr,"   Status: ");
                    if (exist_db(USERTONICK_DB,ppLogins[i])) {
                        strcat(pMsgStr,"ON ");
                    } else {
                        strcat(pMsgStr,"OFF");
                    }

                    // send notice out
                    notice(pNick,pMsgStr);
                    free(pMsgStr);
                }
                free(pKey);

            }
        } else {
            notice(pNick,MSG_USERLIST_EMPTY);
        }
    }
    notice(pNick,MSG_USERLIST_END);
}
