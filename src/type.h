#ifndef TYPE_H
#define TYPE_H

#define RECV_BUFFER_SIZE		0x1000
#define HOSTNAME_BUFFER_SIZE	0xFF
#define MAX_READ_BUFFER_SIZE 	256



typedef enum boolEnum {false,true} boolean;
		
typedef struct ConfStruct{
	char *botname;
	char *realname;
	char *server;
	char *port;
	char *configfile;
	char *database_path;
	unsigned int thread_limit;
	unsigned int AccountLiveTime;
	unsigned int AutoLoggoff;
	int sendDelay;
	boolean newMaster;
} ConfType;
	  
	  
typedef enum CmdEnum {
	CMD_NONE,
	CMD_PING, 
	CMD_HELP,
    CMD_VERSION,
    CMD_HELLO,
	CMD_PASS,
    CMD_IDENT,
	CMD_ADDCHANNEL,
	CMD_JOIN,
	CMD_PART,
	CMD_RMCHANNEL,
	CMD_LOGOFF,
	CMD_DIE,
	CMD_NICK,
	CMD_CHANNELS,
	CMD_NAMES,
	CMD_GREATING,
	CMD_SET_GREATING,
	CMD_TOPIC,
	CMD_SET_TOPIC,
	CMD_SAY,
	CMD_KICK,
	CMD_USERMODE
}CmdType;

			  
typedef struct MsgBufStruct {
	long	mtype;
	CmdType	identify;
	char	msg_line[RECV_BUFFER_SIZE];
} MsgBufType;

#endif
