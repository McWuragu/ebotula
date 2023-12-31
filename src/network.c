/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2023 Steffen Laube <Laube.Steffen@gmx.de>
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
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
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

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "queue.h"
#include "ircbot.h"
#include "dbaccess.h"
#include "type.h"
#include "utilities.h"
#include "messages.h"
#include "irc.h"
#include "command.h"
#include "network.h"

static int sockid;

static int nCharMinute[60];
static int nCharPerMinute=0;
static time_t lastTime=0;
static int nLastIndex=0;
static int nItemCount=1;
static int nFirstIndex=-1;

static pthread_mutex_t mutexSend;


static int GetCharPerMin(unsigned int nCharCount);
/* #############################################################################  */
boolean connectServer(void) {
    extern ConfigSetup_t sSetup;
    
    struct sockaddr_in socketaddr;
    struct hostent *hostaddr;
    struct protoent *protocol;

    errno=0;

    if ((sSetup.sHostname==NULL) && (sSetup.sPort==NULL)) {
        errno=EINVAL;
        logger(LOG_ERR,_("The hostname or portnumber isn't set."));
        return false;
    }
    logger(LOG_INFO,_("Try to connect to %s"),sSetup.sHostname);


    pthread_mutex_init(&mutexSend,NULL);

    /* init the  socketaddr */
    bzero((char*) &socketaddr,sizeof(socketaddr));
    socketaddr.sin_family=AF_INET;
    socketaddr.sin_port=htons(atoi(sSetup.sPort));

    /* resolve hostname */
    hostaddr=gethostbyname(sSetup.sHostname);
    if (!hostaddr) {
        logger(LOG_ERR,_("Couldn't resolve the hostname %s."),sSetup.sHostname);
        return false;
    }
    
    memcpy(&socketaddr.sin_addr,hostaddr->h_addr,hostaddr->h_length);

    /* resolve protocol */
    protocol = getprotobyname("tcp");
    if (!protocol) {
        logger(LOG_CRIT,_("Couldn't found the tcp protocol."));
        return false;
    }

    /* create the socket */
    sockid=socket(PF_INET,SOCK_STREAM,protocol->p_proto);
    if (sockid <= 0) {
        logger(LOG_CRIT,_("Couldn't create a tcp socket.- %s"),strerror(errno));
        return false;
    }


    /* connect to server */
    if(connect(sockid,(struct sockaddr *)&socketaddr,sizeof(socketaddr))<0) {
        logger(LOG_ERR,_("Couldn't connect to %s:%s - %s"),sSetup.sHostname,sSetup.sPort,strerror(errno));
        return false;
    }
    logger(LOG_NOTICE,_("The bot is connect to %s"),sSetup.sHostname);

    return true;
}
/* ############################################################################# */
void disconnectServer(void){
    shutdown(sockid,0);
    pthread_mutex_destroy(&mutexSend);
}
/* ############################################################################ */
void SendLine(char* pMsg){
    extern ConfigSetup_t sSetup;
    extern int stop;
    int nSendLength;

    if (pMsg) {
        pthread_mutex_lock(&mutexSend);
        /* protect excess flood */
        if (nCharPerMinute < sSetup.nFastSendingCharLimit) {
            msleep(sSetup.iSendDelay);
        } else {
            msleep(sSetup.nSlowSendDelay);
        }
                
        nSendLength=strlen(pMsg);

        /* send the line */
        if (!send(sockid,pMsg,nSendLength,0)){
            logger(LOG_CRIT,_("Couldn't send a command."));
	     stop=true;
        }
        logger(LOG_DEBUG,_("Send(%d/%d): \"%s\""),
			nCharPerMinute,
			sSetup.nFastSendingCharLimit,pMsg);
        
        nCharPerMinute=GetCharPerMin(nSendLength);
        pthread_mutex_unlock(&mutexSend);
    }
}

