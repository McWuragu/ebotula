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
#include "handles.h"
#include "command.h"


// ################################ BOT commandos ########################## 
void help(char *line) {
	char *netmask=getNetmask(line);
	char *nick=getNickname(line);
	char *parameter;
	char *tmp;
	char *msgstr;


	unsigned int i,j,log_status=false,master_status=false;;

	if (!(parameter=getArgument(line))) {
		DEBUG("Default information");
		
		// Header of help message 
		for (i=0;i<HELP_ITEM_SIZE;i++) {
			// look for the end  of msg 
			if ((irchelp_msg[i][0]=='\0') || (i==HELP_ITEM_SIZE)) {
				break;
			}
			notice(nick,(char *)irchelp_msg[i]);
		}
	
		// checking  login and  master status 
        if ((log_status=exist_db(LOGIN_DB,netmask))) {
			master_status=exist_db(ACCESS_DB,get_db(LOGIN_DB,netmask));
		}
		
    
		i=HELP_ITEM_SIZE;
		while (irchelp_msg[i][0]!='.') {
			// checking  for allow commands
			// first check for  unkwon user
			// secondary check for not master 
			if (!log_status
				&& strcmp(irchelp_msg[i],"!help")
				&& strcmp(irchelp_msg[i],"!version")
				&& strcmp(irchelp_msg[i],"!hello")
				&& strcmp(irchelp_msg[i],"!ident")) {
				// next command 
				i+=HELP_ITEM_SIZE;
				continue;
			} else if (!master_status
					   && !(strcmp(irchelp_msg[i],"!die")
					   && strcmp(irchelp_msg[i],"!join")
					   && strcmp(irchelp_msg[i],"!part")
					   && strcmp(irchelp_msg[i],"!addchannel")
					   && strcmp(irchelp_msg[i],"!rmchannel")
					   && strcmp(irchelp_msg[i],"!nick"))) {
				// next command 
				i+=HELP_ITEM_SIZE;
				continue;
			}

			// calculat the length of buffer 
			msgstr=(char *)calloc(13+strlen(irchelp_msg[i+1]),sizeof(char));
						
			// build string 
			strcpy(msgstr,irchelp_msg[i]);
			strncat(msgstr,"            ",12-strlen(irchelp_msg[i]));
			strcat(msgstr,irchelp_msg[i+1]);
			
			// send notice 
			notice(nick,msgstr);

			// next command 
			i+=HELP_ITEM_SIZE;
		}
		// the tail
		notice(nick,MSG_HELP_END);
	} else {
		DEBUG("Spezial information for a command");
		
		// cut the first word 
		strtok(parameter," ");

		// checking  of a leading '!' 
		if (parameter[0]!='!') {
			tmp=(char*)malloc((strlen(parameter)+2)*sizeof(char));
			sprintf(tmp,"!%s",parameter);
			free(parameter);
			parameter=tmp;
		}

		DEBUG("Looking for information about \"%s\"",parameter);

		// Help for a command 
		i=HELP_ITEM_SIZE;
		while (irchelp_msg[i][0]!='.') {
			
			// look for  the command 
			if (!strcmp((char*)irchelp_msg[i],parameter)) {
				DEBUG("Commando found");
				
				// the head	for help
				tmp=malloc((strlen(MSG_HELP_FOR)+strlen((char *)irchelp_msg[i])+3)*sizeof(char));
				sprintf(tmp,"%s %s:",MSG_HELP_FOR,irchelp_msg[i]);
				notice(nick,tmp);

				// help message	of the  command 
				for (j=1;j<HELP_ITEM_SIZE;j++) {
					// look for  the end of the  msg 
					if ((irchelp_msg[j+i][0]=='\0') || (j==HELP_ITEM_SIZE)) {
						// tail
						notice(nick,MSG_HELP_END);
						return;
					}
					notice(nick,(char *)irchelp_msg[j+i]);
				}
			}
			
			// next command	
			i+=HELP_ITEM_SIZE;
		}
        notice(nick,MSG_NOT_COMMAND);
	}
}
// ############################################################################# 
void hello(char *line) {
	char *netmask;
	char *nick=getNickname(line);

	if (!add_db(USER_DB,nick,"")) {
		notice(nick,MSG_NICK_EXIST);
		return;
	}

	netmask=getNetmask(line);
	log_on(netmask,nick);

	notice(nick,MSG_HELLO);
	notice(nick,MSG_HELLO2);
	notice(nick,MSG_NOT_ACCOUNT);

}
// ############################################################################# 
void password(char *line) {
	char *login,*passwd;
	char *netmask=getNetmask(line);
	char *nick=getNickname(line);
	char *parameter;
	
	// get  the  login name 
	if ((login=get_db(LOGIN_DB,netmask))==NULL) {
		notice(nick,MSG_NOT_LOGON);
		return;
	}
    
	// parse the  password  form  parameter list 
	if ((parameter=getArgument(line))==NULL){
		passwd=(char *)malloc(sizeof(char));
		strcpy(passwd,"");
		notice(nick,MSG_NOT_PASS);
	} else {
		strtok(line," ");
		passwd=(char *)malloc((strlen(parameter)+1)*sizeof(char));
		strcpy(passwd,parameter);
	}


	// set password 
	replace_db(USER_DB,login,passwd);
	notice(nick,MSG_PASSWD);
}
// ############################################################################# 
void logoff(char *line) {
	char *netmask=getNetmask(line);
	char *nick=getNickname(line);

	if (!exist_db(LOGIN_DB,netmask)) {
		notice(nick,MSG_NOT_LOGON);
		return;
	} 

	// user ausloggen 
    del_db(LOGIN_DB,netmask);
	notice(nick,MSG_LOGOFF);
}
// ############################################################################# 
void ident(char *line) {
	char *login,*passwd,*pos;
	char *netmask=getNetmask(line);
	char *nick=getNickname(line);
	char *parameter;

	if (exist_db(LOGIN_DB,netmask)) {
		notice(nick,MSG_ALREADY_LOGON);
		return;
	}
	
	// no parameter found 
	if ((parameter=getArgument(line))==NULL) {
		notice(nick,MSG_IDENT_ERR);
		return;
	}
	
	// parse the password 
	if ((pos=strstr(parameter," "))==NULL) {
		// no Passwd found 
		// try empty pass
		notice(nick,MSG_NOT_PASS);
		passwd=(char *)malloc(sizeof(char));
		strcpy(passwd,"");
	} else {
		passwd=(char *)malloc(strlen(pos)*sizeof(char));
		strcpy(passwd,&pos[1]);
	}
	

	// parse the login 
	strtok(parameter," ");
	login=(char *)malloc((strlen(parameter)+1)*sizeof(char));
	strcpy(login,parameter);

	if(check_db(USER_DB,login,passwd)) {
		log_on(netmask,login);
		notice(nick,MSG_IDENT_OK);
		return;
	}
	notice(nick,MSG_NOT_ACCOUNT);
}
// ############################################################################# 
void channel_add(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);
	char *channelmod;


	login=get_db(LOGIN_DB,netmask);


	// read parameters 
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	} else if (((parameter[0]!='#') && (parameter[0]!='&')) || strpbrk(parameter,CHANNEL_NOT_ALLOW_CHAR)) {
		notice(nick,MSG_CHANNEL_INVALID);
		return;
	}

	// checking of channel exist 
	if (exist_db(CHANNEL_DB,parameter)) {
		notice(nick,MSG_ADDCHANNEL_ALREADY);
	} else {
		// add channel 
		channelmod=(char *)malloc(3*sizeof(char));
		sprintf(channelmod,"\t\t");
		add_db(CHANNEL_DB,parameter,channelmod);
		notice(nick,MSG_ADDCHANNEL_OK);
	}


	
	// join the channel 
	join(parameter);
	notice(nick,MSG_JOIN_OK);

}
// ############################################################################# 
void channel_rm(char *line){
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	login=get_db(LOGIN_DB,netmask);
	
	// read parameters 
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_RMCHANNEL_ERR);
		return;
	} else if (((parameter[0]!='#') && (parameter[0]!='&'))  || strpbrk(parameter,CHANNEL_NOT_ALLOW_CHAR)) {
		notice(nick,MSG_CHANNEL_INVALID);
		DEBUG("\"%s\"",parameter);
		return;
	}

	// checking of channel exists 
	if (!exist_db(CHANNEL_DB,parameter)) {
		notice(nick,MSG_NOT_CHANNEL);
	} else {
		del_db(CHANNEL_DB,parameter);
		notice(nick,MSG_RMCHANNEL_OK);
	}

	// part the channel 
	part(parameter);
	notice(nick,MSG_PART_OK);

}

