/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>			 
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "config.h"
#include "extract.h"
#include "irc.h"
#include "dbaccess.h"
#include "command.h"
#include "utilities.h"
#include "messages.h"
#include "parser.h"


MsgBufType preParser(char *line) {
    MsgBufType msg;
	char *str,*first_part,*pos;

	// init the buffer with zero
	bzero(&msg,sizeof(MsgBufType));
		
	// get the first part of the  answer from server
	if (!(first_part=getCommand(line))) {
		return msg;
	}

	if (!(pos=strchr(first_part,' '))){
		return msg;
	}
								 
	// preparse the line
	if (!strncmp(first_part,"PING",strlen("PING"))) {
		msg.mtype=2;
		msg.identify=CMD_PING;
	} else if (strstr(pos,"QUIT")) {
		msg.mtype=2;
		msg.identify=CMD_LOGOFF;
	} else if (strstr(pos,"JOIN")) {
		msg.mtype=2;
		msg.identify=CMD_GREATING;
	} else if (strstr(pos,"NICK")) {
		msg.mtype=2;
		msg.identify= CMD_NICKCHG;
	} else if (strstr(pos,"353")) {
		msg.mtype=2;
		msg.identify=CMD_NAMES;
	} else if ((str=strstr(line," :!"))!=NULL) {

		if (strlen(str)>=3) {
			str+=3;
	
			if (!strncmp(str,"help",strlen("help"))){
				msg.mtype=1;
				msg.identify=CMD_HELP;
			} else if (!strncmp(str,"version",strlen("version"))){
				msg.mtype=1;
				msg.identify=CMD_VERSION;
			}  else if (!strncmp(str,"hello",strlen("hello"))){
				msg.mtype=1;
				msg.identify=CMD_HELLO;
			} else if (!strncmp(str,"pass",strlen("pass"))){
				msg.mtype=1;
				msg.identify=CMD_PASS;
			} else if (!strncmp(str,"ident",strlen("ident"))){
				msg.mtype=1;
				msg.identify=CMD_IDENT;
			} else if (!strncmp(str,"addchannel",strlen("addchannel"))) {
				msg.mtype=1;
				msg.identify=CMD_ADDCHANNEL;
			} else if (!strncmp(str,"rmchannel",strlen("rmchannel"))) {
				msg.mtype=1;
				msg.identify=CMD_RMCHANNEL;
			} else if (!strncmp(str,"join",strlen("join"))) {
				msg.mtype=1;
				msg.identify=CMD_JOIN;
			} else if (!strncmp(str,"part",strlen("part"))) {
				msg.mtype=1;
				msg.identify=CMD_PART;
			} else if (!strncmp(str,"logoff",strlen("logoff"))) {
				msg.mtype=1;
				msg.identify=CMD_LOGOFF;
			} else if (!strncmp(str,"die",strlen("die"))) {
				msg.mtype=1;
				msg.identify=CMD_DIE;
			} else if (!strncmp(str,"nick",strlen("nick"))) {
				msg.mtype=1;
				msg.identify=CMD_NICK;
			} else if (!strncmp(str,"channels",strlen("channels"))) {
				msg.mtype=1;
				msg.identify=CMD_CHANNELS;
			} else if (!strncmp(str,"greating",strlen("greating"))) {
				msg.mtype=1;
				msg.identify=CMD_SET_GREATING;
			} else if (!strncmp(str,"viewgreat",strlen("viewgreat"))) {
				msg.mtype=1;
				msg.identify=CMD_GREATING;
			} else if (!strncmp(str,"topic",strlen("topic"))) {
				msg.mtype=1;
				msg.identify=CMD_SET_TOPIC;
			} else if (!strncmp(str,"say",strlen("say"))) {
				msg.mtype=1;
				msg.identify=CMD_SAY;
			} else if (!strncmp(str,"kick",strlen("kick"))) {
				msg.mtype=1;
				msg.identify=CMD_KICK;
			} else if (!strncmp(str,"usermode",strlen("usermode"))) {
				msg.mtype=1;
				msg.identify=CMD_USERMODE;
			}
		}
	}

	strcpy(msg.msg_line,line);
	return msg;
}

