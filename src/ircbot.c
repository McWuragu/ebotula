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
#include <sys/time.h>

#include "utilities.h"
#include "parameter.h"
#include "messages.h"
#include "network.h"
#include "irc.h"
#include "command.h"
#include "dbaccess.h"
#include "config.h"



CONFIG_TYPE setup;
int key;
int stop;

pthread_mutex_t	send_mutex;
pthread_mutex_t	dbaccess_mutex;

int main(int argc,const char *argv[]) {
	int i;
	int msgid;
	char buffer[RECV_BUFFER_SIZE],*pos,*str,*tmp;;
	pthread_t *thread;
	struct MSGBUF_DS msg;

	bzero(&msg,sizeof(struct MSGBUF_DS));

    
	setup.createMaster=false;

	// check for parameter
	if (argc>1) {
		cmd_line(argc,argv);
	}

	//  checking  config  file path
	if (setup.configfile==NULL) {
		setup.configfile=(char *)malloc((strlen(CONFDIR)+strlen(CONFFILE)+1)*sizeof(char));
		sprintf(setup.configfile,"%s%s",CONFDIR,CONFFILE);

	}

	DEBUG("File %s",setup.configfile);

	// read config file
	read_config_file();

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

	DEBUG("-----------------------------------------------");
	DEBUG("Server %s",setup.server);
	DEBUG("Port %s",setup.port);
	DEBUG("Nickname %s", setup.botname);
	DEBUG("Realname %s", setup.realname);
	DEBUG("Threads %d",setup.thread_limit);
	DEBUG("Config file %s",setup.configfile);
	DEBUG("Database path %s",setup.database_path);
	DEBUG("-----------------------------------------------");


	// init Database and the mutex for  access to the database
	init_database();
	pthread_mutex_init(&dbaccess_mutex,NULL);

	// create maste
	if (setup.createMaster) {
		DEBUG("Create a master of bot");
		dialog_master();
	}

	// create the network connection
	if ((setup.server!=NULL) && (setup.port!=NULL)) {
		printf("Try connect to %s:%s\n",setup.server,setup.port);
		connectServer();
		printf("Bot is connected.\n");
	} else {
		closeDatabase();
		errno=EINVAL;
		perror(ERR_FAILED_NETPARA);
		exit(errno);
	}

	// connect to the server and init the mutex  for sending
	pthread_mutex_init(&send_mutex,NULL);
    irc_connect();
	printf("The irc service is connected.\n");
	printf("The bot is runing....\n");

	// open msg queue
	key=getpid();
	if ((msgid=msgget(key,0600 | IPC_CREAT ))<0) {
		perror(ERR_MSG);
		exit(errno);
	}

	//set signal handler
	signal(SIGINT,stopParser);
	signal(SIGTERM,stopParser);
	signal(SIGABRT,stopParser);

	#ifndef _DEBUG
	// make a daemon 
	daemon(true,true);
	#endif

	// create the threads
	thread=(pthread_t *)malloc(setup.thread_limit*sizeof(pthread_t));
	for (i=0;i<setup.thread_limit;i++) {
		pthread_create(&thread[i],NULL,action_thread,NULL);
		DEBUG("Thread %d is running",i);
	}

	// join the channels
	join_all_channels();

	stop=false;
	while (!stop) {
		recv_line(buffer,RECV_BUFFER_SIZE);

		tmp=buffer;
		while ((pos=strchr(tmp,'\n'))) {
			*pos='\0';

			str=(char *)calloc(strlen(tmp)+1,sizeof(char));
			strcpy(str,tmp);

			DEBUG("Receive: %s",str);
			msg=preParser(str);

			// put the identifiy line  on the  queue
			if (msg.identify!=0) {
				msgsnd(msgid,&msg,sizeof(struct MSGBUF_DS)-sizeof(msg.mtype),0);
			}

			free(str);
			tmp=pos+1;
		}

		

	}
	DEBUG("Stopped");

	
	
	// wait of  terminat all threads
	for (i=0;i<setup.thread_limit;i++) {
		DEBUG("Stop thread %d",i);
		pthread_cancel(thread[i]);
		pthread_join(thread[i],NULL);
	}


	// destroy the mutex
	pthread_mutex_destroy(&send_mutex);
	pthread_mutex_destroy(&dbaccess_mutex);

	// clear the wait queue
	msgctl(msgid,IPC_RMID,NULL);
	disconnectServer();
    closeDatabase();

	return(0);

}


