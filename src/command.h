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

#define CHECK_LOGIN(_netmask,_nick)	if (!exist_db(LOGIN_DB,_netmask)) { \
									notice(_nick,MSG_NOT_LOGON);	\
									return;							\
								} 
								
#define CHECK_MASTER(_nick,_login ) if (!exist_db(ACCESS_DB,_login)) { \
										notice(_nick,MSG_NOT_MASTER);  \
										return;						   \
									}




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
void greating(char *line);



#endif
