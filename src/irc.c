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
#include <pwd.h>

#include "utilities.h"
#include "messages.h"
#include "network.h"
#include "dbaccess.h"
#include "irc.h"






// ############################################################################# 
void user(void) {
	extern ConfType setup;
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
void action(char *target, char *text){
	char *buffer;
	buffer=(char *)malloc((strlen("ACTION ")+strlen(text)+1)*sizeof(char));
	sprintf(buffer,"ACTION %s",text);
	privmsg(target,buffer);
}
// ############################################################################# 
void privmsg(char *target, char *text){
	char *buffer;
	buffer=(char *)calloc(strlen("PRIVMSG ")+strlen(target)+strlen(text)+5,sizeof(char));
	sprintf(buffer,"PRIVMSG %s :%s\r\n",target,text);
	send_line(buffer);
}
// ############################################################################# 
void notice(char *nick,char *text) {
	char *buffer;
	buffer=(char *)calloc(strlen("NOTICE ")+strlen(nick)+strlen(text)+5,sizeof(char));
	sprintf(buffer,"NOTICE %s :%s\r\n",nick,text);
	send_line(buffer);
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
	send_line(buffer);
}
// ############################################################################# 
void part(char *channel) {
	char *buffer;
	buffer=(char *)calloc(strlen("PART ")+strlen(channel)+3,sizeof(char));
	sprintf(buffer,"PART %s\r\n",channel);
	send_line(buffer);
}
// ############################################################################# 
void pong(void) {
	char *buffer,hostname[HOSTNAME_BUFFER_SIZE];
    
	// get  hostname
	gethostname(hostname,HOSTNAME_BUFFER_SIZE);

	buffer=(char *)calloc(strlen("PONG ")+strlen(hostname)+3,sizeof(char));
	sprintf(buffer,"PONG %s\r\n",hostname);
	send_line(buffer);
}
// ############################################################################# 
void ping(char *target) {
	char *buffer;
	buffer=(char *)calloc(strlen("PING ")+strlen(target)+3,sizeof(char));
	sprintf(buffer,"PING %s\r\n",target);
	send_line(buffer);
}
// ############################################################################# 
void nick(char *nick) {
	char *buffer;
	buffer=(char *)calloc(strlen("NICK ")+strlen(nick)+3,sizeof(char));
	sprintf(buffer,"NICK %s\r\n",nick);
	send_line(buffer);
}
// ############################################################################# 
void topic(char *channel, char* text) {
	char *buffer;
	buffer=(char*)calloc(strlen("TOPIC ")+strlen(channel)+strlen(text)+4,sizeof(char));
	sprintf(buffer,"TOPIC %s %s\r\n",channel,text);
	send_line(buffer);
}
// ############################################################################# 
void kick(char *channel, char *nick, char *reason) {
	char *buffer;

	// check  optional parameters and  set  it of default values
	if (reason==NULL) {
		reason=(char*)malloc(sizeof(char));
		*reason='\0';
	}
	buffer=(char*)calloc(strlen("KICK ")+strlen(channel)+strlen(nick)+strlen(reason)+5,sizeof(char));
	sprintf(buffer,"KICK %s %s %s\r\n",channel,nick,reason);
	send_line(buffer);
}
// ############################################################################# 
void ban(char *channel,char *mask){
	char *buffer;
	buffer=(char*)calloc(strlen("MODE ")+strlen(channel)+strlen(mask)+6,sizeof(char));
	sprintf(buffer,"MODE %s +b %s\r\n",channel,mask);
	send_line(buffer);

}
// ############################################################################# 
void deban(char *channel,char *mask){
	char *buffer;
	buffer=(char*)calloc(strlen("MODE ")+strlen(channel)+strlen(mask)+6,sizeof(char));
	sprintf(buffer,"MODE %s -b %s\r\n",channel,mask);
	send_line(buffer);

}
// ############################################################################# 
void mode(char *channel, char *mods, char *modsparam) {
	char *buffer;

	// check  optional parameters and  set  it of default values
	if (modsparam==NULL) {
		modsparam=(char*)malloc(sizeof(char));
		*modsparam='\0';
	}
	buffer=(char*)calloc(strlen("MODE ")+strlen(channel)+strlen(mods)+strlen(modsparam)+5,sizeof(char));
	sprintf(buffer,"TOPIC %s %s %s\r\n",channel,mods,modsparam);
	send_line(buffer);
}





