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

#ifndef CMDTYPES_H
#define CMDTYPES_H

#include "account.h"
           
#define ACCOUNT_LIST_TAB    16
#define HELP_TAB        12

#define CHANNEL_MODES   "kltnmsprqi"


/**
* This enum define all comand ids for the events and commands. This is used by the
* parser and the threads for execution the  commands or handle the events. The ids identify
* the comands or events in the message queue.
*/
typedef enum {
    CMD_NONE,

    /*  Event handler ids */
    CMD_ONPING,
    CMD_ONQUIT,
    CMD_ONNICKCHG,
    CMD_ONMODE,
    CMD_ONKICK,
    CMD_ONJOIN,
    CMD_ONNAMES,
    CMD_ONTOPIC,
    CMD_ONWHOIS,
    CMD_ONNONICK,

    /* CTCP commands */
    CMD_CTCPPING,
    CMD_CTCPVERSION,
    CMD_CTCPTIME,

    /* others */
    CMD_VIEWGREAT,
    CMD_HELP,
    CMD_VERSION,
    CMD_HELLO,
    CMD_IDENT,


    /* Logged user */
    CMD_LOGOFF,
    CMD_PASS,

    /* friends commands */
    CMD_INVITE,

    /* owner commands */
    CMD_CHANMODE,
    CMD_ACCOUNTLIST,
    CMD_ACCOUNTMODE,
    CMD_SAY,
    CMD_KICK,
    CMD_SET_TOPIC,
    CMD_SET_GREATING,
    CMD_BAN,
    CMD_DEBAN,

    /* Master commands */
    CMD_RESTART,
    CMD_ALLSAY,
    CMD_RMUSER,
    CMD_NICK,
    CMD_DIE,
    CMD_CHANNELS,
    CMD_RMCHANNEL,
    CMD_ADDCHANNEL,
    CMD_JOIN,
    CMD_PART,
    CMD_COUNT

}Cmd_t;

/*#define CMD_COUNT    (CMD_PART-CMD_NONE+1) */

#define CMD_MASTER  CMD_RESTART
#define CMD_OWNER   CMD_CHANMODE
#define CMD_FRIEND  CMD_INVITE
#define CMD_LOGGED  CMD_LOGOFF
#define CMD_EVENT   CMD_ONPING
#define CMD_OTHERS  CMD_VIEWGREAT
#define CMD_FIRST   CMD_OTHERS

/** This is the structure for the entries in the message queue */
typedef struct MsgBufStruct {
    Cmd_t identify;
    /** the complete string which receive from the irc server */
    char   *pMsgLine;
} MsgBuf_t;

/** This is a container for the parameter of the command handler */
typedef struct MsgItem_s {
    /** user access level of the  calling user */
    UserLevel_t UserLevel;
    /** the  type of the return channel */
    AnswerMode_t AnswerMode;
    /** channel of the  command target */
    char *pAccessChannel;
    /** nick name of the comamnd  caller */
    char *pCallingNick;
    /** the netmask of  this user */
    char *pNetmask;
    /** the complete string which receive from the irc server */
    char *pRawLine;
    /** interactive mode **/
    boolean  bInteractiveMode;
    /** parameter string **/
    char *pParameter;
} MsgItem_t;  

typedef struct CmdHandleRef_s {
    const Cmd_t  CmdId;  /* only for the  assert check */
    void(*CommandHandler)(MsgItem_t *) ;
    const char * pCmdString;
    boolean bIsCommand;
} CmdHandleRef_t;

#endif
