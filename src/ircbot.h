/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#ifndef IRCBOT_H
#define IRCBOT_H

#define VERSIONSTR 	"%s v%s Built [%s - %s] (C) 2003 Steffen Laube",PACKAGE,PACKAGE_VERSION,__DATE__,__TIME__

#define DEFAULT_USER_DATABASEDIR        "/db"
#define DEFAULT_USER_CONFDIR            "/.ebotula"
#define CONFFILE 				        "/ebotula.conf"

#define DEFAULT_THREAD_LIMIT	3
#define DEFAULT_REALNAME		"I'm a stupid bot"
#define DEFAULT_BOTNAME			"Ebotula"
#define DEFAULT_PORT            "6667"
#define DEFAULT_IRC             "irc.fh-lausitz.de"

#define DEFAULT_PING_TIMEOUT	90

#define DEFAULT_SLOW_SEND_DELAY  2000
#define DEFAULT_FAST_SEND_DELAY  1500
#define DEFAULT_FAST_SEND_LIMIT  500

#define DEFAULT_LOGOFF           3
#define DEFAULT_ALT             90

#define DEFAULT_USER            "nobody"
#define DEFAULT_GROUP           "nobody"

#define MIN_LOGOFF				1
#define MIN_ALT					1
#define MIN_PINGTIME			60
#define MAX_NICKS				3
#define MAX_THREADS_LIMIT       25


#define	PARAMETER_CHAR	'-'
#define COMMENT_CHAR	'#'

#define KEY_SERVER			"servername"
#define KEY_PORT			"port"
#define KEY_BOTNAME			"botname"
#define KEY_REALNAME		"realname"
#define KEY_THREADLIMIT		"threadlimit"
#define	KEY_DATABASEPATH	"databasepath"
#define KEY_AUTOLOGOFF		"autologoff"
#define KEY_FASTSENDDELAY	"fastsenddelay"
#define KEY_ALT				"accountlivetime"
#define KEY_PINGTIMEOUT		"pingtimeout"
#define KEY_SLOWSENDDELAY   "slowsenddelay"
#define KEY_FASTSENDLIMIT   "fastsendlimit"
#define KEY_EXEUSER         "user"
#define KEY_EXEGROUP        "group"
#endif												
