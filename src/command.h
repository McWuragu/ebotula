/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef COMMAND_H
#define COMMAND_H

#define USER_MODS		"ovm"
#define CHANNEL_MODS	"tnmaiqpsrkl"

#define USERLIST_TAB	20
#define HELP_TAB		12
				  
// ######################### Bot commands ################################

void help(char *pLine);
void password(char *pLine);
void ident(char *pLine);
void logoff(char *pLine);
void version(char *pLine);
void hello(char *pLine);
void channel_add(char *pLine);
void channel_rm(char *pLine);
void join_channel(char *pLine);
void part_channel(char *pLine);
void die(char *pLine);
void set_nick(char *pLine);
void channel_list(char *pLine);
void setGreating(char *pLine);
void setTopic(char *pLine);
void greating(char *pLine);
void say(char *pLine);
void kickuser(char *pLine);
void usermode(char *pLine);
void chanmode(char *pLine);
void rmuser(char *pLine);
void userlist(char *pLine);
#endif
