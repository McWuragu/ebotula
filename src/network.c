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
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>			 
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "config.h"
#include "utilities.h"
#include "messages.h"
#include "irc.h"
#include "command.h"
#include "network.h"

int sockid;

void connectServer(void) {
	extern int sockid;
	extern CONFIG_TYPE setup;

	
	struct sockaddr_in socketaddr;
	struct hostent *hostaddr;
	struct protoent *protocol;

	errno=0;

	// init the  socketaddr
	bzero((char*) &socketaddr,sizeof(socketaddr));
	socketaddr.sin_family=AF_INET;
    socketaddr.sin_port=htons(atoi(setup.port));

	// resolve hostname
	hostaddr=gethostbyname(setup.server);
	if (!hostaddr) {
		perror(ERR_RESOLVE_HOSTNAME);
		exit(errno);
	}
	DEBUG("Connecting to %s",hostaddr->h_name);
	
	memcpy(&socketaddr.sin_addr,hostaddr->h_addr,hostaddr->h_length);

	// resolve protocol
	protocol = getprotobyname("tcp");
	if (!protocol) {
		perror(ERR_RESOLVE_PROTOCOL);
		exit(errno);
	}

	// create the socket
	sockid=socket(PF_INET,SOCK_STREAM,protocol->p_proto);
	if (sockid <= 0) {
		perror(ERR_SOCKET);
		exit(errno);
	}


	// connect to server
	if(connect(sockid,(struct sockaddr *)&socketaddr,sizeof(socketaddr))<0) {
		perror(ERR_CONNECT);
		exit(errno);
	}
}

void disconnectServer(void){
	shutdown(sockid,0);
}

void  send_line(char *line) {
	extern int sockid;
	extern pthread_mutex_t send_mutex;
	
	pthread_mutex_lock(&send_mutex);

	if (!send(sockid,line,strlen(line),0)){
		perror(ERR_SEND);
		exit(errno);
	}

	DEBUG("Send(%d): %s",getpid(),line);

	// protect excess flood
	millisleep(1500);

	pthread_mutex_unlock(&send_mutex);

	
}

void  recv_line(char *line,unsigned int len) {
	extern int sockid;
	int str_len;
	
	if (!(str_len=recv(sockid,line,len,0))){
		perror(ERR_RECV);
		exit(errno);
	}
	line[str_len]='\0';
	
}


struct MSGBUF_DS preParser(char *line) {
    struct MSGBUF_DS	msg;
	char *str,*first_part,*pos;

	// init the buffer with zero
	bzero(&msg,sizeof(struct MSGBUF_DS));
		
	// get the first part of the  answer from server
	if (!(first_part=getCommand(line))) {
		return msg;
	}

	if (!(pos=strchr(first_part,' '))){
		pos=NULL;
	}
	
	// preparse the line
	if (!strncmp(first_part,"PING",strlen("PING"))) {
		msg.mtype=2;
		msg.identify=CMD_PING;
	} else if (strstr(pos,"QUIT")) {
		msg.mtype=2;
		msg.identify=CMD_LOGOFF;
	} else if ((str=strstr(line," :!"))!=NULL) {

		str+=3;

		if (!strncmp(str,"help",strlen("help"))){
			msg.mtype=1;
			msg.identify=CMD_HELP;
		} else if (!strncmp(str,"version",strlen("version"))){
			msg.mtype=1;
			msg.identify=CMD_VERSION;
		} else if (!strncmp(str,"help",strlen("help"))){
			msg.mtype=1;
			msg.identify=CMD_HELP;
		} else if (!strncmp(str,"hello",strlen("hello"))){
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
		}
	}

	strcpy(msg.msg_line,line);
	return msg;
}

void *action_thread(void *argv) {
	int msgid;
	extern int key;
	struct MSGBUF_DS	msg;

	// set the thread cancelable 
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	
	// open the message queue 
	msgid=msgget(key,0600 | IPC_EXCL );

	// execute loop 
	while(1) {
		msgrcv(msgid,&msg,sizeof(struct MSGBUF_DS)-sizeof(msg.mtype),0,0);

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
			change_nick(msg.msg_line);
			break;
		case CMD_CHANNELS:
			channel_list(msg.msg_line);
			break;
		default:
            break;
		}
		
		// clear buffer	
		bzero(&msg,sizeof(struct MSGBUF_DS));
	}

	return NULL;
}
