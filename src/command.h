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

typedef enum CmdEnum {
	CMD_NONE,
	CMD_PING, 
	CMD_HELP,
    CMD_VERSION,
    CMD_HELLO,
	CMD_PASS,
    CMD_IDENT,
	CMD_ADDCHANNEL,
	CMD_JOIN,
	CMD_PART,
	CMD_RMCHANNEL,
	CMD_LOGOFF,
	CMD_DIE,
	CMD_NICK,
	CMD_CHANNELS,
	CMD_NAMES,
	CMD_JOIN_GREATING,
	CMD_GREATING 
}CmdType;


				  
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