void *action_thread(void *argv) {
	int msgid;
	extern int key;
	MsgBufType msg;

	// set the thread cancelable 
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	
	// open the message queue 
	msgid=msgget(key,0600 | IPC_EXCL );

	// execute loop 
	while(1) {
		msgrcv(msgid,&msg,sizeof(MsgBufType)-sizeof(msg.mtype),0,0);

		if (AccessRight(msg.msg_line,msg.identify)) {
		
			switch (msg.identify) {
			case CMD_PING:
				pong();
				break;
			case CMD_LOGOFF:
				logoff(msg.msg_line);
				break;
			case CMD_HELP:
				help(msg.msg_line);
				break;
			case CMD_VERSION:
				version(msg.msg_line);
				break;
			case CMD_HELLO:
				hello(msg.msg_line);
				break;
			case CMD_PASS:
				password(msg.msg_line);
				break;
			case CMD_IDENT:
				ident(msg.msg_line);
				break;
			case CMD_ADDCHANNEL:
				channel_add(msg.msg_line);
				break;
			case CMD_RMCHANNEL:
				channel_rm(msg.msg_line);
				break;
			case CMD_JOIN:
				join_channel(msg.msg_line);
				break;
			case CMD_PART:
				part_channel(msg.msg_line);
				break;
			case CMD_DIE:
				die(msg.msg_line);
				break;
			case CMD_NICK:
				set_nick(msg.msg_line);
				break;
			case CMD_CHANNELS:
				channel_list(msg.msg_line);
				break;
			case CMD_NAMES:
				bot_op(msg.msg_line);
				break;
			case CMD_GREATING:
				greating(msg.msg_line);
				break;
			case CMD_SET_GREATING:
				setGreating(msg.msg_line);
				break;
			case CMD_SET_TOPIC:
				setTopic(msg.msg_line);
				break;
			case CMD_SAY:
				say(msg.msg_line);
				break;
			case CMD_KICK:
				kickuser(msg.msg_line);
				break;
			case CMD_USERMODE:
				usermode(msg.msg_line);
				break;
			case CMD_NICKCHG:
				nickchg(msg.msg_line);
				break;
			default:
				break;
			}
		}
		
		// clear buffer	
		bzero(&msg,sizeof(MsgBufType));
	}

	return NULL;
}

// ############################################################################# 
int AccessRight(char *line,CmdType cmd_id) {
	char *channel;
	char *login;
	char *nick;
	char *access;
	char *mods;
	char *channelstr;

	nick=getNickname(line);

	switch (cmd_id) {
	// handlers
	case CMD_PING:
	case CMD_NAMES:
	case CMD_GREATING:
	case CMD_TOPIC:
	// any users
	case CMD_HELP:
	case CMD_HELLO:
	case CMD_VERSION:
	case CMD_IDENT:
		return true;
	// logged in user
	case CMD_NICKCHG:
	case CMD_LOGOFF:
	case CMD_PASS:
		if (!exist_db(NICKTOUSER_DB,nick)) {
			notice(nick,MSG_NOT_LOGON);	
			return false;
		} else {
			return true;
		}
	// owner
	case CMD_SET_GREATING:
	case CMD_SET_TOPIC:
	case CMD_SAY:
	case CMD_KICK:
	case CMD_USERMODE:
		if (!exist_db(NICKTOUSER_DB,nick)) {
			notice(nick,MSG_NOT_LOGON);	
			return false;
		}

		login=get_db(NICKTOUSER_DB,nick);
		
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
		if (!exist_db(NICKTOUSER_DB,nick)) {
			notice(nick,MSG_NOT_LOGON);	
			return false;
		}

		login=get_db(NICKTOUSER_DB,nick);

		if (!exist_db(ACCESS_DB,login)) {
			notice(nick,MSG_NOT_MASTER);  
		} else {
			notice(nick,MSG_MASTER);  
			return true;
		}
	default:
		DEBUG("Unknown command-id");
		return false;
	}
}
// ############################################################################# 
void stopParser(int sig) {
	extern int stop;
	if (!stop) {
		quit();
	}

	stop=true;
	DEBUG("Stop IRCBot");
}
