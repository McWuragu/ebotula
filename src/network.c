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
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <syslog.h>

#include "ircbot.h"
#include "dbaccess.h"
#include "type.h"
#include "utilities.h"
#include "messages.h"
#include "irc.h"
#include "command.h"
#include "network.h"

static int sockid;

// ############################################################################# 
void connectServer(void) {
	extern ConfType sSetup;
	
	struct sockaddr_in socketaddr;
	struct hostent *hostaddr;
	struct protoent *protocol;

	errno=0;

	// init the  socketaddr
	bzero((char*) &socketaddr,sizeof(socketaddr));
	socketaddr.sin_family=AF_INET;
    socketaddr.sin_port=htons(atoi(sSetup.port));

	// resolve hostname
	hostaddr=gethostbyname(sSetup.server);
	if (!hostaddr) {
		perror(SYSLOG_RESOLVE_HOSTNAME);
		syslog(LOG_ERR,SYSLOG_RESOLVE_HOSTNAME);
		exit(errno);
	}
	DEBUG("Connecting to %s",hostaddr->h_name);
	
	memcpy(&socketaddr.sin_addr,hostaddr->h_addr,hostaddr->h_length);

	// resolve protocol
	protocol = getprotobyname("tcp");
	if (!protocol) {
		perror(SYSLOG_RESOLVE_PROTOCOL);
		syslog(LOG_CRIT,SYSLOG_RESOLVE_PROTOCOL);
		exit(errno);
	}

	// create the socket
	sockid=socket(PF_INET,SOCK_STREAM,protocol->p_proto);
	if (sockid <= 0) {
		perror(SYSLOG_SOCKET);
		syslog(LOG_CRIT,SYSLOG_SOCKET);
		exit(errno);
	}


	// connect to server
	if(connect(sockid,(struct sockaddr *)&socketaddr,sizeof(socketaddr))<0) {
		perror(SYSLOG_CONNECT);
		syslog(LOG_CRIT,SYSLOG_CONNECT);
		exit(errno);
	}
}
// ############################################################################# 
void disconnectServer(void){
	shutdown(sockid,0);
}
// ############################################################################# 
void  send_line(char *pLine) {
	extern ConfType sSetup;
	extern int stop;
	extern pthread_mutex_t send_mutex;


	pthread_mutex_lock(&send_mutex);

	// protect excess flood
	msleep(sSetup.sendDelay);
	
	if (!send(sockid,pLine,strlen(pLine),0)){
		syslog(LOG_CRIT,SYSLOG_SEND);
		stop=true;
	}

	DEBUG("Send(%d): %s",getpid(),pLine);

	pthread_mutex_unlock(&send_mutex);

	
}
// ############################################################################# 
void  recv_line(char *pLine,unsigned int len) {
	extern int stop;
	extern ConfType sSetup;
	struct pollfd  sPoll;
	int str_len=0;
	
	sPoll.fd=sockid;
	sPoll.events=POLLIN;
	
	if (poll(&sPoll,1,sSetup.iTimeout*1000)) {
		if (!(str_len=recv(sockid,pLine,len,0))){
			syslog(LOG_CRIT,SYSLOG_RECV);
			stop=true;
		}
	} else {
		stop=true;
		syslog(LOG_NOTICE,SYSLOG_TIMEOUT);
	}

	pLine[str_len]='\0';
	
}


// ############################################################################# 
void ConnectToIrc(void){
	char recv_buffer[RECV_BUFFER_SIZE], *tmp;
	int i,trying=0;
	extern ConfType sSetup;
	

	// send the  USER commado
	user();
    
	// Try to set the nickname
	// If the nickname are  using then try again with a leading underline.
    do {
		i=0;
		trying++;
		
		
		nick(sSetup.botname);
        recv_line(recv_buffer,RECV_BUFFER_SIZE);

		// check fpr  nickname alread in use
		// if he in use then put a leading underline on the front of the name 
		if (strstr(recv_buffer,"Nickname is already in use.")) {
			tmp=(char *)calloc(strlen(sSetup.botname)+2,sizeof(char));
			sprintf(tmp,"_%s",sSetup.botname);
			free(sSetup.botname);
			sSetup.botname=tmp;
			i=1;
		}
		
		// Try MAX_NICKS times to set
		if ( trying>MAX_NICKS) {
			errno=EAGAIN;
			perror(ERR_NICK);
			exit(errno);
		}
	} while (i==1);
}
// ############################################################################# 
void join_all_channels(void) {
	char **channelliste;
	unsigned int i;
	channelliste=list_db(CHANNEL_DB);
	
	// join_Channels
	for (i=0;channelliste[i]!=NULL;i++) {
		join(channelliste[i]);
	}
	
}
