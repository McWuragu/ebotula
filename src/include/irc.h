/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2023 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
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
/* void quit(void);*/
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

void sendMsg(AnswerMode_t AnswerMode,char * pNick,char * pMsgStr,...);
#endif
