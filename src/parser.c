/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <syslog.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "extract.h"
#include "irc.h"
#include "dbaccess.h"
#include "command.h"
#include "handles.h"
#include "utilities.h"
#include "messages.h"
#include "parser.h"


MsgBuf_t preParser(char *pLine) {
    MsgBuf_t sMsg;
    char *pStr,*pPreamble,*pPos;
    int i;

    // init the buffer with zero
    bzero(&sMsg,sizeof(MsgBuf_t));

    // get the first part of the  answer from server
    pPreamble=getCommand(pLine);

    if (!(pPos=strchr(pPreamble,' '))){
        return sMsg;
    }

    // preparse the line
    // identify events and commands

    if (!strncmp(pPreamble,CmdList[CMD_ONPING],strlen(CmdList[CMD_ONPING]))) {
        sMsg.mtype=2;
        sMsg.identify=CMD_ONPING;
    } else if (strstr(pPos,CmdList[CMD_ONQUIT])) {
        sMsg.mtype=2;
        sMsg.identify=CMD_ONQUIT;
    } else if (strstr(pPos,CmdList[CMD_ONJOIN])) {
        sMsg.mtype=2;
        sMsg.identify=CMD_ONJOIN;
    } else if (strstr(pPos,CmdList[CMD_ONNICKCHG])) {
        sMsg.mtype=2;
        sMsg.identify= CMD_ONNICKCHG;
    } else if (strstr(pPos,CmdList[CMD_ONMODE])) {
        sMsg.mtype=2;
        sMsg.identify= CMD_ONMODE;
    } else if (strstr(pPos,CmdList[CMD_ONTOPIC])) {
        sMsg.mtype=2;
        sMsg.identify= CMD_ONTOPIC;
    } else if (strstr(pPos,CmdList[CMD_ONNAMES])) {
        sMsg.mtype=2;
        sMsg.identify=CMD_ONNAMES;
    } else if ((pStr=strstr(pLine," :!"))!=NULL) {

        if (strlen(pStr)>=3) {
            pStr+=3;
            DEBUG("%s",pStr);
            // command parser
            for (i=CMD_OTHERS;i<CMDCOUNT;i++) {
                DEBUG("Look for %s",CmdList[i]);

                if (!strncmp(pStr,CmdList[i],strlen(CmdList[i]))){
                    sMsg.mtype=1;
                    sMsg.identify=i;

                    i=CMDCOUNT;
                }
            }
        }
    }

    strcpy(sMsg.pMsgLine,pLine);
    return sMsg;
}

void *ComandExecutionThread(void *argv) {
    int msgid;
    extern int key;
    MsgBuf_t sMsg;

    // set the thread cancelable
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

    // open the message queue
    msgid=msgget(key,0600 | IPC_EXCL );

    // execute loop
    while(1) {
        msgrcv(msgid,&sMsg,sizeof(MsgBuf_t)-sizeof(sMsg.mtype),0,0);

        if (AccessRight(sMsg.pMsgLine,sMsg.identify)) {

            switch (sMsg.identify) {
            case CMD_ONPING:
                pong();
                break;
            case CMD_ONQUIT:
            case CMD_LOGOFF:
                logoff(sMsg.pMsgLine);
                break;
            case CMD_HELP:
                help(sMsg.pMsgLine);
                break;
            case CMD_VERSION:
                version(sMsg.pMsgLine);
                break;
            case CMD_HELLO:
                hello(sMsg.pMsgLine);
                break;
            case CMD_PASS:
                password(sMsg.pMsgLine);
                break;
            case CMD_IDENT:
                ident(sMsg.pMsgLine);
                break;
            case CMD_ADDCHANNEL:
                addChannel(sMsg.pMsgLine);
                break;
            case CMD_RMCHANNEL:
                rmChannel(sMsg.pMsgLine);
                break;
            case CMD_JOIN:
                joinChannel(sMsg.pMsgLine);
                break;
            case CMD_PART:
                partChannel(sMsg.pMsgLine);
                break;
            case CMD_DIE:
                die(sMsg.pMsgLine);
                break;
            case CMD_NICK:
                setNick(sMsg.pMsgLine);
                break;
            case CMD_CHANNELS:
                chanlist(sMsg.pMsgLine);
                break;
            case CMD_ONNAMES:
                hBotNeedOp(sMsg.pMsgLine);
                break;
            case CMD_ONJOIN:
                hSetModUser(sMsg.pMsgLine);
            case CMD_VIEWGREAT:
                greeting(sMsg.pMsgLine);
                break;
            case CMD_SET_GREATING:
                setGreeting(sMsg.pMsgLine);
                break;
            case CMD_SET_TOPIC:
                setTopic(sMsg.pMsgLine);
                break;
            case CMD_SAY:
                say(sMsg.pMsgLine);
                break;
            case CMD_KICK:
                kickuser(sMsg.pMsgLine);
                break;
            case CMD_USERMODE:
                usermode(sMsg.pMsgLine);
                break;
            case CMD_ONNICKCHG:
                hNickChange(sMsg.pMsgLine);
                break;
            case CMD_RMUSER:
                rmuser(sMsg.pMsgLine);
                break;
            case CMD_USERLIST:
                userlist(sMsg.pMsgLine);
                break;
            case CMD_ONMODE:
                hResetModes(sMsg.pMsgLine);
                break;
            case CMD_ALLSAY:
                allsay(sMsg.pMsgLine);
                break;
            case CMD_ONTOPIC:
                hResetTopic(sMsg.pMsgLine);
                break;
            case CMD_CHANMODE:
                chanmode(sMsg.pMsgLine);
                break;
            case CMD_RESTART:
                restart(sMsg.pMsgLine);
                break;
            default:
                syslog(LOG_CRIT,SYSLOG_UNKNOWN_CMDID,sMsg.identify);
                break;
            }
        }

        // clear buffer
        bzero(&sMsg,sizeof(MsgBuf_t));
    }

    return NULL;
}

