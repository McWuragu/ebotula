/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>             
#include <string.h>
#include <errno.h>
#include <pthread.h>
 
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif
 
#include "utilities.h"
#include "irc.h"
#include "type.h"
#include "account.h"
#include "timing.h"


void *TimingThread(void *argv){
    extern ConfigSetup_t sSetup;
    extern int iLineCount;
    time_t newTime;
    time_t lastPing=0;
    time_t lastLineCount=0;
    time_t lastRemoveDeadLogins=0;
    time_t lastRemoveDeadAccounts=0;

    // set the thread cancelable
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    DEBUG("Synchronize Thread is running");

    while (1) {
        sleep(1);
        
        time(&newTime);

        if ((newTime-lastPing)>=(sSetup.iTimeout)) {
            ping(sSetup.server);
            lastPing=newTime;
        }
        

        // remove dead logins
        if ((newTime-lastRemoveDeadLogins)>=3600) {
            DEBUG("Remove dead logins");
            rmDeadLogins(newTime-sSetup.AutoLoggoff*86400);
            lastRemoveDeadLogins=newTime;
        }

        // remove dead accounts
        if ((newTime-lastRemoveDeadAccounts)>=3600) {
            DEBUG("Remove dead accounts");
            rmDeadAccounts(newTime-sSetup.AccountLiveTime*86400);
            lastRemoveDeadAccounts=newTime;
        }
        
        // increment the line count
        if ((newTime-lastLineCount)>=((sSetup.iSendSafeDelay*2)/1000)) {
            if (iLineCount>=1) {
                iLineCount--;
            }
            lastLineCount=newTime;
        }

    }
}

//#######################################################################

