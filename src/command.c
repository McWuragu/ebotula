
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#include "config.h"
#include "macro.h"
#include "messages.h"
#include "irc.h"
#include "dbaccess.h"
#include "ircbot.h"
#include "command.h"

char *getNickname(char *line){
	char *nick,*str;
	
	str=malloc(sizeof(char)*(strlen(line)+1));
	strcpy(str,line);

	if (!strtok(str,"!")) {
		free(str);
		return NULL;
	}

	nick=malloc(sizeof(char)*(strlen(str)+1));
	strcpy(nick,&str[1]);
	free(str);
	return nick;
}

char *getNetmask(char *line){ 
	char *netmask,*str, *pos;
		
	str=malloc(sizeof(char)*(strlen(line)+1));
	strcpy(str,line);
    
	if (!(pos=strchr(str,'!'))) {
		free(str);
		return  NULL;
	}
	
	strtok(pos," ");

	netmask=malloc(sizeof(char)*strlen(pos));
	strcpy(netmask,&pos[1]);

	free(str);
	return netmask;
}

char *getCommand(char *line) {
	char *str,*pos,*tmp;

	// mirror  of the orginal string
	tmp=malloc(sizeof(char)*(strlen(line)+1));
	strcpy(tmp,line);

	// find the  secondary double point
	// and put after this a null byte
	if (!(pos=strchr(&tmp[1],':'))) {
		free(tmp);
		return NULL;
	}
	pos[1]='\0';
	
	// cut out  the first part of the server answer
	str=malloc(sizeof(char)*(strlen(tmp)+1));
	strcpy(str,tmp);

	free(tmp);
	return str;
}

char *getArgument(char *line) {
	char *str,*pos;
	int i;
	
	// found  the begining  of Parameter
	if ((str=strstr(line," :!"))==NULL) {
		return NULL;
	} else {
		str+=3;
		for (i=0;i<strlen(str)+1;i++) {
			if (str[i]=='\0') {
				return NULL;
			} else if (str[i]==' ') {
				pos=&str[i];
				break;
			}
		}
	}
	

	
	str=malloc(sizeof(char)*(strlen(pos)+1));
	strcpy(str,pos);

	trim(str);


	return str;
}

/* ################################ BOT commandos ########################## */
void help(char *line) {
	char *netmask=getNetmask(line);
	char *nick=getNickname(line);
	char *parameter;

	// checking the  login status
	if (exist_db(LOGIN_DB,netmask)) {
		DEBUG("Access to the help system");
		// Help for user whiche are login
		if (!(parameter=getArgument(line))) {
			DEBUG("Default information");
			// Help defaulttext
		} else {
			DEBUG("Spezial information for a command");
			// Help for a command

			free(parameter);
		}
	} else {
		DEBUG("The small help system");
		// Help for unknown user
	}

	free(netmask);
	free(nick);
}

void hello(char *line) {
	char *netmask;
	char *nick=getNickname(line);

	if (!add_db(USER_DB,nick,"")) {
		notice(nick,MSG_NICK_EXIST);
		free(nick);
		return;
	}

	netmask=getNetmask(line);
	login(netmask,nick);
	free(netmask);

	notice(nick,MSG_HELLO);
	notice(nick,MSG_HELLO2);

	free(nick);
}

void password(char *line) {
	char *login,*passwd;
	char *netmask=getNetmask(line);
	char *parameter;
	
	// get  the  login name
	if ((login=get_db(LOGIN_DB,netmask))==NULL) {
		notice(getNickname(line),MSG_NOT_LOGON);
		free(netmask);
		return;
	}
	free(netmask);
    
	// parse the  password  form  parameter list
	if ((parameter=getArgument(line))==NULL){
		passwd=malloc(sizeof(char));
		strcpy(passwd,"");
		notice(getNickname(line),MSG_NO_PASS);
	} else {
		strtok(line," ");
		passwd=malloc(sizeof(char)*(strlen(parameter)+1));
		strcpy(passwd,parameter);
	}

	free(parameter);

	// set password
	replace_db(USER_DB,login,passwd);
	free(passwd);
	free(login);

	notice(getNickname(line),MSG_PASSWD);
}

void logoff(char *line) {
	char *netmask;
	char *nick;

	netmask=getNetmask(line);

	if (!exist_db(LOGIN_DB,netmask)) {
		free(netmask);
		return;
	} else {
		// user ausloggen
		nick=getNickname(line);
		del_db(LOGIN_DB,netmask);
		notice(nick,MSG_LOGOFF);
		free(netmask);
		free(nick);
	}
}

