#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>

#include "utilities.h"
#include "messages.h"
#include "config.h"
#include "network.h"
#include "dbaccess.h"
#include "command.h"
#include "irc.h"




// ############################################################################# 
void irc_connect(void){
	char recv_buffer[RECV_BUFFER_SIZE], *tmp;
	int i,trying=0;
	extern CONFIG_TYPE setup;
	

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
void user(void) {
	extern CONFIG_TYPE setup;
	struct passwd *pw;
	char hostname[HOSTNAME_BUFFER_SIZE];
	char *buffer;
	int buffer_size;

	// get the username	 and hostname
	pw = getpwuid(getuid());
	gethostname(hostname,HOSTNAME_BUFFER_SIZE);

	// calculat the command size
	buffer_size=strlen("USER")+strlen(pw->pw_name)+strlen(hostname)+strlen(setup.server)+strlen(setup.realname)+8;
    buffer=(char *)calloc(sizeof(char),buffer_size);
	
	// create the  commando string
	sprintf(buffer,"USER %s %s %s :%s\r\n",pw->pw_name,hostname,setup.server,setup.realname);

	// send commando
	send_line(buffer);
}
// ############################################################################# 
void notice(char *nick,char *text) {
	char *buffer;
	buffer=(char *)calloc(strlen("NOTICE ")+strlen(nick)+strlen(text)+3,sizeof(char));
	
	sprintf(buffer,"NOTICE %s :%s\r\n",nick,text);

	// send commando
	send_line(buffer);
}
// ############################################################################# 
void version(char *line) {
    char *str;
	str=(char *)calloc(strlen(PROGNAME)+strlen("Version ")+strlen(VERSION)+4,sizeof(char));
			
	// creat Versions String
	sprintf(str,"%s Version %s\r\n",PROGNAME,VERSION);
	notice(getNickname(line),str);
}
// ############################################################################# 
void quit(void) {
	send_line("QUIT\r\n");
}
// ############################################################################# 
void join(char *channel) {
	char *buffer;
	buffer=(char *)calloc(strlen("JOIN ")+strlen(channel)+3,sizeof(char));
	
	sprintf(buffer,"JOIN %s\r\n",channel);
	
	// send commando
	send_line(buffer);
}
// ############################################################################# 
void part(char *channel) {
	char *buffer;
	buffer=(char *)calloc(strlen("PART ")+strlen(channel)+3,sizeof(char));

	sprintf(buffer,"PART %s\r\n",channel);

	// send commando
	send_line(buffer);
}
// ############################################################################# 
void pong(void) {
	char *buffer,hostname[HOSTNAME_BUFFER_SIZE];
    
	// get  hostname
	gethostname(hostname,HOSTNAME_BUFFER_SIZE);


	// create commando string
	buffer=(char *)calloc(strlen("PONG ")+strlen(hostname)+3,sizeof(char));
	
	sprintf(buffer,"PONG %s\r\n",hostname);


	// send commando
	send_line(buffer);
}
// ############################################################################# 
void nick(char *nick) {
	char *buffer;
	buffer=(char *)calloc(strlen("NICK ")+strlen(nick)+3,sizeof(char));

	sprintf(buffer,"NICK %s\r\n",nick);

	send_line(buffer);
}






