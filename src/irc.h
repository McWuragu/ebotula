#ifndef IRC_H
#define IRC_H




void irc_connect(void);
void user(void);
void pong(void);
void quit(void);
void nick(char *nick);
void part(char *channel);
void join(char *channel);
void notice(char *nick,char *text);


#endif
