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

	if (strtok(pStr,"!") && pLine!=NULL) {
		if (NickStringCheck(pStr)) {
			pNick=(char *)malloc((strlen(pStr)+1)*sizeof(char));
			strcpy(pNick,pStr);
			return pNick;
		}
	}
	return "";
}
// ############################################################################# 
char *getNetmask(char *pLine){ 
	char *pNetmask,*pStr;

		
	pStr=(char *)malloc((strlen(pLine)+1)*sizeof(char));
	strcpy(pStr,pLine);
    strtok(&pStr[1]," ");

	if (!strchr(pStr,'!') || !strchr(pStr,'@') && pLine==NULL) {
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
	if (pTmp[0]==':' && pLine!=NULL) {
		// find the next colon and replace  him with null byte
		strtok(&pTmp[1],":");
	
		// cut out  the first part of the server answer 
		pStr=(char *)malloc((strlen(pTmp)+1)*sizeof(char));
		strcpy(pStr,pTmp);

		return pStr;
	}
	return "";
}
// ############################################################################# 
char *getArgument(char *pLine) {
	char *pStr,*pPos,*pArg;
	int i,iLineLen;


	if (!pLine) return "";
	
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
	
	if (!pLine) return "";

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

	if (!pLine) return "";

	pParameter=getArgument(pLine);

	// look channel name  in preamble
	if (pParameter[0]!='#') {

		// look for channelname  as preamble
		pChannel=getChannel(pLine);
		if (!strlen(pChannel)) {
			if ((pPos=strstr(pLine," :#"))) {
				// move to '#'
				pPos+=2;
				// marked the end of str and  copy out
				strtok(pPos," ");

				pChannel=(char *)malloc((strlen(pPos)+1)*sizeof(char));
				strcpy(pChannel,pPos);
			} else {
				return "";
			}
		}

	} else {
		// parse Channel name
		strtok(pParameter," ");

		// check the  chrakter in the  channel name
		if (ChannelStringCheck(pParameter)) {
			pChannel=(char *)malloc((strlen(pParameter)+1)*sizeof(char));
			strcpy(pChannel,pParameter);
        	} else {
			return "";
		}
    }

	StrToLower(pChannel);
	return pChannel;
}
// #########################################################################
char  *getTopic(char *pChannelSet) {
	char *pTopic;
	char *pPos,*pPos2;
	char *pStr;
	
	if (!pChannelSet) return "";

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
char  *getGreeting(char *pChannelSet) {
	char *pGreeting;
	char *pPos;
	
	if (!pChannelSet) return "";
    
	// look for the begin  of greeting
	if (!(pPos=strrchr(pChannelSet,'\t'))) {
		return NULL;		  
    }

	pPos++;

	// check length
	if (!strlen(pPos)) {
		return NULL;
	}

	pGreeting=(char *)malloc((strlen(pPos)+1)*sizeof(char));
	strcpy(pGreeting,pPos);

	return pGreeting;
}
// ######################################################################### 
char *getChannelMode(char *pChannelSet){
	char *pMod;
	char *pStr;
	char *pPos;

	if (!pChannelSet) return "";
	
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

	if (!pLine) return "";
	
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
// #############################################################################
char ** splitString(char* pLine) {
    char **ppStrings;
    char *pPos,*pPos2;
    unsigned int iCount=0,i;

    // check of NULL pointer
    if (!pLine) {
        return NULL;
    }

    pPos2=pLine;

    // word count
    do {
        pPos=++pPos2;
        iCount++;
    } while ( pPos2=strchr(pPos,' ' ));

    ppStrings=(char**)malloc(iCount*sizeof(char*));

    // built the array
    for (i=iCount;i>0;i--) {
        pPos=strrchr(pLine,' ');
        if (pPos) {
            pPos[0]='\0';
            pPos++;
        } else {
            pPos=pLine;
        }
        
        ppStrings[i-1]=(char*)malloc((strlen(pPos)+1)*sizeof(char));
        strcpy(ppStrings[i-1],pPos);
    }
    return ppStrings;
}
