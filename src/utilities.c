/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#if HAVE_CONFIG_H
    #include "config.h"
#endif 
 
#include "type.h"
#include "messages.h"
#include "extract.h"
#include "utilities.h"



// ############################################################################# 
void trim(char* pStr) {
    boolean bIsAlpha;
    int iLineLength,i,j;

    // remove leading space
    // remove multispace
    // remove newline
    // remove carge return
    // remove tabs
    bIsAlpha=false;
    j=0;
    iLineLength=strlen(pStr);
    for (i=0;i<=iLineLength;i++) {
        if ((pStr[i]==' ') && bIsAlpha) {
            pStr[j]=pStr[i];
            bIsAlpha=false;
            j++;
        } else if ((pStr[i]!='\n') && (pStr[i]!='\r') && (pStr[i]!=' ') && (pStr[i]!='\t')) {
            pStr[j]=pStr[i];
            bIsAlpha=true;
            j++;
        }
    }

    // remove the folling space
    if (pStr[--j]==' ') {
        pStr[j]='\0';
    }
}
// ############################################################################# 
void clearspace(char *pStr) {
    int i,j,noclr;
    int iLineLength;

    iLineLength=strlen(pStr);
    j=noclr=0;
    for (i=0;i<=iLineLength;i++) {
        if ((pStr[i]!=' ') || (noclr==1)) {
            if ((pStr[i]=='\"') && (noclr==0)) {
                noclr=1;
            } else if ((pStr[i]=='\"') && (noclr==1)) {
                noclr=0;
            } else {
                pStr[j]=pStr[i];
                j++;
            }
        }
    }
}
// ############################################################################# 
void printMsg( char *const pMsg[]) {
    int i;
    for (i=0;pMsg[i][0]!=EOM;i++) {
        printf("%s",pMsg[i]);
    }

}
// ############################################################################# 
void StrToLower(char *pStr) {
     unsigned int str_length,i;

     if (!pStr) {return;}

     str_length=strlen(pStr);

     for (i=0;i<str_length;i++) {
         if (isalpha(pStr[i])) {
            pStr[i]=tolower(pStr[i]);
         }
    
     }
}
// ############################################################################# 
char *ChannelModeToStr(ChannelMode_t *pMode){
    char *pModeStr;
    int nStrSize;

    /* calcuate the maximal length of the string of the channel modes line */
    nStrSize=MAX_MODES+4;
    if (pMode->pKeyword) {nStrSize+=strlen(pMode->pKeyword);}
    if (pMode->pLimit) {nStrSize+=strlen(pMode->pLimit);}

    pModeStr=malloc(nStrSize*sizeof(char));
    strcpy(pModeStr,pMode->pModeStr);
    clearspace(pModeStr);

    // add the parameter for +k and/or +l 
    // the order is: keyword is first then come limit
    if (strchr(pModeStr,'k') && pMode->pKeyword) {
        strcat(pModeStr," ");
        strcat(pModeStr,pMode->pKeyword);
    }

    if (strchr(pModeStr,'l') && pMode->pLimit) {
        strcat(pModeStr," ");
        strcat(pModeStr,pMode->pLimit);
    }

    trim(pModeStr);

    return pModeStr;
}
// ############################################################################# 
void StrToChannelMode(char *pModeStr,ChannelMode_t * pMode) {
    char *ppArgv[3];
    char *pPos,*pPos2;
    int i,j,iLength;

    if (!pMode) {return;}

    // init  the struct
    pMode->pKeyword=NULL;
    pMode->pLimit=NULL;

    strcpy(pMode->pModeStr,"            ");

    // split the mode line in three parts
    pPos=pModeStr;
    for (i=0;i<3;i++) {
        if (pPos) {
            pPos2=pPos;
            pPos=strchr(pPos2,' ');
        
            if (pPos)  {
                *pPos='\0';
                pPos++;
            }
            ppArgv[i]=(char*)malloc((strlen(pPos2)+1)*sizeof(char));
            strcpy(ppArgv[i],pPos2);
        } else {
            ppArgv[i]=NULL;
        }
    }

    // fill the struct
    if (ppArgv[0]) {
        iLength=strlen(ppArgv[0]);
        j=1;
        for (i=0;i<iLength;i++) {
            switch (ppArgv[0][i]) {
            case '+':
                pMode->pModeStr[MOD_TYPE]='+';
                break;
            case '-':
                pMode->pModeStr[MOD_TYPE]='-';
                break;
            case 'k':
                pMode->pModeStr[MOD_KEYWORD]='k';
                pMode->pKeyword=(char*)malloc((strlen(ppArgv[j])+1)*sizeof(char));
                strcpy(pMode->pKeyword,ppArgv[j]);
                j++;
                break;
            case 'l':
                pMode->pModeStr[MOD_LIMIT]='l';

                pMode->pLimit=(char*)malloc((strlen(ppArgv[j])+1)*sizeof(char));
                strcpy(pMode->pLimit,ppArgv[j]);
                j++;
                break;
            case 't':
                pMode->pModeStr[MOD_TOPIC]='t';
                break;
            case 'n':
                pMode->pModeStr[MOD_MESSAGE]='n';
                break;
            case 'm':
                pMode->pModeStr[MOD_MODERAT]='m';
                break;
            case 'p':                   
                pMode->pModeStr[MOD_PRIVAT]='p';
                break;
            case 's':
                pMode->pModeStr[MOD_SECRET]='s';
                break;
            case 'a':
                pMode->pModeStr[MOD_ANONYM]='a';
                break;
            case 'r':
                pMode->pModeStr[MOD_REOP]='r';
                break;
            case 'q':
                pMode->pModeStr[MOD_QUIET]='q';
                break;
            case 'i':
                pMode->pModeStr[MOD_INVITE]='i';
                break;
            default:
                break;
            }
        }
    }

    for (i=0;i<3;i++) {
        free (ppArgv[i]);
    }
    return;
}
// ############################################################################# 
void StrToChannelData(char *pChannelSet,ChannelData_t * pChannelData) {
    char *pMode;

    pChannelData->pGreeting=getGreeting(pChannelSet);
    pChannelData->pTopic=getTopic(pChannelSet);
    
    pMode=getChannelMode(pChannelSet);
    StrToChannelMode(pMode,&(pChannelData->sModes));
    
    free(pMode);

    return;
}
// ############################################################################# 
char *ChannelDataToStr(ChannelData_t *pChannelData) {
    char *pChannelSet;
    char *pMode;
    int iLenght;

    pMode=ChannelModeToStr(&(pChannelData->sModes));
    iLenght=strlen(pMode);

    if (pChannelData->pGreeting) {
        iLenght+=strlen(pChannelData->pGreeting);
    }

    if (pChannelData->pTopic) {
        iLenght+=strlen(pChannelData->pTopic);
    }
    pChannelSet=(char*)malloc((iLenght+3)*sizeof(char));
    sprintf(pChannelSet,"%s\t%s\t%s",pMode,
        (pChannelData->pTopic)?pChannelData->pTopic:"",
        (pChannelData->pGreeting)?pChannelData->pGreeting:"");
    free(pMode);

    return pChannelSet;
}

