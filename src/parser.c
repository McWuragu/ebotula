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

    // look for  command position
    if (!(pPos=strchr(pPreamble,' '))){
        return pMsg;
    } else {
        //set the pointr to the comaman start
        pPos++;
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
		if (!(pStr=strstr(pPreamble," "))) {		
			pong(NULL);
		}else {		
			pStr++; /* get next symbol */
			pong(pStr);
		}
	} else if (!strncmp(pPos,CmdList[CMD_ONQUIT],strlen(CmdList[CMD_ONQUIT]))) {
        pMsg->identify=CMD_ONQUIT;
    } else if (!strncmp(pPos,CmdList[CMD_ONJOIN],strlen(CmdList[CMD_ONJOIN]))) {
        pMsg->identify=CMD_ONJOIN;
    } else if (!strncmp(pPos,CmdList[CMD_ONNICKCHG],strlen(CmdList[CMD_ONNICKCHG]))) {
        pMsg->identify= CMD_ONNICKCHG;
    } else if (!strncmp(pPos,CmdList[CMD_ONMODE],strlen(CmdList[CMD_ONMODE]))) {
        pMsg->identify= CMD_ONMODE;
    } else if (!strncmp(pPos,CmdList[CMD_ONKICK],strlen(CmdList[CMD_ONKICK]))) {
        pMsg->identify= CMD_ONKICK;
    } else if (!strncmp(pPos,CmdList[CMD_ONTOPIC],strlen(CmdList[CMD_ONTOPIC]))) {
        pMsg->identify= CMD_ONTOPIC;
    } else if (!strncmp(pPos,CmdList[CMD_ONNAMES],strlen(CmdList[CMD_ONNAMES]))) {
        pMsg->identify=CMD_ONNAMES;
    } else if (!strncmp(pPos,CmdList[CMD_ONWHOIS],strlen(CmdList[CMD_ONWHOIS]))) {
        pMsg->identify=CMD_ONWHOIS;
    } else if (!strncmp(pPos,CmdList[CMD_ONNONICK],strlen(CmdList[CMD_ONNONICK]))) {
        pMsg->identify=CMD_ONNONICK;
    } else if (!strncmp(pPos,CmdList[CMD_ONPRIVMSG],strlen(CmdList[CMD_ONPRIVMSG])) ||
               !strncmp(pPos,CmdList[CMD_ONNOTICE],strlen(CmdList[CMD_ONNOTICE]))) {
        if ((pStr=strstr(pLine," :!"))!=NULL) {
            /* bot command parser */
            if (strlen(pStr)>=3) {
                pStr+=3;
                
                for (i=CMD_OTHERS;i<CMDCOUNT;i++) {
                    if (!strncmp(pStr,CmdList[i],strlen(CmdList[i]))){
                        pMsg->identify=i;
                        DEBUG("Found Command %s\n",CmdList[i]);
                        i=CMDCOUNT;
                    }
                }
            }
        } else if ((pStr=strstr(pLine," :\001"))!=NULL) {
            /* ctcp command parser */
            if (strlen(pStr)>=3) {
                pStr+=2;
                // command parser
               if (!strncmp(pStr,CmdList[CMD_CTCPPING],strlen(CmdList[CMD_CTCPPING]))){
                         pMsg->identify=CMD_CTCPPING;
                     DEBUG("Found Command %s\n",CmdList[CMD_CTCPPING]);
                } else if (!strncmp(pStr,CmdList[CMD_CTCPVERSION],strlen(CmdList[CMD_CTCPVERSION]))){
                         pMsg->identify=CMD_CTCPVERSION;
                     DEBUG("Found Command %s\n",CmdList[CMD_CTCPVERSION]);
                } else if (!strncmp(pStr,CmdList[CMD_CTCPTIME],strlen(CmdList[CMD_CTCPTIME]))){
                            pMsg->identify=CMD_CTCPTIME;
                    DEBUG("Found Command %s\n",CmdList[CMD_CTCPTIME]);
                }
            }
        }
    }
	
	pMsg->pMsgLine=(char*)malloc((strlen(pLine)+1)*sizeof(char));
    strcpy(pMsg->pMsgLine,pLine);
    return pMsg;
}

