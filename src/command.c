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

#include "config.h"
#include "extract.h"
#include "utilities.h"
#include "messages.h"
#include "dbaccess.h"
#include "irc.h"
#include "irchelp.h"
#include "parser.h"
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

	pNetmask=getNetmask(pLine);
	pNick=getNickname(pLine);

	pParameter=getParameters(pLine);

	unsigned int i,j;
	boolean bIsLogin=false,bIsMaster=false;

	// check for parameters
	if (!strlen(pParameter)) {
		DEBUG("Default information");
		
		// Header of help message 
		for (i=0;i<HELP_ITEM_SIZE;i++) {
			// look for the end  of msg 
			if ((irchelp_msg[i][0]=='\0') || (i==HELP_ITEM_SIZE)) {
				break;
			}
			notice(pNick,(char *)irchelp_msg[i]);
		}
	
		// checking  login and  master status 
        if ((bIsLogin=exist_db(NICKTOUSER_DB,pNetmask))) {
			bIsMaster=exist_db(ACCESS_DB,get_db(NICKTOUSER_DB,pNetmask));
		}
		
    
		i=HELP_ITEM_SIZE;
		while (irchelp_msg[i][0]!='.') {
			// checking  for allow commands
			// first check for  unkwon user
			// secondary check for not master 
			if (!bIsLogin
				&& strcmp(irchelp_msg[i],"!help")
				&& strcmp(irchelp_msg[i],"!version")
				&& strcmp(irchelp_msg[i],"!hello")
				&& strcmp(irchelp_msg[i],"!ident")) {
				// next command 
				i+=HELP_ITEM_SIZE;
				continue;
			} else if (!bIsMaster
					   && !(strcmp(irchelp_msg[i],"!die")
					   && strcmp(irchelp_msg[i],"!join")
					   && strcmp(irchelp_msg[i],"!part")
					   && strcmp(irchelp_msg[i],"!addchannel")
					   && strcmp(irchelp_msg[i],"!rmchannel")
					   && strcmp(irchelp_msg[i],"!channels")
					   && strcmp(irchelp_msg[i],"!rmuser")
					   && strcmp(irchelp_msg[i],"!allsay")
					   && strcmp(irchelp_msg[i],"!nick"))) {
				// next command 
				i+=HELP_ITEM_SIZE;
				continue;
			}

			// calculat the length of buffer 
			pMsgStr=(char *)calloc(HELP_TAB+1+strlen(irchelp_msg[i+1]),sizeof(char));
						
			// build string 
			strcpy(pMsgStr,irchelp_msg[i]);
			
			// fill
			for (j=0;j<(HELP_TAB-strlen(irchelp_msg[i]));j++) {
				strcat(pMsgStr," ");
			}
			strcat(pMsgStr,irchelp_msg[i+1]);
			
			// send notice 
			notice(pNick,pMsgStr);

			// next command 
			i+=HELP_ITEM_SIZE;
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
		i=HELP_ITEM_SIZE;
		while (irchelp_msg[i][0]!='.') {
			
			// look for  the command 
			if (!strcmp((char*)irchelp_msg[i],pParameter)) {
				DEBUG("Commando found");
				
				// the head	for help
				pTmp=malloc((strlen(MSG_HELP_FOR)+strlen((char *)irchelp_msg[i])+3)*sizeof(char));
				sprintf(pTmp,"%s %s:",MSG_HELP_FOR,irchelp_msg[i]);
				notice(pNick,pTmp);

				// help message	of the  command 
				for (j=1;j<HELP_ITEM_SIZE;j++) {
					// look for  the end of the  msg 
					if ((irchelp_msg[j+i][0]=='\0') || (j==HELP_ITEM_SIZE)) {
						// tail
						notice(pNick,MSG_HELP_END);
						return;
					}
					notice(pNick,(char *)irchelp_msg[j+i]);
				}
			}
			
			// next command	
			i+=HELP_ITEM_SIZE;
		}
        notice(pNick,MSG_NOT_COMMAND);
	}
}
// ######################################################################### 
// Bot comand: !hello
// ######################################################################### 
void hello(char *pLine) {
	char *pNetmask;
	char *pNick;
	char *pLogin;
	
	pNetmask=getNetmask(pLine);
	pNick=getNickname(pLine);
	pLogin=malloc((strlen(pNick)+1)*sizeof(char));
	
	strcpy(pLogin,pNick);
	
	StrToLower(pLogin);

	if (!add_db(USER_DB,pLogin,"")) {
		notice(pNick,MSG_NICK_EXIST);
		return;
	}

	if (exist_db(NICKTOUSER_DB,pNetmask)) {
		notice(pNick,MSG_ALREADY_LOGON);
		return;
	}

	// autoidentify after create an new account
	pNetmask=getNetmask(pLine);
	log_on(pNetmask,pLogin);

	notice(pNick,MSG_HELLO);
	notice(pNick,MSG_HELLO2);
	notice(pNick,MSG_IDENT_OK);

}
// ######################################################################### 
// Bot comand: !pass <password>
// ######################################################################### 
void password(char *pLine) {
	char *pLogin;
	char *pPasswd;
	char *pNick;
	char *pNetmask;
	
	pNetmask=getNetmask(pLine);
	pNick=getNickname(pLine);
	pLogin=get_db(NICKTOUSER_DB,pNetmask);

	// get  the  login name 
	if (!strlen(pLogin)) {
		notice(pNick,MSG_NOT_LOGON);
		return;
	}
    
	pPasswd=getParameters(pLine);

	// parse the  password  form  parameter list 
	if (!strlen(pPasswd)){
		notice(pNick,MSG_NOT_PASS);
	}

	// set password 
	replace_db(USER_DB,pLogin,pPasswd);
	notice(pNick,MSG_PASSWD);
}
// ######################################################################### 
// Bot comand: !logoff
// ######################################################################### 
void logoff(char *pLine) {
	char *pLogin;
	char *pNick;
	char *pNetmask;
	char **ppChannels;

	int i;

	pNetmask=getNetmask(pLine);
	pNick=getNickname(pLine);
    pLogin=get_db(NICKTOUSER_DB,pNetmask);
	
	log_out(pLogin);
	

	// remove the mod  for  this account
	ppChannels=list_db(CHANNEL_DB);
	for (i=0;ppChannels[i]!=NULL;i++) {
		mode(ppChannels[i],"-o",pNick);
		mode(ppChannels[i],"-v",pNick);
	}

	notice(pNick,MSG_LOGOFF);
}
// ######################################################################### 
// Bot comand: !ident login <password>
// ######################################################################### 
void ident(char *pLine) {
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
	
	if (exist_db(NICKTOUSER_DB,pNetmask)) {
		notice(pNick,MSG_ALREADY_LOGON);
		return;
	}
	
	pParameter=getParameters(pLine);

	// no parameter found 
	if (!strlen(pParameter)) {
		notice(pNick,MSG_IDENT_ERR);
		return;
	}
	
	// parse the password 
	if ((pPos=strstr(pParameter," "))==NULL) {
		// no Passwd found 
		// try empty pass
		notice(pNick,MSG_NOT_PASS);
		pPasswd=(char *)malloc(sizeof(char));
		*pPasswd='\0';
	} else {
		pPasswd=(char *)malloc(strlen(pPos)*sizeof(char));
		strcpy(pPasswd,&pPos[1]);
	}
	

	// parse the login 
	strtok(pParameter," ");
	pLogin=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
	strcpy(pLogin,pParameter);
	StrToLower(pLogin);

	// check the account 
	if(check_db(USER_DB,pLogin,pPasswd)) {
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
				pKey=malloc((strlen(ppChannels[i])+login_len+1)*sizeof(char));
				sprintf(pKey,"%s%s",pLogin,ppChannels[i]);
				pMod=get_db(ACCESS_DB,pKey);

				if (strlen(pMod)) {
					mode(ppChannels[i],pMod,pNick);
					free(pMod);
				} 
				free(pKey);
			}
		}
		return;
	}
	notice(pNick,MSG_NOT_ACCOUNT);



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

	// part the channel
	part(pChannel);
	notice(pNick,MSG_PART_OK);
}
// ######################################################################### 
// Bot comand: !die
// ######################################################################### 
void die(char *pLine) {
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
	} else if (strpbrk(pParameter,USER_NOT_ALLOW_CHAR)) {
		notice(pNick,MSG_NICK_INVALID);
		return;
	}

	notice(pNick,MSG_NICK_SET);
	nick(pParameter);
	
}
// ######################################################################### 
// Bot comand: !channels
// ######################################################################### 
void listChannels(char *pLine){
	char *pNick,*buffer;
	char **ppChannels;
	int i=0,buffer_size=0;

	pNick=getNickname(pLine);

	// get  the channel list form the DB
	ppChannels=list_db(CHANNEL_DB);
	
	while (ppChannels[i]!=NULL) {
		buffer_size+=strlen(ppChannels[i])+1;
		i++;
	}
	
	// create the buffer
	buffer=(char *)malloc((strlen(MSG_CHANNELS)+buffer_size+1 )*sizeof(char));
	sprintf(buffer,MSG_CHANNELS);
	
	i=0;
    while (ppChannels[i]!=NULL) {
        strcat(buffer," ");
		strcat(buffer,ppChannels[i]);
		i++;
	}
	DEBUG("%s",buffer);
	notice(pNick,buffer); 
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
// Bot comand: !greating <#channel> <text>
// ######################################################################### 
void setGreating(char *pLine) {
	char *pChannel;
	char *channelstr;
	char *_channelstr;
	char *mode;
	char *pTopic;
	char *pNick;
	char *greatLine;

	pNick=getNickname(pLine);
	pChannel=getAccessChannel(pLine);

	DEBUG("Greating seting for %s",pChannel);

	greatLine=getParameters(pLine);

    // check of  existenz of the channel
	channelstr=get_db(CHANNEL_DB,pChannel);
	
	if (!strlen(channelstr)) {
		notice(pNick,MSG_NOT_CHANNEL);
		return;
	}

	// insert or replace the greating  in the DB
	mode=getMode(channelstr);
	pTopic=getTopic(channelstr);
	
	_channelstr=(char *)malloc((strlen(mode)+strlen(pTopic)+strlen(greatLine)+3)*sizeof(char));
	sprintf(_channelstr,"%s\t%s\t%s",mode,pTopic,greatLine);

	replace_db(CHANNEL_DB,pChannel,_channelstr);
    
	// message
	if (greatLine[0]=='\0') {
		notice(pNick,MSG_RM_GREATING);
	} else {
		notice(pNick,MSG_SET_GREATING);
	}
}
// ######################################################################### 
// Bot comand: !topic <#channel> <topic>
// ######################################################################### 
void setTopic(char *pLine) {
	char *pChannel;
	char *channelstr;
	char *_channelstr;
	char *mode;
	char *pTopic;
	char *pNick;
	char *greatLine;

	pNick=getNickname(pLine);
	pChannel=getAccessChannel(pLine);
	
	DEBUG("Topic seting for %s",pChannel);
	pTopic=getParameters(pLine);
	channelstr=get_db(CHANNEL_DB,pChannel);

	// check of  existenz of the channel
	if (!strlen(channelstr)) {
		notice(pNick,MSG_NOT_CHANNEL);
		return;
	}

	// insert or replace the greating  in the DB
	mode=getMode(channelstr);
	greatLine=getGreating(channelstr);
	
	_channelstr=(char *)malloc((strlen(mode)+strlen(pTopic)+strlen(greatLine)+3)*sizeof(char));
	sprintf(_channelstr,"%s\t%s\t%s",mode,pTopic,greatLine);

	replace_db(CHANNEL_DB,pChannel,_channelstr);
    
	// message
	if (pTopic[0]=='\0') {
		notice(pNick,MSG_RM_TOPIC);
	} else {
		notice(pNick,MSG_SET_TOPIC);
	}

	topic(pChannel,pTopic);
}
// ######################################################################### 
// Bot comand: !viewgreat <#channel>
// ######################################################################### 
void greating(char *pLine) {
	char *pChannel;
	char *channelstr;
	char *greating;
	char *pNick;

    pNick=getNickname(pLine);
	pChannel=getAccessChannel(pLine);

	DEBUG("Greating for %s",pChannel);

	channelstr=get_db(CHANNEL_DB,pChannel);
	greating=getGreating(channelstr);

	if (!strlen(greating)) {
		return;
	}

	DEBUG("Greating line %s",greating);
	notice(pNick,greating);

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
	if(!strlen(pParameter)) {
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
		reason=(char*)malloc(sizeof(char));
		*reason='\0';
    } else {
		// set null byte  for  nick
		*pPos='\0';
		// over jump the  space
		pPos++;
		reason=(char*)malloc((strlen(pPos)+1)*sizeof(char));
		strcpy(reason,pPos);

	}

	// parse nick
	kicknick=malloc((strlen(pParameter)+1)*sizeof(char));
	strcpy(kicknick,pParameter);

	kick(pChannel,kicknick,reason);
	notice(pNick,MSG_KICK_OK);
}
// ######################################################################### 
// Bot comand: !usermode [#channel] <login> <mod>
// ######################################################################### 
void usermode(char *pLine){
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
	accesslogin=get_db(NICKTOUSER_DB,getNetmask(pLine));

	// check the channel
	if (!(exist_db(CHANNEL_DB,pChannel))) {
		notice(pNick,MSG_NOT_CHANNEL);
		return;
	}

	// get parameters
	pParameter=getParameters(pLine);

	// user mod
	DEBUG("Modify user mod");
	
	// look for the space and separat the login for the user which want modify
	if (!(pPos=strchr(pParameter,' '))) {
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
		notice(pNick,MSG_NOT_ACCOUNT);
		return;
	} else if (!strcmp(pLogin,accesslogin)){
		notice(pNick,MSG_NOT_SELF);
		return;
	}

	// build the key  for access rights  db
	pKey=(char*)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char));
	sprintf(pKey,"%s%s",pLogin,pChannel);

	oldmod=get_db(ACCESS_DB,pKey);
	
	// get the  old mod for  a  user from the  access db
	if (!strlen(oldmod)) {
        // get the old mod for user as master
		oldmod=get_db(ACCESS_DB,pLogin);

		// only a master  can modify  a other master
		if (strchr(oldmod,'m') && !exist_db(ACCESS_DB,accesslogin)) {
				notice(pNick,MSG_NOT_MASTER);
				return;
		}
	}

	DEBUG("Modify account %s in channel %s",pLogin,pChannel);

	//check for add or remove  mod
	if (pPos[0]!='+' && pPos[0]!='-') {
		notice(pNick,MSG_USERMODE_ERR);
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

	// set end mark	for the  mode line
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

	notice(pNick,MSG_USERMODE_OK);
}
// ######################################################################### 
// Bot comand: !chanmode [#channel] <mod>
// ######################################################################### 
void chanmode(char *pLine) {
}
// ######################################################################### 
// Bot comand: !rmuser <login>
// ######################################################################### 
void rmuser(char *pLine) {
	char *pLogin;
	char *pNick;
	char *rmnick;
	char **ppChannels;
	int i;

	pNick=getNickname(pLine);
	pLogin=getParameters(pLine);
	rmnick=getNickname(get_db(USERTONICK_DB,pLogin));


	DEBUG("Remove %s from the user list",pLogin);
	// extract the first parameter
	strtok(pLogin," ");

	if (!exist_db(USER_DB,pLogin)) {
		notice(pNick,MSG_NOT_ACCOUNT);
		return;
	}

	// remove the account
	rmAccount(pLogin);

	// remove the mod  for  this account
	ppChannels=list_db(CHANNEL_DB);
    if (strlen(rmnick)) {
		for (i=0;ppChannels[i]!=NULL;i++) {
			 mode(ppChannels[i],"-o",rmnick);
			 mode(ppChannels[i],"-v",rmnick);
		}
	}

	notice(pNick,MSG_RMUSER_OK);
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

	// read the lsit of Logins
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
				pMsgStr=malloc((USERLIST_TAB+strlen("Master   Status:")+5)*sizeof(char));
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
			}else {
				// normal user
				if (ppChannels) {
					for (j=0;ppChannels[j]!=NULL;j++) {
						iChanLen=strlen(ppChannels[j]);

						// build key for access.dbf
						pKey=malloc((iLoginLen+iChanLen+1)*sizeof(char));
						sprintf(pKey,"%s%s",ppLogins[i],ppChannels[j]);

						if((pMod=get_db(ACCESS_DB,pKey))) {
							pMsgStr=malloc((USERLIST_TAB+iChanLen+strlen("Status:")+16)*sizeof(char));
							strcpy(pMsgStr,ppLogins[i]);
							
							// fill
							for (k=0;k<(USERLIST_TAB-iLoginLen);k++) {
							   strcat(pMsgStr," ");
							}
		
							strcat(pMsgStr,ppChannels[j]);

							// set access rights
							if (pMod[1]=='o') {
								strcat(pMsgStr,"->Owner ");
							} else if (pMod[1]=='v'){
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
			
			for(i=0;ppLogins[i]!=NULL;i++) {
				iLoginLen=strlen(ppLogins[i]);
				
				// build the key  for  access.dbf
				pKey=malloc((iChanLen+1+iLoginLen)*sizeof(char));
				sprintf(pKey,"%s%s",ppLogins[i],pAccessChannel);

				if((pMod=get_db(ACCESS_DB,pKey))) {
					pMsgStr=malloc((USERLIST_TAB+strlen("Status:")+14)*sizeof(char));
					strcpy(pMsgStr,ppLogins[i]);
					
					// fill
					for (j=0;j<(USERLIST_TAB-iLoginLen);j++) {
					   strcat(pMsgStr," ");
					}

					// set access rights
					if (pMod[1]=='o') {
						strcat(pMsgStr,"Owner ");
					} else if (pMod[1]=='v'){
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
