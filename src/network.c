/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
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
#include <syslog.h>

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


static int GetCharPerMin(unsigned int nCharCount);
/* #############################################################################  */
void connectServer(void) {
    extern ConfigSetup_t sSetup;
    
    struct sockaddr_in socketaddr;
    struct hostent *hostaddr;
    struct protoent *protocol;

    errno=0;


    /* init the  socketaddr */
    bzero((char*) &socketaddr,sizeof(socketaddr));
    socketaddr.sin_family=AF_INET;
    socketaddr.sin_port=htons(atoi(sSetup.port));

    /* resolve hostname */
    hostaddr=gethostbyname(sSetup.server);
    if (!hostaddr) {
        perror(getSyslogString(SYSLOG_RESOLVE_HOSTNAME));
        syslog(LOG_ERR,getSyslogString(SYSLOG_RESOLVE_HOSTNAME));
        exit(errno);
    }
    DEBUG("Connecting to %s\n",hostaddr->h_name);
    
    memcpy(&socketaddr.sin_addr,hostaddr->h_addr,hostaddr->h_length);

    /* resolve protocol */
    protocol = getprotobyname("tcp");
    if (!protocol) {
        perror(getSyslogString(SYSLOG_RESOLVE_PROTOCOL));
        syslog(LOG_CRIT,getSyslogString(SYSLOG_RESOLVE_PROTOCOL));
        exit(errno);
    }

    /* create the socket */
    sockid=socket(PF_INET,SOCK_STREAM,protocol->p_proto);
    if (sockid <= 0) {
        perror(getSyslogString(SYSLOG_SOCKET));
        syslog(LOG_CRIT,getSyslogString(SYSLOG_SOCKET));
        exit(errno);
    }


    /* connect to server */
    if(connect(sockid,(struct sockaddr *)&socketaddr,sizeof(socketaddr))<0) {
        perror(getSyslogString(SYSLOG_CONNECT));
        syslog(LOG_CRIT,getSyslogString(SYSLOG_CONNECT));
        exit(errno);
    }

}
/* ############################################################################# */
void disconnectServer(void){
    shutdown(sockid,0);
}
/* ############################################################################ */
void SendLine(char* pMsg){
    extern ConfigSetup_t sSetup;
    extern pthread_mutex_t mutexSend;
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
            syslog(LOG_CRIT,getSyslogString(SYSLOG_SEND));
            stop=true;
        }
        DEBUG("send(%d/%d): %s",nCharPerMinute,sSetup.nFastSendingCharLimit,pMsg);
    
        
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
            syslog(LOG_CRIT,getSyslogString(SYSLOG_RECV));
            stop=true;
        }
    } else {
        stop=true;
        syslog(LOG_NOTICE,getSyslogString(SYSLOG_TIMEOUT));
    }

    pLine[str_len]='\0';
    
}

/* ############################################################################# */

int GetCharPerMin(unsigned int nCharCount) {
    time_t currTime;
    int diffMinTime;
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
void ConnectToIrc(void){
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
        
        
        nick(sSetup.botname);
        RecvLine(recv_buffer,RECV_BUFFER_SIZE);

        /* check for  nickname alread in use */
        /* if he in use then put a leading underline on the front of the name */
        if (strstr(recv_buffer,"Nickname is already in use.")) {
            tmp=(char *)calloc(strlen(sSetup.botname)+2,sizeof(char));
            sprintf(tmp,"_%s",sSetup.botname);
            free(sSetup.botname);
            sSetup.botname=tmp;
            i=1;
        }
        
        /* Try MAX_NICKS times to set */
        if ( trying>MAX_NICKS) {
            errno=EAGAIN;
            perror(getMsgString(ERR_NICK));
            exit(errno);
        }
    } while (i==1);
}
/* ############################################################################# */
void join_all_channels(void) {
    PQueue pChannelQueue;
	QueueData *pChannel;
    unsigned int i;
    pChannelQueue=list_db(CHANNEL_DB);
    
    /* join_Channels */
	while (isfullQueue(pChannelQueue)) {
        pChannel=popQueue(pChannelQueue);
        join((char*)pChannel->data);
        free(pChannel->data);
		free(pChannel);
	}
	deleteQueue(pChannelQueue);
}
