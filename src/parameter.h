/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef PARAMETER_H
#define PARAMETER_H


#define	PARAMETER_CHAR	'-'
#define COMMENT_CHAR	'#'
#define MAX_READ_BUFFER_SIZE 	256

#define KEY_SERVER			"servername"
#define KEY_PORT			"port"
#define KEY_BOTNAME			"botname"
#define KEY_REALNAME		"realname"
#define KEY_THREADLIMIT		"threadlimit"
#define	KEY_DATABASEPATH	"databasepath"


void cmd_line(int argc,const char *argv[]);
void read_config_file(void);
void dialogMaster(void);


#endif
