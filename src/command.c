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
#include <time.h>

#include "config.h"
#include "extract.h"
#include "utilities.h"
#include "messages.h"
#include "dbaccess.h"
#include "irc.h"
#include "irchelp.h"
#include "parser.h"
#include "command.h"


// ######################################################################### 
// Bot comand: !help <command>
// ######################################################################### 
void help(char *line) {
	char *nick;
	char *parameter;
	char *tmp;
	char *msgstr;

	nick=getNickname(line);

	unsigned int i,j;
	boolean log_status=false,master_status=false;

	// check for parameters
	if (!(parameter=getParameters(line))) {
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
        if ((log_status=exist_db(NICKTOUSER_DB,nick))) {
			master_status=exist_db(ACCESS_DB,get_db(NICKTOUSER_DB,nick));
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
					   && strcmp(irchelp_msg[i],"!channels")
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
// ######################################################################### 
// Bot comand: !hello
// ######################################################################### 
void hello(char *line) {
	char *nick;
	
	nick=getNickname(line);

	if (!add_db(USER_DB,nick,"")) {
		notice(nick,MSG_NICK_EXIST);
		return;
	}

	// autoidentify after create an new account
	log_on(nick,nick);

	notice(nick,MSG_HELLO);
	notice(nick,MSG_HELLO2);
	notice(nick,MSG_NOT_ACCOUNT);

}
// ######################################################################### 
// Bot comand: !pass <password>
// ######################################################################### 
void password(char *line) {
	char *login;
	char *passwd;
	char *nick;
	char *parameter;
	
	nick==getNickname(line);

	// get  the  login name 
	if ((login=get_db(NICKTOUSER_DB,nick))==NULL) {
		notice(nick,MSG_NOT_LOGON);
		return;
	}
    
	// parse the  password  form  parameter list 
	if (!(parameter=getParameters(line))){
		passwd=(char *)malloc(sizeof(char));
		*passwd='\0';
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
// ######################################################################### 
// Bot comand: !logoff
// ######################################################################### 
void logoff(char *line) {
	extern pthread_mutex_t account_mutex;
	char *login;
	char *nick;
	
	nick=getNickname(line);
    login=get_db(NICKTOUSER_DB,nick);
	
	pthread_mutex_lock(&account_mutex);
	del_db(NICKTOUSER_DB,nick);
	del_db(USERTONICK_DB,login);
	pthread_mutex_unlock(&account_mutex);

	notice(nick,MSG_LOGOFF);
}
// ######################################################################### 
// Bot comand: !ident login <password>
// ######################################################################### 
void ident(char *line) {
	char *login;
	char *passwd;
	char *pos;
	char *nick;
	char *parameter;

	nick=getNickname(line);
	
	if (exist_db(NICKTOUSER_DB,nick)) {
		notice(nick,MSG_ALREADY_LOGON);
		return;
	}
	
	// no parameter found 
	if (!(parameter=getParameters(line))) {
		notice(nick,MSG_IDENT_ERR);
		return;
	}
	
	// parse the password 
	if ((pos=strstr(parameter," "))==NULL) {
		// no Passwd found 
		// try empty pass
		notice(nick,MSG_NOT_PASS);
		passwd=(char *)malloc(sizeof(char));
		*passwd='\0';
	} else {
		passwd=(char *)malloc(strlen(pos)*sizeof(char));
		strcpy(passwd,&pos[1]);
	}
	

	// parse the login 
	strtok(parameter," ");
	login=(char *)malloc((strlen(parameter)+1)*sizeof(char));
	strcpy(login,parameter);

	// check the account 
	if(check_db(USER_DB,login,passwd)) {
		log_on(nick,login);
		notice(nick,MSG_IDENT_OK);
		return;
	}
	notice(nick,MSG_NOT_ACCOUNT);
}
// ######################################################################### 
// Bot comand: !addchannel #channel
// ######################################################################### 
void channel_add(char *line) {
	char *login;
	char *channel;
	char *nick;
	char *channelmod;

	nick=getNickname(line);
    login=get_db(NICKTOUSER_DB,nick);
	
	if (!(channel=getAccessChannel(line))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_ADDCHANNEL_ERR);
		return;
	}

	if (!(strcmp(channel,getChannel(line)))) {
		notice(nick,MSG_ADDCHANNEL_ERR);
		notice(nick,MSG_NOT_CHANNELOPT);
		return;
	}


	// checking of channel exist 
	if (exist_db(CHANNEL_DB,channel)) {
		notice(nick,MSG_ADDCHANNEL_ALREADY);
	} else {
		// add channel 
		channelmod=(char *)malloc(3*sizeof(char));
		strcpy(channelmod,"\t\t");
		add_db(CHANNEL_DB,channel,channelmod);
		notice(nick,MSG_ADDCHANNEL_OK);
	}


	
	// join the channel 
	join(channel);
	notice(nick,MSG_JOIN_OK);

}
// ######################################################################### 
// Bot comand: !rmchannel <#channel>
// ######################################################################### 
void channel_rm(char *line){
	char *login,*channel;
	char *nick;

	nick=getNickname(line);
	login=get_db(NICKTOUSER_DB,nick);

	if (!(channel=getAccessChannel(line))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_RMCHANNEL_ERR);
		return;
	}
	
	// checking of channel exists 
	if (!del_db(CHANNEL_DB,channel)) {
		notice(nick,MSG_NOT_CHANNEL);
	} else {
		notice(nick,MSG_RMCHANNEL_OK);
	}

	// part the channel 
	part(channel);
	notice(nick,MSG_PART_OK);

}
// ######################################################################### 
// Bot comand: !join #channel
// ######################################################################### 
void join_channel(char *line) {
	char *login;
	char *channel;
	char *nick;


	nick=getNickname(line);
	login=get_db(NICKTOUSER_DB,nick);


	// get the channel name
	if (!(channel=getAccessChannel(line))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_JOIN_ERR);
		return;
	}

	// compare the current channel and  the channel for joining
	if (!(strcmp(channel,getChannel(line)))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_JOIN_ERR);
		return;
	}

	// join the channel 
	join(channel);
	notice(nick,MSG_JOIN_OK);
	
}
// ######################################################################### 
// Bot comand: !part <#channel>
// ######################################################################### 
void part_channel(char *line) {
	char *nick;
	char *login;
	char *channel;

	nick=getNickname(line);
	login=get_db(NICKTOUSER_DB,nick);

	if (!(channel=getAccessChannel(line))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_RMCHANNEL_ERR);
		return;
	}
	
	// part the channel
	part(channel);
	notice(nick,MSG_PART_OK);
}
// ######################################################################### 
// Bot comand: !die
// ######################################################################### 
void die(char *line) {
	stopParser(0);
}
// ######################################################################### 
// Bot comand: !nick nickname
// ######################################################################### 
void change_nick(char *line){
	char *parameter;
	char *_nick;

	_nick=getNickname(line);

	// read parameters
	if (!(parameter=getParameters(line))) {
		notice(_nick,MSG_NICK_ERR);
		return;
	} else if (strpbrk(parameter,USER_NOT_ALLOW_CHAR)) {
		notice(_nick,MSG_NICK_INVALID);
		return;
	}

	notice(_nick,MSG_NICK_SET);
	nick(parameter);
	
}
// ######################################################################### 
// Bot comand: !channels
// ######################################################################### 
void channel_list(char *line){
	char *nick,*buffer;
	char **channels;
	int i=0,buffer_size=0;

	nick=getNickname(line);

	// get  the channel list form the DB
	channels=list_db(CHANNEL_DB);
	
	while (channels[i]!=NULL) {
		buffer_size+=strlen(channels[i])+1;
		i++;
	}
	
	// create the buffer
	buffer=(char *)malloc((strlen(MSG_CHANNELS)+buffer_size+1 )*sizeof(char));
	sprintf(buffer,MSG_CHANNELS);
	
	i=0;
    while (channels[i]!=NULL) {
        strcat(buffer," ");
		strcat(buffer,channels[i]);
		i++;
	}
	DEBUG("%s",buffer);
	notice(nick,buffer); 
}
// ######################################################################### 
// Bot comand: !version
// ######################################################################### 
void version(char *line) {
    char str[64];
	// creat Versions String
    sprintf(str,VERSIONSTR);
	strcat(str,"\r\n");
	notice(getNickname(line),str);
}
// ######################################################################### 
// Bot comand: !greating <#channel> <text>
// ######################################################################### 
void setGreating(char *line) {
	char *parameter;
	char *channel;
	char *channelstr;
	char *_channelstr;
	char *mode;
	char *topic;
	char *nick;
	char *greatLine;

	nick=getNickname(line);

	if (!(channel=getAccessChannel(line))){
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_GREATING_ERR);
		return;
	}

	DEBUG("Greating seting for %s",channel);

	// parse greating string
	if (!(parameter=getParameters(line))) {				 
		// empty greating line 
		greatLine=(char *)malloc(sizeof(char));
		greatLine[0]='\0';
		DEBUG("Remove greating line");
	} else {
		// new gearing line
		greatLine=(char*)malloc((strlen(parameter)+1)*sizeof(char));
		strcpy(greatLine,parameter);
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
// ######################################################################### 
// Bot comand: !topic <#channel> <topic>
// ######################################################################### 
void setTopic(char *line) {
	char *parameter;
	char *channel;
	char *channelstr;
	char *_channelstr;
	char *mode;
	char *_topic;
	char *nick;
	char *greatLine;

	nick=getNickname(line);

	
	if (!(channel=getAccessChannel(line))){
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_TOPIC_ERR);
		return;
	}

	DEBUG("Greating seting for %s",channel);

	// parse greating string
	if (!(parameter=getParameters(line))) {				 
		// empty topic		 
		_topic=(char *)malloc(sizeof(char));
		_topic[0]='\0';
		DEBUG("Remove topic");
	} else {
		// new topic
		_topic=(char*)malloc((strlen(parameter)+1)*sizeof(char));
		strcpy(_topic,parameter);
		DEBUG("Set new topic");
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
	if (!(greatLine=getGreating(channelstr))) {
		greatLine=(char*)malloc(sizeof(char));
		greatLine[0]='\0';
	}
	
	_channelstr=(char *)malloc((strlen(mode)+strlen(_topic)+strlen(greatLine)+3)*sizeof(char));
	sprintf(_channelstr,"%s\t%s\t%s",mode,_topic,greatLine);

	replace_db(CHANNEL_DB,channel,_channelstr);
    
	// message
	if (_topic[0]=='\0') {
		notice(nick,MSG_RM_TOPIC);
	} else {
		notice(nick,MSG_SET_TOPIC);
	}

	topic(channel,_topic);
}
// ######################################################################### 
// Bot comand: !viewgreat <#channel>
// ######################################################################### 
void greating(char *line) {
	char *channel;
	char *channelstr;
	char *greating;
	char *nick;

    nick=getNickname(line);
	

	if (!(channel=getAccessChannel(line))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_VIEWGREAT_ERR);
		return;
	}

	DEBUG("Greating for %s",channel);

	if (!(channelstr=get_db(CHANNEL_DB,channel))) {
		return;
	}
						   

	if (!(greating=getGreating(channelstr))) {
		return;
	}

	DEBUG("Greating line %s",greating);
	notice(getNickname(line),greating);

}
// ######################################################################### 
// Event handler: JOIN
// ######################################################################### 
void bot_op(char *line){
	extern ConfType setup;
	char *channel;
	char *names;
	char *pos;
	char *searchstr;

	channel=getChannel(line);
	
	// extrakt Namelist
	pos=strchr(&line[1],':');
	names=(char*)malloc((strlen(pos)+1)*sizeof(char));
	strcpy(names,pos);

	searchstr=(char *) malloc((strlen(setup.botname)+2)*sizeof(char));
	sprintf(searchstr,"@%s",setup.botname);

	DEBUG("Look for OP right for %s",searchstr);
	if (strstr(names,searchstr)) {
		return;
	}

  	privmsg(channel,MSG_NEED_OP);
}
// ######################################################################### 
// Bot comand: !say <#channel> text
// ######################################################################### 
void say(char *line) {
	char *channel;
	char *parameters;
	char *nick;

    nick=getNickname(line);

	if (!(channel=getAccessChannel(line))){
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_SAY_ERR);
		return;
	}

	if(!(parameters=getParameters(line))) {
		notice(nick,MSG_SAY_ERR);
		return;
	}

	privmsg(channel,parameters);
}
// ######################################################################### 
// Bot comand: !kick <#channel> nick reason
// ######################################################################### 
void kickuser(char *line) {
	char *nick;
	char *kicknick;
	char *channel;
	char *reason;
	char *parameter;
	char *pos;

	nick=getNickname(line);
	

	// get  the  channel
	if (!(channel=getAccessChannel(line))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_KICK_ERR);
		return;
	}

	// get parameters
	if (!(parameter=getParameters(line))) {
		notice(nick,MSG_KICK_ERR);
		return;
	}

	// parse reason
	if (!(pos=strchr(parameter,' '))) {
		// empty reason
		reason=(char*)malloc(sizeof(char));
		*reason='\0';
    } else {
		// over jump the  space
		pos++;
		reason=(char*)malloc((strlen(pos)+1)*sizeof(char));
		strcpy(reason,pos);

	}

	// parse nick
	strtok(parameter," ");
	kicknick=malloc((strlen(parameter)+1)*sizeof(char));
	strcpy(kicknick,parameter);

	kick(channel,kicknick,reason);
	notice(nick,MSG_KICK_OK);
}
// ######################################################################### 
// Bot comand: !usermode [#channel] <login> <mods>
// ######################################################################### 
void usermode(char *line){
	char *channel;
	char *nick;
	char *parameters;
	char *pos;
	char *login;
	char *usernick;
	char *key;
	char *mods;
	char *oldmods;

	int len,i,j;

    nick=getNickname(line);
	

	// get the channel for modify
	if (!(channel=getAccessChannel(line))) {
		notice(nick,MSG_NOT_CHANNELOPT);
		notice(nick,MSG_USERMODE_ERR);
		return;
	}

	// check the channel
	if (!(exist_db(CHANNEL_DB,channel))) {
		notice(nick,MSG_NOT_CHANNEL);
		return;
	}

	// get parameters
	if (!(parameters=getParameters(line))) {
		notice(nick,MSG_USERMODE_ERR);
		return;
	}

	// user mods
	DEBUG("Modify user mods");
	
	// look for the space and separat the login for  the user whiche  modify
	if (!(pos=strchr(parameters,' '))) {
		notice(nick,MSG_USERMODE_ERR);
		return;
	}

	// set the end mark for loginstring and  set the pointer of the substr
	*pos='\0';
	pos++;

	// extract the  login
	login=(char *)malloc((strlen(parameters)+1)*sizeof(char));
	strcpy(login,parameters);

	// check login in the user db
	if (!(exist_db(USER_DB,login))) {
		notice(nick,MSG_NOT_ACCOUNT);
		return;
	}

	DEBUG("Modify account %s in channel %s",login,channel);


	// check and parse the mods
	// build a correct mode parameter line
	len=strlen(pos);
	j=0;
	mods=(char *)malloc((len+1)*sizeof(char));
	for(i=0;i<len;i++){
		switch (pos[i]) {
		case '-':
		case '+':
			// ignore plus and minus after the  first position
			if (i>0) {
				break;
			}
		case 'O':
		case 'o':
		case 'v':
		case 'm':
			// build the mods line
			mods[j]=pos[i];
			j++;
			break;
		default:
			notice(nick,MSG_UNKNOWN_MODS);
			return;
		}
	}

	// set end mark	for the  mode line
	mods[j]='\0';

	DEBUG("Found mods %s",mods);
	
	
	// build the key  for access rights  db
	key=(char*)malloc((strlen(login)+strlen(channel)+1)*sizeof(char));
	sprintf(key,"%s%s",login,channel);

	// get the  old mods from the  access db
	oldmods=get_db(ACCESS_DB,key);

	// add or remove the mods
	len=strlen(mods);
	if (mods[0]=='+') {
		// add mods
		for(i=1;i<len;i++){
			if (!(pos=strchr(oldmods,mods[i]))) {
				// add the new mode
				oldmods=(char*)realloc(oldmods,(strlen(oldmods)+2)*sizeof(char));
				sprintf(oldmods,"%s%c",oldmods,mods[i]);
			}
		}

	} else {
		// remove mods
		for(i=1;i<len;i++){
			if ((pos=strchr(oldmods,mods[i]))) {
				//replace old  mode entry
				*pos=' ';
			}
		}
		clearspace(oldmods);
	}

	// try to add
	// if  this failed then make replace
	if (!(add_db(ACCESS_DB,key,oldmods))) {
		replace_db(ACCESS_DB,key,oldmods);
	}
	
	if ((usernick=get_db(USERTONICK_DB,login))) {
		mode(channel,mods,usernick);
	}
	notice(nick,MSG_USERMODE_OK);
}
// ######################################################################### 
// Bot comand: !chanmode [#channel] <mods>
// ######################################################################### 
void chanmode(char *line) {
}

// ############################################################################# 
boolean log_on(char *nick,char *login) {
	extern pthread_mutex_t account_mutex;
	time_t timestamp;
	char str_timestamp[32];

	if (exist_db(NICKTOUSER_DB,nick)) {
		return false;
	}
    
	pthread_mutex_lock(&account_mutex);
	add_db(NICKTOUSER_DB,nick,login);
	add_db(USERTONICK_DB,login,nick);
	pthread_mutex_unlock(&account_mutex);

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
	return true;
}
