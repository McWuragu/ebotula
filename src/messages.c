

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

char * getSyslogString(eSyslogID_t const MsgID){
    assert(MsgID==Msg[MsgID].MsgID);
    return Syslog[MsgID].MsgString;
}

char ** getCmdLineHelp() {
    return help_msg;
}

char ** getCmdLineError() {
    return unknow_parameter;
}
