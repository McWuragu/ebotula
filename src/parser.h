/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include "type.h"

/**
* This parse a line whiche was receive by the  client
* @param line  a pointer of a string
* @return a struct withe the command id, the type of 
* message and a string as  argument for the  command
*/
MsgBufType preParser(char *line);

/**
* the function for  the  thread
* @param argv a pointer  of arguements for  the
* starting of  this thread
*/
void *action_thread(void *argv);

int AccessRight(char *line,CmdType cmd_id);
void stopParser(int sig);

#endif