void ident(char *line) {
	char *login,*passwd,*pos;
	char *netmask=getNetmask(line);
	char *parameter;

	if (exist_db(LOGIN_DB,netmask)) {
		notice(getNickname(line),MSG_ALREADY_LOGON);
		free(netmask);
		return;
	}
	
	// no parameter found
	if ((parameter=getArgument(line))==NULL) {
		notice(getNickname(line),MSG_IDENT_ERR);
		free(netmask);
		return;
	}
	
	// parse the password
	if ((pos=strstr(parameter," "))==NULL) {
		// no Passwd found 
		// try empty pass
		notice(getNickname(line),MSG_NO_PASS);
		passwd=malloc(sizeof(char));
		strcpy(passwd,"");
	} else {
		passwd=malloc(sizeof(char)*strlen(pos));
		strcpy(passwd,&pos[1]);
	}
	
	DEBUG("Parse password");

	// parse the login
	strtok(parameter," ");
	login=malloc(sizeof(char)*(strlen(parameter)+1));
	strcpy(login,parameter);
	free(parameter);
	DEBUG("Parse login");

	if(check_db(USER_DB,login,passwd)) {
		add_db(LOGIN_DB,netmask,login);
		notice(getNickname(line),MSG_IDENT_OK);
		free(netmask);
		free(login);
		return;
	}
	notice(getNickname(line),MSG_IDENT_NO);
	free(netmask);
	free(login);
}

void channel_add(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);
	char *channelmod;


	// checking of login
	if (!exist_db(LOGIN_DB,netmask)) {
		notice(nick,MSG_NOT_LOGON);
		return;
	} else {
		login=get_db(LOGIN_DB,netmask);
	}
	free(netmask);

    // checking of master
	if (!exist_db(ACCESS_DB,login)) {
		notice(nick,MSG_NOT_MASTER);
		free(login);
		free(nick);
		return;
	}
	free(login);

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		free(nick);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		free(nick);
		free(parameter);
		DEBUG("\"%s\"",parameter);
		return;
	}

	//checking of channel exists
	if (exist_db(CHANNEL_DB,parameter)) {
		notice(nick,MSG_ADDCHANNEL_ALREADY);
	} else {
		// add channel
		channelmod=malloc(sizeof(char)*3);
		sprintf(channelmod,"\t\t");
		add_db(CHANNEL_DB,parameter,channelmod);
		notice(nick,MSG_ADDCHANNEL_OK);
		free(channelmod);
	}


	
	// join the channel
	join(parameter);
	notice(nick,MSG_JOIN_OK);

	free(parameter);
	free(nick);
}

void channel_rm(char *line){
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	// checking of login
	if (!exist_db(LOGIN_DB,netmask)) {
		notice(nick,MSG_NOT_LOGON);
		free(netmask);
		free(nick);
		return;
	} else {
		login=get_db(LOGIN_DB,netmask);
	}
	free(netmask);
	
    // checking of master
	if (!exist_db(ACCESS_DB,login)) {
		notice(nick,MSG_NOT_MASTER);
		free(nick);
		free(login);
		return;
	}
	free(login);
	
	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_RMCHANNEL_ERR);
		free(nick);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		free(nick);
		free(parameter);
		DEBUG("\"%s\"",parameter);
		return;
	}

	//checking of channel exists
	if (!exist_db(CHANNEL_DB,parameter)) {
		notice(nick,MSG_RMCHANNEL_FAIL);
	} else {
		del_db(CHANNEL_DB,parameter);
		notice(nick,MSG_RMCHANNEL_OK);
	}

	// part the channel
	part(parameter);
	notice(nick,MSG_PART_OK);

	free(parameter);
	free(nick);
}

void join_all_channels(void) {
	char **channelliste;
	unsigned int i;

	channelliste=list_db(CHANNEL_DB);

	for (i=0;channelliste[i]!=NULL;i++) {
		join(channelliste[i]);
//		free(channelliste[i]);
	}

	free(channelliste);
	
}

void join_channel(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	// checking of login
	if (!exist_db(LOGIN_DB,netmask)) {
		notice(nick,MSG_NOT_LOGON);
		free(netmask);
		free(nick);
		return;
	} else {
		login=get_db(LOGIN_DB,netmask);
		free(netmask);
	}
	
    // checking of master
	if (!exist_db(ACCESS_DB,login)) {
		notice(nick,MSG_NOT_MASTER);
		free(login);
		free(nick);
		return;
	}

	free(login);

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		free(nick);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		free(nick);
		free(parameter);
		return;
	}

	// join the channel
	join(parameter);
	notice(nick,MSG_JOIN_OK);
	
	free(parameter);
	free(nick);
}

void part_channel(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	// checking of login
	if (!exist_db(LOGIN_DB,netmask)) {
		notice(nick,MSG_NOT_LOGON);
		free(netmask);
		free(nick);
		return;
	} else {
		login=get_db(LOGIN_DB,netmask);
		free(netmask);
	}

	
    // checking of master
	if (!exist_db(ACCESS_DB,login)) {
		notice(nick,MSG_NOT_MASTER);
		free(nick);
		free(login);
		return;
	}
	free(login);

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		free(nick);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		free(parameter);
		free(nick);
		return;
	}

	// part the channel
	part(parameter);
	notice(nick,MSG_PART_OK);
	
	free(parameter);
	free(nick);
}

void login(char *netmask,char *login) {
	if (exist_db(LOGIN_DB,netmask)) {
		notice(getNickname(netmask),MSG_ALREADY_LOGON);
		return;
	}
    
	add_db(LOGIN_DB,netmask,login);
}
