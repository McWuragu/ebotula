#ifndef CONFIG_H
#define CONFIG_H



#define CONF_FILE 				"ebotula.conf"
#define DATABASEPATH			"./"

#define DEFAULT_REALNAME		"LUGse Bot"
#define DEFAULT_BOTNAME			"Ircbot"
#define DEFAULT_SERVERNAME		"irc.fh-lausitz.de"
#define DEFAULT_PORT			6667
#define DEFAULT_THREAD_LIMIT	1

#define MAX_NICKS				3
#define MAX_THREADS_LIMIT       25
#define NOT_ALLOW_CHAR 			" :;<>*#/\"\'"
		
typedef struct CONFIG_STRUCT {
	char *botname;
	char *realname;
	char *server;
	char *port;
	char *configfile;
	char *database_path;
 	int createMaster;
	unsigned thread_limit;
} CONFIG_TYPE;





#endif												
