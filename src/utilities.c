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
#include <ctype.h>

#include "type.h"
#include "messages.h"
#include "utilities.h"

// ############################################################################# 
void trim(char* line) {
	int alpha,i,j;
	int line_len;

	// remove leading space
	// remove multispace
	// remove newline
	// remove carge return
	// remove tabs
	alpha=j=0;
	line_len=strlen(line);
	for (i=0;i<=line_len;i++) {
		if ((line[i]==' ') && (alpha==1)) {
			line[j]=line[i];
			alpha=0;
			j++;
		} else if ((line[i]!='\n') && (line[i]!='\r') && (line[i]!=' ') && (line[i]!='\t')) {
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
void msleep(unsigned long ms){
	usleep(ms*1000);
}
// ############################################################################# 
void print_msg(const char *msg[]) {
    int i;
	for (i=0;msg[i][0]!=EOM;i++) {
		printf("%s",msg[i]);
	}
	exit(true);
}
// ############################################################################# 
void StrToLower(char *str) {
	 unsigned int str_length,i;

	 str_length=strlen(str);

	 for (i=0;i<str_length;i++) {
		 if (isalpha(str[i])) {
   			str[i]=tolower(str[i]);
		 }
	
	 }
}