/* ############################################################################# */
void  RecvLine(char *pLine,unsigned int len) {
    extern int stop;
    extern ConfigSetup_t sSetup;
    struct pollfd  sPoll;
    int str_len=0;
    
    sPoll.fd=sockid;
    sPoll.events=POLLIN;
    
    if (poll(&sPoll,1,sSetup.iTimeout*1000)) {
        if (!(str_len=recv(sockid,pLine,len,0))){
            logger(LOG_CRIT,_("Can't receive a line."));
	    stop=true;
        }
    } else {
        stop=true;
        logger(LOG_ERR,_("Receiving timeout"));
    }

    pLine[str_len]='\0';
    
}

/* ############################################################################# */

int GetCharPerMin(unsigned int nCharCount) {
    time_t currTime;
    int diffMinTime=0;
    int i;
    int nSum=0;

    struct tm *formatCurrTime;

    if (time(&currTime)) {
        formatCurrTime=localtime(&currTime);

        /* the differnze from the last update */
        if (lastTime) {
            diffMinTime=(currTime-lastTime)/60;
        }
        lastTime=currTime;


        /* reset fieldes */
        if (diffMinTime < 60) {
            if (diffMinTime==0) {
                nCharCount+=nCharMinute[formatCurrTime->tm_min];
            } else if (nLastIndex > formatCurrTime->tm_min ) {
                for (i=nLastIndex;i < 60;i++) {nCharMinute[i]=0;}
                for (i=0; i<diffMinTime;i++) {nCharMinute[i]=0;}
            } else {
                for (i=(nLastIndex+1);i<formatCurrTime->tm_min;i++) {nCharMinute[i]=0;}
            }
        } else {
            for (i=0;i<60;i++) {nCharMinute[i]=0;}
        }

        /* index of the first item */
        nFirstIndex=(nFirstIndex<0)?formatCurrTime->tm_min:nFirstIndex;

        /* count  of the first  60 items */
        if (nItemCount<60 && nLastIndex!=formatCurrTime->tm_min) {
            if (formatCurrTime->tm_min >= nFirstIndex ) {
                nItemCount=(formatCurrTime->tm_min-nFirstIndex)+1;
            } else {
                nItemCount=60-nFirstIndex+formatCurrTime->tm_min+1;
            }
        }

        /* set the new value */
        nCharMinute[formatCurrTime->tm_min]=nCharCount;
        nLastIndex=formatCurrTime->tm_min;


        /* calculate the char per min */
        for (i=0;i<60;i++) {
            nSum+=nCharMinute[i];
        }
    }

    
    return (nSum/nItemCount);
}

/* ############################################################################# */
boolean ConnectToIrc(void){
    char recv_buffer[RECV_BUFFER_SIZE], *tmp;
    int i,trying=0;
    extern ConfigSetup_t sSetup;
    

    /* send the  USER commado */
    user();
    
    /* Try to set the nickname */
    /* If the nickname are  using then try again with a leading underline. */
    do {
        i=0;
        trying++;
        
        
        nick(sSetup.pBotname);
        RecvLine(recv_buffer,RECV_BUFFER_SIZE);

        /* check for  nickname alread in use */
        /* if he in use then put a leading underline on the front of the name */
        if (strstr(recv_buffer,"Nickname is already in use.")) {
            tmp=(char *)calloc(strlen(sSetup.pBotname)+2,sizeof(char));
            sprintf(tmp,"_%s",sSetup.pBotname);
            free(sSetup.pBotname);
            sSetup.pBotname=tmp;
            i=1;
        }
        
        /* Try MAX_NICKS times to set */
        if ( trying>MAX_NICKS) {
            errno=EAGAIN;
            logger(LOG_ERR,_("Couldn't set the nickname %s."),sSetup.pBotname);
            return false;
        }
    } while (i==1);
    return true;
}
/* ############################################################################# */
void * JoinAllChannelsThread(void * args) {
    int nSettling;
    PQueue pChannelQueue;
	QueueData *pChannel;
    pChannelQueue=list_db(CHANNEL_DB);
    
    if (args) {
        nSettling=*(int*)args;
    }  else {
        nSettling=10;
    }

    sleep(nSettling);

    /* join_Channels */
	while (isfullQueue(pChannelQueue)) {
        pChannel=popQueue(pChannelQueue);
        join((char*)pChannel->data);
        free(pChannel->data);
		free(pChannel);
	}
	deleteQueue(pChannelQueue);
	return NULL;
}
