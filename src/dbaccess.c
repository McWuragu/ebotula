#include <gdbm.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>

#include "macro.h"
#include "messages.h"
#include "config.h"
#include "dbaccess.h"

GDBM_FILE dbf_user;
GDBM_FILE dbf_channel;
GDBM_FILE dbf_login;
GDBM_FILE dbf_access;
GDBM_FILE dbf_banlist;


void init_database(void) {
	extern CONFIG_TYPE	setup;

	char *user,*channel,*login,*access,*banlist;
			
	// generate the filenames
	user=malloc(sizeof(char)*(strlen(setup.database_path)+strlen("/user.dbf")+1));
	channel=malloc(sizeof(char)*(strlen(setup.database_path)+strlen("/channel.dbf")+1));
	login=malloc(sizeof(char)*(strlen(setup.database_path)+strlen("/login.dbf")+1));
	access=malloc(sizeof(char)*(strlen(setup.database_path)+strlen("/access.dbf")+1));
	banlist=malloc(sizeof(char)*(strlen(setup.database_path)+strlen("/banlist.dbf")+1));

	// create filenames
	sprintf(user,"%s/user.dbf",setup.database_path);
	sprintf(channel,"%s/channel.dbf",setup.database_path);
	sprintf(login,"%s/login.dbf",setup.database_path);
	sprintf(access,"%s/access.dbf",setup.database_path);
	sprintf(banlist,"%s/banlist.dbf",setup.database_path);

	// open the databases
	dbf_user=gdbm_open(user,512,GDBM_WRCREAT,0600,NULL);
	dbf_channel=gdbm_open(channel,512,GDBM_WRCREAT,0600,NULL);
	dbf_login=gdbm_open(login,512,GDBM_NEWDB,0600,NULL);
	dbf_access=gdbm_open(access,512,GDBM_WRCREAT,0600,NULL);
	dbf_banlist=gdbm_open(banlist,512,GDBM_WRCREAT,0600,NULL);
	DEBUG("Initizale the database");
}

void closeDatabase(void) {
	// close the databases
	gdbm_close(dbf_user);
	gdbm_close(dbf_channel);
	gdbm_close(dbf_login);
	gdbm_close(dbf_access);
	gdbm_close(dbf_banlist);
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
	default:
		DEBUG("Unkown database");
		return 0;

	}
}

int add_db(int db,char *_key, char *_value) {
	datum key,value;
	GDBM_FILE dbf;

	// check of exist  of this key in the database
	if (exist_db(db,_key)){
		return false;
	}

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

	gdbm_store(dbf,key,value,GDBM_INSERT);
	DEBUG("Add datum to database");

	return true;
}

int replace_db(int db,char *_key, char *_value){
	datum key,value;
	GDBM_FILE dbf;

	
	if (!exist_db(db,_key)){
		return false;
	}


	if (!(dbf=get_dbf(db))) {
		return false;
	}
	


	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;

	
	if (db==USER_DB) {
		value.dptr=crypt(_value,"SL");
	} else {
		value.dptr=_value;
	}
    value.dsize=strlen(value.dptr)+1;

	gdbm_store(dbf,key,value,GDBM_REPLACE);
	DEBUG("Replace datum to database");

	return true;

}

int del_db(int db,char *_key){
	datum key;
	GDBM_FILE dbf;

	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;
	
	if (!exist_db(db,_key)){
		return false;
	}

	if (!(dbf=get_dbf(db))) {
		return false;
	}
	
	gdbm_delete(dbf,key);
	DEBUG("Datum removed");
	return true;
}

int check_db(int db,char *_key,char* _value){
	datum key;
	datum value;
	char *__value;
	GDBM_FILE dbf;

	key.dptr=_key;
	key.dsize=strlen(_key)+1;
	
	if (!exist_db(db,_key)){
		return false;
	}
	
	if (!(dbf=get_dbf(db))) {
		return false;
	}

	value=gdbm_fetch(dbf,key);

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

int exist_db(int db,char *_key){
	datum key;
	GDBM_FILE dbf;
	
	if (!(dbf=get_dbf(db))) {
		return false;
	}

	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;

	if (!gdbm_exists(dbf,key)){
		DEBUG("Datum not exist");
		return false;
	}
	DEBUG("Datum exist");
	return true;
}

char * get_db(int db,char *_key){
	datum key,value;
	GDBM_FILE dbf;
	char *datum;
	
	if (!exist_db(db,_key)){
		return NULL;
	}

	if (!(dbf=get_dbf(db))) {
		return false;
	}

	key.dptr=_key;
	key.dsize=strlen(key.dptr)+1;
	
	value=gdbm_fetch(dbf,key);

	datum=malloc(sizeof(char)*value.dsize);
	strcpy(datum,value.dptr);
	
	free(value.dptr);
	
	return datum;
}

char ** list_db(int db){
	char ** channellist;
	GDBM_FILE dbf;
	datum key,nextkey;
	unsigned int count=0,i;
	
	// get the database handle
	if (!(dbf=get_dbf(db))) {
		return NULL;
	}

	key=gdbm_firstkey(dbf);
	if (!key.dptr) {
		return NULL;
	}

	// calculat the  size of  database
	do {
		count++;
		nextkey=gdbm_nextkey(dbf,key);
		free(key.dptr);
		key=nextkey;

    } while ( key.dptr );

	DEBUG("%d channels found",count);

	// allocat the memory and set  end mark
	channellist=malloc(sizeof(char *)*(count+1));
	channellist[count]=NULL;

	key=gdbm_firstkey(dbf);

	for (i=0;i<count;i++) {
		channellist[i]=malloc(sizeof(char)*key.dsize);
		strcpy(channellist[i],key.dptr);
		
		nextkey=gdbm_nextkey(dbf,key);
		free(key.dptr);
		key=nextkey;

		DEBUG("Found channel %s",channellist[i]);
	}

	return channellist;
}



