#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "messages.h"
//#include "network.h"
#include "irc.h"
#include "command.h"
#include "dbaccess.h"
#include "config.h"
#include "utilities.h"

void stopParser(int sig) {
	extern int stop;
	if (!stop) {
		quit();
	}

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
		if ((line[i]==' ') && (alpha==1)) {
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
	if (line[--j]==' ') {
		line[j]='\0';
	}
}

// ############################################################################# 
void clearspace(char *line) {
	int i,j,noclr;

	j=noclr=0;
	for (i=0;i<=strlen(line);i++) {
		if ((line[i]!=' ') || (noclr==1)) {
			if ((line[i]=='\"') && (noclr==0)) {
				noclr=1;
			} else if ((line[i]=='\"') && (noclr==1)) {
				noclr=0;
			} else {
				line[j]=line[i];
				j++;
			}
		}
	}
}
// ############################################################################# 
void dialog_master(void){
	char  name[18],passwd[10],repasswd[10];

	// insert the login name
	printf(MSG_MASTER_TITLE);
	printf(MSG_MASTER_LOGIN);
	fgets(name,17,stdin);

	// check loging
	if (strpbrk(name,NOT_ALLOW_CHAR)) {
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
	} else if (strpbrk(passwd," ")) {
		fprintf(stderr,ERR_NOT_ALLOW_CHAR);
		exit(1);
	}

	trim(name);
	trim(passwd);


	// create account
	if (!add_db(USER_DB,name,passwd)) {
		printf(MSG_MASTER_EXISTS);
		exit(1);
	}


	if (!add_db(ACCESS_DB,name,"+ov")) {
		printf(MSG_MASTER_ERR);
		exit(1);
	}
}
// ############################################################################# 
void millisleep(unsigned long ms){
	struct timeval wait;
	
	wait.tv_sec=0;
	wait.tv_usec=ms*1000;

	select(0,NULL,NULL,NULL,&wait);
}

