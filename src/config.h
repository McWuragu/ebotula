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

#define VERSIONSTR 	"%s v%s Build [%s - %s] (C) 2003 Steffen Laube\n",PROGNAME,VERSION,__DATE__,__TIME__

#define PROGNAME				"ebotula"
#define CONFFILE 				"ebotula.conf"

#define DEFAULT_THREAD_LIMIT	3
#define DEFAULT_REALNAME		"I'm a stupid bot"
#define DEFAULT_BOTNAME			"Ebotula"
#define DEFAULT_PING_TIMEOUT	90

#define MIN_LOGOFF				1
#define MIN_ALT					1
#define MAX_NICKS				3
#define MAX_THREADS_LIMIT       25
#define NOT_ALLOW_CHAR 			" :;<>*#/\"\'\\{}[]()=?§$"




#define	PARAMETER_CHAR	'-'
#define COMMENT_CHAR	'#'

#define KEY_SERVER			"servername"
#define KEY_PORT			"port"
#define KEY_BOTNAME			"botname"
#define KEY_REALNAME		"realname"
#define KEY_THREADLIMIT		"threadlimit"
#define	KEY_DATABASEPATH	"databasepath"
#define KEY_AUTOLOGOFF		"autologoff"
#define KEY_SENDDELAY		"senddelay"
#define KEY_ALT				"accountlivetime"




#endif												
