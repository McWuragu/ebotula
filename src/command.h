#ifndef COMMAND_H
#define COMMAND_H

char *getNickname(char *line);
char *getNetmask(char *line);
char *getArgument(char *line);
char *getCommand(char *line);

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

void join_all_channels(void);
void login(char *netmask,char *login);


#endif
