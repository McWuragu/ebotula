/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
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

/* ############################################################################# */
boolean initDatabases(void) {
    extern ConfigSetup_t sSetup;
    DIR *pDir;
	int i;
    char *pDBPath;

    logger(LOG_NOTICE,_("Initialization of the database" ));
    
    /* check directory
       if  this not existe then try to create
       this  create only the lowest  subdir*/
    if (!(pDir=opendir(sSetup.pDatabasePath))) {
        errno=0;
        if (mkdir(sSetup.pDatabasePath,0700)) {
            logger(LOG_ERR,_("Couldn't create the directory %s for the database - %s"),sSetup.pDatabasePath,strerror(errno) );
            return false;
        } else {
            logger(LOG_INFO,_("Create the directory %s for the database."),sSetup.pDatabasePath);
        }
    }
    closedir(pDir);
    
    /* open the databases*/
    for (i=0;i<MAX_DB;i++) {
        assert(i==pDB[i].id);
        
        pDBPath=(char *)malloc((strlen(sSetup.pDatabasePath)+strlen(pDB[i].pName)+2)*sizeof(char));
        sprintf(pDBPath,"%s/%s",sSetup.pDatabasePath,pDB[i].pName);
        dbf[i]=gdbm_open(pDBPath,512,pDB[i].Access,0600,NULL);
        
        if (!dbf[i]) {
            /*errno=EBUSY;*/
            logger(LOG_ERR,_("Couldn't open the databases %s - %s"),pDBPath,strerror(errno));
            return false;
        }



        /* init the mutexs*/
        pthread_mutex_init(&dbaccess_mutex[i],NULL);

        free(pDBPath);
    }
    
    return true;
}
/* ############################################################################# */
void closeDatabase(void) {
	int i;
   
	/*/ destroy the mutex */
	for (i=0;i<MAX_DB;i++){
		pthread_mutex_destroy(&dbaccess_mutex[i]);
        /* close the databases */
        gdbm_close(dbf[i]);
	}
    logger(LOG_INFO,_("Database is closed"));
}

