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

#define USER_MODS	"Oovm"
#define CHANNEL_MODS	"tnmaiqpsrkl"
				  
// ######################### Bot commands ################################

void help(char *line);
void password(char *line);
void ident(char *line);
void logoff(char *line);
void version(char *line);
void hello(char *line);
void channel_add(char *line);
void channel_rm(char *line);
void join_channel(char *line);
void part_channel(char *line);
void die(char *line);
void change_nick(char *line);
void channel_list(char *line);
void setGreating(char *line);
void setTopic(char *line);
void greating(char *line);
void bot_op(char *line);
void say(char *line);
void kickuser(char *line);
void usermode(char *line);
void chanmode(char *line);

boolean log_on(char *nick,char *login);
#endif
