/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
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
    time_t lastNickServIdent=0;

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
    	
		/* send login to ident server */
		if (sSetup.nEnableNickServIdent!=0 && sSetup.sNickServIdent!=NULL &&(newTime-lastNickServIdent)>=(sSetup.nNickServIdentTimeout/2))
		{
			char *sBuf=NULL;
			
			sBuf=(char*) malloc((strlen("identify")+1+strlen(sSetup.sNickServIdent)+2)*sizeof(char));
			if (NULL != sBuf)
			{
				strcpy(sBuf,"identify ");
				strcat(sBuf,sSetup.sNickServIdent);
				privmsg("nickserv",sBuf);
				free(sBuf);
			}
			lastNickServIdent=newTime;

		}
        msleep(100);
    }
    logger(LOG_DEBUG,_("The timing thread is stopped"));
    return NULL;
}

/*####################################################################### */

