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
	gdbm_close(dbf_user);
	gdbm_close(dbf_channel);
	gdbm_close(dbf_login);
	gdbm_close(dbf_access);
	DEBUG("Close databases");
}

//######################### User database ##############################


int add_user(char *login, char* passwd){
	datum key,value;

	if (exists_user(login)){
		return false;
	}

	key.dptr=login;
	key.dsize=strlen(login)+1;


	value.dptr=crypt(passwd,"SL");
	value.dsize=strlen(value.dptr)+1;

	gdbm_store(dbf_user,key,value,GDBM_INSERT);
	DEBUG("User added");

	return true;
}

int replace_passwd(char *login, char* passwd){
	datum key,value;

	if (!exists_user(login)){
		return false;
	}

	key.dptr=login;
	key.dsize=strlen(login)+1;


	value.dptr=crypt(passwd,"SL");
	value.dsize=strlen(value.dptr)+1;

	gdbm_store(dbf_user,key,value,GDBM_REPLACE);
	DEBUG("Password change");
	return true;
}

int exists_user(char *login){
	datum key;

	key.dptr=login;
	key.dsize=strlen(key.dptr)+1;

	if (!gdbm_exists(dbf_user,key)){
		DEBUG("User not exist");
		return false;
	}
	DEBUG("User exist");
	return true;

}

int check_user(char *login, char *passwd) {
	datum key;
	datum value;

	key.dptr=login;
	key.dsize=strlen(login)+1;
	
	if (!exists_user(login)){
		return false;
	}
	
	value=gdbm_fetch(dbf_user,key);

	if (strcmp(value.dptr,crypt(passwd,"SL"))) {
		DEBUG("Password isn't right");
		return false;
	}
	DEBUG("Password is right");
	return true;
}

int del_user(char *login) {
	datum key;

	key.dptr=login;
	key.dsize=strlen(login)+1;
	
	if (!exists_user(login)){
		return false;
	}

	gdbm_delete(dbf_user,key);
	DEBUG("User removed");
	return true;
}

//######################### Login database #############################

int add_login(char *netmask,char *login){
	datum key,value;

	key.dptr=netmask;
	key.dsize=strlen(key.dptr)+1;

	if (check_login(netmask)){
		return false;
	}

	value.dptr=login;
	value.dsize=strlen(value.dptr)+1;

	gdbm_store(dbf_login,key,value,GDBM_INSERT);
	DEBUG("User logon");

	return true;
}

int check_login(char *netmask){
	datum key;

	key.dptr=netmask;
	key.dsize=strlen(key.dptr)+1;

	if (!gdbm_exists(dbf_login,key)){
		DEBUG("User isn't logon");
		return false;
	}
	DEBUG("User is logon");
	return true;
}

int del_login(char *netmask) {
	datum key;
    
	if (!check_login(netmask)){
		return false;
	}
    
	key.dptr=netmask;
	key.dsize=strlen(key.dptr)+1;
	gdbm_delete(dbf_login,key);
	DEBUG("User is logoff");
	return true;
}

char *get_login(char *netmask){
	datum key,value;
	char *login;
	
	if (!check_login(netmask)){
		return NULL;
	}

	key.dptr=netmask;
	key.dsize=strlen(key.dptr)+1;
	value=gdbm_fetch(dbf_login,key);

	login=malloc(sizeof(char)*value.dsize);
	strcpy(login,value.dptr);

	return login;
}


//######################### Access database ###########################
int add_channel_access(char *name,char *access) {
	datum key,value;
	if (check_channel_access(name)) {
		DEBUG("Already exist this access entry");
		return false;
	}

	key.dptr=name;
	key.dsize=strlen(key.dptr)+1;

	value.dptr=access;
	value.dsize=strlen(value.dptr)+1;
	
	gdbm_store(dbf_access,key,value,GDBM_INSERT);
	DEBUG("Set the access rights");
	return true;
}

int replace_channel_access(char *name, char *access) {
	datum key,value;
	
	if (!check_channel_access(name)) {
		DEBUG("This access entry not found");
		return false;
	}

	key.dptr=name;
	key.dsize=strlen(key.dptr)+1;

	value.dptr=access;
	value.dsize=strlen(value.dptr)+1;

	gdbm_store(dbf_login,key,value,GDBM_REPLACE);
	DEBUG("The access rights replaced");
	return true;
}

int check_channel_access(char *name){
	datum key;

	key.dptr=name;
	key.dsize=strlen(key.dptr)+1;

	if (!gdbm_exists(dbf_access,key)){
		DEBUG("Not access");
		return false;
	}
	DEBUG("Access");
	return true;
}

int del_channel_access(char *name){
	datum key;

	if (!check_channel_access(name)) {
		DEBUG("This access entry not found");
		return false;
	}

	key.dptr=name;
	key.dsize=strlen(key.dptr)+1;

	gdbm_delete(dbf_access,key);
	DEBUG("Channel access entry remove");
	return true;
}

char *get_channel_access(char *name){
	datum key,value;
	char *access;

	if (!check_channel_access(name)) {
		DEBUG("This access entry not found");
		return false;
	}	

	key.dptr=name;
	key.dsize=strlen(key.dptr)+1;

	value=gdbm_fetch(dbf_access,key);

	access=malloc(sizeof(char)*value.dsize);
	strcpy(access,value.dptr);


	return access;
}

//######################### Access database ###########################

int add_channel(char *channel,char *mode){
	return true;
}

int replace_channel(char *channel, char *mode){ 
	return true;
}

int check_channel(char *channel){
	datum key;

	key.dptr=channel;
	key.dsize=strlen(key.dptr)+1;

	if (!gdbm_exists(dbf_channel,key)){
		DEBUG("Not channel found");
		return false;
	}
	DEBUG("Channel found");
	return true;
}

int del_channel(char *channel){
	datum key;

	if (!check_channel(channel)) {
		return false;
	}

	key.dptr=channel;
	key.dsize=strlen(key.dptr)+1;

	gdbm_delete(dbf_channel,key);
	DEBUG("Channel remove");
	return true;
}

char *get_channel(char *channel){
	return NULL;
}

