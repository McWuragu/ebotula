/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */
/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
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
    char *pLogin;
    char *pLevel;
    UserLevel_t UserLevel=NoneLevel;

    // lock for the  login of this netmask
    if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
        UserLevel=LoggedLevel;
        
        // check the  other level
        if (pChannel) {
            // built the  key
            pAccessLevelKey=(char*)malloc((strlen(pChannel)+strlen(pLogin)+1)*sizeof(char));
            sprintf(pAccessLevelKey,"%s%s",pLogin,pChannel);

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
                	logger(LOG_DEBUG,"The account %s was automatically logged off",(char*)pLogin->data);
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

    StrToLower(pLogin);
    StrToLower(pNetmask);


    pthread_mutex_lock(&mutexAccount);
    if (exist_db(USERTONICK_DB,pLogin)) {
        // relog on
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

    logger(LOG_INFO,"User log in %s",pLogin);

    /* build the timestamp */
    time(&timestamp);
    sprintf(pTime,"%ld",timestamp);

    /* set the last login timestamp */
    if (exist_db(TIMELOG_DB,pLogin)) {
        replace_db(TIMELOG_DB,pLogin,pTime);
        logger(LOG_DEBUG,"Update timestamp %s for %s",pTime,pLogin);
    } else {
        add_db(TIMELOG_DB,pLogin,pTime);
        logger(LOG_DEBUG,"Add timepstamp %s for %s",pTime,pLogin);
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
    StrToLower(pLogin);
    if ((pNetmask=get_db(USERTONICK_DB,pLogin))){
        /* Log off */
	    del_db(NICKTOUSER_DB,pNetmask);
    	del_db(USERTONICK_DB,pLogin);
    	logger(LOG_INFO,"%s logged off",pLogin);
        free(pNetmask);
	}
}
/* ############################################################################# */
void rmAccount(char *pLogin) {
    extern pthread_mutex_t mutexAccount;
    
	logger(LOG_INFO,"Remove the account %s",pLogin);

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
    int iLoginLen;

    logger(LOG_DEBUG,"Remove access rights from %s",pLogin);

    /* remove as master */
    del_db(ACCESS_DB,pLogin);

    pChannelQueue=list_db(CHANNEL_DB);

    iLoginLen=strlen(pLogin);
    /* remove access rights from the user */
    while (isfullQueue(pChannelQueue)) {
        pChannel=popQueue(pChannelQueue);

        /* build  the key for access.dbf */
        pKey=(char *)malloc((pChannel->t_size+iLoginLen+1)*sizeof(char));
        sprintf(pKey,"%s%s",pLogin,(char*)pChannel->data);

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
                logger(LOG_INFO,gettext("The account %s is removed"),(char*)pLogin->data);
        	}
        	free(pTime);
		}
        free(pLogin->data);
		free(pLogin);
    }
	deleteQueue(pLoginQueue);
}
// #############################################################################
boolean checkUserLevel(char *pCmdLogin, char *pLogin, char const *pChannel) {
    char *pMod;
    char *pAccessKey;
    boolean bStatus;

    /* read mods */
    if (pLogin){
        /* built db access key */
        pAccessKey=(char*)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char));
        sprintf(pAccessKey,"%s%s",pLogin,pChannel);
    
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

