/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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
#include "queue.h"
#include "handles.h"
#include "utilities.h"
#include "messages.h"
#include "parser.h"


MsgBuf_t* preParser(char *pLine) {
    MsgBuf_t *pMsg;
    char *pStr,*pPreamble,*pPos;
    int i;

    // init the buffer with zero
    pMsg=(MsgBuf_t*)calloc(sizeof(MsgBuf_t),1);

    // get the first part of the  answer from server
    pPreamble=getCommand(pLine);

    if (!(pPos=strchr(pPreamble,' '))){
        return pMsg;
    }

    // preparse the line
    // identify events and commands

    if (!strncmp(pPreamble,CmdList[CMD_ONPING],strlen(CmdList[CMD_ONPING]))) {
        /*
         * The ping can't send over the threads. If you have high usage of the
         * threads then is the execution the pong request to slow. It must send
         * direct from the parser
         *
         * pMsg->identify=CMD_ONPING;
         */
        pong();
    } else if (strstr(pPos,CmdList[CMD_ONQUIT])) {
        pMsg->identify=CMD_ONQUIT;
    } else if (strstr(pPos,CmdList[CMD_ONJOIN])) {
        pMsg->identify=CMD_ONJOIN;
    } else if (strstr(pPos,CmdList[CMD_ONNICKCHG])) {
        pMsg->identify= CMD_ONNICKCHG;
    } else if (strstr(pPos,CmdList[CMD_ONMODE])) {
        pMsg->identify= CMD_ONMODE;
    } else if (strstr(pPos,CmdList[CMD_ONKICK])) {
        pMsg->identify= CMD_ONKICK;
    } else if (strstr(pPos,CmdList[CMD_ONTOPIC])) {
        pMsg->identify= CMD_ONTOPIC;
    } else if (strstr(pPos,CmdList[CMD_ONNAMES])) {
        pMsg->identify=CMD_ONNAMES;
    } else if (strstr(pPos,CmdList[CMD_ONWHOIS])) {
        pMsg->identify=CMD_ONWHOIS;
    } else if ((pStr=strstr(pLine," :!"))!=NULL) {

        if (strlen(pStr)>=3) {
            pStr+=3;
            // command parser
            for (i=CMD_OTHERS;i<CMDCOUNT;i++) {
                if (!strncmp(pStr,CmdList[i],strlen(CmdList[i]))){
                    pMsg->identify=i;
                    DEBUG("Found Command %s\n",CmdList[i]);
                    i=CMDCOUNT;
                }
            }
        }
    }
	
	pMsg->pMsgLine=(char*)malloc((strlen(pLine)+1)*sizeof(char));
    strcpy(pMsg->pMsgLine,pLine);
    return pMsg;
}

