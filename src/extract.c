/* -------------------------------------------------------------
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
 -------------------------------------------------------------*/

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
char *getNickname(char const *pLine){
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
char *getNetmask(char const *pLine){ 
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
char *getCommand(char const *pLine) {
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
char *getArgument(char const *pLine) {
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
char *getChannel(char const *pLine){
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
char *getAccessChannel(char const *pLine) {
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
char  *getTopic(char const *pChannelSet) {
	char *pTopic;
	char *pPos,*pPos2;
	char *pStr;
	
	if (!pChannelSet) return NULL;

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
char  *getGreeting(char const * pChannelSet) {
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
char *getChannelMode(char const * pChannelSet){
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
char *getParameters(char const *pLine){
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
char ** splitString(char const * pString,int nRetArraySize) {
    char **ppStrings=NULL;
    char *pPos,*pSpacePos;
    char *pTmp;
    unsigned int iCount=0,i;

    // check of NULL pointer
    if (pString) {
        pTmp=(char*)malloc((strlen(pString)+1)*sizeof(char));
        strcpy(pTmp,pString);
   
        pPos=pTmp;
    
        ppStrings=(char**)malloc(nRetArraySize*sizeof(char*));
    
        // fill the array
        for (i=0;i<nRetArraySize-1;i++) {
            pSpacePos=strchr(pPos,' ');

            if (pSpacePos) {
                pSpacePos[0]='\0';
                
                // put the string in the array
                ppStrings[i]=(char*)malloc((strlen(pPos)+1)*sizeof(char));
                strcpy(ppStrings[i],pPos);

                pPos=++pSpacePos;

            } else {
                break;
            }
            
        }
        
        // put the rest in the array
        if (strlen(pPos)) {
            ppStrings[i]=(char*)malloc((strlen(pPos)+1)*sizeof(char));
            strcpy(ppStrings[i],pPos);
        } else {
            ppStrings[i+1]=NULL;
        }

    }
    return ppStrings;
}
// #############################################################################
char * getBanmask(char const *pLine){ 
	char *pBanmask,*pStr;
	char * pChar;
	// getting netmask to get banmask
	pStr=getNetmask(pLine);
	if (pStr[0]!='\0')
	{
		// get the position of '!'
		pChar=(char *)strchr(pStr,'!');
		// jump over '!'
		pChar;
		// copy from '!' to pStr
		if ((pBanmask=(char*) malloc(strlen(pChar)+3))==NULL)
		{
			DEBUG("getBanmask: Error: Not enoug Memory!");
			return NULL;
		}
		strcpy(pBanmask,"*");
		strcat(pBanmask,pChar);
		strcpy(pStr,pBanmask);
		// setting after @ a NULL-Byte
		pChar=(char *)strchr(pBanmask,'@');
		pChar[1]='*';
		pChar[2]='\0';
		pChar=(char *)strchr(pStr,'.');
		// add rest of the netmask
		strcat(pBanmask,pChar);
		
		return pBanmask;
	}
	
	return NULL; 
}