// ############################################################################# 
boolean ChannelStringCheck(char *const pStr) {
    if (pStr) {
        if (CommonStringCheck(pStr)) {
            if (pStr[0]=='#' || pStr[0]=='&') {
                return true;
            }
        }
    }
    return false;
}

// ############################################################################# 
boolean NickStringCheck(char *const pStr) {
    unsigned int i;

    if (pStr) {
        if (CommonStringCheck(pStr)) {
            for (i=0;pStr[i]!='\0';i++) {
                if (!(isalnum(pStr[i]))) {
                    if (!strchr(USER_ALLOW_CHAR,pStr[i])) {
                        return false;
                    }
                }
            }
                return true;
        }
    }
    return false;
}
// ############################################################################# 
int logger(int priority, char *format, ...)
{
    extern ConfigSetup_t sSetup;
	char buf[1<<12];
	struct timespec stamp; 
    struct tm *td; 
    va_list az;
	
    if (priority>sSetup.nLogLevel) { return 0;}

	/**/
	va_start(az,format);
	/* put message in to data*/
	vsprintf(buf,format,az);
#ifdef NDEBUG
    #if HAVE_SYSLOG_H
	syslog(priority,&buf);
    #endif
#else
    clock_gettime(CLOCK_REALTIME,&stamp);
    td=localtime(&stamp.tv_sec);
    fprintf(stderr,"%02d:%02d:%02d.%03d %s\n",td->tm_hour,td->tm_min,td->tm_sec,(stamp.tv_nsec/1000000),buf); 
#endif
	/**/
	va_end(az);
	return 0;
}

