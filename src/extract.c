/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "irc.h"
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
	char *str,*pos,*parameter;;
	int i,line_len;
	
	// found  the begining  of Parameter 
	if ((str=strstr(line," :!"))==NULL) {
		return NULL;
	} else {
		
		
		
		// set the begin of comand string
		str+=3;
        line_len=strlen(str);

		// search for the first space or end of string
		for (i=0;i<=line_len;i++) {
				
			if (str[i]==' ') {
				pos=&str[i];
                pos++;
				trim(pos);
                // looking  for empty string
				if (strlen(pos)>0) {
					parameter=(char *)malloc((strlen(pos)+1)*sizeof(char));
                    strcpy(parameter,pos);
					return parameter;
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

	StrToLower(channel);
	return channel;
}
// ######################################################################### 
char *getAccessChannel(char *line) {
	char *parameter;
	char *channel;
	char *pos;

	parameter=getArgument(line);

	// look channel name  in preamble
	if ((parameter == NULL) || (parameter[0]!='#')) {
	
		// look for channelname  as preamble
		if (!(channel=getChannel(line))) {
			if (!(pos=strstr(line," :#"))) {
			return NULL;
			}
			// move to '#'
			pos+=2;
			// marked the end of str and  copy out
			strtok(pos," ");
			channel=(char *)malloc((strlen(pos)+1)*sizeof(char));
		    strcpy(channel,pos);
		}
	} else {
		
		// parse Channel name
		strtok(parameter," ");
		// check the  chrakter in the  channel name
		if (strpbrk(parameter,CHANNEL_NOT_ALLOW_CHAR)) {
			return NULL;
		}
		channel=(char *)malloc((strlen(parameter)+1)*sizeof(char));
		strcpy(channel,parameter);
    }

	StrToLower(channel);
	return channel;
}
// ######################################################################### 
char  *getTopic(char *channelstr) {
	char *topic;
	char *pos,*pos2;
	char *str;

	str=(char *)malloc((strlen(channelstr)+1)*sizeof(char));
	strcpy(str,channelstr);

	// look for topic;
	if (!(pos=strchr(str,'\t'))) {
		return NULL; 
	}
		
	pos++;

	// look for the end  of topic
	if (!(pos2=strchr(pos,'\t'))) {
		return NULL;
	}
	*pos2='\0';

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
// ######################################################################### 
char *getMode(char *channelstr){
	char *mode;
	char *str;
	char *pos;

	str=(char *)malloc((strlen(channelstr)+1)*sizeof(char));
	strcpy(str,channelstr);

	if (!(pos=strchr(str,'\t'))) {
		return NULL;
	} else {
		*pos='\0';
		mode=(char*)malloc((strlen(str)+1)*sizeof(char));
		strcpy(mode,str);
	}
	return mode;
}
// ############################################################################# 
char *getParameters(char *line){
	char *parameters;
	char *arg;
	char *pos; 

	if (!(arg=getArgument(line))) {
		return NULL;
	}

	if (arg[0]!='#') {
		return arg;
	} else if(!(pos=strchr(arg,' '))) {
		return NULL;
	} else {
		// jump over the space
		pos++;
		parameters=(char*)malloc((strlen(pos)+1)*sizeof(char));
		strcpy(parameters,pos);
		return parameters;
	}
}
