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
void hNickChange(char *line) {
	char *netmask;
	char *login;
	char *newnetmask;

	netmask=getNetmask(line);
	login=get_db(NICKTOUSER_DB,netmask);

	newnetmask=strstr(line," :");
	newnetmask+=2;
    strtok(newnetmask,"\r");

	// replace  the netmask
	if (del_db(NICKTOUSER_DB,netmask)) {
		add_db(NICKTOUSER_DB,newnetmask,login);
		replace_db(USERTONICK_DB,login,newnetmask);
	}

	DEBUG("Changethe netmask \"%s\" to \"%s\"",netmask,newnetmask);

}
// ######################################################################### 
// Event handler: JOIN 
// Action: Request OP
// ######################################################################### 
void hBotNeedOp(char *line){
	extern ConfType setup;
	char *channel;
	char *names;
	char *pos;
	char *searchstr;

	channel=getChannel(line);
	
	// extrakt Namelist
	pos=strchr(&line[1],':');
	names=(char*)malloc((strlen(pos)+1)*sizeof(char));
	strcpy(names,pos);

	searchstr=(char *) malloc((strlen(setup.botname)+2)*sizeof(char));
	sprintf(searchstr,"@%s",setup.botname);

	DEBUG("Look for OP right for %s",searchstr);
	if (strstr(names,searchstr)) {
		return;
	}

  	privmsg(channel,MSG_NEED_OP);
}
// ######################################################################### 
// Event handler: JOIN 
// Action: set the mods  for the user
// ######################################################################### 
void hSetMods(char *line) {
	char *login;
	char *nick;
	char *key;
	char *channel;
	char *mods;

	login=get_db(NICKTOUSER_DB,getNetmask(line));
	nick=getNickname(line);


	if (strlen(login)) {
		channel=getAccessChannel(line);

	// build key for access.dbf
		key=malloc((strlen(login)+strlen(channel)+1)*sizeof(char));
		sprintf(key,"%s%s",login,channel);
	
		// read  the  mods
		mods=get_db(ACCESS_DB,key);
	
		// set the mods  for this nick
		if (strlen(mods)) {
			mode(channel,mods,nick);
		} else if (exist_db(ACCESS_DB,login)) {
			mode(channel,"+o",nick);
		}
	}
}
