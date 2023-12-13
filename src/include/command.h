/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
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

#ifndef COMMAND_H
#define COMMAND_H


#include "cmdtypes.h"
#include "handles.h"
#include "ctcp.h"

/* ######################### Bot commands ################################ */




/* new command format */
void help(MsgItem_t *pMsg);
void password(MsgItem_t *pMsg);
void ident(MsgItem_t *pMsg);
void logoff(MsgItem_t *pMsg);
void version(MsgItem_t *pMsg);
void hello(MsgItem_t *pMsg);
void addChannel(MsgItem_t *pMsg);
void rmChannel(MsgItem_t *pMsg);
void joinChannel(MsgItem_t *pMsg);
void partChannel(MsgItem_t *pMsg);
void die(MsgItem_t *pMsg);
void restart(MsgItem_t *pMsg);
void setNick(MsgItem_t *pMsg);
void chanlist(MsgItem_t *pMsg);
void setGreeting(MsgItem_t *pMsg);
void setTopic(MsgItem_t *pMsg);
void greeting(MsgItem_t *pMsg);
void say(MsgItem_t *pMsg);
void allsay(MsgItem_t *pMsg);
void banuser(MsgItem_t *pMsg);
void debanuser(MsgItem_t *pMsg);
void kickuser(MsgItem_t *pMsg);
void accountmode(MsgItem_t *pMsg);
void chanmode(MsgItem_t *pMsg);
void rmaccount(MsgItem_t *pMsg);
void accountlist(MsgItem_t *pMsg);
void inviteuser(MsgItem_t *pMsg);


#endif


