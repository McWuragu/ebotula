/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


#define PROGNAME				"ebotula"
#define CONFFILE 				"ebotula.conf"

#define DEFAULT_THREAD_LIMIT	3
#define DEFAULT_REALNAME		"I'm a stupid bot"
#define DEFAULT_BOTNAME			"Ebotula"

#define MAX_NICKS				3
#define MAX_THREADS_LIMIT       25
#define NOT_ALLOW_CHAR 			" :;<>*#/\"\'\\{}[]()=?§$"


#define USER_ALLOW_CHAR			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-[]\\`^{}~"
#define CHANNEL_NOT_ALLOW_CHAR	" \a\r\n,"
		
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
