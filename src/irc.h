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




void irc_connect(void);
void user(void);
void pong(void);
void quit(void);
void nick(char *nick);
void part(char *channel);
void join(char *channel);
void notice(char *nick,char *text);


#endif
