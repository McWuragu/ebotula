/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef MESSAGES_H
#define MESSAGES_H

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#define	EOM			'.'

#ifdef LANG_DE
	// Here insert all message in german

#else
	// syslog
	#define SYSLOG_READ_CMD		"Comand line parameters found and read"
	#define SYSLOG_READ_CONFFILE	"Configuration file found and read"
	#define SYSLOG_INIT_DB		"Initialization of the database"
	#define SYSLOG_TRY_CONNECT	"Try connect to %s:%s"
	#define SYSLOG_IS_CONNECT	"Connected to the server"
	#define SYSLOG_BOT_RUN		"Runing...."
	#define SYSLOG_BOT_STOP		"Stopped...."
	#define SYSLOG_THREAD_RUN	"Thread %d is running"
	#define SYSLOG_BOT_START	"Starting..."
	#define SYSLOG_UNKNOWN_CMDID	"Unknown command id %d"
	#define SYSLOG_LOGIN_RM		"The account %s was automatically loged off"
	#define SYSLOG_ACCOUNT_RM       "The account %s is removed from the list"
	#define SYSLOG_FAILED_NETPARA	"The servername or portnumber isn't set."
	#define SYSLOG_MSG_QUEUE	"Can't create the message queue."
	#define SYSLOG_RESOLVE_HOSTNAME	"Can't resolve the hostname."
	#define SYSLOG_RESOLVE_PROTOCOL	"Tcp is a unknown protocol."
	#define SYSLOG_SOCKET		"Can't create a tcp socket."
	#define SYSLOG_CONNECT		"Connection failed."
	#define SYSLOG_CONFIG_FILE	"Can't open the config file."
	#define SYSLOG_SEND		"	Can't send a command."
	#define SYSLOG_RECV			"Can't receive a line."
	#define SYSLOG_TIMEOUT		"Receiving timeout"
	#define SYSLOG_CREATE_DIR	"Create the directory for the database."
	#define SYSLOG_CREAT_DIR_ERR "Can't create the directory for the database."
	#define SYSLOG_DATABASE_ERR	"Can't open the databases."
	#define SYSLOG_RESTART		"Restart..."
	#define SYSLOG_STOPPED		"Bot down..."
	
	// excute error
	// output of strerr
	#define ERR_RESTART				"Can't restart"
	#define ERR_MISSING_PARAM		"Missing value"
	#define ERR_PORT_PARAMETER		"The portvalue is out of range."
	#define ERR_WRONG_SERVERNAME	"The servername is irregular."
	#define ERR_WRONG_BOTNAME		"The botname is irregular."
	#define ERR_ALT_RANGE			"The account live time is invalid."
	#define ERR_PINGTIMEOUT_RANGE	"The ping time is invalid."
	#define ERR_SENDDELAY_RANGE     "The send delay time is invalid."
	#define ERR_LOGOFF_RANGE		"The auto logoff time is invalid."
	#define ERR_THREAD_RANGE		"The threadlimit is out of range."
	#define ERR_NICK				"Can't set the nickname."
	#define ERR_MUTEX				"Can't synchronize the threads."
	#define ERR_NOT_ALLOW_CHAR		"Not allowed characters used."
	
	
	// command acknowledgment
	#define MSG_HELLO				"Nice to meet you."
	#define MSG_HELLO2				"You have got a new account now."
	#define MSG_PASSWD				"Password is changed."
	#define MSG_IDENT_OK			"You're now identified."
	#define MSG_JOIN_OK				"The bot has joined the channel."
	#define MSG_PART_OK				"The bot has part the channel."
	#define MSG_RMCHANNEL_OK		"The channel is removed form the channel list."
	#define MSG_ADDCHANNEL_OK		"The channel is added to the channel list"
	#define MSG_NICK_SET			"The bot tries to set the new nickname."
	#define MSG_LOGOFF				"You're now logged off."
	#define MSG_SET_GREATING		"The new greeting is set"
	#define MSG_RM_GREATING			"The greeting is removed"
	#define MSG_SET_TOPIC			"The new topic is set"
	#define MSG_RM_TOPIC			"The topic is removed"
	#define MSG_KICK_OK				"The user are kicked by the bot"
	#define MSG_USERMODE_OK			"The user mod are changed"
	#define MSG_RMUSER_OK			"The account is removed"
	#define MSG_MASTER				"You are a bot master"
	#define MSG_DIE_OK				"The bot is going  down"
	#define MSG_RESTART_OK			"The bot make a restart"
	
	// Informations for invalid input or using of the bot
	#define MSG_NOT_LOGON			"You're not identified."
	#define MSG_NOT_MASTER			"You're not a irc bot master."
	#define MSG_NOT_OWNER			"You're not the bot owner of this channel."
	#define MSG_NOT_CHANNELOPT		"This command need a channel name."
	#define MSG_NOT_PASS			"You've set a empty password."
	#define MSG_NOT_COMMAND			"No help found for this command."
	#define MSG_NOT_ACCOUNT			"Account isn't found."
	#define MSG_NOT_CHANNEL			"This channel isn't in the channel list."
	#define MSG_NOT_ACCESS			"Access denied or unknown command"
	#define MSG_NOT_SELF			"You can't modify yourself"
	
	// Errors
	#define MSG_ADDCHANNEL_ALREADY	"This channel is already in the channel list."
	#define MSG_NICK_INVALID		"Nickname is invalid."
	#define MSG_NICK_EXIST			"A account with this nickname already exists."
	#define MSG_ALREADY_LOGON		"You're already identified."
	#define MSG_NEED_OP				"I need operator access rights."
	#define MSG_UNKNOWN_MODS		"This mod are invalid."
	#define MSG_MASTER_PASS_ERR		"Typing error..not equal passwords"
	#define MSG_MASTER_ERR			"Can't set this account as master."
	#define MSG_USERLIST_EMPTY		"The userlist is empty"
	#define MSG_UNKNOW_MODE			"Unkown mode parameter"
	
	// other informations
	#define MSG_HELP_END			"End of help."
	#define MSG_HELP_FOR			"Help for"
	#define MSG_USERLIST_BEGIN		"User list:"
	#define MSG_USERLIST_END		"End of user list."
	#define MSG_CHANNELLIST_BEGIN	"Channel list:"
	#define MSG_CHANNELLIST_END		"End of channel list."
	#define MSG_CHANNELLIST_MODE    "Channel modes:"
	#define MSG_CHANNELLIST_TOPIC	"Topic:"
	#define MSG_CHANNELLIST_GREAT	"Greeting:"
	#define MSG_DEFAULT_REASON		"You are kicked from the bot."
	#define MSG_INIT_CHANNEL		"I have initialize this channnel"
	
	// command  format erros
	#define MSG_ADDCHANNEL_ERR		"Invalid command: !addchannel <#channel>"
	#define MSG_JOIN_ERR			"Invalid command: !join <#channel>"
	#define MSG_IDENT_ERR			"Invalid command: !ident <login password>"
	#define MSG_NICK_ERR			"Invalid command: !nick <nickname>"			
	#define MSG_SAY_ERR			    "Invalid command: !say [#channel] <message>"			
	#define MSG_KICK_ERR			"Invalid command: !kick [#channel] <nickname> [reason]"
	#define MSG_USERMODE_ERR		"Invalid command: !usermode [#channel] <login> <+/-><mod>"
	#define MSG_CHANMODE_ERR		"Invalid command: !chamode [#channel] <+/-><mods>"
	
	// Messages for the master dialog
	#define MSG_MASTER_TITLE		"Create a master account\n\n"
	#define MSG_MASTER_LOGIN		"Type the login (max %d): "
	#define MSG_MASTER_PASS			"\nEnter the password(max. %d): "
	#define MSG_MASTER_REPASS		"Reenter the password: "
	
	
	
	#ifdef	CMD_MSG
		static const char *unknow_parameter[]=
		{
			"Unknow option\n",
			"Type ",PACKAGE," -h for help.\n",
			"."
		};
		
		
		
		static const char *help_msg[]=
		{
			"\n",
			PACKAGE," [Options]\n",
			"\n\tOptions:\n",
			"\t-a days\t\tSet the time in days for the automatic logoff.\n",
			"\t-b name\t\tSet the name of the bot.\n",
			"\t-c sec\t\tSet the ping timeout of the connection.\n",
			"\t-d path\t\tUse this path for the database\n",
			"\t-f file\t\tUse this config file.\n",
			"\t-h\t\tPrint this help information.\n",
			"\t-k days\t\tSet the  account live time in days.\n",
			"\t-m\t\tCreate a bot master.\n",
			"\t-n ms\t\tSet the delay time in millisecond for sending.\n",
			"\t-p port\t\tSet the port of the irc server.\n"
			"\t-r string\tSet the output  for a request of the real name.\n",
			"\t-s server\tSet the irc server name.\n",
			"\t-t number\tSet the number of creating  threads.\n"
			"\t-v\t\tPrint version.\n",
			"."
		};
	#endif
#endif

#endif
