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

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "dbaccess.h"
#include "messages.h"
#include "utilities.h"
#include "account.h"
// #############################################################################
void rmDeadLogins(long lCheckTime) {
   	PQueue pLoginQueue;
	QueueData *pLogin; 
    char *pTime=NULL;

    // get the  list
	pLoginQueue=list_db(USER_DB);
		
    while ((pLogin=popQueue(pLoginQueue))) {

        // read time
   	    if ((pTime=get_db(TIMELOG_DB,(char*)pLogin->data))) {
	
	        // check the time
    	    if (lCheckTime>atol(pTime)) {
	            log_out((char*)pLogin->data);
            	syslog(LOG_NOTICE,SYSLOG_LOGIN_RM,(char*)pLogin->data);
        	}
    	    free(pTime);
		}
		free(pLogin);
    }
	deleteQueue(pLoginQueue);
}
// #############################################################################
void log_on(char *pNetmask,char *pLogin) {
    extern pthread_mutex_t account_mutex;
    time_t timestamp;
    char pTime[32];
    char *pOldNetmask;

    pthread_mutex_lock(&account_mutex);
    if (exist_db(USERTONICK_DB,pLogin)) {
        if ((pOldNetmask=get_db(USERTONICK_DB,pLogin))) {
        	
				del_db(NICKTOUSER_DB,pOldNetmask);
        	add_db(NICKTOUSER_DB,pNetmask,pLogin);

        	replace_db(USERTONICK_DB,pLogin,pNetmask);
		}
    } else {
        add_db(NICKTOUSER_DB,pNetmask,pLogin);
        add_db(USERTONICK_DB,pLogin,pNetmask);
    }
    pthread_mutex_unlock(&account_mutex);

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


    
    pthread_mutex_lock(&account_mutex);
    if ((pNetmask=get_db(USERTONICK_DB,pLogin))){
	    del_db(NICKTOUSER_DB,pNetmask);
    	del_db(USERTONICK_DB,pLogin);
    	DEBUG("%s logged off",pLogin);
	}
    pthread_mutex_unlock(&account_mutex);
}

// #############################################################################
void rmAccount(char *pLogin) {

	DEBUG("Remove the account %s",pLogin);
    
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
    PQueue pChannelQueue;
	QueueData *pChannel;
    char *pKey;
    int iLoginLen;

    DEBUG("Remove access rights from %s",pLogin);

    // remove as master
    del_db(ACCESS_DB,pLogin);

    pChannelQueue=list_db(CHANNEL_DB);

    iLoginLen=strlen(pLogin);
    // remove access rights from the user
    while ((pChannel=popQueue(pChannelQueue))) {

        // build  the key for access.dbf
        pKey=(char *)malloc((pChannel->t_size+iLoginLen+1)*sizeof(char));
        sprintf(pKey,"%s%s",pLogin,(char*)pChannel->data);

        del_db(ACCESS_DB,pKey);

        free(pKey);
		free(pChannel);
    }
	deleteQueue(pChannelQueue);
}
// #############################################################################
void rmDeadAccounts(long lCheckTime) {
    extern pthread_mutex_t account_mutex;
    PQueue pLoginQueue;
	QueueData *pLogin;
	char *pTime;

    pthread_mutex_lock(&account_mutex);
    
	// get the list of all Logins
	pLoginQueue=list_db(TIMELOG_DB);
    
    // get the  list
    while ((pLogin=popQueue(pLoginQueue))) {

        // read time
        if ((pTime=get_db(TIMELOG_DB,(char*)pLogin->data))) {
        	// check the time
    	    if (lCheckTime>atol(pTime)) {
	            rmAccount((char*)pLogin->data);
            	syslog(LOG_NOTICE,SYSLOG_ACCOUNT_RM,(char*)pLogin->data);
        	}
        	free(pTime);
		}
		free(pLogin);
    }
    pthread_mutex_unlock(&account_mutex);
	deleteQueue(pLoginQueue);
}
