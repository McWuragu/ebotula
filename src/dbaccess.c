#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>


#include "utilities.h"
#include "messages.h"
#include "config.h"
#include "dbaccess.h"

GDBM_FILE dbf_user;
GDBM_FILE dbf_channel;
GDBM_FILE dbf_login;
GDBM_FILE dbf_access;
GDBM_FILE dbf_banlist;
GDBM_FILE dbf_timelog;

// ############################################################################# 
void init_database(void) {
	extern CONFIG_TYPE	setup;

	char *user,*channel,*login,*access,*banlist,*timelog;
			
	// generate the filenames
	user=(char *)malloc((strlen(setup.database_path)+strlen("/user.dbf")+1)*sizeof(char));
	channel=(char *)malloc((strlen(setup.database_path)+strlen("/channel.dbf")+1)*sizeof(char));
	login=(char *)malloc((strlen(setup.database_path)+strlen("/login.dbf")+1)*sizeof(char));
	access=(char *)malloc((strlen(setup.database_path)+strlen("/access.dbf")+1)*sizeof(char));
	banlist=(char *)malloc((strlen(setup.database_path)+strlen("/banlist.dbf")+1)*sizeof(char));
	timelog=(char *)malloc((strlen(setup.database_path)+strlen("/timelog.dbf")+1)*sizeof(char));

	// create filenames
	sprintf(user,"%s/user.dbf",setup.database_path);
	sprintf(channel,"%s/channel.dbf",setup.database_path);
	sprintf(login,"%s/login.dbf",setup.database_path);
	sprintf(access,"%s/access.dbf",setup.database_path);
	sprintf(banlist,"%s/banlist.dbf",setup.database_path);
	sprintf(timelog,"%s/timelog.dbf",setup.database_path);

	// open the databases
	dbf_user=gdbm_open(user,512,GDBM_WRCREAT,0600,NULL);
	dbf_channel=gdbm_open(channel,512,GDBM_WRCREAT,0600,NULL);
	dbf_login=gdbm_open(login,512,GDBM_NEWDB,0600,NULL);
	dbf_access=gdbm_open(access,512,GDBM_WRCREAT,0600,NULL);
	dbf_banlist=gdbm_open(banlist,512,GDBM_WRCREAT,0600,NULL);
	dbf_timelog=gdbm_open(timelog,512,GDBM_WRCREAT,0600,NULL);
	DEBUG("Initizale the database");
}
// ############################################################################# 
void closeDatabase(void) {
	// close the databases
	gdbm_close(dbf_user);
	gdbm_close(dbf_channel);
	gdbm_close(dbf_login);
	gdbm_close(dbf_access);
	gdbm_close(dbf_banlist);
	gdbm_close(dbf_timelog);
	DEBUG("Close databases");
}

