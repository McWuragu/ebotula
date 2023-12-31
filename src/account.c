/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2023 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 * ############################################################# 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "dbaccess.h"
#include "messages.h"
#include "utilities.h"
#include "account.h"

/* ############################################################################# */
UserLevel_t getUserLevel(char *const pChannel, char *const pNetmask) {
    char *pAccessLevelKey;
	size_t nAccessLevelKeySize;
    char *pLogin;
    char *pLevel;
    UserLevel_t UserLevel=NoneLevel;

    /* lock for the  login of this netmask */
    if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
        UserLevel=LoggedLevel;
        
        /* check the  other level */
        if (pChannel) {
            /* built the  key */
           nAccessLevelKeySize = (strlen(pChannel)+strlen(pLogin)+1)*sizeof(char);
		   pAccessLevelKey = (char*)malloc(nAccessLevelKeySize);
            snprintf(pAccessLevelKey,nAccessLevelKeySize,"%s%s",pLogin,pChannel);

            if ((pLevel=get_db(ACCESS_DB,pAccessLevelKey))) {
                if (strchr(pLevel,'o')) {
                    UserLevel=OwnerLevel;
                } else if (strchr(pLevel,'v')) {
                    UserLevel=FriendLevel;
                }
            } else if ((pLevel=get_db(ACCESS_DB,pLogin))) {
                UserLevel=MasterLevel;
                free(pLevel);
            }

            free(pAccessLevelKey);
        } else if ((pLevel=get_db(ACCESS_DB,pLogin))) {
                UserLevel=MasterLevel;
                free(pLevel);
        }

        free(pLogin);
    }
    return UserLevel;
}
/* ############################################################################# */
void rmDeadLogins(long lCheckTime) {
   	PQueue pLoginQueue;
	QueueData *pLogin; 
    char *pTime=NULL;

    /* get the  list */
	pLoginQueue=list_db(USER_DB);
		
    while (isfullQueue(pLoginQueue)) {
        pLogin=popQueue(pLoginQueue);

        /* read time */
   	    if ((pTime=get_db(TIMELOG_DB,(char*)pLogin->data))) {
	        /* check login status */
            if (exist_db(USERTONICK_DB,(char*)pLogin->data)) {
                /* check the time */
        	    if (lCheckTime>atol(pTime)) {
    	            log_out((char*)pLogin->data);
                	logger(LOG_DEBUG,_("The account %s was automatically logged off"),(char*)pLogin->data);
            	}
            }
    	    free(pTime);
		}
        free(pLogin->data);
		free(pLogin);
    }
	deleteQueue(pLoginQueue);
}
/* ############################################################################# */
void log_on(char *pNetmask,char *pLogin) {
    extern pthread_mutex_t mutexAccount;
    time_t timestamp;
    char pTime[32];
    char *pOldNetmask;

    pthread_mutex_lock(&mutexAccount);
    if (exist_db(USERTONICK_DB,pLogin)) {
        /* relog on */
        if ((pOldNetmask=get_db(USERTONICK_DB,pLogin))) {

			del_db(NICKTOUSER_DB,pOldNetmask);
        	add_db(NICKTOUSER_DB,pNetmask,pLogin);

        	replace_db(USERTONICK_DB,pLogin,pNetmask);
		}
    } else {
        add_db(NICKTOUSER_DB,pNetmask,pLogin);
        add_db(USERTONICK_DB,pLogin,pNetmask);
    }
    pthread_mutex_unlock(&mutexAccount);

    logger(LOG_INFO,_("Account %s logged in"),pLogin);

    /* build the timestamp */
    time(&timestamp);
    sprintf(pTime,"%ld",timestamp);

    /* set the last login timestamp */
    if (exist_db(TIMELOG_DB,pLogin)) {
        replace_db(TIMELOG_DB,pLogin,pTime);
        logger(LOG_DEBUG,_("Update the timestamp %s for the account %s"),pTime,pLogin);
    } else {
        add_db(TIMELOG_DB,pLogin,pTime);
        logger(LOG_DEBUG,_("Add the timepstamp %s for the account %s"),pTime,pLogin);
    }
}
/* ############################################################################# */
void log_out(char *pLogin) {
    extern pthread_mutex_t mutexAccount;

    pthread_mutex_lock(&mutexAccount);
    
    __log_out(pLogin);

    pthread_mutex_unlock(&mutexAccount);
}

