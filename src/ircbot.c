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

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include "utilities.h"
#include "parameter.h"
#include "messages.h"
#include "network.h"
#include "parser.h"
#include "timing.h"
#include "dbaccess.h"
#include "config.h"
  
	 

ConfType setup;	// global config structur
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
	setup.newMaster=false;
	setup.AccountLiveTime=MIN_ALT;
	setup.AutoLoggoff=MIN_ALT;
	setup.sendDelay=-1;
	setup.thread_limit=0;

	// container for a message for the queue
	MsgBufType msg;
	bzero(&msg,sizeof(MsgBufType));
    
	// versions ausgabe
	printf(VERSIONSTR);

	openlog(PROGNAME,0,LOG_DAEMON);
	syslog(LOG_NOTICE,SYSLOG_BOT_START);

	// check for parameter
	if (argc>1) {
		cmd_line(argc,argv);
		syslog(LOG_NOTICE,SYSLOG_READ_CMD);
	}

	//  checking  config  file path
	if (setup.configfile==NULL) {
		setup.configfile=(char *)malloc((strlen(CONFDIR)+strlen(CONFFILE)+1)*sizeof(char));
		sprintf(setup.configfile,"%s%s",CONFDIR,CONFFILE);

	}

	DEBUG("File %s",setup.configfile);

	// read config file
	read_config_file();
    syslog(LOG_NOTICE,SYSLOG_READ_CONFFILE);
	

	// check config datums
	if (!setup.thread_limit) {
		setup.thread_limit=DEFAULT_THREAD_LIMIT;
	}

	if (!setup.botname) {
		setup.botname=(char *)malloc((strlen(DEFAULT_BOTNAME)+1)*sizeof(char));
		strcpy(setup.botname,DEFAULT_BOTNAME);
	}

	if (!setup.realname) {
		setup.realname=(char *)malloc((strlen(DEFAULT_REALNAME)+1)*sizeof(char));
		strcpy(setup.realname,DEFAULT_REALNAME);
	}

	if (setup.sendDelay<0) {
		setup.sendDelay=0;
	}

	DEBUG("-----------------------------------------------");
	DEBUG("Server %s",setup.server);
	DEBUG("Port %s",setup.port);
	DEBUG("Nickname %s", setup.botname);
	DEBUG("Realname %s", setup.realname);
	DEBUG("Threads %d",setup.thread_limit);
	DEBUG("Config file %s",setup.configfile);
	DEBUG("Database path %s",setup.database_path);
	DEBUG("Sending delay %dms",setup.sendDelay);
	DEBUG("Account live time %dd",setup.AccountLiveTime);
	DEBUG("Autolog of after %dd",setup.AutoLoggoff);
	DEBUG("-----------------------------------------------");


	// init Database and the mutex for  access to the database
	init_database();


	if (setup.newMaster) {
		if (!dialogMaster()){
			closeDatabase();
			exit(-1);
		}
	}
	
	// create the network connection
	if ((setup.server!=NULL) && (setup.port!=NULL)) {
		printf(SYSLOG_TRY_CONNECT,setup.server,setup.port);
		printf("\n");
		
		syslog(LOG_INFO,SYSLOG_TRY_CONNECT,setup.server,setup.port);
		
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
	
	irc_connect();
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
	pthread_create(&timeThread,NULL,synchron,NULL);
	threads=(pthread_t *)malloc(setup.thread_limit*sizeof(pthread_t));
	for (i=0;i<setup.thread_limit;i++) {
		pthread_create(&threads[i],NULL,action_thread,NULL);
		syslog(LOG_NOTICE,SYSLOG_THREAD_RUN,i);
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
            msg=preParser(str);

			// put the identified line  on the  queue
			if (msg.identify!=CMD_NONE) {
				msgsnd(msgid,&msg,sizeof(MsgBufType)-sizeof(msg.mtype),0);
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
	for (i=0;i<setup.thread_limit;i++) {
		DEBUG("Stop thread %d",i);
		pthread_cancel(threads[i]);
		pthread_join(threads[i],NULL);
	}


	// destroy the mutex
	pthread_mutex_destroy(&send_mutex);
	pthread_mutex_destroy(&dbaccess_mutex);

	// clear the wait queue
	msgctl(msgid,IPC_RMID,NULL);
	disconnectServer();
    closeDatabase();
	closelog();
	return(0);

}


