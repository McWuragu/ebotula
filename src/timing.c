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
	time_t timestamp;

	// set the thread cancelable
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	DEBUG("Synchronize Thread is started.");

	while (1) {
        sleep(90);
		time(&timestamp);

		DEBUG("Send ping");
		ping(setup.server);
		
    }

}
