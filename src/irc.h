#ifndef IRC_H
#define IRC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>


void irc_connect(void);
void user(void);
void pong(void);
void nick(char *nick);
void part(char *channel);
void join(char *channel);
void notice(char *nick,char *text);


#endif
