/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef DBACCESS_H
#define DBACCESS_H

#include <gdbm.h>
					
#define USER_DB			1
#define CHANNEL_DB		2
#define ACCESS_DB		3
#define BANLIST_DB		4
#define LOGIN_DB		5
#define TIMELOG_DB		6

#define CHECK_EXIST(_db,__key)			if (exist_db(_db,__key)){return false;}
#define CHECK_NO_EXIST(_db,__key)		if (!exist_db(_db,__key)){return false;}

void init_database(void);
void closeDatabase(void);


// ########################## DB commands ################################
int add_db(int db,char *_key, char *_value);
int replace_db(int db,char *_key, char *_value);
int del_db(int db,char *_key);
int check_db(int db,char *_key,char *_value);
int exist_db(int db,char *_key);
char * get_db(int db,char *_key);
char ** list_db(int db);
GDBM_FILE get_dbf(int db);


#endif
