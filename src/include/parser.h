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

#ifndef PARSER_H
#define PARSER_H


#include "config.h"
#include "cmdlist.h"


/**
* This parse a line whiche was receive by the  client
* @param pLine  a pointer of a string
* @param pMsg a struct withe the command id, the type of 
* message and a string as  argument for the  command
*/
void preParser(char *pLine,MsgBuf_t *pMsg);

/**
* the function for  the  thread
* @param argv a pointer  of arguements for  the
* starting of  this thread
*/
void *CommandExecutionThread(void *argv);

static int AccessRight(UserLevel_t Level,Cmd_t cmd_id);


void stopParser(int sig);

#endif
