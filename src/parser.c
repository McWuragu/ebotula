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
#include <syslog.h>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include "ircbot.h"
#include "extract.h"
#include "irc.h"
#include "dbaccess.h"
#include "command.h"
#include "handles.h"
#include "utilities.h"
#include "messages.h"
#include "parser.h"


MsgBufType preParser(char *pLine) {
    MsgBufType sMsg;
	char *pStr,*pPreamble,*pPos;

	// init the buffer with zero
	bzero(&sMsg,sizeof(MsgBufType));
		
	// get the first part of the  answer from server
	pPreamble=getCommand(pLine);

	if (!(pPos=strchr(pPreamble,' '))){
		return sMsg;
	}
								 
	// preparse the line
	// identify events and commands
	if (!strncmp(pPreamble,"PING",strlen("PING"))) {
		sMsg.mtype=2;
		sMsg.identify=CMD_ONPING;
	} else if (strstr(pPos,"QUIT")) {
		sMsg.mtype=2;
		sMsg.identify=CMD_ONQUIT;
	} else if (strstr(pPos,"JOIN")) {
		sMsg.mtype=2;
		sMsg.identify=CMD_ONJOIN;
	} else if (strstr(pPos,"NICK")) {
		sMsg.mtype=2;
		sMsg.identify= CMD_ONNICKCHG;
	} else if (strstr(pPos,"MODE")) {
		sMsg.mtype=2;
		sMsg.identify= CMD_ONMODE;
	} else if (strstr(pPos,"TOPIC")) {
		sMsg.mtype=2;
		sMsg.identify= CMD_ONTOPIC;
	} else if (strstr(pPos,"353")) {
		sMsg.mtype=2;
		sMsg.identify=CMD_ONNAMES;
	} else if ((pStr=strstr(pLine," :!"))!=NULL) {

		if (strlen(pStr)>=3) {
			pStr+=3;
	
			if (!strncmp(pStr,"help",strlen("help"))){
				sMsg.mtype=1;
				sMsg.identify=CMD_HELP;
			} else if (!strncmp(pStr,"version",strlen("version"))){
				sMsg.mtype=1;
				sMsg.identify=CMD_VERSION;
			}  else if (!strncmp(pStr,"hello",strlen("hello"))){
				sMsg.mtype=1;
				sMsg.identify=CMD_HELLO;
			} else if (!strncmp(pStr,"pass",strlen("pass"))){
				sMsg.mtype=1;
				sMsg.identify=CMD_PASS;
			} else if (!strncmp(pStr,"ident",strlen("ident"))){
				sMsg.mtype=1;
				sMsg.identify=CMD_IDENT;
			} else if (!strncmp(pStr,"addchannel",strlen("addchannel"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_ADDCHANNEL;
			} else if (!strncmp(pStr,"rmchannel",strlen("rmchannel"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_RMCHANNEL;
			} else if (!strncmp(pStr,"join",strlen("join"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_JOIN;
			} else if (!strncmp(pStr,"part",strlen("part"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_PART;
			} else if (!strncmp(pStr,"logoff",strlen("logoff"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_LOGOFF;
			} else if (!strncmp(pStr,"die",strlen("die"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_DIE;
			} else if (!strncmp(pStr,"nick",strlen("nick"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_NICK;
			} else if (!strncmp(pStr,"chanlist",strlen("chanlist"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_CHANNELS;
			} else if (!strncmp(pStr,"greating",strlen("greating"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_SET_GREATING;
			} else if (!strncmp(pStr,"viewgreat",strlen("viewgreat"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_VIEWGREAT;
			} else if (!strncmp(pStr,"topic",strlen("topic"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_SET_TOPIC;
			} else if (!strncmp(pStr,"say",strlen("say"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_SAY;
			} else if (!strncmp(pStr,"kick",strlen("kick"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_KICK;
			} else if (!strncmp(pStr,"usermode",strlen("usermode"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_USERMODE;
			} else if (!strncmp(pStr,"rmuser",strlen("rmuser"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_RMUSER;
			} else if (!strncmp(pStr,"userlist",strlen("userlist"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_USERLIST;
			} else if (!strncmp(pStr,"allsay",strlen("allsay"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_ALLSAY;
			} else if (!strncmp(pStr,"chanmode",strlen("chanmode"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_CHANMODE;
			} else if (!strncmp(pStr,"restart",strlen("restart"))) {
				sMsg.mtype=1;
				sMsg.identify=CMD_RESTART;
			}
		}
	}

	strcpy(sMsg.pMsgLine,pLine);
	return sMsg;
}

void *ComandExecutionThread(void *argv) {
	int msgid;
	extern int key;
	MsgBufType sMsg;

	// set the thread cancelable 
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	
	// open the message queue 
	msgid=msgget(key,0600 | IPC_EXCL );

	// execute loop 
	while(1) {
		msgrcv(msgid,&sMsg,sizeof(MsgBufType)-sizeof(sMsg.mtype),0,0);

		if (AccessRight(sMsg.pMsgLine,sMsg.identify)) {
		
			switch (sMsg.identify) {
			case CMD_ONPING:
				pong();
				break;
			case CMD_ONQUIT:
			case CMD_LOGOFF:
				logoff(sMsg.pMsgLine);
				break;
			case CMD_HELP:
				help(sMsg.pMsgLine);
				break;
			case CMD_VERSION:
				version(sMsg.pMsgLine);
				break;
			case CMD_HELLO:
				hello(sMsg.pMsgLine);
				break;
			case CMD_PASS:
				password(sMsg.pMsgLine);
				break;
			case CMD_IDENT:
				ident(sMsg.pMsgLine);
				break;
			case CMD_ADDCHANNEL:
				addChannel(sMsg.pMsgLine);
				break;
			case CMD_RMCHANNEL:
				rmChannel(sMsg.pMsgLine);
				break;
			case CMD_JOIN:
				joinChannel(sMsg.pMsgLine);
				break;
			case CMD_PART:
				partChannel(sMsg.pMsgLine);
				break;
			case CMD_DIE:
				die(sMsg.pMsgLine);
				break;
			case CMD_NICK:
				setNick(sMsg.pMsgLine);
				break;
			case CMD_CHANNELS:
				chanlist(sMsg.pMsgLine);
				break;
			case CMD_ONNAMES:
				hBotNeedOp(sMsg.pMsgLine);
				break;
			case CMD_ONJOIN:
				hSetModUser(sMsg.pMsgLine);
            case CMD_VIEWGREAT:    
				greating(sMsg.pMsgLine);
				break;
			case CMD_SET_GREATING:
				setGreating(sMsg.pMsgLine);
				break;
			case CMD_SET_TOPIC:
				setTopic(sMsg.pMsgLine);
				break;
			case CMD_SAY:
				say(sMsg.pMsgLine);
				break;
			case CMD_KICK:
				kickuser(sMsg.pMsgLine);
				break;
			case CMD_USERMODE:
				usermode(sMsg.pMsgLine);
				break;
			case CMD_ONNICKCHG:
				hNickChange(sMsg.pMsgLine);
				break;
			case CMD_RMUSER:
				rmuser(sMsg.pMsgLine);
				break;
			case CMD_USERLIST:
				userlist(sMsg.pMsgLine);
				break;
			case CMD_ONMODE:
				hResetModes(sMsg.pMsgLine);
				break;
			case CMD_ALLSAY:
				allsay(sMsg.pMsgLine);
				break;
			case CMD_ONTOPIC:
				hResetTopic(sMsg.pMsgLine);
				break;
			case CMD_CHANMODE:
				chanmode(sMsg.pMsgLine);
				break;
			case CMD_RESTART:
				restart(sMsg.pMsgLine);
				break;
			default:
				syslog(LOG_CRIT,SYSLOG_UNKNOWN_CMDID,sMsg.identify);
				break;
			}
		}
		
		// clear buffer	
		bzero(&sMsg,sizeof(MsgBufType));
	} 
	
	return NULL;
}

// ############################################################################# 
static int AccessRight(char *pLine,CmdType cmd_id) {
	char *pChannel;
	char *pLogin;
	char *pNick;
	char *pNetmask;
	char *pKey;
	char *pMod;

	pNetmask=getNetmask(pLine);
	pNick=getNickname(pLine);

	switch (cmd_id) {
	// handlers
	case CMD_ONPING:
	case CMD_ONNAMES:
	case CMD_ONJOIN:
	case CMD_ONMODE:
	case CMD_ONTOPIC:
	// any users
	case CMD_HELP:
	case CMD_HELLO:
	case CMD_VERSION:
	case CMD_IDENT:
	case CMD_VIEWGREAT:
		return true;
	// logged in user
	case CMD_ONNICKCHG:
	case CMD_ONQUIT:
	case CMD_LOGOFF:
	case CMD_PASS:
		if (!exist_db(NICKTOUSER_DB,pNetmask)) {
			notice(pNick,MSG_NOT_LOGON);	
			return false;
		} else {
			return true;
		}
	// owner
	case CMD_SET_GREATING:
	case CMD_SET_TOPIC:
	case CMD_USERLIST:
	case CMD_SAY:
	case CMD_KICK:
	case CMD_USERMODE:
	case CMD_CHANMODE:
		if (!exist_db(NICKTOUSER_DB,pNetmask)) {
			notice(pNick,MSG_NOT_LOGON);	
			return false;
		}

		pLogin=get_db(NICKTOUSER_DB,pNetmask);
		pChannel=getAccessChannel(pLine);

		if (!strlen(pChannel)) {
			notice(pNick,MSG_NOT_CHANNELOPT);
			return false;
		}

		pKey=(char*)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char*));
		sprintf(pKey,"%s%s",pLogin,pChannel);

		pMod=get_db(ACCESS_DB,pKey);
		
		if (strchr(pMod,'o')) {
				return true;
		}

		notice(pNick,MSG_NOT_OWNER);
		
	// master with necassary channel
	case CMD_JOIN:
	case CMD_PART:
	case CMD_RMCHANNEL:
	case CMD_ADDCHANNEL:
	case CMD_ALLSAY:
	case CMD_RMUSER:
		pChannel=getAccessChannel(pLine);

		if (!strlen(pChannel)) {
			notice(pNick,MSG_NOT_CHANNELOPT);
			return false;
		}
	// master without necassary channel
	case CMD_NICK:
	case CMD_CHANNELS:
	case CMD_DIE:
	case CMD_RESTART:
		if (!exist_db(NICKTOUSER_DB,pNetmask)) {
			notice(pNick,MSG_NOT_LOGON);	
			return false;
		}



		pLogin=get_db(NICKTOUSER_DB,pNetmask);

		if (!exist_db(ACCESS_DB,pLogin)) {
			notice(pNick,MSG_NOT_MASTER);  
		} else {
			notice(pNick,MSG_MASTER);  
			return true;
		}
	default:
		notice(pNick,MSG_NOT_ACCESS);
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
