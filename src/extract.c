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

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include "irc.h"
#include "utilities.h"
#include "extract.h"
// ############################################################################# 
char *getNickname(char *pLine){
	char *pNick,*pStr;
	
	pStr=getNetmask(pLine);


	if (!strtok(pStr,"!")) {
		return "";
	} else {
		pNick=(char *)malloc((strlen(pStr)+1)*sizeof(char));
		strcpy(pNick,pStr);
		return pNick;
	}


}
// ############################################################################# 
char *getNetmask(char *pLine){ 
	char *pNetmask,*pStr;

		
	pStr=(char *)malloc((strlen(pLine)+1)*sizeof(char));
	strcpy(pStr,pLine);
    strtok(&pStr[1]," ");

	if (!strchr(pStr,'!') || !strchr(pStr,'@')) {
		return "";
	} else if (pStr[0]==':'){
		pNetmask=(char *)malloc(strlen(pStr)*sizeof(char));
		strcpy(pNetmask,&pStr[1]);
		return pNetmask;
	} else {
		pNetmask=(char *)malloc((strlen(pStr)+1)*sizeof(char));
		strcpy(pNetmask,pStr);
		return pNetmask;
	}



}
// ############################################################################# 
char *getCommand(char *pLine) {
	char *pStr,*pTmp;

	// mirror  of the orginal string 
	pTmp=(char *)malloc((strlen(pLine)+1)*sizeof(char));
	strcpy(pTmp,pLine);

	// find the  secondary double point
	// and put after this a null byte
	if (pTmp[0]!=':') {
		return "";
	}
	// find the next colon and replace  him with null byte
	strtok(&pTmp[1],":");
	
	// cut out  the first part of the server answer 
	pStr=(char *)malloc((strlen(pTmp)+1)*sizeof(char));
	strcpy(pStr,pTmp);

	return pStr;
}
// ############################################################################# 
char *getArgument(char *pLine) {
	char *pStr,*pPos,*pArg;
	int i,iLineLen;
	
	// found  the begining  of Parameter 
	if ((pStr=strstr(pLine," :!"))==NULL) {
		return "";
	} else {
		
		
		
		// set the begin of comand string
		pStr+=3;
        iLineLen=strlen(pStr);

		// search for the first space or end of string
		for (i=0;i<=iLineLen;i++) {
				
			if (pStr[i]==' ') {
				pPos=&pStr[i];
                pPos++;
				trim(pPos);
                // looking  for empty string
				if (strlen(pPos)>0) {
					pArg=(char *)malloc((strlen(pPos)+1)*sizeof(char));
                    strcpy(pArg,pPos);
					return pArg;
				}
			}
		}
	}
	
	return "";
}
// ######################################################################### 
char *getChannel(char *pLine){
	char *pPreamble;
	char *pPos;
	char *pChannel;

	// extract  the substring
	pPreamble=getCommand(pLine);

	// look for the channelname
	if (!(pPos=strchr(pPreamble,'#'))) {
		return "";
	}

	// market the end  of channelname
	strtok(pPos," ");
    
	// extract the channelname
	pChannel=(char *)malloc((strlen(pPos)+1)*sizeof(char));
	strcpy(pChannel,pPos);

	StrToLower(pChannel);
	return pChannel;
}
// ######################################################################### 
char *getAccessChannel(char *pLine) {
	char *pParameter;
	char *pChannel;
	char *pPos;

	pParameter=getArgument(pLine);

	// look channel name  in preamble
	if (pParameter[0]!='#') {
	
		// look for channelname  as preamble
		pChannel=getChannel(pLine);
		if (!strlen(pChannel)) {
			if (!(pPos=strstr(pLine," :#"))) {
			return "";
			}
			// move to '#'
			pPos+=2;
			// marked the end of str and  copy out
			strtok(pPos," ");
			pChannel=(char *)malloc((strlen(pPos)+1)*sizeof(char));
		    strcpy(pChannel,pPos);
		}
	} else {
		
		// parse Channel name
		strtok(pParameter," ");
		// check the  chrakter in the  channel name
		if (strpbrk(pParameter,CHANNEL_NOT_ALLOW_CHAR)) {
			return "";
		}
		pChannel=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
		strcpy(pChannel,pParameter);
    }

	StrToLower(pChannel);
	return pChannel;
}
// ######################################################################### 
char  *getTopic(char *pChannelSet) {
	char *pTopic;
	char *pPos,*pPos2;
	char *pStr;

	pStr=(char *)malloc((strlen(pChannelSet)+1)*sizeof(char));
	strcpy(pStr,pChannelSet);

	// look for topic;
	if (!(pPos=strchr(pStr,'\t'))) {
		return NULL; 
	}
		
	pPos++;

	// look for the end  of topic
	if (!(pPos2=strchr(pPos,'\t'))) {
		return NULL;
	}
	*pPos2='\0';

	// check length
	if (!strlen(pPos)) {
		return NULL;
	}
	pTopic=(char *)malloc((strlen(pPos)+1)*sizeof(char));
	strcpy(pTopic,pPos);

	return pTopic;
}
// ######################################################################### 
char  *getGreating(char *pChannelSet) {
	char *pGreating;
	char *pPos;

    // look for the begin  of greating
	if (!(pPos=strrchr(pChannelSet,'\t'))) {
		return NULL;		  
    }

	pPos++;

	// check length
	if (!strlen(pPos)) {
		return NULL;
	}

	pGreating=(char *)malloc((strlen(pPos)+1)*sizeof(char));
	strcpy(pGreating,pPos);

	return pGreating;
}
// ######################################################################### 
char *getChannelMode(char *pChannelSet){
	char *pMod;
	char *pStr;
	char *pPos;

	pStr=(char *)malloc((strlen(pChannelSet)+1)*sizeof(char));
	strcpy(pStr,pChannelSet);

	if (!(pPos=strchr(pStr,'\t'))) {
		return "";
	} else {
		*pPos='\0';
		pMod=(char*)malloc((strlen(pStr)+1)*sizeof(char));
		strcpy(pMod,pStr);
	}
	return pMod;
}
// ############################################################################# 
char *getParameters(char *pLine){
	char *pParameter;
	char *pArg;
	char *pPos; 

	pArg=getArgument(pLine);

	if (pArg[0]!='#') {
		return pArg;
	} else if(!(pPos=strchr(pArg,' '))) {
		return "";
	} else {
		// jump over the space
		pPos++;
		pParameter=(char*)malloc((strlen(pPos)+1)*sizeof(char));
		strcpy(pParameter,pPos);
		return pParameter;
	}
}

