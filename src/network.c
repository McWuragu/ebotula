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
#include "dbaccess.h"
#include "type.h"
#include "utilities.h"
#include "messages.h"
#include "irc.h"
#include "command.h"
#include "network.h"

int sockid;

void connectServer(void) {
	extern int sockid;
	extern ConfType setup;

	
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
	extern ConfType setup;
	extern int sockid;
	extern pthread_mutex_t send_mutex;


	pthread_mutex_lock(&send_mutex);

	// protect excess flood
	millisleep(setup.sendDelay);
	
	if (!send(sockid,line,strlen(line),0)){
		perror(ERR_SEND);
		exit(errno);
	}

	DEBUG("Send(%d): %s",getpid(),line);

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


// ############################################################################# 
void irc_connect(void){
	char recv_buffer[RECV_BUFFER_SIZE], *tmp;
	int i,trying=0;
	extern ConfType setup;
	

	// send the  USER commado
	user();
    
	// Try to set the nickname
	// If the nickname are  using then try again with a leading underline.
    do {
		i=0;
		trying++;
		
		
		nick(setup.botname);
        recv_line(recv_buffer,RECV_BUFFER_SIZE);

		// check fpr  nickname alread in use
		// if he in use then put a leading underline on the front of the name 
		if (strstr(recv_buffer,"Nickname is already in use.")) {
			tmp=(char *)calloc(strlen(setup.botname)+2,sizeof(char));
			sprintf(tmp,"_%s",setup.botname);
			free(setup.botname);
			setup.botname=tmp;
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

	if ((channelliste=list_db(CHANNEL_DB))) {
		for (i=0;channelliste[i]!=NULL;i++) {
			join(channelliste[i]);
		}
	}
	
}