// ############################################################################# 
void join_channel(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	login=get_db(LOGIN_DB,netmask);

	// read parameters 
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	} else if (((parameter[0]!='#') && (parameter[0]!='&')) || strpbrk(parameter,CHANNEL_NOT_ALLOW_CHAR)) {
		notice(nick,MSG_CHANNEL_INVALID);
		return;
	}

	// join the channel 
	join(parameter);
	notice(nick,MSG_JOIN_OK);
	
}
// ############################################################################# 
void part_channel(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	login=get_db(LOGIN_DB,netmask);

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	} else if (((parameter[0]!='#') && (parameter[0]!='&')) || strpbrk(parameter,CHANNEL_NOT_ALLOW_CHAR)) {
		notice(nick,MSG_CHANNEL_INVALID);
		return;
	}

	// part the channel
	part(parameter);
	notice(nick,MSG_PART_OK);
	
}

// ############################################################################# 
void die(char *line) {
	char *netmask=getNetmask(line);
	char *login;
        
	login=get_db(LOGIN_DB,netmask);
	
	stopParser(0);
}
// ############################################################################# 
void change_nick(char *line){
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *_nick=getNickname(line);


	login=get_db(LOGIN_DB,netmask);


	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(_nick,MSG_NICK_ERR);
		return;
	} else if (!strpbrk(parameter,USER_ALLOW_CHAR)) {
		notice(_nick,MSG_NICK_INVALID);
		return;
	}

	notice(_nick,MSG_NICK_SET);
	nick(parameter);
	
}
// ############################################################################# 
void channel_list(char *line){
	char *nick,*netmask,*buffer,*login;
	char **channels;
	int i=0,buffer_size=0;

	nick=getNickname(line);
	netmask=getNetmask(line);

	
	login=get_db(LOGIN_DB,netmask);

	// get  the channel list form the DB
	channels=list_db(CHANNEL_DB);
	
	while (channels[i]!=NULL) {
		buffer_size+=strlen(channels[i])+1;
		i++;
	}
	
	// create the buffer
	buffer=(char *)malloc((strlen(MSG_CHANNELS)+buffer_size+1 )*sizeof(char));
	sprintf(buffer,MSG_CHANNELS);
	
	DEBUG("%s",buffer);
	
	i=0;
    while (channels[i]!=NULL) {
        strcat(buffer," ");
		strcat(buffer,channels[i]);
		i++;
	}
	DEBUG("%s",buffer);
	notice(nick,buffer); 
}

