#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>			 
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "utilities.h"
#include "irc.h"
#include "dbaccess.h"
#include "type.h"
#include "timing.h"


void *synchron(void *argv){
	extern ConfType setup;
	time_t newTime;
	time_t lastRemoveDeadLogins=0;
	time_t lastRemoveDeadAccounts=0;

	// set the thread cancelable
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	DEBUG("Synchronize Thread is running");

	while (1) {
        sleep(90);
		time(&newTime);

		ping(setup.server);

		// remove dead logins
		if ((newTime-lastRemoveDeadLogins)>=(setup.AutoLoggoff*86400)) {
			DEBUG("Remove dead logins");
			time(&lastRemoveDeadLogins);
		}

		// remove dead accounts
		if ((newTime-lastRemoveDeadAccounts)>=(setup.AccountLiveTime*86400)) {
			DEBUG("Remove dead accounts");
			time(&lastRemoveDeadAccounts);
		}
    }
}
