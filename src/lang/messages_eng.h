/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * #############################################################
 */

#ifndef MESSAGES_ENG_H
#define MESSAGES_ENG_H

#include "../messages.h"

static MsgStruct_t  Syslog[] = {
	// syslog
	{ SYSLOG_READ_CMD,		"Command line parameters found and read" },
	{ SYSLOG_READ_CONFFILE,		"Configuration file found and read" },
	{ SYSLOG_INIT_DB,		"Initialization of the database" },
	{ SYSLOG_TRY_CONNECT,		"Trying to connect" },
	{ SYSLOG_IS_CONNECT,		"Connected to the server" },
	{ SYSLOG_BOT_RUN,		"Running...."  },
	{ SYSLOG_BOT_STOP,		"Stopped...." },
	{ SYSLOG_BOT_START,		"Starting..." },
	{ SYSLOG_UNKNOWN_CMDID,		"Unknown command id" },
	{ SYSLOG_ACCOUNT_RM,		"An account is removed from the list" },
	{ SYSLOG_FAILED_NETPARA,	"The servername or portnumber isn't set." },
	{ SYSLOG_MSG_QUEUE,		"Can't create the message queue." },
	{ SYSLOG_RESOLVE_HOSTNAME,	"Can't resolve the hostname." },
	{ SYSLOG_RESOLVE_PROTOCOL,	"TCP is an unknown protocol." },
	{ SYSLOG_SOCKET,		"Can't create a tcp socket." },
	{ SYSLOG_CONNECT,		"Connection failed." },
	{ SYSLOG_CONFIG_FILE,		"Can't open the config file." },
	{ SYSLOG_SEND,			"Can't send a command." },
	{ SYSLOG_RECV,			"Can't receive a line." },
	{ SYSLOG_TIMEOUT,		"Receiving timeout" },
	{ SYSLOG_CREATE_DIR,		"Create the directory for the database." },
	{ SYSLOG_CREATE_DIR_ERR, 	"Can't create the directory for the database." },
	{ SYSLOG_DATABASE_ERR,		"Can't open the databases." },
    { SYSLOG_RESTART,		"Restart..." },
	{ SYSLOG_STOPPED,		"Bot down..." }
};

