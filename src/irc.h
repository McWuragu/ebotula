/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#ifndef IRC_H
#define IRC_H

#define  quit()  SendLine("QUIT\r\n");

#include "type.h"

void user(void);
void pong(char *pPong);
void ping(char *pTarget);
void topic(char *pChannel,char *pMsgStr);
//void quit(void);
void invite(char *pChannel,char *pNick);
void nick(char *pNick);
void part(char *pChannel);
void join(char *pChannel);
void notice(char *pNick,char *pMsgStr);
void privmsg(char *pTarget, char *pMsgStr);
void action(char *pTarget, char *pMsgStr);
void kick(char *pChannel, char *pNick, char *pMsgStr);
void ban(char *pChannel, char *pMask);
void deban(char *pChannel,char *pMask);
void mode(char *pChannel, char *pMod, char *pModParameter);
void whois(char *pNickname);

void sendMsg(AnswerMode_t AnswerMode,char * pNick,char * pMsgStr);
#endif
