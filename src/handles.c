/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "extract.h"
#include "utilities.h"
#include "dbaccess.h"
#include "messages.h"
#include "irc.h"
#include "handles.h"


// ######################################################################### 
// Event handler: NICK
// ######################################################################### 
void hNickChange(char *pLine) {
	char *pNetmask;
	char *pLogin;
	char *pNewNetmask;

	pNetmask=getNetmask(pLine);
	pLogin=get_db(NICKTOUSER_DB,pNetmask);

	pNewNetmask=strstr(pLine," :");
	pNewNetmask+=2;
    strtok(pNewNetmask,"\r");

	// replace  the netmask
	if (del_db(NICKTOUSER_DB,pNetmask)) {
		add_db(NICKTOUSER_DB,pNewNetmask,pLogin);
		replace_db(USERTONICK_DB,pLogin,pNewNetmask);
	}

	DEBUG("Changethe netmask \"%s\" to \"%s\"",pNetmask,pNewNetmask);

}
// ######################################################################### 
// Event handler: JOIN 
// Action: Request OP
// ######################################################################### 
void hBotNeedOp(char *pLine){
	extern ConfType sSetup;
	char *pChannel;
	char *pNickList;
	char *pPos;
	char *pSearchStr;

	pChannel=getChannel(pLine);
	
	// extrakt Namelist
	pPos=strchr(&pLine[1],':');
	pNickList=(char*)malloc((strlen(pPos)+1)*sizeof(char));
	strcpy(pNickList,pPos);

	pSearchStr=(char *) malloc((strlen(sSetup.botname)+2)*sizeof(char));
	sprintf(pSearchStr,"@%s",sSetup.botname);

	DEBUG("Look for OP right for %s",pSearchStr);
	if (strstr(pNickList,pSearchStr)) {
		return;
	}

  	privmsg(pChannel,MSG_NEED_OP);
}
// ######################################################################### 
// Event handler: JOIN 
// Action: set the mod  for the user
// ######################################################################### 
void hSetModUser(char *pLine) {
	char *pLogin;
	char *pNick;
	char *pKey;
	char *pChannel;
	char *pMod;

	pLogin=get_db(NICKTOUSER_DB,getNetmask(pLine));
	pNick=getNickname(pLine);

	DEBUG("Set the mod for Account %s with nickname %s",pLogin,pNick);

	if (strlen(pLogin)) {
		pChannel=getAccessChannel(pLine);

		// build key for access.dbf
		pKey=malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char));
		sprintf(pKey,"%s%s",pLogin,pChannel);
	
		// read  the  mod
		pMod=get_db(ACCESS_DB,pKey);
	
		// set the mod  for this nick
		if (strlen(pMod)) {
			mode(pChannel,pMod,pNick);
		} else if (exist_db(ACCESS_DB,pLogin)) {
			mode(pChannel,"+o",pNick);
		}
	}
}
// ######################################################################### 
// Event handler: MODE 
// Action: reset the mod  for the user if this chang not by bot self
// ######################################################################### 
void hResetModUser(char *pLine) {
	extern ConfType sSetup;
	char *pPos;
	char *pChannel;
	char pMod[3];
	char *pNick;

	if (strcmp(getNickname(pLine),sSetup.botname)) {
		DEBUG("Reset the Mod");
		
		// set pPos of the position of changed mode
		pPos=strstr(pLine,"MODE");
        pPos=strchr(pPos,' ');
		pPos++;
		pPos=strchr(pPos,' ');
		pPos++;

		pChannel=getAccessChannel(pLine);
		if (pPos[1]=='o' || pPos[1]=='v') {
			// build the replacing mod string
			pMod[0]=(pPos[0]=='+')?'-':'+';
			pMod[1]=pPos[1];
			pMod[3]='\0';

			// set the pointer of  the nick name
			pNick=strchr(pPos,' ');
			pNick++;
			strtok(pNick," ");
			
			// remove
			mode(pChannel,pMod,pNick);
		}
	}
}
