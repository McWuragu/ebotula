/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

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
    extern boolean stop;
    time_t newTime;
    time_t lastPing=0;
    time_t lastRemoveDeadLogins=0;
    time_t lastRemoveDeadAccounts=0;

    /* set the thread not cancelable */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

    logger(LOG_DEBUG,_("The timing thread is running (pid%d)"),getpid());

    while (!stop) {
        time(&newTime);

        /* ping the server for detection of connection lost */
        if ((newTime-lastPing)>=(sSetup.iTimeout/2)) {
            ping(sSetup.sHostname);
            lastPing=newTime;
        }
        

        /* remove dead logins  */
        if ((newTime-lastRemoveDeadLogins)>=3600) {
            logger(LOG_INFO,_("Remove the dead logins"));
            rmDeadLogins(newTime-sSetup.AutoLoggoff*86400);
            lastRemoveDeadLogins=newTime;
        }

        /* remove dead accounts  */
        if ((newTime-lastRemoveDeadAccounts)>=3600) {
            logger(LOG_INFO,_("Remove the dead accounts"));
            rmDeadAccounts(newTime-sSetup.AccountLiveTime*86400);
            lastRemoveDeadAccounts=newTime;
        }
        
        msleep(100);
    }
    logger(LOG_DEBUG,_("The timing thread is stopped"));
    return NULL;
}

/*####################################################################### */

