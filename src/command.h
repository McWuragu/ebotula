/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#ifndef COMMAND_H
#define COMMAND_H


#include "network.h"
#include "account.h"

#define USERLIST_TAB    20
#define HELP_TAB        12

#define CHANNEL_MODES   "kltnmsprqi"


/**
* This enum define all comand ids for the events and commands. This is used by the
* parser and the threads for execution the  commands or handle the events. The ids identify
* the comands or events in the message queue.
*/
typedef enum CmdEnum {
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
    CMD_USERLIST,
    CMD_USERMODE,
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
    CMD_PART
}Cmd_t;

#define CMDCOUNT    (CMD_PART-CMD_NONE+1)

#define CMD_MASTER  CMD_RESTART
#define CMD_OWNER   CMD_CHANMODE
#define CMD_FRIEND  CMD_INVITE
#define CMD_LOGGED  CMD_LOGOFF
#define CMD_EVENT   CMD_ONPING
#define CMD_OTHERS  CMD_VIEWGREAT

#define CmdIdToHelpId(m)        ((m)-CMD_OTHERS+1)
#define HelpIdToCmdId(m)        ((m)+CMD_OTHERS-1)

static char * const CmdList[]= {
    "none",
    /* Events */
    "PING","QUIT","NICK","MODE","KICK","JOIN","353","TOPIC","311",
    /* any commands */
    "viewgreet","help","version","hello","ident",
    /* logged user commands */
    "logoff","pass",
    /* friend commands */
    "invite",
    /* channel owner commands */
    "chanmode","userlist","usermode","say","kick","topic","greeting","ban","deban",
    /* master commands */
    "restart","allsay","rmuser","nick","die","chanlist","rmchannel","addchannel","join","part"
};

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
    /** channel of the  command target */
    char *pAccessChannel;
    /** nick name of the comamnd  caller */
    char *pCallingNick;
    /** the complete string which receive from the irc server */
    char *pRawLine;
} MsgItem_t;  

/* ######################### Bot commands ################################ */

void help(char *pLine);
void password(char *pLine);
void ident(char *pLine);
void logoff(char *pLine,int iRemoveMode);
void version(char *pLine);
void hello(char *pLine);
void addChannel(char *pLine);
void rmChannel(char *pLine);
void joinChannel(char *pLine);
void partChannel(char *pLine);
void die(char *pLine);
void setNick(char *pLine);
void chanlist(char *pLine);
void setGreeting(char *pLine);
void setTopic(char *pLine);
void greeting(char *pLine);
void say(char *pLine);
void allsay(char *pline);
void banuser(char *pLine);
void debanuser(char *pLine);
void kickuser(char *pLine);
void usermode(char *pLine);
void chanmode(char *pLine);
void rmuser(char *pLine);
void userlist(char *pLine);

// new command format
void inviteuser(MsgItem_t *pMsg);
#endif
