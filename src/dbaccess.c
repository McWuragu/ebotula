/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif


#include "utilities.h"
#include "messages.h"
#include "dbaccess.h"

static GDBM_FILE dbf[MAX_DB];

static pthread_mutex_t dbaccess_mutex[MAX_DB]; 
      
typedef struct DatabaseStruct {
    DatabaseID_t  id;
    char *pName;
    int Access;
} Database_t;

static Database_t pDB[MAX_DB]= {
    {USER_DB,"user.dbf",GDBM_WRCREAT}, 
	{CHANNEL_DB,"channel.dbf",GDBM_WRCREAT},
	{ACCESS_DB,"access.dbf", GDBM_WRCREAT},
	{BANLIST_DB,"banlist.dbf",GDBM_WRCREAT},
	{USERTONICK_DB,"usertonick.dbf",GDBM_NEWDB},
	{TIMELOG_DB,"timelog.dbf",GDBM_WRCREAT},
	{NICKTOUSER_DB,"nicktouser.dbf",GDBM_NEWDB}
};

// ############################################################################# 
boolean initDatabases(void) {
    extern ConfigSetup_t sSetup;
    DIR *pDir;
	int i;
    char *pDBPath;

    logger(LOG_NOTICE,gettext("Initialization of the database" ));
    
    // check directory
    // if  this not existe then try to create
    // this  create only the lowest  subdir
    if (!(pDir=opendir(sSetup.pDatabasePath))) {
        errno=0;
        if (mkdir(sSetup.pDatabasePath,0700)) {
            logger(LOG_ERR,gettext("Couldn't create the directory %s for the database.",sSetup.pDatabasePath ));
            #ifdef NDEBUG
            fprintf(stderr,gettext("Couldn't create the directory %s for the database.",sSetup.pDatabasePath ));
            fprintf(stderr,"\n");
            #endif
            return false;
        } else {
            logger(LOG_INFO,gettext("Create the directory %s for the database."),sSetup.pDatabasePath);
        }
    }
    closedir(pDir);
    
    // open the databases
    for (i=0;i<MAX_DB;i++) {
        assert(i==pDB[i].id);
        
        pDBPath=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen(pDB[i].pName)+2)*sizeof(char));
        sprintf(pDBPath,"%s/%s",sSetup.pDatabasePath,pDB[i].pName);
        dbf[i]=gdbm_open(pDBPath,512,pDB[i].Access,0600,NULL);
        
        if (!dbf[i]) {
            //errno=EBUSY;
            logger(LOG_ERR,gettext("Couldn't open the databases %s.",pDBPath));
            #ifdef NDEBUG
            fprintf(stderr,gettext("Couldn't open the databases %s.",pDBPath));
            fprintf(stderr,"\n");
            #endif
            return false;
        }

        // init the mutexs
        pthread_mutex_init(&dbaccess_mutex[i],NULL);

        free(pDBPath);
    }
    
    return true;
}
// ############################################################################# 
void closeDatabase(void) {
	int i;
   
	// destroy the mutex
	for (i=0;i<MAX_DB;i++){
		pthread_mutex_destroy(&dbaccess_mutex[i]);
        // close the databases
        gdbm_close(dbf[i]);
	}
    logger(LOG_INFO,"Close databases");
}

//######################### database access ##############################
static GDBM_FILE get_dbf(int db) {
    
    if (db>=MAX_DB || db <0) {
        logger(LOG_DEBUG,gettext("Unkown database id %d"),db);
        return 0;
    } else {
        return dbf[db];
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
    boolean bRet=false;

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
            bRet=true;
        }

        free(value.dptr);
    }
    return bRet;
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
PQueue list_db(int db){
    PQueue pList;
	QueueData QueueItem;
	GDBM_FILE dbf;
    datum key,nextkey,firstkey;
	
	// initialize the  queue
	pList=initQueue();

    // get the database handle
    if ((dbf=get_dbf(db))) {
	    pthread_mutex_lock(&dbaccess_mutex[db]);
		key=gdbm_firstkey(dbf);

		/* iterate the date base */
		while (key.dptr) {
			/* built  queue element */
			QueueItem.t_size=key.dsize;
            QueueItem.data=(void*)key.dptr;

			/* push in the  queue */
			pushQueue(pList,QueueItem);
			

			/* read next key */
			nextkey=gdbm_nextkey(dbf,key);
            free (key.dptr);
            key=nextkey;
		}

		pthread_mutex_unlock(&dbaccess_mutex[db]);
	}
    return pList;
}

