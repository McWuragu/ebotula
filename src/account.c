/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

					 
#include "dbaccess.h"
#include "messages.h"
#include "utilities.h"
#include "account.h"
// ############################################################################# 
void rmDeadLogins(long checkTime) {
	char **login;
	char *time,*netmask;
	int i;
    
	// get the  list
	if ((login=list_db(TIMELOG_DB)))	{
		for (i=0;login[i]!=NULL;i++) {
			
			// read time
			time=get_db(TIMELOG_DB,login[i]);
			
			// check the time
			if (checkTime>atol(time)) {
				netmask=get_db(USERTONICK_DB,login[i]);
				
				if (strlen(netmask)) {
					log_out(netmask,login[i]);
                    syslog(LOG_NOTICE,SYSLOG_LOGIN_RM,login[i],netmask);
					free(netmask);
				}
			}
			free(time);
		}
	}

}
// ############################################################################# 
void log_on(char *netmask,char *login) {
	extern pthread_mutex_t account_mutex;
	time_t timestamp;
	char str_timestamp[32];
	char *oldnetmask;

	if (exist_db(USERTONICK_DB,login)) {
		oldnetmask=get_db(USERTONICK_DB,login);

		pthread_mutex_lock(&account_mutex);
		del_db(NICKTOUSER_DB,oldnetmask);
		add_db(NICKTOUSER_DB,netmask,login);
		
		replace_db(USERTONICK_DB,login,netmask);
		pthread_mutex_unlock(&account_mutex);
	} else {
		pthread_mutex_lock(&account_mutex);
		add_db(NICKTOUSER_DB,netmask,login);
		add_db(USERTONICK_DB,login,netmask);
		pthread_mutex_unlock(&account_mutex);
	}
    
	
	DEBUG("User log in");
	
	// build the timestamp
    time(&timestamp);
	sprintf(str_timestamp,"%ld",timestamp);

	// set the last login timestamp
	if (exist_db(TIMELOG_DB,login)) {
		replace_db(TIMELOG_DB,login,str_timestamp);
		DEBUG("Update timestamp %s for %s",str_timestamp,login);
	} else {
		add_db(TIMELOG_DB,login,str_timestamp);
		DEBUG("Add timepstamp %s for %s",str_timestamp,login);
	}
}
// ############################################################################# 
void log_out(char *netmask,char *login) {
	extern pthread_mutex_t account_mutex;

	pthread_mutex_lock(&account_mutex);
	del_db(NICKTOUSER_DB,netmask);
	del_db(USERTONICK_DB,login);
	pthread_mutex_unlock(&account_mutex);
}

