/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#ifndef PARSER_H
#define PARSER_H


#include "config.h"
#include "command.h"


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


RETSIGTYPE stopParser(int sig);

#endif