// ############################################################################# 
void version(char *line) {
    char *str;
	str=(char *)calloc(strlen(PROGNAME)+strlen(" Version ")+strlen(VERSION)+3,sizeof(char));
			
	// creat Versions String
	sprintf(str,"%s Version %s\r\n",PROGNAME,VERSION);
	notice(getNickname(line),str);
}
// ############################################################################# 
void greating(char *line) {
	char *parameter;
	char *channel;
	char *channelstr;
	char *_channelstr;
	char *pos;
	char *mode;
	char *topic;
	char *nick;
	char *netmask;
	char *login;
	char *greatLine;
	int paramlen;

	nick=getNickname(line);
	netmask=getNetmask(line);
	
	parameter=getArgument(line);
	paramlen=strlen(parameter);

	login=get_db(LOGIN_DB,netmask);

	
	if (!(channel=getAccessChannel(line))){
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_GREATING_ERR);
		return;
	}

	DEBUG("Greating seting for %s",channel);

	// parse greating string
	if ((paramlen==0)|| (parameter[0]=='#' && !strchr(parameter,' '))) {				 
		 
		greatLine=(char *)malloc(sizeof(char));
		greatLine[0]='\0';
		DEBUG("Remove greating line");
	} else {
		if (parameter[0]=='#') {
			pos=strchr(parameter,' ');
			greatLine=(char*)malloc((strlen(pos))*sizeof(char));
			pos++;
			strcpy(greatLine,pos);
		}else {
			greatLine=(char*)malloc((strlen(parameter)+1)*sizeof(char));
			strcpy(greatLine,parameter);
		}
		DEBUG("Set new greating line");
	}

	// check of  existenz of the channel
	if (!(channelstr=get_db(CHANNEL_DB,channel))) {
		notice(nick,MSG_NOT_CHANNEL);
		return;
	}

	// insert or replace the greating  in the DB
	if (!(mode=getMode(channelstr))) {
		mode=(char*)malloc(sizeof(char));
		mode[0]='\0';
	}
	if (!(topic=getTopic(channelstr))) {
		topic=(char*)malloc(sizeof(char));
		topic[0]='\0';
	}
	
	_channelstr=(char *)malloc((strlen(mode)+strlen(topic)+strlen(greatLine)+3)*sizeof(char));
	sprintf(_channelstr,"%s\t%s\t%s",mode,topic,greatLine);

	replace_db(CHANNEL_DB,channel,_channelstr);
    
	// message
	if (greatLine[0]=='\0') {
		notice(nick,MSG_RM_GREATING);
	} else {
		notice(nick,MSG_SET_GREATING);
	}
}
