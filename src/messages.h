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


#define	EOM			'.'


// excute error
// output of strerr
#define ERR_MSSING_PARA			"Missing value"
#define ERR_PORT_PARAMETER		"The portvalue is out of range."
#define ERR_WRONG_SERVERNAME	"The servername is irregular."
#define ERR_WRONG_BOTNAME		"The botname is irregular."
#define ERR_THREAD_RANGE		"The threadlimit is out of range."
#define ERR_READ_CONFIG_FILE	"Can't read the config file."
#define ERR_READ_PARAMETER		"Can't parse the commando line."
#define ERR_CONFIG_FILE			"Can't open the config file."
#define ERR_FAILED_NETPARA		"The servername or portnumber isn't set."
#define ERR_RESOLVE_HOSTNAME	"Can't resolve the hostname."
#define ERR_RESOLVE_PROTOCOL	"Tcp is a unknown protocol."
#define ERR_SOCKET				"Can't create a tcp socket."
#define ERR_CONNECT				"Connection failed."
#define ERR_SEND				"Can't send a command."
#define ERR_RECV				"Can't receive a line."
#define ERR_NICK				"Can't set the nickname."
#define ERR_MSG					"Can't create the message queue."
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


// Informations for invalid input or using of the bot
#define MSG_NICK_EXIST			"A account with this nickname already exists."
#define MSG_ALREADY_LOGON		"You're already identified."
#define MSG_NOT_LOGON			"You're not identified."
#define MSG_NOT_MASTER			"You're not a irc bot master."
#define MSG_NOT_OWNER			"You're not the bot owner of thiis channel."
#define MSG_NOT_CHANNELOPT		"This command need a channel name."
#define MSG_NOT_PASS			"You've set a empty password."
#define MSG_NOT_COMMAND			"No help found for this command."
#define MSG_NOT_ACCOUNT			"Account isn't found."
#define MSG_NOT_CHANNEL			"This channel isn't in the channel list."

#define MSG_ADDCHANNEL_ALREADY	"This channel is already in the channel list."
#define MSG_CHANNEL_INVALID		"Channel name is invalid."
#define MSG_NICK_INVALID		"Nickname is invalid."

#define MSG_HELP_END			"End of help."
#define MSG_HELP_FOR			"Help for"
#define MSG_CHANNELS			"Channel list:"
#define MSG_NEED_OP				"I need operator access rights."
#define MSG_SET_GREATING		"The new greating is set"
#define MSG_RM_GREATING			"The greating is removed"


// command  format erros
#define MSG_ADDCHANNEL_ERR		"Invalid command: !addchannel #CHANNEL"
#define MSG_RMCHANNEL_ERR		"Invalid command: !rmchannel #CHANNEL"
#define MSG_IDENT_ERR			"Invalid command: !ident LOGIN PASSWORD"
#define MSG_NICK_ERR			"Invalid command: !nick NICKNAME"			
#define MSG_GREATING_ERR		"Invalid command: !greating <#channel> MESSAGE"			

// Messages for the master dialog
#define MSG_MASTER_TITLE		"Create a master account\n\n"
#define MSG_MASTER_LOGIN		"Type the login (max 16): "
#define MSG_MASTER_PASS			"\nEnter the password(max. 8): "
#define MSG_MASTER_REPASS		"Reenter the password: "
#define MSG_MASTER_PASS_ERR		"Typing error..not equal passwords\n"
#define MSG_MASTER_EXISTS		"User is already exists.\n"
#define MSG_MASTER_ERR			"Can't set this account as master.\n"



#ifdef	CMD_MSG
static const char *unknow_parameter[]=
{
	"Unknow option\n",
	"Type ",PROGNAME," -h for help.\n",
	"."
};

static const char *version_msg[]=
{
	PROGNAME," Vers. ",VERSION," (C) 2003 Steffen Laube \n",
	"."
};

static const char *help_msg[]=
{
	PROGNAME," [Options]\n",
	"\n\tOptions:\n",
	"\t-b name\t\tSet  the name  of the bot.\n",
	"\t-d path\t\tUse this path for the database\n",
	"\t-f file\t\tUse this config file.\n",
	"\t-h\t\tPrint this help info.\n",
	"\t-m\t\tCreate a bot master.\n",
	"\t-p port\t\tSet the port of the irc server.\n"
	"\t-r string\tSet the output  for a request of the real name.\n",
	"\t-s server\tSet the irc server name.\n",
	"\t-t number\tSet the number of creating  threads.\n"
	"\t-v\t\tPrint version.\n",
	"."
};
#endif


#endif