void *CommandExecutionThread(void *argv) {
	extern boolean stop;
    QueueData *pCommand;
	MsgBuf_t *pMsg;
    MsgItem_t *pMsgItem;
	
    char *pNetmask;
    char *pChannel;
    char *pNick;
    UserLevel_t Level;
	AnswerMode_t AnswerMode;

	PQueue pCommandQueue=(PQueue)argv;	

    // set the thread cancelable
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

    DEBUG("Execution thread is running (%d)\n",getpid());

    // execute loop
    while(!stop) {

        pCommand=popQueue(pCommandQueue);
        
        if (pCommand) {
            pMsg=(MsgBuf_t*)pCommand->data;


            // extract the needed values
            pNetmask=getNetmask(pMsg->pMsgLine);
            pChannel=getAccessChannel(pMsg->pMsgLine);
            pNick=getNickname(pMsg->pMsgLine);
            AnswerMode=getAnswerMode(pMsg->pMsgLine);
            Level=getUserLevel(pChannel,pNetmask);

            // check the access level
            if (AccessRight(Level,pMsg->identify)) {
                
                // fill the parameter
                pMsgItem=(MsgItem_t *)malloc(sizeof(MsgItem_t));
                pMsgItem->pAccessChannel=pChannel;
                pMsgItem->pCallingNick=pNick;
                pMsgItem->pNetmask=pNetmask;
                pMsgItem->UserLevel=Level;
                pMsgItem->pRawLine=pMsg->pMsgLine;
                pMsgItem->AnswerMode=AnswerMode;
                

                // command router 
                switch (pMsg->identify) {
                // Event handler
                case CMD_ONPING:
					pong(NULL);
                    break;
                case CMD_ONNICKCHG:
                    hNickChange(pMsg->pMsgLine);
                    break;
                case CMD_ONMODE:
                    hResetModes(pMsg->pMsgLine);
                    break;
                case CMD_ONKICK:
                    hRejoinAfterKick(pMsg->pMsgLine);
                    break;
                case CMD_ONTOPIC:
                    hResetTopic(pMsg->pMsgLine);
                    break;
                case CMD_ONWHOIS:
                    hCallback(pMsg->pMsgLine);
                    break;
                case CMD_ONNONICK:
                    hWhoisFailed(pMsg->pMsgLine);
                    break;
                case CMD_ONNAMES:
                    hBotNeedOp(pMsg->pMsgLine);
                    break;
                case CMD_ONJOIN:
                    hSetModUser(pMsg->pMsgLine);
                
                // Command handler
                case CMD_VIEWGREAT:
                    greeting(pMsgItem);
                    break;
                case CMD_ONQUIT:
                    logoff(pMsgItem,false);
                case CMD_LOGOFF:
                    logoff(pMsgItem,true);
                    break;
                case CMD_HELP:
                    help(pMsgItem);
                    break;
                case CMD_VERSION:
                    version(pMsgItem);
                    break;
                case CMD_HELLO:
                    hello(pMsgItem);
                    break;
                case CMD_PASS:
                    password(pMsgItem);
                    break;
                case CMD_IDENT:
                    ident(pMsgItem);
                    break;
                case CMD_ADDCHANNEL:
                    addChannel(pMsgItem);
                    break;
                case CMD_RMCHANNEL:
                    rmChannel(pMsgItem);
                    break;
                case CMD_JOIN:
                    joinChannel(pMsgItem);
                    break;
                case CMD_PART:
                    partChannel(pMsgItem);
                    break;
                case CMD_DIE:
                    die(pMsgItem);
                    break;
                case CMD_NICK:
                    setNick(pMsgItem);
                    break;
                case CMD_CHANNELS:
                    chanlist(pMsgItem);
                    break;
                case CMD_SET_GREATING:
                    setGreeting(pMsgItem);
                    break;
                case CMD_SET_TOPIC:
                    setTopic(pMsgItem);
                    break;
                case CMD_SAY:
                    say(pMsgItem);
                    break;
                case CMD_KICK:
                    kickuser(pMsgItem);
                    break;
                case CMD_USERMODE:
                    usermode(pMsgItem);
                    break;
                case CMD_RMUSER:
                    rmuser(pMsgItem);
                    break;
                case CMD_USERLIST:
                    userlist(pMsgItem);
                    break;
                case CMD_ALLSAY:
                    allsay(pMsgItem);
                    break;
                case CMD_CHANMODE:
                    chanmode(pMsgItem);
                    break;
                case CMD_RESTART:
                    restart(pMsgItem);
                    break;
                case CMD_BAN:
                    banuser(pMsgItem);
                    break;
                case CMD_DEBAN:
                    debanuser(pMsgItem);
                    break;
                case CMD_INVITE:
                    inviteuser(pMsgItem);
                    break;
                case CMD_CTCPPING:
        		    ctcpping(pMsgItem);
        		    break;
           		case CMD_CTCPVERSION:
                    ctcpversion(pMsgItem);
                    break;
           		case CMD_CTCPTIME:
                    ctcptime(pMsgItem);
                    break;
           		default:
                    syslog(LOG_CRIT,getSyslogString(SYSLOG_UNKNOWN_CMDID));
                    break;
                }

                // remove  destroy the last popped Command
                free(pMsg->pMsgLine);
                free(pMsg);
                
                free(pMsgItem);
                free(pCommand);
            
            } else {
                notice(pNick,getMsgString(ERR_NOT_ACCESS));
            }

            if (pNetmask) free(pNetmask);
            if (pNick) free(pNick);
            if (pChannel) {free(pChannel);}
        }
    }
    DEBUG("Execution thread is stopped\n");
    return NULL;
}

// #############################################################################
static int AccessRight(UserLevel_t Level,Cmd_t cmd_id) {
    boolean ret=false;  

    // check Accesslevel
    if (cmd_id >= CMD_EVENT) {
        if (((cmd_id == CMD_ONNICKCHG) || (cmd_id==CMD_ONQUIT)) && Level>=LoggedLevel) {
            ret=true;;
        }else if ((cmd_id != CMD_ONNICKCHG) && (cmd_id!=CMD_ONQUIT)) {
            ret=true;
        }
	if (cmd_id >= CMD_OTHERS) {
            ret=true;
            if (cmd_id >= CMD_LOGGED) {
                if (Level>= LoggedLevel) {
                    ret=true;
                    if (cmd_id >= CMD_FRIEND) {
                        if (Level>= FriendLevel) {
                            ret=true;
                                if (cmd_id >= CMD_OWNER) {
                                    if (Level>= OwnerLevel) {
                                        ret=true;
                                        if (cmd_id >= CMD_MASTER) {
                                            if (Level==MasterLevel) {
                                                ret=true;
                                            }else {ret=false;}
                                        }
                                    }else{ret=false;}
                                }
                        } else{ret=false;}
                    }
                }else{ret=false;}
            }
        } 
    }
   DEBUG("Userlevel %i AccessStatus %i of the command id %i\n",Level,ret,cmd_id);
    return ret;
}
// #############################################################################
RETSIGTYPE stopParser(int sig) {
    extern int stop;
    if (!stop) {
        quit();
    }

    stop=true;
    DEBUG("Stop IRCBot\n");
}