//######################### database access ##############################
GDBM_FILE get_dbf(int db) {

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
	case LOGIN_DB:
		return dbf_login;
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
int add_db(int db,char *_key, char *_value) {
	datum key,value;
	GDBM_FILE dbf;
	extern pthread_mutex_t dbaccess_mutex;

	// check of exist  of this key in the database
    CHECK_EXIST(db,_key);

	// get the datebase handle
	if (!(dbf=get_dbf(db))) {
		return false;
	}
	
	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;

	// if try to use  the  user database then make  a crypt value
	if (db==USER_DB) {
		value.dptr=crypt(_value,"SL");
	} else {
		value.dptr=_value;
	}
	
    value.dsize=strlen(value.dptr)+1;
	pthread_mutex_lock(&dbaccess_mutex);
	gdbm_store(dbf,key,value,GDBM_INSERT);
	pthread_mutex_unlock(&dbaccess_mutex);
	
	DEBUG("Add datum to database");

	return true;
}
// ############################################################################# 
int replace_db(int db,char *_key, char *_value){
	datum key,value;
	GDBM_FILE dbf;
	extern pthread_mutex_t dbaccess_mutex;
	
	CHECK_NO_EXIST(db,_key);
    
	// get the datebase handle
	if (!(dbf=get_dbf(db))) {
		return false;
	}

	// build key
	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;

	
	if (db==USER_DB) {
		value.dptr=crypt(_value,"SL");
	} else {
		value.dptr=_value;
	}
    value.dsize=strlen(value.dptr)+1;

	pthread_mutex_lock(&dbaccess_mutex);
	gdbm_store(dbf,key,value,GDBM_REPLACE);
	pthread_mutex_unlock(&dbaccess_mutex);
	
	DEBUG("Replace datum to database");

	return true;

}
// ############################################################################# 
int del_db(int db,char *_key){
	datum key;
	GDBM_FILE dbf;
	extern pthread_mutex_t dbaccess_mutex;

	CHECK_NO_EXIST(db,_key);

	if (!(dbf=get_dbf(db))) {
		return false;
	}

	// build the  key
	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;
    
	
	pthread_mutex_lock(&dbaccess_mutex);
	gdbm_delete(dbf,key);
	pthread_mutex_unlock(&dbaccess_mutex);

	DEBUG("Datum removed");
	return true;
}
// ############################################################################# 
int check_db(int db,char *_key,char* _value){
	datum key;
	datum value;
	char *__value;
	GDBM_FILE dbf;
	extern pthread_mutex_t dbaccess_mutex;

	key.dptr=_key;
	key.dsize=strlen(_key)+1;
	
	CHECK_NO_EXIST(db,_key);
	
	if (!(dbf=get_dbf(db))) {
		return false;
	}

	// fetch the entry
	pthread_mutex_lock(&dbaccess_mutex);
	value=gdbm_fetch(dbf,key);
	pthread_mutex_unlock(&dbaccess_mutex);

	if (db==USER_DB) {
		__value=crypt(_value,"SL");	
	} else {
		__value=_value;
    }


	if (strcmp(value.dptr,__value)) {
		DEBUG("Datum not equal");
		return false;
	}
	DEBUG("Datum equal");
	return true;
}
// ############################################################################# 
int exist_db(int db,char *_key){
	datum key;
	GDBM_FILE dbf;
	extern pthread_mutex_t dbaccess_mutex;

	if (!(dbf=get_dbf(db))) {
		return false;
	}

	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;

	pthread_mutex_lock(&dbaccess_mutex);
	if (!gdbm_exists(dbf,key)){
		pthread_mutex_unlock(&dbaccess_mutex);
		
		return false;
	}
	pthread_mutex_unlock(&dbaccess_mutex);
	
	return true;
}
// ############################################################################# 
char * get_db(int db,char *_key){
	datum key,value;
	GDBM_FILE dbf;
	char *str;
	extern pthread_mutex_t dbaccess_mutex;

	CHECK_NO_EXIST(db,_key);

	if (!(dbf=get_dbf(db))) {
		return false;
	}

	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;
	
	pthread_mutex_lock(&dbaccess_mutex);
	value=gdbm_fetch(dbf,key);
	pthread_mutex_unlock(&dbaccess_mutex);

	str=(char *)malloc(value.dsize*sizeof(char));
	strcpy(str,value.dptr);
	
	free(value.dptr);
	
	return str;
}
// ############################################################################# 
char ** list_db(int db){
	char ** channellist;
	GDBM_FILE dbf;
	datum key,nextkey;
	unsigned int count=0,i;
	extern pthread_mutex_t dbaccess_mutex;

	// get the database handle
	if (!(dbf=get_dbf(db))) {
		return NULL;
	}
	
	pthread_mutex_lock(&dbaccess_mutex);
	key=gdbm_firstkey(dbf);
	pthread_mutex_unlock(&dbaccess_mutex);
	
	if (key.dptr) {
        // calculat the  size of  database
		do {
			count++;
			pthread_mutex_lock(&dbaccess_mutex);
			nextkey=gdbm_nextkey(dbf,key);
			pthread_mutex_unlock(&dbaccess_mutex);
			free(key.dptr);
			key=nextkey;
	
		} while ( key.dptr );
	}


	DEBUG("%d channels found",count);

	// allocat the memory and set  end mark
	channellist=(char **)malloc((count+1)*sizeof(char *));
	channellist[count]=NULL;

	pthread_mutex_lock(&dbaccess_mutex);
	key=gdbm_firstkey(dbf);
	pthread_mutex_unlock(&dbaccess_mutex);

	for (i=0;i<count;i++) {
		channellist[i]=(char *)malloc(key.dsize*sizeof(char));
		strcpy(channellist[i],key.dptr);
		
		pthread_mutex_lock(&dbaccess_mutex);
		nextkey=gdbm_nextkey(dbf,key);
		pthread_mutex_unlock(&dbaccess_mutex);

		free(key.dptr);
		key=nextkey;

		DEBUG("Found channel %s",channellist[i]);
	}

	return channellist;
}



