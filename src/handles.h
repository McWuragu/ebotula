#ifndef HANDLES_H
#define HANDLES_H

#include "command.h"
					
void bot_op(char *line);
void print_greating(char *line);
void join_all_channels(void);
void log_on(char *netmask,char *login);
int AccessRight(char *line,CmdType cmd_id);

#endif
