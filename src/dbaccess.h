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
#include "type.h"
					
#define USER_DB			1
#define CHANNEL_DB		2
#define ACCESS_DB		3
#define BANLIST_DB		4
#define USERTONICK_DB	5
#define TIMELOG_DB		6
#define NICKTOUSER_DB	7

#define CHECK_EXIST(_db,__key)			if (exist_db(_db,__key)){return false;}
#define CHECK_NO_EXIST(_db,__key)		if (!exist_db(_db,__key)){return false;}

void initDatabases(void);
void closeDatabase(void);


// ########################## DB commands ################################
boolean add_db(int db,char *_key, char *_value);
boolean replace_db(int db,char *_key, char *_value);
boolean del_db(int db,char *_key);
boolean check_db(int db,char *_key,char *_value);
boolean exist_db(int db,char *_key);
char * get_db(int db,char *_key);
char ** list_db(int db);
GDBM_FILE get_dbf(int db);




#endif
