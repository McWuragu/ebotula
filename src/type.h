/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef TYPE_H
#define TYPE_H

/** the size of  the receive buffer */
#define RECV_BUFFER_SIZE		0x1000
/** the size of the buffer for the  hostname */
#define HOSTNAME_BUFFER_SIZE	0xFF
/** the  maximum length which read form the configuration file */
#define MAX_READ_BUFFER_SIZE 	256

/// the number of channel mode options 
#define MAX_MODES				11
#define _MAX_MODES				MAX_MODES+2


/** This is definetion of boolean datatype */
typedef enum boolEnum {false,true} boolean;
		
/// This stuctur is a data container for  all setup parameters.
typedef struct ConfStruct{
	/** the nickname of the bot */
	char *botname;
	/** the string  for  the field realname */
	char *realname;
	/** the url of the  irc server */
	char *server;
	/** the port for connection to the server */
	char *port;
	/** the path to a not default configuration file */
	char *configfile;
	/** the path to the database */
	char *pDatabasePath;
	/** number of threads which at the begin was create */
	unsigned int thread_limit;
	/** the limit for the existenz of a account in days */
	unsigned int AccountLiveTime;
	/** the limit for login in days */
	unsigned int AutoLoggoff;
	/** the delay time for sending in  millisecond */
	int sendDelay;
	/// the connection timeout in second
	int iTimeout;
	/** a flag to signal that a new master want create */
	boolean newMaster;
} ConfType;
	  
/**
* This enum define all comand ids for the events and commands. This is used by the
* parser and the threads for execution the  commands or handle the events. The ids identify
* the comands or events in the message queue. 
*/	  
typedef enum CmdEnum {
	CMD_NONE,
	CMD_ONPING, 
	CMD_ONQUIT,
	CMD_ONNICKCHG,
	CMD_ONMODE,
	CMD_ONJOIN,
	CMD_ONNAMES,
	CMD_ONTOPIC,
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
    CMD_SET_GREATING,
	CMD_SET_TOPIC,
	CMD_SAY,
	CMD_KICK,
	CMD_USERMODE,
	CMD_RMUSER,
	CMD_VIEWGREAT,
	CMD_USERLIST,
	CMD_ALLSAY,
	CMD_CHANMODE,
	CMD_RESTART
}CmdType;

/// This  enum is  used as index for value pModeStr from the struct ChannelModeStruct
enum ChanModeEnum {
	MOD_TYPE,
	MOD_KEYWORD,
	MOD_LIMIT,
	MOD_TOPIC,
	MOD_MESSAGE,
	MOD_MODERAT,
	MOD_PRIVAT,
	MOD_SECRET,
	MOD_ANONYM,
	MOD_QUIET,
	MOD_REOP,
	MOD_INVITE
};

/// This is the structure for the entries in the message queue
typedef struct MsgBufStruct {
	long	mtype;
	CmdType	identify;
	/** the complete string which receive from the irc server */
	char	pMsgLine[RECV_BUFFER_SIZE];
} MsgBufType;



/// a structure as container for the channel modes
typedef struct ChannelModeStruct {
	char pModeStr[_MAX_MODES];
	char *pKeyword;
	char *pLimit;
} ChannelModeType;

/// This container is used for  the values from a channel
typedef struct ChannelDataStruct {
	ChannelModeType* pModes;
	char *pTopic;
	char *pGreeting;
} ChannelDataType;
#endif
