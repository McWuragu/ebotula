/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>
#include <syslog.h>
#include <dirent.h>
#include <sys/types.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "utilities.h"
#include "messages.h"
#include "dbaccess.h"

static GDBM_FILE dbf_user;
static GDBM_FILE dbf_channel;
static GDBM_FILE dbf_access;
static GDBM_FILE dbf_usertonick;
static GDBM_FILE dbf_nicktouser;
static GDBM_FILE dbf_banlist;
static GDBM_FILE dbf_timelog;

static pthread_mutex_t dbaccess_mutex[MAX_DB];

// ############################################################################# 
void initDatabases(void) {
    extern ConfigSetup_t sSetup;
    DIR *pDir;
	int i;
    char *user,*channel,*access,*nicktouser,*usertonick,*banlist,*timelog;
            
    // generate the filenames
    user=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen("/user.dbf")+1)*sizeof(char));
    channel=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen("/channel.dbf")+1)*sizeof(char));
    usertonick=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen("/usertonick.dbf")+1)*sizeof(char));
    nicktouser=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen("/nicktouser.dbf")+1)*sizeof(char));
    access=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen("/access.dbf")+1)*sizeof(char));
    banlist=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen("/banlist.dbf")+1)*sizeof(char));
    timelog=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen("/timelog.dbf")+1)*sizeof(char));

    // create filenames
    sprintf(user,"%s/user.dbf",sSetup.pDatabasePath);
    sprintf(channel,"%s/channel.dbf",sSetup.pDatabasePath);
    sprintf(usertonick,"%s/usertonick.dbf",sSetup.pDatabasePath);
    sprintf(nicktouser,"%s/nicktouser.dbf",sSetup.pDatabasePath);
    sprintf(access,"%s/access.dbf",sSetup.pDatabasePath);
    sprintf(banlist,"%s/banlist.dbf",sSetup.pDatabasePath);
    sprintf(timelog,"%s/timelog.dbf",sSetup.pDatabasePath);

    // check directory
    // if  this not existe then try to create
    if (!(pDir=opendir(sSetup.pDatabasePath))) {
        errno=0;
        if (mkdir(sSetup.pDatabasePath,0700)) {
            syslog(LOG_ERR,SYSLOG_CREAT_DIR_ERR);
            perror(SYSLOG_CREAT_DIR_ERR);
            exit(errno);
        } else {
            syslog(LOG_INFO,SYSLOG_CREATE_DIR);
        }
    }
    closedir(pDir);
                                            
    // open the databases
    dbf_user=gdbm_open(user,512,GDBM_WRCREAT,0600,NULL);
    dbf_channel=gdbm_open(channel,512,GDBM_WRCREAT,0600,NULL);
    dbf_usertonick=gdbm_open(usertonick,512,GDBM_NEWDB,0600,NULL);
    dbf_nicktouser=gdbm_open(nicktouser,512,GDBM_NEWDB,0600,NULL);
    dbf_access=gdbm_open(access,512,GDBM_WRCREAT,0600,NULL);
    dbf_banlist=gdbm_open(banlist,512,GDBM_WRCREAT,0600,NULL);
    dbf_timelog=gdbm_open(timelog,512,GDBM_WRCREAT,0600,NULL);
    
    if (!dbf_user || !dbf_channel || !dbf_usertonick || !dbf_nicktouser || !dbf_access || !dbf_banlist || !dbf_timelog) {
        //errno=EBUSY;
        syslog(LOG_ERR,SYSLOG_DATABASE_ERR);
        perror(SYSLOG_DATABASE_ERR);
        exit(errno);
        
    }

	// init the mutexs
	for (i=0;i<MAX_DB;i++){
		pthread_mutex_init(&dbaccess_mutex[i],NULL);
	}
	
    syslog(LOG_INFO,SYSLOG_INIT_DB);
}
// ############################################################################# 
void closeDatabase(void) {
	int i;
   
	// destroy the mutex
	for (i=0;i<MAX_DB;i++){
		pthread_mutex_destroy(&dbaccess_mutex[i]);
	}
	
	// close the databases
    gdbm_close(dbf_user);
    gdbm_close(dbf_channel);
    gdbm_close(dbf_usertonick);
    gdbm_close(dbf_nicktouser);
    gdbm_close(dbf_access);
    gdbm_close(dbf_banlist);
    gdbm_close(dbf_timelog);
    DEBUG("Close databases");
}

