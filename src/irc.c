#include "macro.h"
#include "messages.h"
#include "config.h"
#include "network.h"
#include "dbaccess.h"
#include "command.h"
#include "irc.h"





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
			tmp=malloc(sizeof(char)*(strlen(setup.botname)+2));
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
	buffer_size=strlen("USER")+strlen(pw->pw_name)+strlen(hostname)+strlen(setup.server)+strlen(setup.realname)+7;
    buffer=malloc(sizeof(char)*buffer_size);
	
	// create the  commando string
	sprintf(buffer,"USER %s %s %s :%s\n",pw->pw_name,hostname,setup.server,setup.realname);

	// send commando
	send_line(buffer);
	free(buffer);
}

void notice(char *nick,char *text) {
	char *buffer=malloc(sizeof(char)*(strlen("NOTICE ")+strlen(nick)+strlen(text)+2));
	
	sprintf(buffer,"NOTICE %s :%s\n",nick,text);

	// send commando
	send_line(buffer);
	free(buffer);
}

void version(char *line) {
    char *str=malloc(sizeof(char)*(strlen(PROGNAME)+strlen("Version ")+strlen(VERSION)+3));
			
	// creat Versions String
	sprintf(str,"%s Version	%s\n",PROGNAME,VERSION);
	notice(getNickname(line),str);
}

void join(char *channel) {
	char *buffer=malloc(sizeof(char)*(strlen("JOIN ")+strlen(channel)+2));
	
	sprintf(buffer,"JOIN %s \n",channel);
	
	// send commando
	send_line(buffer);
    free(buffer);
}

void part(char *channel) {
	char *buffer=malloc(sizeof(char)*(strlen("PART ")+strlen(channel)+2));

	sprintf(buffer,"PART %s \n",channel);

	// send commando
	send_line(buffer);
	free(buffer);
}

void pong(void) {
	char *buffer,hostname[HOSTNAME_BUFFER_SIZE];
    
	// get  hostname
	gethostname(hostname,HOSTNAME_BUFFER_SIZE);


	// create commando string
	buffer=malloc(sizeof(char)*(strlen("PONG ")+strlen(hostname)+2));
	
	sprintf(buffer,"PONG %s\n",hostname);


	// send commando
	send_line(buffer);
	free(buffer);
}


void nick(char *nick) {
	char *buffer;
	buffer=malloc(sizeof(char)*(strlen("NICK ")+strlen(nick)+2));

	sprintf(buffer,"NICK %s\n",nick);

	send_line(buffer);
	free(buffer);
}