static MsgStruct_t Msg[] = {
	// Messages for the master dialog
	{ INFO_MASTER_TITLE,		"Create a master account"},
	{ INFO_MASTER_LOGIN,		"Type the login: " },
	{ INFO_MASTER_PASS,		"Enter the password: "},
	{ INFO_MASTER_REPASS,		"Reenter the password: "},
	{ ERR_MASTER_PASS,		"Typing error... passwords not equal" },
	{ ERR_MASTER,			"Can't set this account as master." },

    // output of strerr
	{ ERR_RESTART,			"Can't restart"},
	{ ERR_MISSING_PARAM,		"Missing value"},
	{ ERR_PORT_PARAMETER,		"The portvalue is out of range."},
	{ ERR_WRONG_SERVERNAME,		"The servername is irregular."},
	{ ERR_WRONG_BOTNAME,		"The botname is irregular."},
	{ ERR_ALT_RANGE,		"The account living time is invalid."},
	{ ERR_PINGTIMEOUT_RANGE,	"The ping time is invalid."},
	{ ERR_LOGLEVEL_RANGE,        "The log level is invalid."},
    { ERR_SENDDELAY_RANGE,		"The send delay time is invalid."},
	{ ERR_SENDLINE_RANGE,		"The limit for sending with the first send delay is invalid." },
    { ERR_INIT_DELAY,           "The delay of the startup initialization is invalid."},
    { ERR_LOGOFF_RANGE,			"The auto logoff time is invalid."},
	{ ERR_THREAD_RANGE,		"The threadlimit is out of range." },
	{ ERR_NICK,			"Can't set the nickname."},
	{ ERR_MUTEX,			"Can't synchronize the threads." },
	{ ERR_NOT_ALLOW_CHAR,		"Forbidden characters used." },


	// command acknowledgment
	{ OK_HELLO,			"Nice to meet you."},
	{ OK_HELLO2,			"You have got a new account now."},
	{ OK_PASSWD,			"Password is changed."},
	{ OK_IDENT,			"You're now identified." },
	{ OK_JOIN,			"The bot has joined the channel."},
	{ OK_PART,			"The bot has left the channel."},
	{ OK_RMCHANNEL,			"The channel is removed from the channel list."},
	{ OK_ADDCHANNEL,		"The channel is added to the channel list"},
	{ OK_NICK_SET,			"The bot tries to set the new nickname." },
	{ OK_LOGOFF,			"You're now logged off."},
	{ OK_SET_GREETING,		"The new greeting is set" },
	{ OK_RM_GREETING,		"The greeting is removed"},
	{ OK_SET_TOPIC,			"The new topic is set"},
	{ OK_RM_TOPIC,			"The topic is removed"},
	{ OK_KICK,			"The user is kicked by the bot" },
	{ OK_BAN,			"The user is banned by the bot" },
    	{ OK_DEBAN,			"The ban is removed by the bot" },
	{ OK_USERMODE,			"The user mod are changed" },
	{ OK_RMUSER,			"The account is removed"},
	{ OK_MASTER,			"You are a bot master"},
	{ OK_DIE,			"The bot is going  down"},
	{ OK_RESTART,			"The bot is going to restart"},

	// Informations for invalid input or using of the bot
	{ ERR_NOT_LOGON,			"You're not identified."},
	{ ERR_NOT_MASTER,			"You're not a irc bot master."},
	{ ERR_NOT_OWNER,			"You're not the bot owner of this channel."},
    	{ ERR_NOT_FRIEND,			"You're not a friend of this channel."},
	{ ERR_NOT_CHANNELOPT,		"This command requieres a channel name."},
    	{ ERR_NOT_PARAMETER,       	"I couldn't found regulare parameters."},
	{ ERR_NOT_COMMAND,		"No help found for this command."},
	{ ERR_NOT_ACCOUNT,		"Account was not found." },
	{ ERR_NOT_CHANNEL,		"This channel isn't in the channel list."},
	{ ERR_NOT_ACCESS,		"Access denied" },
	{ ERR_NOT_SELF,			"You can't modify yourself"},
    	{ ERR_NOT_KICK,			"Can't kick this user"},
    	{ ERR_NOT_BAN,			"Can't ban this user"},

    // Errors
	{ ERR_ADDCHANNEL_ALREADY,	"This channel is already in the channel list." },
	{ ERR_NICK_INVALID,		"Nickname is invalid."},
	{ ERR_NICK_EXIST,		"A account with this nickname already exists." },
	{ ERR_ALREADY_LOGON,		"You're already identified." },
	{ ERR_UNKNOWN_MODS,		"This mod is invalid."},
	{ ERR_USERLIST_EMPTY,		"The userlist is empty"},
	{ ERR_UNKNOW_MODE,		"Unknown mode parameter"},
    { ERR_NOTSELF_KICK,			"Can't kick myself"},
    { ERR_NOTSELF_BAN,			"Can't ban myself"},
    { ERR_WRONG_PW,             "Invalid password"},

	// other informations
	{ INFO_HELP_END,		"End of help."},
	{ INFO_HELP_FOR,		"Help for"},
	{ INFO_USERLIST_BEGIN,		"User list:"},
	{ INFO_USERLIST_END,		"End of user list."},
	{ INFO_CHANNELLIST_BEGIN,	"Channel list:"},
	{ INFO_CHANNELLIST_END,		"End of channel list."},
	{ INFO_CHANNELLIST_MODE,    	"Channel modes:"},
	{ INFO_CHANNELLIST_TOPIC,	"Topic:"},
	{ INFO_CHANNELLIST_GREET,	"Greeting:"},
	{ INFO_DEFAULT_REASON,		"You are kicked from the bot."},
	{ INFO_INIT_CHANNEL,		"has initialized this channnel"},
	{ INFO_NEED_OP,			"needs operator access rights." },
    { INFO_NOT_PASS,			"You've set a empty password."},


};

	static  char * unknow_parameter[]=
	{
		"Unknow option\n",
		"Type \"",PACKAGE," -h\" for help.\n",
		"."
	};



	static  char * help_msg[]=
	{
		"\n",
		PACKAGE," [Options]\n",
		"\n\tOptions:\n",
		"\t-a days\t\tSet the time in days for the automatic logoff.\n",
		"\t-b name\t\tSet the name of the bot.\n",
		"\t-c sec\t\tSet the ping timeout of the connection.\n",
		"\t-d path\t\tUse this path for the database\n",
        "\t-D Level\tSet the log level (0-7)\n",
		"\t-e ms\t\tSet the delay time in millisecond for the slow sending.\n",
       	"\t-f file\t\tUse this config file.\n",
	    "\t-g group\tSet the group under wich the bot run (only root).\n",
        "\t-h\t\tPrint this help information.\n",
		"\t-i sec\t\tSet the startup intialization delay.\n",
        "\t-k days\t\tSet the  account live time in days.\n",
        "\t-l char/min\tSet the chrakter limit for the first sending delay level.\n",
		"\t-m\t\tCreate a bot master.\n",
		"\t-n ms\t\tSet the delay time in millisecond for the fast sending.\n",
		"\t-p port\t\tSet the port of the irc server.\n"
		"\t-r string\tSet the output  for a request of the real name.\n",
		"\t-s server\tSet the irc server name.\n",
		"\t-t count\tSet the number of creating  threads.\n",
		"\t-u user\t\tSet the user under which the bot run (root only).\n",
        "\t-v\t\tPrint version.\n",
		"."
	};
#endif
