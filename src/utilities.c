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
#include <errno.h>

#include "messages.h"
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
	int line_len;

	// remove leading space
	// remove multispace
	// remove newline
	// remove carge return
	alpha=j=0;
	line_len=strlen(line);
	for (i=0;i<=line_len;i++) {
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
	int line_len;

	line_len=strlen(line);
	j=noclr=0;
	for (i=0;i<=line_len;i++) {
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
void millisleep(unsigned long ms){
	struct timeval wait;
	
	wait.tv_sec=0;
	wait.tv_usec=ms*1000;

	select(0,NULL,NULL,NULL,&wait);
}
// ############################################################################# 
void print_msg(const char *msg[]) {
    int i;
	for (i=0;msg[i][0]!=EOM;i++) {
		printf("%s",msg[i]);
	}
	exit(true);
}