/* ######################### database access ##############################*/
static GDBM_FILE get_dbf(int db) {
    
    if (db>=MAX_DB || db <0) {
        logger(LOG_DEBUG,_("Unkown database id %d"),db);
        return 0;
    } else {
        return dbf[db];
    }

}
/* ############################################################################# */
boolean add_db(const int db,const char *_key,const  char *_value) {
    datum key,value;
    boolean bResult=false;
    GDBM_FILE dbf;
    int iErr;
    char *pKey;

    /* parameters */
    if (!_key || !_value) {return false;}
    
    /* copy key */
    pKey=StrToLower(_key);
    if (!pKey) {return false;}

    /* check of exist  of this key in the database */
    CHECK_EXIST(db,pKey);

    /* get the datebase handle */
    if ((dbf=get_dbf(db))) {
        key.dptr=pKey;
        key.dsize=strlen(pKey)+1;
    
        /* if try to use  the  user database then make  a crypt value */
        if (db==USER_DB) {
            char *pCrypt=crypt(_value,"SL");
            value.dptr=(char*)malloc((strlen(pCrypt)+1)*sizeof(char));
            strcpy(value.dptr,pCrypt);

        } else {
            value.dptr=malloc((strlen(_value)+1)*sizeof(char));
            strcpy(value.dptr,_value);
        }
        
        value.dsize=strlen(value.dptr)+1;
        
        /* database access */
        pthread_mutex_lock(&dbaccess_mutex[db]);
        if (!gdbm_store(dbf,key,value,GDBM_INSERT))
            bResult=true;
        pthread_mutex_unlock(&dbaccess_mutex[db]);
        
        free(value.dptr);
    }

    free(pKey);
    return bResult;
}
/* ############################################################################# */
boolean replace_db(const int db,const char *_key,const char *_value){
    datum key,value;
    GDBM_FILE dbf;
    char *pKey;
    boolean bResult=false;

    /* parameters */
    if (!_key || !_value) {return false;}
    
    /* copy key */
    pKey=StrToLower(_key);
    if (!pKey) {return false;}

    CHECK_NO_EXIST(db,_key);
    
    /* get the datebase handle */
    if ((dbf=get_dbf(db))) {
    	/* build key */
	   	key.dptr=pKey;
	    key.dsize=strlen(pKey)+1;

    	/* distinction of user database */
	    if (db==USER_DB) {
    	    char * pCrypt=crypt(_value,"SL");
            value.dptr=(char*)malloc((strlen(pCrypt)+1)*sizeof(char));
            strcpy(value.dptr,pCrypt);
	    } else {
            value.dptr=(char*)malloc((strlen(_value)+1)*sizeof(char));
            strcpy(value.dptr,_value);
        }

    	value.dsize=strlen(value.dptr)+1;

	    pthread_mutex_lock(&dbaccess_mutex[db]);
    	if (!gdbm_store(dbf,key,value,GDBM_REPLACE))
            bResult=true;
	    pthread_mutex_unlock(&dbaccess_mutex[db]);

        free (value.dptr);
	}

    free(pKey);
    return bResult;

}
/* ############################################################################# */
boolean del_db(const int db,const char *_key){
    datum key;
    GDBM_FILE dbf;
    char *pKey;
    boolean bResult=false;

    /* parameters */
    if (!_key) {return false;}
    
    /* copy key */
    pKey=StrToLower(_key);
    if (!pKey) {return false;}

    CHECK_NO_EXIST(db,_key);

    if ((dbf=get_dbf(db))) {
        /* build the  key */
        key.dptr=pKey;
        key.dsize=strlen(pKey)+1;
        
        
        pthread_mutex_lock(&dbaccess_mutex[db]);
        if (!gdbm_delete(dbf,key))
            bResult=true;
        pthread_mutex_unlock(&dbaccess_mutex[db]);
    }
    free(pKey);
    return bResult;;
}
/* ############################################################################# */
boolean check_db(const int db,const char *_key,const char* _value){
    datum key;
    datum value;
    char *__value;
    char *pKey;
    GDBM_FILE dbf;
    boolean bRet=false;

    if (!_key || !_value) { return false;}

    /* normalize the key */
    pKey=StrToLower(_key);
    if (!pKey) {return false;}

    CHECK_NO_EXIST(db,pKey);
    
    key.dptr=pKey;
    key.dsize=strlen(pKey)+1;

    if ((dbf=get_dbf(db))) {
        /* fetch the entry */
        pthread_mutex_lock(&dbaccess_mutex[db]);
        value=gdbm_fetch(dbf,key);
        pthread_mutex_unlock(&dbaccess_mutex[db]);
    
        if (db==USER_DB) {
            char *pCrypt=crypt(_value,"SL");
            __value=(char*)malloc((strlen(pCrypt)+1)*sizeof(char));
            strcpy(__value,pCrypt);
        } else {
            __value=(char*)malloc((strlen(_value)+1)*sizeof(char));
            strcpy(__value,_value);
        }
    
        /* checke  the values */
        if (strcmp(value.dptr,__value)==0) {
            bRet=true;
        }

        free(value.dptr);
        free(__value);
    }

    free(pKey);
    return bRet;
}
/* ############################################################################# */
boolean exist_db(const int db,const char *_key){
    datum key;
    GDBM_FILE dbf;
    int iRet=0;
    char *pKey;

    if (!_key) { return false;}

    /* normalize the key */
    pKey=StrToLower(_key);
    if (!pKey) {return false;}

    if ((dbf=get_dbf(db))) {
        key.dptr=pKey;
        key.dsize=strlen(pKey)+1;
    
        pthread_mutex_lock(&dbaccess_mutex[db]);
        iRet=gdbm_exists(dbf,key);
        pthread_mutex_unlock(&dbaccess_mutex[db]);
    }
    free(pKey);

    return iRet;
}
/* ############################################################################# */
char * get_db(const int db,const char *_key){
    datum key,value;
    GDBM_FILE dbf;
    char *str=NULL;
    char *pKey;

    if (!_key) { return NULL;}

    /* normalize the key */
    pKey=StrToLower(_key);
    if (!pKey) {return NULL;}

    if ((dbf=get_dbf(db))) {
    
	    key.dptr=pKey;
	    key.dsize=strlen(pKey)+1;
    
	    pthread_mutex_lock(&dbaccess_mutex[db]);
	    value=gdbm_fetch(dbf,key);
    	pthread_mutex_unlock(&dbaccess_mutex[db]);

	    if (value.dptr) {
    		str=(char *)malloc(value.dsize*sizeof(char));
	    	strcpy(str,value.dptr);
    		free(value.dptr);
	    }
	}

    free(pKey);
    
    return str;
}
/* ############################################################################# */
PQueue list_db(const int db){
    PQueue pList;
	QueueData QueueItem;
	GDBM_FILE dbf;
    datum key,nextkey;
	
	/* initialize the  queue */
	pList=initQueue();

    /* get the database handle */
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

