/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>


#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include "utilities.h"
#include "parameter.h"
#include "messages.h"
#include "network.h"
#include "parser.h"
#include "timing.h"
#include "dbaccess.h"
#include "ircbot.h"
  
	 

ConfType sSetup;	// global config structure
int key;			// key of the message  queue
int stop;			// singal for stop the endless loop



pthread_mutex_t	send_mutex;			// mutex for synchronize of send command
pthread_mutex_t	dbaccess_mutex;		// mutex for synchronize of database access
pthread_mutex_t account_mutex;		// mutex for synchronize of accessing to the login db 

int main(int argc,const char *argv[]) {
	int i;
	int msgid;
	char buffer[RECV_BUFFER_SIZE],*pos,*str,*tmp;
	pthread_t *threads;
	pthread_t timeThread;
	
	// init config
	sSetup.botname=(char *)malloc((strlen(DEFAULT_BOTNAME)+1)*sizeof(char));
	strcpy(sSetup.botname,DEFAULT_BOTNAME);
	sSetup.pDatabasePath=(char *)malloc((strlen(DATABASEDIR)+9)*sizeof(char));
	sprintf(sSetup.pDatabasePath,"%s/ebotula",DATABASEDIR);
	sSetup.realname=(char *)malloc((strlen(DEFAULT_REALNAME)+1)*sizeof(char));
	strcpy(sSetup.realname,DEFAULT_REALNAME);
	sSetup.configfile=(char *)malloc((strlen(CONFDIR)+strlen(CONFFILE)+1)*sizeof(char));
	sprintf(sSetup.configfile,"%s%s",CONFDIR,CONFFILE);
	sSetup.newMaster=false;
	sSetup.AccountLiveTime=MIN_ALT;
	sSetup.AutoLoggoff=MIN_LOGOFF;
	sSetup.sendDelay=0;
	sSetup.iTimeout=DEFAULT_PING_TIMEOUT;
	sSetup.thread_limit=DEFAULT_THREAD_LIMIT;

	

	// container for a message for the queue
	MsgBufType sMsg;
	bzero(&sMsg,sizeof(MsgBufType));
    
	// versions ausgabe
	printf(VERSIONSTR);

	#ifndef	_DEBUG
	openlog(PACKAGE,0,LOG_DAEMON);
	#else
	openlog(PACKAGE,LOG_PERROR,LOG_DAEMON);
	#endif
	
    
    // Look for the config file  parameter
	for (i=1;i<argc;i++) {
		if (argv[i][0]=='-') {
			if (argv[i][1]=='f') {
				free(sSetup.configfile);
				sSetup.configfile=(char *)malloc((strlen(argv[++i])+1)*sizeof(char));
				strcpy(sSetup.configfile,argv[i]);
			}
		}

	}
	DEBUG("File %s",sSetup.configfile);

	// read config file
	ConfigFileParser();
    syslog(LOG_NOTICE,SYSLOG_READ_CONFFILE);
	
	// check for parameter
	if (argc>1) {
		ComandLineParser(argc,argv);
		syslog(LOG_NOTICE,SYSLOG_READ_CMD);
	}

	// check the automatic times
	if (sSetup.AccountLiveTime<sSetup.AutoLoggoff) {
		sSetup.AutoLoggoff=sSetup.AccountLiveTime;
	}

	DEBUG("-----------------------------------------------");
	DEBUG("Server %s",sSetup.server);
	DEBUG("Port %s",sSetup.port);
	DEBUG("Nickname %s", sSetup.botname);
	DEBUG("Realname %s", sSetup.realname);
	DEBUG("Threads %d",sSetup.thread_limit);
	DEBUG("Config file %s",sSetup.configfile);
	DEBUG("Database path %s",sSetup.pDatabasePath);
	DEBUG("Ping timeout %ds",sSetup.iTimeout);
	DEBUG("Sending delay %dms",sSetup.sendDelay);
	DEBUG("Account live time %dd",sSetup.AccountLiveTime);
	DEBUG("Autolog of after %dd",sSetup.AutoLoggoff);
	DEBUG("-----------------------------------------------");

	syslog(LOG_NOTICE,SYSLOG_BOT_START);

	// init Database and the mutex for  access to the database
	initDatabases();


	if (sSetup.newMaster) {
		if (!dialogMaster()){
			closeDatabase();
			exit(-1);
		}
	}
	
	

	// create the network connection
	if ((sSetup.server!=NULL) && (sSetup.port!=NULL)) {
		printf(SYSLOG_TRY_CONNECT,sSetup.server,sSetup.port);
		printf("\n");
		
		syslog(LOG_INFO,SYSLOG_TRY_CONNECT,sSetup.server,sSetup.port);
		
		connectServer();
		
		syslog(LOG_INFO,SYSLOG_IS_CONNECT);
		
		printf(SYSLOG_IS_CONNECT);
		printf("\n");
	} else {
		closeDatabase();
		errno=EINVAL;
		perror(SYSLOG_FAILED_NETPARA);
		syslog(LOG_ERR,SYSLOG_FAILED_NETPARA);
		exit(errno);
	}

	

	// connect to the server and init the mutex  for sending
	pthread_mutex_init(&send_mutex,NULL);
    pthread_mutex_init(&dbaccess_mutex,NULL);
	pthread_mutex_init(&account_mutex, NULL);
	
	ConnectToIrc();
    printf(SYSLOG_BOT_RUN);
	printf("\n");
	syslog(LOG_NOTICE,SYSLOG_BOT_RUN);

	// open msg queue
	key=getpid();
	if ((msgid=msgget(key,0600 | IPC_CREAT ))<0) {
		perror(SYSLOG_MSG_QUEUE);
		syslog(LOG_CRIT,SYSLOG_MSG_QUEUE);
		exit(errno);
	}

	// redefine the signal handler for to stop the bot
	signal(SIGINT,stopParser);
	signal(SIGTERM,stopParser);
	signal(SIGABRT,stopParser);
	signal(SIGQUIT,stopParser);

	#ifndef _DEBUG
	// make a daemon 
	daemon(true,true);
	#endif
	
	// create the threads
	pthread_create(&timeThread,NULL,TimingThread,NULL);
	threads=(pthread_t *)malloc(sSetup.thread_limit*sizeof(pthread_t));
	for (i=0;i<sSetup.thread_limit;i++) {
		pthread_create(&threads[i],NULL,ComandExecutionThread,NULL);
		DEBUG(SYSLOG_THREAD_RUN,i);
	}

	// join the channels
	join_all_channels();


	// Main execution loop
	stop=false;
	while (!stop) {

		// read line from tcp stack
		recv_line(buffer,RECV_BUFFER_SIZE);

		// parse all substrings of the  receiving line
		tmp=buffer;
        while ((pos=strchr(tmp,'\r'))) {
		//while ((pos=strchr(tmp,'\n'))) {
			*pos='\0';

			// cut out a part of the  complete line
			str=(char *)calloc(strlen(tmp)+1,sizeof(char));
			strcpy(str,tmp);

			// parse the part line
			DEBUG("Parse \"%s\"",str);
            sMsg=preParser(str);

			// put the identified line  on the  queue
			if (sMsg.identify!=CMD_NONE) {
				msgsnd(msgid,&sMsg,sizeof(MsgBufType)-sizeof(sMsg.mtype),0);
			}

			free(str);
			
			// checking the length of the next substring
			// a irc answer line can't are ten charakter 
			// it is to small for a complete line
			if (strlen(++pos)>0) {
				// set the begin of new substring of the old end
				// and plus one for the NL
				tmp=pos+1;
			}
			
		}

		

	}

	syslog(LOG_NOTICE,SYSLOG_BOT_STOP);
    DEBUG("Stop the timing thread");
	pthread_cancel(timeThread);
	pthread_join(timeThread,NULL);

	// wait of  terminat all threads
	for (i=0;i<sSetup.thread_limit;i++) {
		DEBUG("Stop thread %d",i);
		pthread_cancel(threads[i]);
		pthread_join(threads[i],NULL);
	}


	// destroy the mutex
	pthread_mutex_destroy(&send_mutex);
	pthread_mutex_destroy(&dbaccess_mutex);

	// clear the wait queue
	msgctl(msgid,IPC_RMID,NULL);
    
	// disconnect from server
	disconnectServer();
    
	closeDatabase();
	closelog();
	return(0);

}


