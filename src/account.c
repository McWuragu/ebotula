/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <syslog.h>
					 
#include "dbaccess.h"
#include "messages.h"
#include "utilities.h"
#include "account.h"
// ############################################################################# 
void rmDeadLogins(long lCheckTime) {
	char **ppLogins;
	char *pTime;
	int i;
    
	// get the  list
	ppLogins=list_db(TIMELOG_DB);
	for (i=0;ppLogins[i]!=NULL;i++) {
		
		// read time
		pTime=get_db(TIMELOG_DB,ppLogins[i]);
		
		// check the time
		if (lCheckTime>atol(pTime)) {
			log_out(ppLogins[i]);
			syslog(LOG_NOTICE,SYSLOG_LOGIN_RM,ppLogins[i]);
		}
		free(pTime);
	}

}
// ############################################################################# 
void log_on(char *pNetmask,char *pLogin) {
	extern pthread_mutex_t account_mutex;
	time_t timestamp;
	char pTime[32];
	char *pOldNetmask;

	if (exist_db(USERTONICK_DB,pLogin)) {
		pOldNetmask=get_db(USERTONICK_DB,pLogin);

		pthread_mutex_lock(&account_mutex);
		del_db(NICKTOUSER_DB,pOldNetmask);
		add_db(NICKTOUSER_DB,pNetmask,pLogin);
		
		replace_db(USERTONICK_DB,pLogin,pNetmask);
		pthread_mutex_unlock(&account_mutex);
	} else {
		pthread_mutex_lock(&account_mutex);
		add_db(NICKTOUSER_DB,pNetmask,pLogin);
		add_db(USERTONICK_DB,pLogin,pNetmask);
		pthread_mutex_unlock(&account_mutex);
	}
    
	
	DEBUG("User log in");
	
	// build the timestamp
    time(&timestamp);
	sprintf(pTime,"%ld",timestamp);

	// set the last login timestamp
	if (exist_db(TIMELOG_DB,pLogin)) {
		replace_db(TIMELOG_DB,pLogin,pTime);
		DEBUG("Update timestamp %s for %s",pTime,pLogin);
	} else {
		add_db(TIMELOG_DB,pLogin,pTime);
		DEBUG("Add timepstamp %s for %s",pTime,pLogin);
	}
}
// ############################################################################# 
void log_out(char *pLogin) {
	extern pthread_mutex_t account_mutex;
	char *pNetmask;

	DEBUG("%s logged off",pLogin);

	pNetmask=get_db(USERTONICK_DB,pLogin);

	pthread_mutex_lock(&account_mutex);
	del_db(NICKTOUSER_DB,pNetmask);
	del_db(USERTONICK_DB,pLogin);
	pthread_mutex_unlock(&account_mutex);
}

// ############################################################################# 
void rmAccount(char *pLogin) {

	// logoff the user
	log_out(pLogin);

	// remove the  rights
	rmAccessRights(pLogin);
	
	// remove login
	del_db(USER_DB,pLogin);

	// remove the time log
	del_db(TIMELOG_DB,pLogin);
}
// ############################################################################# 
void rmAccessRights(char *pLogin){
	char **ppChannels;
	char *pKey;
	int i,iLoginLen;

	DEBUG("Remove Access Rights for %s",pLogin);

	// remove as master
	del_db(ACCESS_DB,pLogin);
	
	ppChannels=list_db(CHANNEL_DB);

	iLoginLen=strlen(pLogin);
	// remove access rights from the user
	for (i=0;ppChannels[i]!=NULL;i++) {
		
		// build  the key for access.dbf
		pKey=malloc((strlen(ppChannels[i])+iLoginLen+1)*sizeof(char));
		sprintf(pKey,"%s%s",pLogin,ppChannels[i]);
		
		del_db(ACCESS_DB,pKey);
			
		free(pKey);
	}
}
// ############################################################################# 
void rmDeadAccounts(long lCheckTime) {
	char **ppLogins;
	char *pTime;
	int i;
    
	ppLogins=list_db(TIMELOG_DB);

	// get the  list
	for (i=0;ppLogins[i]!=NULL;i++) {
		
		// read time
		pTime=get_db(TIMELOG_DB,ppLogins[i]);
		
		// check the time
		if (lCheckTime>atol(pTime)) {
			rmAccount(ppLogins[i]);
			syslog(LOG_NOTICE,SYSLOG_ACCOUNT_RM,ppLogins[i]);
		}
		free(pTime);
	}
}