/* ############################################################################# */
void __log_out(char *pLogin) {
    char *pNetmask;
    if ((pNetmask=get_db(USERTONICK_DB,pLogin))){
        /* Log off */
	    del_db(NICKTOUSER_DB,pNetmask);
    	del_db(USERTONICK_DB,pLogin);
    	logger(LOG_INFO,_("Account %s logged off"),pLogin);
        free(pNetmask);
	}
}
/* ############################################################################# */
void rmAccount(char *pLogin) {
    extern pthread_mutex_t mutexAccount;
    
	logger(LOG_INFO,_("Remove the account %s"),pLogin);

    pthread_mutex_lock(&mutexAccount);
	/* logoff the user */
    __log_out(pLogin);
    
    /* remove the  rights */
    rmAccessRights(pLogin);

    /* remove login */
    del_db(USER_DB,pLogin);

    /* remove the time log */
    del_db(TIMELOG_DB,pLogin);
    pthread_mutex_unlock(&mutexAccount);
}
/* ############################################################################# */
void rmAccessRights(char *pLogin){
    PQueue pChannelQueue;
	QueueData *pChannel;
    char *pKey;
	size_t nKeySize;
    int iLoginLen;

    logger(LOG_DEBUG,_("Remove the access rights for the account %s"),pLogin);

    /* remove as master */
    del_db(ACCESS_DB,pLogin);

    pChannelQueue=list_db(CHANNEL_DB);

    iLoginLen=strlen(pLogin);
    /* remove access rights from the user */
    while (isfullQueue(pChannelQueue)) {
        pChannel=popQueue(pChannelQueue);

        /* build  the key for access.dbf */
		nKeySize = (pChannel->t_size+iLoginLen+1)*sizeof(char);
        pKey=(char *)malloc(nKeySize);
        snprintf(pKey,nKeySize,"%s%s",pLogin,(char*)pChannel->data);

        del_db(ACCESS_DB,pKey);

        free(pKey);
        free(pChannel->data);
		free(pChannel);
    }
	deleteQueue(pChannelQueue);
}
/* ############################################################################# */
void rmDeadAccounts(long lCheckTime) {
    PQueue pLoginQueue;
	QueueData *pLogin;
	char *pTime;

	/* get the list of all Logins */
	pLoginQueue=list_db(TIMELOG_DB);
    
    /* get the  list */
    while (isfullQueue(pLoginQueue)) {
        pLogin=popQueue(pLoginQueue);

        /* read time */
        if ((pTime=get_db(TIMELOG_DB,(char*)pLogin->data))) {
        	/* check the time */
    	    if (lCheckTime>atol(pTime)) {
	            rmAccount((char*)pLogin->data);
                logger(LOG_INFO,_("The account %s is removed"),(char*)pLogin->data);
        	}
        	free(pTime);
		}
        free(pLogin->data);
		free(pLogin);
    }
	deleteQueue(pLoginQueue);
}
/* ############################################################################# */
boolean checkUserLevel(char *pCmdLogin, char *pLogin, char const *pChannel) {
    char *pMod;
    char *pAccessKey;
    size_t nAccessKeyLen;
    boolean bStatus;

    /* read mods */
    if (pLogin){
        /* built db access key */
		nAccessKeyLen = (strlen(pLogin)+strlen(pChannel)+1)*sizeof(char);
        pAccessKey=(char*)malloc(nAccessKeyLen);
        snprintf(pAccessKey,nAccessKeyLen,"%s%s",pLogin,pChannel);
    
        /* read the access level of  the  user wiche want baning user */
        if ((pMod=get_db(ACCESS_DB,pAccessKey))) {}
        else if ((pMod=get_db(ACCESS_DB,pLogin))) {
                
        }
        else 
            pMod=NULL;

        free(pAccessKey);
    }

    /* look for ban */
    if (pMod==NULL) {
        bStatus=true;
    } else if (pMod[1]=='m') {
        bStatus=false;
    } else if (exist_db(ACCESS_DB,pCmdLogin)) {
        /* kick by master */
        bStatus=true;
    } else if (pMod[1]!='o'){
        /* ban by owner */
        bStatus=true;
    } else {
        bStatus=false;
    }
    return bStatus;
}

