

#include <assert.h>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef LANG_DE
    #include "lang/messages_de.h"
#else 
    #include "lang/messages_eng.h"
#endif

#include "messages.h"

char * getMsgString(eMsgID_t const MsgID){
    assert(MsgID==Msg[MsgID].MsgID);
    return Msg[MsgID].MsgString;   
}

void printCmdLineHelp() {

		printf(gettext("\n%s [Options]\n"),PACKAGE);
		printf(gettext("\n\tOptions:\n"));
		printf(gettext("\t-a days\t\tSet the time in days for the automatic logoff.\n"));
		printf(gettext("\t-b name\t\tSet the name of the bot.\n"));
		printf(gettext("\t-c sec\t\tSet the ping timeout of the network connection.\n"));
		printf(gettext("\t-d path\t\tSet the path of the database direcotry\n"));
        printf(gettext("\t-D Level\tSet the log level (0-7)\n"));
		printf(gettext("\t-e ms\t\tSet the delay time in millisecond for the slow sending.\n"));
       	printf(gettext("\t-f file\t\tSet the config file.\n"));
	    printf(gettext("\t-g group\tSet the group under wich the bot run (only root).\n"));
        printf(gettext("\t-h\t\tPrint this help information.\n"));
		printf(gettext("\t-i sec\t\tSet the delay of the channel initialization.\n"));
        printf(gettext("\t-k days\t\tSet the account live time in days.\n"));
        printf(gettext("\t-l char/min\tSet the charakter limit for the first sending delay level.\n"));
		printf(gettext("\t-m\t\tStart the dialog for creating of a new master.\n"));
		printf(gettext("\t-n ms\t\tSet the delay time in millisecond for the fast sending.\n"));
		printf(gettext("\t-p port\t\tSet the portnumber of the irc server.\n"));
		printf(gettext("\t-r string\tSet the output for a request of the real name.\n"));
		printf(gettext("\t-s server\tSet the irc server name.\n"));
		printf(gettext("\t-t count\tSet the number of created  threads.\n"));
		printf(gettext("\t-u user\t\tSet the user under which the bot run (root only).\n"));
        printf(gettext("\t-v\t\tPrint version.\n"));
}
