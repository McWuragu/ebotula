/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 * ############################################################# 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
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



/* ############################################################################# */
void user(void) {
    extern ConfigSetup_t sSetup;
    struct passwd *pw;
    char hostname[HOSTNAME_BUFFER_SIZE];
    char *buffer;
    int buffer_size;

    /* get the username  and hostname */
    pw = getpwuid(geteuid());
    gethostname(hostname,HOSTNAME_BUFFER_SIZE);

    /* calculat the command size */
    buffer_size=strlen("USER")+strlen(pw->pw_name)+strlen(hostname)+strlen(sSetup.sHostname)+strlen(sSetup.realname)+8;

	if ((buffer=(char *)malloc(sizeof(char)*buffer_size))==NULL)
   	{
	   logger(LOG_ERR,_("Couldn't allocate memory!"));
	   return;
   	}	   
    /* create the  commando string */
    snprintf(buffer,buffer_size,"USER %s %s %s :%s\r\n",pw->pw_name,hostname,sSetup.sHostname,sSetup.realname);

    /* send commando */
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void action(char *pTarget, char *pMsgStr){
    int nStrSize;
    char *buffer;

	// check if message is valid
	if ( NULL == pTarget)
	{
	   logger(LOG_ERR,_("Invalid target. Null pointer."));
	   return;
	}

	// check if message is valid
	if ( NULL == pMsgStr)
	{
	   logger(LOG_ERR,_("Invalid message. Null pointer."));
	   return;
	}

    nStrSize=(strlen("ACTION ")+strlen(pMsgStr)+3)*sizeof(char);

    if ((buffer=(char *)malloc(nStrSize))==NULL)
	{
	   logger(LOG_ERR,_("Couldn't allocate memory!"));
	   return;
    }	   

    buffer[0]=1;
    snprintf(&buffer[1],nStrSize-2,"ACTION %s",pMsgStr);
    buffer[nStrSize-2]=1;
    buffer[nStrSize-1]='\0';

    privmsg(pTarget,buffer);
    free(buffer);
}
/* ############################################################################# */
void privmsg(char *pTarget, char *pMsgStr){
    char *buffer;
   	size_t buffer_size; 
	// check if target or message is valid
	if ( NULL == pTarget)
	{
	   logger(LOG_ERR,_("Invalid target. Null pointer."));
	   return;
	}
   	if ( NULL == pMsgStr)
	{
	   logger(LOG_ERR,_("Invalid msg str. Null pointer."));
	   return;
	}

	buffer_size = (strlen("PRIVMSG ")+strlen(pTarget)+strlen(pMsgStr)+5)*sizeof(char);
	if ((buffer=(char *)malloc(buffer_size))==NULL)
	{
	   logger(LOG_ERR,_("Couldn't allocate memory!"));
	   return;
    }	   

    snprintf(buffer,buffer_size,"PRIVMSG %s :%s\r\n",pTarget,pMsgStr);
    SendLine(buffer);
    free(buffer);
}
/* ############################################################################# */
void notice(char * pNick,char *pMsgStr) {
    char *buffer;
   	size_t buffer_size; 

	// check if nickname or message is valid
	if ( NULL == pNick)
	{
	   logger(LOG_ERR,_("Invalid nick name. Null pointer."));
	   return;
	}
   	if ( NULL == pMsgStr)
	{
	   logger(LOG_ERR,_("Invalid msg str. Null pointer."));
	   return;
	}
    
	buffer_size = (strlen("NOTICE ")+strlen(pNick)+strlen(pMsgStr)+5)*sizeof(char);	
	if ((buffer=(char *)malloc(buffer_size))==NULL)
	{
	   logger(LOG_ERR,_("Couldn't allocate memory!"));
	   return;
    }	   

    snprintf(buffer,buffer_size,"NOTICE %s :%s\r\n",pNick,pMsgStr);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void sendMsg(AnswerMode_t AnswerMode,char * pNick,char * pMsgStr,...){
    va_list arg;
    char pMsg[1<<10];

    va_start(arg,pMsgStr);
    vsprintf(pMsg,pMsgStr,arg);
    

    if (AnswerMode==NoticeMode) {
        notice(pNick,pMsg);
    } else if (AnswerMode==PrvMsgMode){
        privmsg(pNick,pMsg);
    }
    va_end(arg);
}
/* ############################################################################# */
/*void quit(void) {
    SendLine("QUIT\r\n");
} */
/* ############################################################################# */
void join(char *pChannel) {
    char *buffer;
   	size_t buffer_size; 

	// check if channel is valid
	if ( NULL == pChannel)
	{
	   logger(LOG_ERR,_("Invalid channel. Null pointer."));
	   return;
	}
	
	buffer_size = (strlen("JOIN ")+strlen(pChannel)+3)*sizeof(char);
	if ((buffer=(char *)malloc(buffer_size))==NULL)
	{
	   logger(LOG_ERR,_("Couldn't allocate memory!"));
	   return;
    }	   

    snprintf(buffer,buffer_size,"JOIN %s\r\n",pChannel);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void part(char *pChannel) {
    char *buffer;
   	size_t buffer_size; 

   	// check if channel is valid
	if ( NULL == pChannel)
	{
	   logger(LOG_ERR,_("Invalid channel. Null pointer."));
	   return;
	} 
	
	buffer_size = (strlen("PART ")+strlen(pChannel)+3)*sizeof(char);
	if ((buffer=(char *)malloc(buffer_size))==NULL)
	{
	   logger(LOG_ERR,_("Couldn't allocate memory!"));
	   return;
    }	   

    snprintf(buffer,buffer_size,"PART %s\r\n",pChannel);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void pong(char *pPong) {
    char *buffer;
	char hostname[HOSTNAME_BUFFER_SIZE];
   	size_t buffer_size; 

/*    gethostname(hostname,HOSTNAME_BUFFER_SIZE); */
	if (pPong == NULL)
	{
		/* I have no parameter for pong */
		buffer_size = (strlen("PONG ")+strlen(hostname)+3)*sizeof(char);
	    gethostname(hostname,HOSTNAME_BUFFER_SIZE);
		if ((buffer=(char *)malloc(buffer_size))==NULL)
		{
		   	logger(LOG_ERR,_("Couldn't allocate memory!"));
		 	return;
    	}	   

		snprintf(buffer,buffer_size,"PONG %s\r\n",hostname);
	}else
	{
		buffer_size = (strlen("PONG ")+strlen(pPong)+3)*sizeof(char);
		if ((buffer=(char *)malloc(buffer_size))==NULL)
		{
	   		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   		return;
    	}	   

	    snprintf(buffer,buffer_size,"PONG %s\r\n",pPong);
	}
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void ping(char *pTarget) {
    char *buffer;
   	size_t buffer_size; 

	// check if channel is valid
	if ( NULL == pTarget)
	{
	   logger(LOG_ERR,_("Invalid target. Null pointer."));
	   return;
	}    
	
	buffer_size = (strlen("PING ")+strlen(pTarget)+3)*sizeof(char);
	if ((buffer=(char *)malloc(buffer_size))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    snprintf(buffer,buffer_size,"PING %s\r\n",pTarget);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void invite(char *pChannel,char *pNick) {
    char *buffer;
    int aloc=0;
   	size_t buffer_size; 

	// check if nickname valid
	if ( NULL == pNick)
	{
	   logger(LOG_ERR,_("Invalid nick name. Null pointer."));
	   return;
	}    
	
	if (!pChannel)
    {
	    aloc=1;
	    pChannel=malloc(sizeof(char));
	    strcpy(pChannel,"");
    }		    

	buffer_size = (strlen("INVITE")+strlen(pNick)+strlen(pChannel)+5)*sizeof(char);
    if ((buffer=(char*)malloc(buffer_size))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    snprintf(buffer,buffer_size,"INVITE %s %s\r\n",pNick,pChannel);
    SendLine(buffer);
    free(buffer);
    if (aloc)
	    free(pChannel);
}
/* ############################################################################# */
void nick(char *pNick) {
    char *buffer;
	size_t buffer_size;

	// check if nickname is valid
	if ( NULL == pNick)
	{
	   logger(LOG_ERR,_("Invalid nick name. Null pointer."));
	   return;
	}

	buffer_size = (strlen("NICK ")+strlen(pNick)+3)*sizeof(char);
	if ((buffer=(char *)malloc(buffer_size))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    snprintf(buffer,buffer_size,"NICK %s\r\n",pNick);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void topic(char *pChannel, char* pMsgStr) {
    char *buffer;
   	size_t buffer_size;

	// check if channel or message is valid
	if ( NULL == pChannel)
	{
	   logger(LOG_ERR,_("Invalid channel. Null pointer."));
	   return;
	}
	if ( NULL == pMsgStr)
	{
	   logger(LOG_ERR,_("Invalid message. Null pointer."));
	   return;
	}
	
	buffer_size = (strlen("TOPIC ")+strlen(pChannel)+strlen(pMsgStr)+5)*sizeof(char);
	if ((buffer=(char*)malloc(buffer_size))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    snprintf(buffer,buffer_size,"TOPIC %s :%s\r\n",pChannel,pMsgStr);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void kick(char *pChannel, char *pNick, char *pMsgStr) {
    char *buffer;

	 // check if channel or message is valid
	if ( NULL == pChannel)
	{
	   logger(LOG_ERR,_("Invalid channel. Null pointer."));
	   return;
	}
	if ( NULL == pNick)
	{
	   logger(LOG_ERR,_("Invalid nick name. Null pointer."));
	   return;
	}
	if ( NULL == pMsgStr)
	{
	   logger(LOG_ERR,_("Invalid message. Null pointer."));
	   return;
	}
	
	if ((buffer=(char*)malloc((strlen("KICK ")+strlen(pChannel)+strlen(pNick)+strlen(pMsgStr)+6)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"KICK %s %s :%s\r\n",pChannel,pNick,pMsgStr);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void ban(char *pChannel,char *pMask){
    char *buffer;
    
	// check if channel or mask is valid
	if ( NULL == pChannel)
	{
	   logger(LOG_ERR,_("Invalid channel. Null pointer."));
	   return;
	}
	if ( NULL == pMask)
	{
	   logger(LOG_ERR,_("Invalid mask. Null pointer."));
	   return;
	}

	if ((buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMask)+6)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"MODE %s +b %s\r\n",pChannel,pMask);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void deban(char *pChannel,char *pMask){
    char *buffer;
   	// check if channel or mask is valid
	if ( NULL == pChannel)
	{
	   logger(LOG_ERR,_("Invalid channel. Null pointer."));
	   return;
	}
	if ( NULL == pMask)
	{
	   logger(LOG_ERR,_("Invalid mask. Null pointer."));
	   return;
	} 
	
	if ((buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMask)+6)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"MODE %s -b %s\r\n",pChannel,pMask);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void mode(char *pChannel, char *pMod, char *pModParameter) {
    char *buffer;

	// check if channel or mask is valid
	if ( NULL == pChannel)
	{
	   logger(LOG_ERR,_("Invalid channel. Null pointer."));
	   return;
	}
	if ( NULL == pMod)
	{
	   logger(LOG_ERR,_("Invalid mod. Null pointer."));
	   return;
	}    
	
	/* check  optional parameters and  set  it of default values */
    if (pModParameter==NULL) {
        pModParameter="";
    }
    if ((buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMod)+strlen(pModParameter)+5)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"MODE %s %s %s\r\n",pChannel,pMod,pModParameter);
    SendLine(buffer);
    free (buffer);
}
/* ############################################################################# */
void whois(char *pNickname) {
    char *buffer;
   	// check if channel or mask is valid
	if ( NULL == pNickname)
	{
	   logger(LOG_ERR,_("Invalid nick name. Null pointer."));
	   return;
	} 
	
	if ((buffer=(char*)malloc((strlen("WHOIS ")+strlen(pNickname)+3)*sizeof(char)))==NULL)
	{
		logger(LOG_ERR,_("Couldn't allocate memory!"));
	   	return;
    }	   

    sprintf(buffer,"WHOIS %s\r\n",pNickname);
    SendLine(buffer);
    free (buffer);
}




