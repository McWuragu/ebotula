#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "messages.h"
#include "extract.h"
#include "irc.h"
#include "dbaccess.h"
#include "command.h"
#include "utilities.h"

#include "handles.h"

// ############################################################################# 
void join_all_channels(void) {
	char **channelliste;
	unsigned int i;

	if ((channelliste=list_db(CHANNEL_DB))) {
		for (i=0;channelliste[i]!=NULL;i++) {
			join(channelliste[i]);
		}
	}
	
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
		add_db(TIMELOG_DB,netmask,str_timestamp);
		DEBUG("Add timepstamp %s for %s",str_timestamp,login);
		DEBUG("Add timepstamp %s for %s",str_timestamp,netmask);
	}
	

}
// ############################################################################# 
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
// ############################################################################# 
void print_greating(char *line) {
	char *channel;
	char *channelstr;
	char *greating;

	if (!(channel=strchr(line,'#'))) {
		return;
	}

	DEBUG("Greating for %s",channel);

	if (!(channelstr=get_db(CHANNEL_DB,channel))) {
		return;
	}
						   
	DEBUG("Channellins %s",channelstr);

	if (!(greating=getGreating(channelstr))) {
		return;
	}

	DEBUG("Greating line %s",greating);
	notice(getNickname(line),greating);

}
// ############################################################################# 
int AccessRight(char *line,CmdType cmd_id) {
	char *netmask;
	char *channel;
	char *login;
	char *nick;
	char *access;
	char *mods;
	char *channelstr;

	netmask=getNetmask(line);
	nick=getNickname(line);

	switch (cmd_id) {
	// handlers
	case CMD_PING:
	case CMD_NAMES:
	case CMD_JOIN_GREATING:
	// any users
	case CMD_HELP:
	case CMD_HELLO:
	case CMD_VERSION:
	case CMD_IDENT:
		return true;
	// logged in user
	case CMD_LOGOFF:
	case CMD_PASS:
		if (!exist_db(LOGIN_DB,netmask)) {
			notice(nick,MSG_NOT_LOGON);	
			return false;
		} else {
			return true;
		}
	// owner
	case CMD_GREATING:
		if (!exist_db(LOGIN_DB,netmask)) {
			notice(nick,MSG_NOT_LOGON);	
			return false;
		}

		login=get_db(LOGIN_DB,netmask);
		
		if (!(channel=getAccessChannel(line))) {
			notice(nick,MSG_NOT_CHANNELOPT);
			return false;
		}

		access=(char*)malloc((strlen(login)+strlen(channel)+1)*sizeof(char*));
		sprintf(access,"%s%s",login,channel);

		if((channelstr=get_db(ACCESS_DB,access))) {
			mods=getMode(channelstr);
			if (strchr(mods,'o')) {
				return true;
			}
		}

		notice(nick,MSG_NOT_OWNER);
		
	// master
	case CMD_DIE:
	case CMD_JOIN:
	case CMD_NICK:
	case CMD_PART:
	case CMD_RMCHANNEL:
	case CMD_ADDCHANNEL:
	case CMD_CHANNELS:
		if (!exist_db(LOGIN_DB,netmask)) {
			notice(nick,MSG_NOT_LOGON);	
			return false;
		}

		login=get_db(LOGIN_DB,netmask);

		if (!exist_db(ACCESS_DB,login)) {
			notice(nick,MSG_NOT_MASTER);  
		} else {
			return true;
		}
	default:
		DEBUG("Unknown command-id");
		return false;
	}
}