//######################### database access ##############################
static GDBM_FILE get_dbf(int db) {

    switch (db) {
    case USER_DB:
        return dbf_user;
        break;
    case CHANNEL_DB:
        return dbf_channel;
        break;
    case ACCESS_DB:
        return dbf_access;
        break;
    case BANLIST_DB:
        return dbf_banlist;
        break;
    case USERTONICK_DB:
        return dbf_usertonick;
        break;
    case NICKTOUSER_DB:
        return dbf_nicktouser;
        break;
    case TIMELOG_DB:
        return dbf_timelog;
        break;
    default:
        DEBUG("Unkown database");
        return 0;

    }
}
// ############################################################################# 
boolean add_db(int db,char *_key, char *_value) {
    datum key,value;
    GDBM_FILE dbf;
    int iErr;

    // check of exist  of this key in the database
    CHECK_EXIST(db,_key);

    // get the datebase handle
    if ((dbf=get_dbf(db)) && _key!=NULL && _value!=NULL) {
        key.dptr=_key;
        key.dsize=strlen(key.dptr)+1;
    
        // if try to use  the  user database then make  a crypt value
        if (db==USER_DB) {
            value.dptr=crypt(_value,"SL");
        } else {
            value.dptr=_value;
        }
        value.dsize=strlen(value.dptr)+1;
        
        
        pthread_mutex_lock(&dbaccess_mutex[db]);
        iErr=gdbm_store(dbf,key,value,GDBM_INSERT);
        pthread_mutex_unlock(&dbaccess_mutex[db]);
        
        if (!iErr) {
            return true;
        }
    }
    return false;
}
// ############################################################################# 
boolean replace_db(int db,char *_key, char *_value){
    datum key,value;
    GDBM_FILE dbf;
    
    CHECK_NO_EXIST(db,_key);
    
    // get the datebase handle
    if ((dbf=get_dbf(db)) && _key!=NULL && _value!=NULL) {
    	// build key
	   	key.dptr=_key;
	    key.dsize=strlen(key.dptr)+1;

    	// distinction of user database 
	    if (db==USER_DB) {
    	    value.dptr=crypt(_value,"SL");
	    } else {
    	    value.dptr=_value;
	    }

    	value.dsize=strlen(value.dptr)+1;

	    pthread_mutex_lock(&dbaccess_mutex[db]);
    	gdbm_store(dbf,key,value,GDBM_REPLACE);
	    pthread_mutex_unlock(&dbaccess_mutex[db]);
    
    return true;
	}

    return false;

}
// ############################################################################# 
boolean del_db(int db,char *_key){
    datum key;
    GDBM_FILE dbf;
    int iErr;

    CHECK_NO_EXIST(db,_key);

    if ((dbf=get_dbf(db)) && _key!=NULL) {
        // build the  key
        key.dptr=_key;
        key.dsize=strlen(key.dptr)+1;
        
        
        pthread_mutex_lock(&dbaccess_mutex[db]);
        iErr=gdbm_delete(dbf,key);
        pthread_mutex_unlock(&dbaccess_mutex[db]);
    
        if (!iErr) {
            return true;
        }
    }
    return false;
}
// ############################################################################# 
boolean check_db(int db,char *_key,char* _value){
    datum key;
    datum value;
    char *__value;
    GDBM_FILE dbf;

    key.dptr=_key;
    key.dsize=strlen(_key)+1;
    
    CHECK_NO_EXIST(db,_key);
    
    if ((dbf=get_dbf(db)) && _key!=NULL && _value!=NULL) {
        // fetch the entry
        pthread_mutex_lock(&dbaccess_mutex[db]);
        value=gdbm_fetch(dbf,key);
        pthread_mutex_unlock(&dbaccess_mutex[db]);
    
        if (db==USER_DB) {
            __value=crypt(_value,"SL"); 
        } else {
            __value=_value;
        }
    
        // checke  the values
        if (!strcmp(value.dptr,__value)) {
            return true;
        }
    }
    return false;
}
// ############################################################################# 
boolean exist_db(int db,char *_key){
    datum key;
    GDBM_FILE dbf;
    int iRet;

    if ((dbf=get_dbf(db)) && _key!=NULL) {
        key.dptr=_key;
        key.dsize=strlen(key.dptr)+1;
    
        pthread_mutex_lock(&dbaccess_mutex[db]);
        iRet=gdbm_exists(dbf,key);
        pthread_mutex_unlock(&dbaccess_mutex[db]);
    
        return iRet;
    }
    return false;
}
// ############################################################################# 
char * get_db(int db,char *_key){
    datum key,value;
    GDBM_FILE dbf;
    char *str=NULL;


    if ((dbf=get_dbf(db)) && _key!=NULL) {
    
	    key.dptr=_key;
	    key.dsize=strlen(key.dptr)+1;
    
	    pthread_mutex_lock(&dbaccess_mutex[db]);
	    value=gdbm_fetch(dbf,key);
    	pthread_mutex_unlock(&dbaccess_mutex[db]);

	    if (value.dptr) {
    		str=(char *)malloc(value.dsize*sizeof(char));
	    	strcpy(str,value.dptr);
    
    		free(value.dptr);
	    }
    	return str;
	}
    
    return NULL;
}
// ############################################################################# 
char ** list_db(int db){
    char **ppList;
    GDBM_FILE dbf;
    datum key,nextkey,firstkey;
    unsigned int count=0,i;

    // get the database handle
    if (!(dbf=get_dbf(db))) {
        ppList=(char **)malloc(sizeof(char *));
        ppList[0]=NULL;
        return ppList;
    }
    
    pthread_mutex_lock(&dbaccess_mutex[db]);
    firstkey=gdbm_firstkey(dbf);
    
    key=firstkey;
    if (key.dptr) {
        // calculat the  size of  database
        do {
            count++;
            
            nextkey=gdbm_nextkey(dbf,key);
            
            free(key.dptr);
            key=nextkey;
    
        } while ( key.dptr );
    }



    // allocat the memory and set  end mark
    ppList=(char **)malloc((count+1)*sizeof(char *));
    ppList[count]=NULL;

    firstkey=gdbm_firstkey(dbf);
    key=firstkey;

    for (i=0;i<count;i++) {
        ppList[i]=(char *)malloc(key.dsize*sizeof(char));
        strcpy(ppList[i],key.dptr);
        
        nextkey=gdbm_nextkey(dbf,key);

        free(key.dptr);
        key=nextkey;

    }

    pthread_mutex_unlock(&dbaccess_mutex[db]);
    return ppList;
}

