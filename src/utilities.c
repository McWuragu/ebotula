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
void printMsg(const char **pMsg) {
    int i;
	for (i=0;pMsg[i][0]!=EOM;i++) {
		printf("%s",pMsg[i]);
	}
	exit(true);
}
// ############################################################################# 
void StrToLower(char *pStr) {
	 unsigned int str_length,i;

	 str_length=strlen(pStr);

	 for (i=0;i<str_length;i++) {
		 if (isalpha(pStr[i])) {
   			pStr[i]=tolower(pStr[i]);
		 }
	
	 }
}
