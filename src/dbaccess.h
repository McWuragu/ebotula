#ifndef DBACCESS_H
#define DBACCESS_H

#include <gdbm.h>

#define USER_DB			1
#define CHANNEL_DB		2
#define ACCESS_DB		3
#define BANLIST_DB		4
#define LOGIN_DB		5


void init_database(void);
void closeDatabase(void);

int add_db(int db,char *_key, char *_value);
int replace_db(int db,char *_key, char *_value);
int del_db(int db,char *_key);
int check_db(int db,char *_key,char *_value);
int exist_db(int db,char *_key);
char * get_db(int db,char *_key);
char ** list_db(int db);
GDBM_FILE get_dbf(int db);


#endif
