/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef IRC_H
#define IRC_H

#define USER_NOT_ALLOW_CHAR			": \t\r\a"
#define CHANNEL_NOT_ALLOW_CHAR	" \a\r\n,"


void user(void);
void pong(void);
void ping(char *target);
void topic(char *channel,char *text);
void quit(void);
void nick(char *nick);
void part(char *channel);
void join(char *channel);
void notice(char *nick,char *text);
void privmsg(char *target, char *text);
void action(char *target, char *text);
void kick(char *channel, char *nick, char *reason);
void ban(char *channel, char *mask);
void deban(char *channel,char *mask);
void mode(char *channel, char *mods, char *modsparam);

#endif
