#ifndef MESSAGES_H
#define MESSAGES_H


#define VERSION		"0.04-alpha"
#define PROGNAME	"ebotula"
#define	EOM			'.'

#define ERR_MSSING_PARA			"Missing value"
#define ERR_PORT_PARAMETER		"The portvalue is out of range"
#define ERR_WRONG_SERVERNAME	"The servername is irregular"
#define ERR_WRONG_BOTNAME		"The botname is irregular"
#define ERR_THREAD_RANGE		"The threadlimit is out of range"
#define ERR_READ_CONFIG_FILE	"Can't read the config file"
#define ERR_READ_PARAMETER		"Can't parse the comando line"
#define ERR_CONFIG_FILE			"Can't open the config file"
#define ERR_FAILED_NETPARA		"Isn't set the servername or port number"
#define ERR_RESOLVE_HOSTNAME	"Can't resolve the hostname"
#define ERR_RESOLVE_PROTOCOL	"Tcp is unknown protocol"
#define ERR_SOCKET				"Can't create a tcp socket"
#define ERR_CONNECT				"Connection failed"
#define ERR_SEND				"Can't send a command"
#define ERR_RECV				"Can't receive a line"
#define ERR_NICK				"Can't set the nickname"
#define ERR_MSG					"Can't create message queue"
#define ERR_MUTEX				"Can't synchronize the threads"
#define ERR_NOT_ALLOW_CHAR		"Using of  not allow charakters\n"

#define MSG_NICK_EXIST			"It exist a account with this nickname"
#define MSG_ALREADY_LOGON		"You're already declared"
#define MSG_HELLO				"Nice to meet you."
#define MSG_HELLO2				"You have an account now"
#define MSG_PASSWD				"Password is changed"
#define MSG_NOT_LOGON			"You're not declared"
#define MSG_NOT_MASTER			"You're not a  irc bot master"
#define MSG_IDENT_ERR			"Invalid command: !ident LOGIN PASSWORD"
#define MSG_IDENT_NO			"Account not found"
#define MSG_IDENT_OK			"You're logon now"
#define MSG_JOIN_OK				"The bot join the channel"
#define MSG_PART_OK				"The bot part the channel"
#define MSG_RMCHANNEL_OK		"The Channel are removed form the list"
#define MSG_ADDCHANNEL_OK		"The Channel are added form the list"
#define MSG_ADDCHANNEL_ALREADY	"This channel is already in the  channel list"
#define MSG_ADDCHANNEL_ERR		"Invalid command: !addchannel #CHANNEL"
#define MSG_NO_PASS				"You've set a empty password"
#define MSG_CHANNEL_INVALID		"Channel name is invalid"

#define MSG_MASTER_TITLE		"Create a master account\n\n"
#define MSG_MASTER_LOGIN		"Type the login (max 16): "
#define MSG_MASTER_PASS			"\nEnter the password(max. 8): "
#define MSG_MASTER_REPASS		"Reenter the password: "
#define MSG_MASTER_PASS_ERR		"Typing error..not equal passwords\n"
#define MSG_MASTER_EXISTS		"User is already exists\n"
#define MSG_MASTER_ERR			"Can't set this account as master\n"



#ifdef	BIG_MSG
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