// #############################################################################
static int AccessRight(char *pLine,Cmd_t cmd_id) {
    char *pChannel;
    char *pLogin;
    char *pNick;
    char *pNetmask;
    char *pKey;
    char *pMod;


    pNetmask=getNetmask(pLine);
    pNick=getNickname(pLine);


    // check Accesslevel
    if (cmd_id >= CMD_MASTER) {
        // check the  login status
        if (!exist_db(NICKTOUSER_DB,pNetmask)) {
            notice(pNick,MSG_NOT_LOGON);
        } else {
            pLogin=get_db(NICKTOUSER_DB,pNetmask);
            if (exist_db(ACCESS_DB,pLogin)) {
                return true;
            }
        }
    }  else if (cmd_id >= CMD_OWNER) {
        // check the  login status
        if (!exist_db(NICKTOUSER_DB,pNetmask)) {
            notice(pNick,MSG_NOT_LOGON);
        } else {
            // check the existe of a channel parameter
            if (strlen(pChannel)) {
                pLogin=get_db(NICKTOUSER_DB,pNetmask);
                pChannel=getAccessChannel(pLine);

                pKey=(char*)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char*));
                sprintf(pKey,"%s%s",pLogin,pChannel);

                pMod=get_db(ACCESS_DB,pKey);

                if (strchr(pMod,'o')) {
                        return true;
                } else {
                    if (exist_db(ACCESS_DB,pLogin)) {
                        notice(pNick,MSG_MASTER);
                        return true;
                    }
                }
            }
            notice(pNick,MSG_NOT_CHANNELOPT);
        }

        notice(pNick,MSG_NOT_OWNER);

    }  else if (cmd_id >= CMD_LOGGED) {
        // check  login status
        if (!exist_db(NICKTOUSER_DB,pNetmask)) {
            notice(pNick,MSG_NOT_LOGON);
        } else {
            return true;
        }
    }  else if (cmd_id >= CMD_OTHERS) {
        return true;
    }  else if (cmd_id >= CMD_EVENT) {
       if ((cmd_id == CMD_ONNICKCHG) || (cmd_id==CMD_ONQUIT)) {
           // check  login status
            if (exist_db(NICKTOUSER_DB,pNetmask)) {
                return true;
            }
       } else {
           return true;
       }
       return false;
    }

    notice(pNick,MSG_NOT_ACCESS);
    return false;
}
// #############################################################################
void stopParser(int sig) {
    extern int stop;
    if (!stop) {
        quit();
    }

    stop=true;
    DEBUG("Stop IRCBot");
}
