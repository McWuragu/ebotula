

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>


#include "config.h"
#include "utilities.h"
#include "messages.h"
#include "dbaccess.h"
#include "irc.h"
#include "irchelp.h"
#include "command.h"

// ############################################################################# 
char *getNickname(char *line){
	char *nick,*str;
	
	str=(char*)malloc((strlen(line)+1)*sizeof(char));
	strcpy(str,line);

	if (!strtok(str,"!")) {
		return NULL;
	}

	nick=(char *)malloc((strlen(str)+1)*sizeof(char));
	strcpy(nick,&str[1]);
	return nick;
}
// ############################################################################# 
char *getNetmask(char *line){ 
	char *netmask,*str, *pos;
		
	str=(char *)malloc((strlen(line)+1)*sizeof(char));
	strcpy(str,line);
    
	if (!(pos=strchr(str,'!'))) {
		return  NULL;
	}
	
	strtok(pos," ");

	netmask=(char *)malloc(strlen(pos)*sizeof(char));
	strcpy(netmask,&pos[1]);

	return netmask;
}
// ############################################################################# 
char *getCommand(char *line) {
	char *str,*pos,*tmp;

	// mirror  of the orginal string 
	tmp=(char *)malloc((strlen(line)+1)*sizeof(char));
	strcpy(tmp,line);

	// find the  secondary double point
	// and put after this a null byte
	if (!(pos=strchr(&tmp[1],':'))) {
		return NULL;
	}
	pos[1]='\0';
	
	// cut out  the first part of the server answer 
	str=(char *)malloc((strlen(tmp)+1)*sizeof(char));
	strcpy(str,tmp);

	return str;
}
// ############################################################################# 
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
	

	
	str=(char *)malloc((strlen(pos)+1)*sizeof(char));
	strcpy(str,pos);

	trim(str);

	return str;
}


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
		for(i=0;i<HELP_ITEM_SIZE;i++) {
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

			// calculat the size oif  buffer 
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
				for(j=1;j<HELP_ITEM_SIZE;j++) {
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
        notice(nick,MSG_NO_COMMAND);
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

}
// ############################################################################# 
void password(char *line) {
	char *login,*passwd;
	char *netmask=getNetmask(line);
	char *parameter;
	
	// get  the  login name 
	if ((login=get_db(LOGIN_DB,netmask))==NULL) {
		notice(getNickname(line),MSG_NOT_LOGON);
		return;
	}
    
	// parse the  password  form  parameter list 
	if ((parameter=getArgument(line))==NULL){
		passwd=(char *)malloc(sizeof(char));
		strcpy(passwd,"");
		notice(getNickname(line),MSG_NO_PASS);
	} else {
		strtok(line," ");
		passwd=(char *)malloc((strlen(parameter)+1)*sizeof(char));
		strcpy(passwd,parameter);
	}


	// set password 
	replace_db(USER_DB,login,passwd);

	notice(getNickname(line),MSG_PASSWD);
}
// ############################################################################# 
void logoff(char *line) {
	char *netmask;
	char *nick;

	netmask=getNetmask(line);
	nick=getNickname(line);

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
	char *parameter;

	if (exist_db(LOGIN_DB,netmask)) {
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
		notice(getNickname(line),MSG_IDENT_OK);
		return;
	}
	notice(getNickname(line),MSG_IDENT_NO);
}
// ############################################################################# 
void channel_add(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);
	char *channelmod;


	// checking of login 
	CHECK_LOGIN(netmask,nick);
	login=get_db(LOGIN_DB,netmask);

    // checking of master 
	CHECK_MASTER(nick,login);

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


	// checking of login 
	CHECK_LOGIN(netmask,nick);
	login=get_db(LOGIN_DB,netmask);
	
    // checking of master 
	CHECK_MASTER(nick,login);
	
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
		notice(nick,MSG_RMCHANNEL_FAIL);
	} else {
		del_db(CHANNEL_DB,parameter);
		notice(nick,MSG_RMCHANNEL_OK);
	}

	// part the channel 
	part(parameter);
	notice(nick,MSG_PART_OK);

}
// ############################################################################# 
void join_all_channels(void) {
	char **channelliste;
	unsigned int i;

	channelliste=list_db(CHANNEL_DB);

	for (i=0;channelliste[i]!=NULL;i++) {
		join(channelliste[i]);
	}

	
}
// ############################################################################# 
void join_channel(char *line) {
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *nick=getNickname(line);


	// checking of login 
	CHECK_LOGIN(netmask,nick);
	login=get_db(LOGIN_DB,netmask);
	
    // checking of master 
	CHECK_MASTER(nick,login);


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


	// checking of login 
	CHECK_LOGIN(netmask,nick);
	login=get_db(LOGIN_DB,netmask);

	
    // checking of master 
	CHECK_MASTER(nick,login);

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
void log_on(char *netmask,char *login) {
	time_t timestamp;
	char str_timestamp[32];

	if (exist_db(LOGIN_DB,netmask)) {
		return;
	}
    
	add_db(LOGIN_DB,netmask,login);
	DEBUG("User log in");
	
	// build the timestamp
	
	time(&timestamp);
	sprintf(str_timestamp,"%ld",timestamp);

	// set the last login timestamp
	if (exist_db(TIMELOG_DB,login)) {
		replace_db(TIMELOG_DB,login,str_timestamp);
		DEBUG("Update timestamp %s for %s",str_timestamp,login);
	} else {
		add_db(TIMELOG_DB,login,str_timestamp);
		DEBUG("Add timepstamp %s for %s",str_timestamp,login);
	}
	

}
// ############################################################################# 
void die(char *line) {
	char *netmask=getNetmask(line);
	char *login;
	char *nick=getNickname(line);


	// checking of login
	CHECK_LOGIN(netmask,nick);
	login=get_db(LOGIN_DB,netmask);
	
    // checking of master
	CHECK_MASTER(nick,login);
	
	stopParser(0);
}
// ############################################################################# 
void change_nick(char *line){
	char *netmask=getNetmask(line);
	char *login,*parameter;
	char *_nick=getNickname(line);


	// checking of login
	CHECK_LOGIN(netmask,_nick);
	login=get_db(LOGIN_DB,netmask);

    // checking of master
	CHECK_MASTER(_nick,login);

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

	// checking of login
	CHECK_LOGIN(netmask,nick);
	
	login=get_db(LOGIN_DB,netmask);

	CHECK_MASTER(nick,login);	

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
