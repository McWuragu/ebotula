#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
 
#include "macro.h"
#include "parameter.h"
#include "messages.h"
#include "network.h"
#include "irc.h"
#include "command.h"
#include "dbaccess.h"
#include "config.h"
#include "ircbot.h"



CONFIG_TYPE setup;
int sockid;
int key;
int stop;

int main(int argc,const char *argv[]) {
    int i;
	int msgid;
	char buffer[RECV_BUFFER_SIZE],*pos,*str,*tmp;;
	pthread_t *thread;
	struct MSGBUF_DS msg;

	setup.createMaster=false;

	// check for parameter
	if (argc>1) {
		cmd_line(argc,argv);
	}
	
	//  checking  config  file path
	if (setup.configfile==NULL) {
		setup.configfile=malloc(sizeof(char)*strlen(CONF_FILE)+1);
		strcpy(setup.configfile,CONF_FILE);
	}

	// read config file
	read_config_file();

	DEBUG("-----------------------------------------------");
	DEBUG("Server %s",setup.server);
	DEBUG("Port %s",setup.port);
	DEBUG("Nickname %s", setup.botname);
	DEBUG("Realname %s", setup.realname);
	DEBUG("Threads %d",setup.thread_limit);
	DEBUG("Config file %s",setup.configfile);
	DEBUG("Database path %s",setup.database_path);
	DEBUG("-----------------------------------------------");


	// init Database
	init_database();

	// create maste
	if (setup.createMaster) {
		DEBUG("Create a master of bot");
		dialog_master();
	}
	
	// create the network connection
	if ((setup.server!=NULL) && (setup.port!=NULL) ) {
		printf("Try connect to %s:%s\n",setup.server,setup.port);
		connectServer();
		printf("Bot is connect.\n");
	} else {
		closeDatabase();
		errno=EINVAL;
		perror(ERR_FAILED_NETPARA);
		exit(errno);
	}

	// connect to the server
	irc_connect();
	
	// open msg queue
	key=getpid();
    if (!(msgid=msgget(key,0600 | IPC_CREAT ))) {
		perror(ERR_MSG);
		exit(errno);
	}

	//set signal handler
	signal(SIGINT,stopParser);
	signal(SIGTERM,stopParser);
	signal(SIGABRT,stopParser);

	
	// create the threads
	thread=malloc(sizeof(pthread_t)*setup.thread_limit);
    for(i=0;i<setup.thread_limit;i++) {
		pthread_create(&thread[i],NULL,action_thread,NULL);
		DEBUG("Thread %d is running",i);
	}
    
	// join the channels
	join_all_channels();

	stop=false;
	while(!stop){
		recv_line(buffer,RECV_BUFFER_SIZE);

		tmp=buffer;
		while ((pos=strchr(tmp,'\n'))) {
			*pos='\0';
			str=malloc(sizeof(char)*(strlen(tmp)+1));
			strcpy(str,tmp);
			
			DEBUG("%s",str);
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

	msgctl(msgid,IPC_RMID,NULL);
	shutdown(sockid,0);
	
	closeDatabase();

	return (0);

}

void stopParser(int sig) {
    stop=true;
	DEBUG("Stop IRCBot");
}



void trim(char* line) {
	int alpha,i,j;


	// remove leading space
	// remove multispace
	// remove newline
	// remove carge return
	alpha=j=0;
	for (i=0;i<=strlen(line);i++) {
		if((line[i]==' ') && (alpha==1)) {
			line[j]=line[i];
			alpha=0;
			j++;
		} else if ((line[i]!='\n') && (line[i]!='\r') && (line[i]!=' ')) {
			line[j]=line[i];
			alpha=1;
			j++;
		}
	}

	// remove the folling space
	if(line[--j]==' '){
		line[j]='\0';
	}
}


void clearspace(char *line) {
	int i,j,noclr;

	j=noclr=0;
	for (i=0;i<=strlen(line);i++) {
		if((line[i]!=' ') || (noclr==1)) {
			if ( ((line[i]=='\"') || (line[i]=='\'')) && (noclr==0)) {
				noclr=1;
			} else  if (((line[i]=='\"') || (line[i]=='\'')) && (noclr==1)) {
				noclr=0;
			} else {
				line[j]=line[i];
				j++;
			}
		}
	}
}

void dialog_master(void){
	char  name[18],passwd[10],repasswd[10];
	
	// insert the login name
	printf(MSG_MASTER_TITLE);
	printf(MSG_MASTER_LOGIN);
	fgets(name,17,stdin);
	
	// check loging
	if(strpbrk(name,NOT_ALLOW_CHAR)) {
		fprintf(stderr,ERR_NOT_ALLOW_CHAR);
		exit(1);
	}

	
	// insert the password
	printf(MSG_MASTER_PASS);
	fgets(passwd,8,stdin);
	printf(MSG_MASTER_REPASS);
	fgets(repasswd,8,stdin);

	
	// check the password
	if (strcmp(passwd,repasswd)) {
		fprintf(stderr,MSG_MASTER_PASS_ERR);
		exit(1);
	}else if(strpbrk(passwd,NOT_ALLOW_CHAR)) {
		fprintf(stderr,ERR_NOT_ALLOW_CHAR);
		exit(1);
	}

	trim(name);
	trim(passwd);
	
	
	// create account
	if (!add_db(USER_DB,name,passwd)){
		printf(MSG_MASTER_EXISTS);
		exit(1);
	}
	
	
	if (!add_db(ACCESS_DB,name,"+ov")) {
		printf(MSG_MASTER_ERR);
		exit(1);
	}
}

