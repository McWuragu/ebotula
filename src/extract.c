/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <errno.h>

#include "utilities.h"
#include "extract.h"
// ############################################################################# 
char *getNickname(char *line){
	char *nick,*str;
	
	str=(char*)malloc((strlen(line)+1)*sizeof(char));
	strcpy(str,line);

	if (!strtok(str,"!")) {
		return NULL;
	}

	nick=(char *)malloc((strlen(str)+1)*sizeof(char));
	strcpy(nick,&str[1]);
	return nick;
}
// ############################################################################# 
char *getNetmask(char *line){ 
	char *netmask,*str, *pos;
		
	str=(char *)malloc((strlen(line)+1)*sizeof(char));
	strcpy(str,line);
    
	if (!(pos=strchr(str,'!'))) {
		return  NULL;
	}
	
	strtok(pos," ");

	netmask=(char *)malloc(strlen(pos)*sizeof(char));
	strcpy(netmask,&pos[1]);

	return netmask;
}
// ############################################################################# 
char *getCommand(char *line) {
	char *str,*pos,*tmp;

	// mirror  of the orginal string 
	tmp=(char *)malloc((strlen(line)+1)*sizeof(char));
	strcpy(tmp,line);

	// find the  secondary double point
	// and put after this a null byte
	if (!(pos=strchr(&tmp[1],':'))) {
		return NULL;
	}
	pos[1]='\0';
	
	// cut out  the first part of the server answer 
	str=(char *)malloc((strlen(tmp)+1)*sizeof(char));
	strcpy(str,tmp);

	return str;
}
// ############################################################################# 
char *getArgument(char *line) {
	char *str,*pos;
	int i,line_len;
	
	// found  the begining  of Parameter 
	if ((str=strstr(line," :!"))==NULL) {
		return NULL;
	} else {
		
		line_len=strlen(str);
		

		// check the length of the substring
		if (line_len<3) {
			return NULL;
		}

		// set the begin of comand string
		str+=3;
        line_len-=3; 

		// search for the first space or end of string
		for (i=0;i<=line_len;i++) {
				
			if (str[i]==' ') {
				pos=&str[i];
				
				trim(pos);

				// looking  for empty string
				if (strlen(pos)>0) {
					str=(char *)malloc((strlen(pos)+1)*sizeof(char));
					strcpy(str,pos);
					return str;
				}
			}
		}
	}
	
	return NULL;
}
// ######################################################################### 
char *getChannel(char *line){
	char *pramble=getCommand(line);
	char *pos;
	char *channel;

	// extract  the substring
	if (!(pramble=getCommand(line))) {
		return NULL;
	}
	

	// look for the channelname
	if (!(pos=strchr(pramble,'#'))) {
		return NULL;
	}

	// market the end  of channelname
	strtok(pos," ");
    
	// extract the channelname
	channel=(char *)malloc((strlen(pos)+1)*sizeof(char));
	strcpy(channel,pos);

	return channel;
}
// ######################################################################### 
char  *getTopic(char *channelstr) {
	char *topic;
	char *pos;

	// look for topic;
	if (!(pos=strchr(channelstr,'\t'))) {
		return NULL;
    }
		
	pos++;

	// look for the end  of topic
	if (!(strtok(pos,"\t"))) {
		return NULL;
	}

	// check length
	if (!strlen(pos)) {
		return NULL;
	}
	topic=(char *)malloc((strlen(pos)+1)*sizeof(char));
	strcpy(topic,pos);

	return topic;
}
// ######################################################################### 
char  *getGreating(char *channelstr) {
	char *greating;
	char *pos;

    // look for the begin  of greating
	if (!(pos=strrchr(channelstr,'\t'))) {
		return NULL;
    }

	pos++;

	// check length
	if (!strlen(pos)) {
		return NULL;
	}

	greating=(char *)malloc((strlen(pos)+1)*sizeof(char));
	strcpy(greating,pos);

	return greating;
}