void *ComandExecutionThread(void *argv) {
	extern boolean stop;
    QueueData *pCommand;
	MsgBuf_t *pMsg;
	
	PQueue pCommandQueue=(PQueue)argv;	

    // set the thread cancelable
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

    DEBUG("Execution thread is running\n");

    // execute loop
    while(!stop) {

        pCommand=popQueue(pCommandQueue);
        
        if (pCommand) {
            pMsg=(MsgBuf_t*)pCommand->data;

            // check the access level
            if (AccessRight(pMsg->pMsgLine,pMsg->identify)) {
                
                // command router 
                switch (pMsg->identify) {
                case CMD_ONPING:
                    pong();
                    break;
                case CMD_ONQUIT:
                    logoff(pMsg->pMsgLine,false);
                case CMD_LOGOFF:
                    logoff(pMsg->pMsgLine,true);
                    break;
                case CMD_HELP:
                    help(pMsg->pMsgLine);
                    break;
                case CMD_VERSION:
                    version(pMsg->pMsgLine);
                    break;
                case CMD_HELLO:
                    hello(pMsg->pMsgLine);
                    break;
                case CMD_PASS:
                    password(pMsg->pMsgLine);
                    break;
                case CMD_IDENT:
                    ident(pMsg->pMsgLine);
                    break;
                case CMD_ADDCHANNEL:
                    addChannel(pMsg->pMsgLine);
                    break;
                case CMD_RMCHANNEL:
                    rmChannel(pMsg->pMsgLine);
                    break;
                case CMD_JOIN:
                    joinChannel(pMsg->pMsgLine);
                    break;
                case CMD_PART:
                    partChannel(pMsg->pMsgLine);
                    break;
                case CMD_DIE:
                    die(pMsg->pMsgLine);
                    break;
                case CMD_NICK:
                    setNick(pMsg->pMsgLine);
                    break;
                case CMD_CHANNELS:
                    chanlist(pMsg->pMsgLine);
                    break;
                case CMD_ONNAMES:
                    hBotNeedOp(pMsg->pMsgLine);
                    break;
                case CMD_ONJOIN:
                    hSetModUser(pMsg->pMsgLine);
                case CMD_VIEWGREAT:
                    greeting(pMsg->pMsgLine);
                    break;
                case CMD_SET_GREATING:
                    setGreeting(pMsg->pMsgLine);
                    break;
                case CMD_SET_TOPIC:
                    setTopic(pMsg->pMsgLine);
                    break;
                case CMD_SAY:
                    say(pMsg->pMsgLine);
                    break;
                case CMD_KICK:
                    kickuser(pMsg->pMsgLine);
                    break;
                case CMD_USERMODE:
                    usermode(pMsg->pMsgLine);
                    break;
                case CMD_ONNICKCHG:
                    hNickChange(pMsg->pMsgLine);
                    break;
                case CMD_RMUSER:
                    rmuser(pMsg->pMsgLine);
                    break;
                case CMD_USERLIST:
                    userlist(pMsg->pMsgLine);
                    break;
                case CMD_ONMODE:
                    hResetModes(pMsg->pMsgLine);
                    break;
                case CMD_ONKICK:
                    hRejoinAfterKick(pMsg->pMsgLine);
                    break;
                case CMD_ALLSAY:
                    allsay(pMsg->pMsgLine);
                    break;
                case CMD_ONTOPIC:
                    hResetTopic(pMsg->pMsgLine);
                    break;
                case CMD_CHANMODE:
                    chanmode(pMsg->pMsgLine);
                    break;
                case CMD_RESTART:
                    restart(pMsg->pMsgLine);
                    break;
                case CMD_ONWHOIS:
                    hCallback(pMsg->pMsgLine);
                    break;
                case CMD_BAN:
                    banuser(pMsg->pMsgLine);
                    break;
                case CMD_DEBAN:
                    debanuser(pMsg->pMsgLine);
                    break;
                case CMD_INVITE:
                    inviteuser(pMsg->pMsgLine);
                    break;
                default:
                    syslog(LOG_CRIT,getSyslogString(SYSLOG_UNKNOWN_CMDID));
                    break;
                }

                // remove  destroy the last popped Command
                free(pMsg->pMsgLine);
                free(pMsg);
                free(pCommand);
            
            }
        }
    }
    DEBUG("Execution thread is stopped\n");
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

    /* extract information form pline*/
    pNetmask=getNetmask(pLine);
    pNick=getNickname(pLine);
    
    // check Accesslevel
    if (cmd_id >= CMD_MASTER) {
        // check the  login status
        if (!exist_db(NICKTOUSER_DB,pNetmask)) {
            notice(pNick,getMsgString(ERR_NOT_LOGON));
        } else {
            if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {
	            if (exist_db(ACCESS_DB,pLogin)) {
        	        return true;
	            }
			}
        }
    }  else if (cmd_id >= CMD_OWNER) {
        // check the  login status
        if (!exist_db(NICKTOUSER_DB,pNetmask)) {
            notice(pNick,getMsgString(ERR_NOT_LOGON));
        } else {
            // check the existe of a channel parameter
            if (pChannel=getAccessChannel(pLine)) {
				if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {

					pKey=(char*)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char*));
					sprintf(pKey,"%s%s",pLogin,pChannel);

					if ((pMod=get_db(ACCESS_DB,pKey))) {
						if (strchr(pMod,'o')) {	return true;}
					} else if (exist_db(ACCESS_DB,pLogin)) {
						notice(pNick,getMsgString(OK_MASTER));
						return true;
					}

				}
            } else {
				notice(pNick,getMsgString(ERR_NOT_CHANNELOPT));
			}
        }

        notice(pNick,getMsgString(ERR_NOT_OWNER));
    } else if (cmd_id >= CMD_FRIEND) {
        // check the  login status
        if (!exist_db(NICKTOUSER_DB,pNetmask)) {
            notice(pNick,getMsgString(ERR_NOT_LOGON));
        } else {
            // check the existe of a channel parameter
            if (pChannel=getAccessChannel(pLine)) {
				if ((pLogin=get_db(NICKTOUSER_DB,pNetmask))) {

					pKey=(char*)malloc((strlen(pLogin)+strlen(pChannel)+1)*sizeof(char*));
					sprintf(pKey,"%s%s",pLogin,pChannel);

					if ((pMod=get_db(ACCESS_DB,pKey))) {
						if (strchr(pMod,'v')) {	return true;}
                        if (strchr(pMod,'o')) {	return true;}
					} else if (exist_db(ACCESS_DB,pLogin)) {
						notice(pNick,getMsgString(OK_MASTER));
						return true;
					}

				}
            } else {
				notice(pNick,getMsgString(ERR_NOT_CHANNELOPT));
			}
        }

        notice(pNick,getMsgString(ERR_NOT_FRIEND)); 
    }  else if (cmd_id >= CMD_LOGGED) {
        // check  login status
        if (!exist_db(NICKTOUSER_DB,pNetmask)) {
            notice(pNick,getMsgString(ERR_NOT_LOGON));
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

    notice(pNick,getMsgString(ERR_NOT_ACCESS));
    return false;
}
// #############################################################################
void stopParser(int sig) {
    extern int stop;
    if (!stop) {
        quit();
    }

    stop=true;
    DEBUG("Stop IRCBot\n");
}
