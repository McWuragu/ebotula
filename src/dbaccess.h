/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#ifndef DBACCESS_H
#define DBACCESS_H

#include <gdbm.h>
#include "type.h"
#include "queue.h"

#define CHECK_EXIST(_db,__key)          if (exist_db(_db,__key)){return false;}
#define CHECK_NO_EXIST(_db,__key)       if (!exist_db(_db,__key)){return false;}

enum database_id_e {
	USER_DB, 
	CHANNEL_DB,
	ACCESS_DB,
	BANLIST_DB,
	USERTONICK_DB,
	TIMELOG_DB,
	NICKTOUSER_DB,
	MAX_DB
};
void initDatabases(void);
void closeDatabase(void);


// ########################## DB commands ################################
boolean add_db(int db,char *_key, char *_value);
boolean replace_db(int db,char *_key, char *_value);
boolean del_db(int db,char *_key);
boolean check_db(int db,char *_key,char *_value);
boolean exist_db(int db,char *_key);
char * get_db(int db,char *_key);
PQueue list_db(int db);
static  GDBM_FILE get_dbf(int db);




#endif
