
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
		return NULL;
	}

	nick=malloc(sizeof(char)*(strlen(str)+1));
	strcpy(nick,&str[1]);
	return nick;
}

char *getNetmask(char *line){ 
	char *netmask,*str, *pos;
		
	str=malloc(sizeof(char)*(strlen(line)+1));
	strcpy(str,line);
    
	if (!(pos=strchr(str,'!'))) {
		return  NULL;
	}
	
	strtok(pos," ");

	netmask=malloc(sizeof(char)*strlen(pos));
	strcpy(netmask,&pos[1]);
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
		return NULL;
	}
	pos[1]='\0';
	
	// cut out  the first part of the server answer
	str=malloc(sizeof(char)*(strlen(tmp)+1));
	strcpy(str,tmp);

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

void help(char *line) {
	char *netmask=getNetmask(line);
	char *nick=getNickname(line);
	char *parameter;

	// checking the  login status
	if (check_login(netmask)) {
		DEBUG("Access to the help system");
		// Help for user whiche are login
		if (!(parameter=getArgument(line))) {
			DEBUG("Default information");
			// Help defaulttext
		} else {
			DEBUG("Spezial information for a command");
			// Help for a command
		}
	} else {
		DEBUG("The small help system");
		// Help for unknown user
	}
}

void hello(char *line) {
	char *netmask=getNetmask(line);
	char *nick=getNickname(line);

	if (!add_user(nick,"")) {
		notice(nick,MSG_NICK_EXIST);
		return;
	}

	
	login(netmask,nick);

	notice(nick,MSG_HELLO);
	notice(nick,MSG_HELLO2);

	free(nick);
}

void password(char *line) {
	char *login,*passwd;
	char *netmask=getNetmask(line);
	char *parameter;
	
	// get  the  login name
	if ((login=get_login(netmask))==NULL) {
		notice(getNickname(line),MSG_NOT_LOGON);
		return;
	}
	
    
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


	// set password
	replace_passwd(login,passwd);

	notice(getNickname(line),MSG_PASSWD);
}

void logoff(char *line) {
	char *netmask;

	netmask=getNetmask(line);

	if (!check_login(netmask)) {
		DEBUG("User is not login");
		return;
	} else {
		// user ausloggen
		del_login(netmask);
	}


}

void ident(char *line) {
	char *login,*passwd,*pos;
	char *netmask=getNetmask(line);
	char *parameter;

	if (check_login(netmask)) {
		notice(getNickname(line),MSG_ALREADY_LOGON);
		return;
	}
	
	// no parameter found
	if ((parameter=getArgument(line))==NULL) {
		notice(getNickname(line),MSG_IDENT_ERR);
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
	DEBUG("Parse login");

	if(check_user(login,passwd)) {
		add_login(netmask,login);
		notice(getNickname(line),MSG_IDENT_OK);
		return;
	}
	notice(getNickname(line),MSG_IDENT_NO);
}

void channel_add(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);
	char *channelmod;


	// checking of login
	if (!check_login(netmask)) {
		notice(nick,MSG_NOT_LOGON);
		return;
	} else {
		login=get_login(netmask);
	}

	
    // checking of master
	if (!check_channel_access(login)) {
		notice(nick,MSG_NOT_MASTER);
		return;
	}

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		DEBUG("\"%s\"",parameter);
		return;
	}

	//checking of channel exists
	if (check_channel(parameter)) {
		notice(nick,MSG_ADDCHANNEL_ALREADY);
	} else {
		// add channel
		channelmod=malloc(sizeof(char)*3);
		sprintf(channelmod,"\t\t");
		add_channel(parameter,channelmod);
		notice(nick,MSG_ADDCHANNEL_OK);
	}


	
	// join the channel
	join(parameter);
	notice(nick,MSG_JOIN_OK);
}

void channel_rm(char *line){
char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	// checking of login
	if (!check_login(netmask)) {
		notice(nick,MSG_NOT_LOGON);
		return;
	} else {
		login=get_login(netmask);
	}

	
    // checking of master
	if (!check_channel_access(login)) {
		notice(nick,MSG_NOT_MASTER);
		return;
	}

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		DEBUG("\"%s\"",parameter);
		return;
	}

	//checking of channel exists
	if (check_channel(parameter)) {
		notice(nick,MSG_ADDCHANNEL_ALREADY);
	} else {
		del_channel(parameter);
		notice(nick,MSG_RMCHANNEL_OK);
	}

	// part the channel
	part(parameter);
	notice(nick,MSG_PART_OK);
}

void join_all_channels(void) {
}

void join_channel(char *line) {
		char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	// checking of login
	if (!check_login(netmask)) {
		notice(nick,MSG_NOT_LOGON);
		return;
	} else {
		login=get_login(netmask);
	}

	
    // checking of master
	if (!check_channel_access(login)) {
		notice(nick,MSG_NOT_MASTER);
		return;
	}

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		return;
	}

	// join the channel
	join(parameter);
	notice(nick,MSG_JOIN_OK);
}

void part_channel(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	// checking of login
	if (!check_login(netmask)) {
		notice(nick,MSG_NOT_LOGON);
		return;
	} else {
		login=get_login(netmask);
	}

	
    // checking of master
	if (!check_channel_access(login)) {
		notice(nick,MSG_NOT_MASTER);
		return;
	}

	// read parameters
	if (!(parameter=getArgument(line))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	} else if ((parameter[0]!='#') && (parameter[0]!='&')) {
		notice(nick,MSG_CHANNEL_INVALID);
		return;
	}

	// part the channel
	part(parameter);
	notice(nick,MSG_PART_OK);
}

void login(char *netmask,char *login) {
	if (check_login(netmask)) {
		notice(getNickname(netmask),MSG_ALREADY_LOGON);
		return;
	}
    
	add_login(netmask,login);
}
