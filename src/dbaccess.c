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

#include "utilities.h"
#include "messages.h"
#include "dbaccess.h"

GDBM_FILE dbf_user;
GDBM_FILE dbf_channel;
GDBM_FILE dbf_access;
GDBM_FILE dbf_usertonick;
GDBM_FILE dbf_nicktouser;
GDBM_FILE dbf_banlist;
GDBM_FILE dbf_timelog;

// ############################################################################# 
void init_database(void) {
	extern ConfType	sSetup;

	char *user,*channel,*access,*nicktouser,*usertonick,*banlist,*timelog;
			
	// generate the filenames
	user=(char *)malloc((strlen(sSetup.database_path)+strlen("/user.dbf")+1)*sizeof(char));
	channel=(char *)malloc((strlen(sSetup.database_path)+strlen("/channel.dbf")+1)*sizeof(char));
	usertonick=(char *)malloc((strlen(sSetup.database_path)+strlen("/usertonick.dbf")+1)*sizeof(char));
	nicktouser=(char *)malloc((strlen(sSetup.database_path)+strlen("/nicktouser.dbf")+1)*sizeof(char));
	access=(char *)malloc((strlen(sSetup.database_path)+strlen("/access.dbf")+1)*sizeof(char));
	banlist=(char *)malloc((strlen(sSetup.database_path)+strlen("/banlist.dbf")+1)*sizeof(char));
	timelog=(char *)malloc((strlen(sSetup.database_path)+strlen("/timelog.dbf")+1)*sizeof(char));

	// create filenames
	sprintf(user,"%s/user.dbf",sSetup.database_path);
	sprintf(channel,"%s/channel.dbf",sSetup.database_path);
	sprintf(usertonick,"%s/usertonick.dbf",sSetup.database_path);
	sprintf(nicktouser,"%s/nicktouser.dbf",sSetup.database_path);
	sprintf(access,"%s/access.dbf",sSetup.database_path);
	sprintf(banlist,"%s/banlist.dbf",sSetup.database_path);
	sprintf(timelog,"%s/timelog.dbf",sSetup.database_path);

	// open the databases
	dbf_user=gdbm_open(user,512,GDBM_WRCREAT,0600,NULL);
	dbf_channel=gdbm_open(channel,512,GDBM_WRCREAT,0600,NULL);
	dbf_usertonick=gdbm_open(usertonick,512,GDBM_NEWDB,0600,NULL);
	dbf_nicktouser=gdbm_open(nicktouser,512,GDBM_NEWDB,0600,NULL);
	dbf_access=gdbm_open(access,512,GDBM_WRCREAT,0600,NULL);
	dbf_banlist=gdbm_open(banlist,512,GDBM_WRCREAT,0600,NULL);
	dbf_timelog=gdbm_open(timelog,512,GDBM_WRCREAT,0600,NULL);
	
	if (!dbf_user || !dbf_channel || !dbf_usertonick || !dbf_nicktouser || !dbf_access || !dbf_banlist || !dbf_timelog) {
		errno=EBUSY;
		perror(SYSLOG_DATABASE_ERR);
		exit(errno);
		syslog(LOG_ERR,SYSLOG_DATABASE_ERR);
	}

	syslog(LOG_INFO,SYSLOG_INIT_DB);
}
// ############################################################################# 
void closeDatabase(void) {
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
	
	return true;
}
// ############################################################################# 
boolean replace_db(int db,char *_key, char *_value){
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

	// distinction of user database 
	if (db==USER_DB) {
		value.dptr=crypt(_value,"SL");
	} else {
		value.dptr=_value;
	}

    value.dsize=strlen(value.dptr)+1;

	pthread_mutex_lock(&dbaccess_mutex);
	gdbm_store(dbf,key,value,GDBM_REPLACE);
	pthread_mutex_unlock(&dbaccess_mutex);
	

	return true;

}
// ############################################################################# 
boolean del_db(int db,char *_key){
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

	return true;
}
// ############################################################################# 
boolean check_db(int db,char *_key,char* _value){
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
		return false;
	}
	return true;
}
// ############################################################################# 
boolean exist_db(int db,char *_key){
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

	//CHECK_NO_EXIST(db,_key);

	if (!exist_db(db,_key)){return "";}

	if (!(dbf=get_dbf(db))) {
		return "";
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

	}
	return channellist;
}

