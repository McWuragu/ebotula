/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "utilities.h"
#include "messages.h"
#include "network.h"
#include "dbaccess.h"
#include "irc.h"



// #############################################################################
void user(void) {
    extern ConfigSetup_t sSetup;
    struct passwd *pw;
    char hostname[HOSTNAME_BUFFER_SIZE];
    char *buffer;
    int buffer_size;

    // get the username  and hostname
    pw = getpwuid(geteuid());
    gethostname(hostname,HOSTNAME_BUFFER_SIZE);

    // calculat the command size
    buffer_size=strlen("USER")+strlen(pw->pw_name)+strlen(hostname)+strlen(sSetup.sHostname)+strlen(sSetup.realname)+8;

	if ((buffer=(char *)malloc(sizeof(char)*buffer_size))==NULL)
   	{
	   logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   return;
   	}	   
    // create the  commando string
    sprintf(buffer,"USER %s %s %s :%s\r\n",pw->pw_name,hostname,sSetup.sHostname,sSetup.realname);

    // send commando
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void action(char *pTarget, char *pMsgStr){
    int nStrSize;
    char *buffer;

    nStrSize=strlen("ACTION ")+strlen(pMsgStr)+3;

    if ((buffer=(char *)malloc(nStrSize*sizeof(char)))==NULL)
	{
	   logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   return;
    }	   

    buffer[0]=1;
    sprintf(&buffer[1],"ACTION %s",pMsgStr);
    buffer[nStrSize-2]=1;
    buffer[nStrSize-1]='\0';

    privmsg(pTarget,buffer);
    free(buffer);
}
// #############################################################################
void privmsg(char *pTarget, char *pMsgStr){
    char *buffer;
    if ((buffer=(char *)malloc((strlen("PRIVMSG ")+strlen(pTarget)+strlen(pMsgStr)+5)*sizeof(char)))==NULL)
	{
	   logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   return;
    }	   

    sprintf(buffer,"PRIVMSG %s :%s\r\n",pTarget,pMsgStr);
    SendLine(buffer);
    free(buffer);
}
// #############################################################################
void notice(char * pNick,char *pMsgStr) {
    char *buffer;
    if ((buffer=(char *)malloc((strlen("NOTICE ")+strlen(pNick)+strlen(pMsgStr)+5)*sizeof(char)))==NULL)
	{
	   logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   return;
    }	   

    sprintf(buffer,"NOTICE %s :%s\r\n",pNick,pMsgStr);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void sendMsg(AnswerMode_t AnswerMode,char * pNick,char * pMsgStr){
    if (AnswerMode==NoticeMode) {
        notice(pNick,pMsgStr);
    } else if (AnswerMode==PrvMsgMode){
        privmsg(pNick,pMsgStr);
    }
}
// #############################################################################
//void quit(void) {
//    SendLine("QUIT\r\n");
//}
// #############################################################################
void join(char *pChannel) {
    char *buffer;
    if ((buffer=(char *)malloc((strlen("JOIN ")+strlen(pChannel)+3)*sizeof(char)))==NULL)
	{
	   logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   return;
    }	   

    sprintf(buffer,"JOIN %s\r\n",pChannel);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void part(char *pChannel) {
    char *buffer;
    if ((buffer=(char *)malloc((strlen("PART ")+strlen(pChannel)+3)*sizeof(char)))==NULL)
	{
	   logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   return;
    }	   

    sprintf(buffer,"PART %s\r\n",pChannel);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void pong(char *pPong) {
    char *buffer,hostname[HOSTNAME_BUFFER_SIZE];

//    gethostname(hostname,HOSTNAME_BUFFER_SIZE);
	if (pPong==NULL)
	{
		/* I have no parameter for pong */
	    gethostname(hostname,HOSTNAME_BUFFER_SIZE);
		if ((buffer=(char *)malloc((strlen("PONG ")+strlen(hostname)+3)*sizeof(char)))==NULL)
		{
		   	logger(LOG_ERR,gettext("Couldn't allocate memory!"));
		 	return;
    	}	   

		sprintf(buffer,"PONG %s\r\n",hostname);
	}else
	{
		if ((buffer=(char *)malloc((strlen("PONG ")+strlen(pPong)+3)*sizeof(char)))==NULL)
		{
	   		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   		return;
    	}	   

	    sprintf(buffer,"PONG %s\r\n",pPong);
	}
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void ping(char *pTarget) {
    char *buffer;
    if ((buffer=(char *)malloc((strlen("PING ")+strlen(pTarget)+3)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"PING %s\r\n",pTarget);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void invite(char *pChannel,char *pNick) {
    char *buffer;
    if ((buffer=(char*)malloc((strlen("INVITE")+strlen(pNick)+strlen(pChannel)+4)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"INVITE %s %s\r\n",pNick,pChannel);
    SendLine(buffer);
    free(buffer);
}
// #############################################################################
void nick(char *pNick) {
    char *buffer;
    if ((buffer=(char *)malloc((strlen("NICK ")+strlen(pNick)+3)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"NICK %s\r\n",pNick);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void topic(char *pChannel, char* pMsgStr) {
    char *buffer;
    if ((buffer=(char*)malloc((strlen("TOPIC ")+strlen(pChannel)+strlen(pMsgStr)+5)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"TOPIC %s :%s\r\n",pChannel,pMsgStr);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void kick(char *pChannel, char *pNick, char *pMsgStr) {
    char *buffer;

    // check  optional parameters and  set  it of default values
    if (pMsgStr==NULL) {
        pMsgStr=getMsgString(INFO_DEFAULT_REASON);;
    }
    if ((buffer=(char*)malloc((strlen("KICK ")+strlen(pChannel)+strlen(pNick)+strlen(pMsgStr)+6)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"KICK %s %s :%s\r\n",pChannel,pNick,pMsgStr);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void ban(char *pChannel,char *pMask){
    char *buffer;
    if ((buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMask)+6)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"MODE %s +b %s\r\n",pChannel,pMask);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void deban(char *pChannel,char *pMask){
    char *buffer;
    if ((buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMask)+6)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"MODE %s -b %s\r\n",pChannel,pMask);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void mode(char *pChannel, char *pMod, char *pModParameter) {
    char *buffer;

    // check  optional parameters and  set  it of default values
    if (pModParameter==NULL) {
        pModParameter="";
    }
    if ((buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMod)+strlen(pModParameter)+5)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"MODE %s %s %s\r\n",pChannel,pMod,pModParameter);
    SendLine(buffer);
    free (buffer);
}
// #############################################################################
void whois(char *pNickname) {
    char *buffer;
    if ((buffer=(char*)malloc((strlen("WHOIS ")+strlen(pNickname)+3)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,gettext("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"WHOIS %s\r\n",pNickname);
    SendLine(buffer);
    free (buffer);
}




