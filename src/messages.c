

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
#ifndef NDEBUG
    assert(MsgID==Msg[MsgID].MsgID);
#endif
    return Msg[MsgID].MsgString;   
}

char * getSyslogString(eSyslogID_t const MsgID){
#ifndef NDEBUG
    assert(MsgID==Msg[MsgID].MsgID);
#endif
    return Syslog[MsgID].MsgString;
}

char ** getCmdLineHelp() {
    return help_msg;
}

char ** getCmdLineError() {
    return unknow_parameter;
}
