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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

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

extern boolean stop;

void preParser(char *pLine,MsgBuf_t *pMsg) {
    char *pStr=NULL,*pPreamble=NULL,*pPos=NULL;
    int i=0;

    /* reset the buffer */
    pMsg->identify=CMD_NONE;
    pMsg->pMsgLine=NULL;

    /* get the first part of the  answer from server */
    pPreamble=getCommand(pLine);
    if (pPreamble){
        /* look for  command position */
        if (!(pPos=strchr(pPreamble,' '))){
		/* print log because no password was send */
            return;
        } else {
            /*set the pointr to the comaman start */
            pPos++;
        }
    } else {
        pPos=pLine;
    }

    

    /* preparse the line */
    /* identify events and commands */
    if (!strncmp(pPos,CmdList[CMD_ONPING],strlen(CmdList[CMD_ONPING]))) {
        /*
         * The ping can't send over the threads. If you have high usage of the
         * threads then is the execution the pong request to slow. It must send
         * direct from the parser
         *
         * pMsg->identify=CMD_ONPING;
         */
		if (!(pStr=strstr(pPos," "))) {		
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
                
                for (i=CMD_FIRST;i<CMD_COUNT;i++) {
                    if (!strncmp(pStr,CmdList[i],strlen(CmdList[i]))){
                        pMsg->identify=i;
                        logger(LOG_DEBUG,_("Found command %s"),CmdList[i]);
                        i=CMD_COUNT;
                    }
                }
            }
        } else if ((pStr=strstr(pLine," :\001"))!=NULL) {
            /* ctcp command parser */
                logger(LOG_DEBUG,_("Receive: \"%s\""),pLine);
		if (strlen(pStr)>=3) {
                pStr+=2;
                /* command parser */
               if (!strncmp(pStr,CmdList[CMD_CTCPPING],strlen(CmdList[CMD_CTCPPING]))){
                         pMsg->identify=CMD_CTCPPING;
                     logger(LOG_DEBUG,_("Found ctcp command %s"),CmdList[CMD_CTCPPING]);
                } else if (!strncmp(pStr,CmdList[CMD_CTCPVERSION],strlen(CmdList[CMD_CTCPVERSION]))){
                         pMsg->identify=CMD_CTCPVERSION;
                     logger(LOG_DEBUG,_("Found ctcp command %s"),CmdList[CMD_CTCPVERSION]);
                } else if (!strncmp(pStr,CmdList[CMD_CTCPTIME],strlen(CmdList[CMD_CTCPTIME]))){
                            pMsg->identify=CMD_CTCPTIME;
                     logger(LOG_DEBUG,_("Found ctcp command %s"),CmdList[CMD_CTCPTIME]);
                }
            }
        }
    } else {
    }
	
    if (pMsg->identify!=CMD_NONE) {
        pMsg->pMsgLine=(char*)malloc((strlen(pLine)+1)*sizeof(char));
        strcpy(pMsg->pMsgLine,pLine);
    }

    free(pPreamble);
    return;
}

void *CommandExecutionThread(void *argv) {
    QueueData *pCommand;
	MsgBuf_t *pMsg;
    MsgItem_t sMsgItem;
	
	PQueue pCommandQueue=(PQueue)argv;	

    /* set the thread cancelable */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL);

    logger(LOG_DEBUG,_("Execution thread is running (pid%d)"),getpid());

    /* execute loop */
    while(!stop) {


        pCommand=popQueue(pCommandQueue);
        
        if (pCommand) {
            pMsg=(MsgBuf_t*)pCommand->data;


            /* extract the needed values */

            /* fill the parameter */
            sMsgItem.pNetmask=getNetmask(pMsg->pMsgLine);
            sMsgItem.pAccessChannel=getAccessChannel(pMsg->pMsgLine);
            sMsgItem.pCallingNick=getNickname(pMsg->pMsgLine);
            sMsgItem.AnswerMode=getAnswerMode(pMsg->pMsgLine);
            sMsgItem.UserLevel=getUserLevel(sMsgItem.pAccessChannel,sMsgItem.pNetmask);
            sMsgItem.pRawLine=pMsg->pMsgLine;
            sMsgItem.bInteractiveMode=true;
            sMsgItem.pParameter=getParameters(pMsg->pMsgLine);
            
            /* check the access level */
            if (AccessRight(sMsgItem.UserLevel,pMsg->identify)) {
            
                /* command router  */
                switch (pMsg->identify) {
                /* Event handler */
                case CMD_ONPING:
                    pong(NULL);
                    break;
                case CMD_ONNICKCHG:
                    hNickChange(&sMsgItem);
                    break;
                case CMD_ONMODE:
                    hResetModes(sMsgItem.pRawLine);
                    break;
                case CMD_ONKICK:
                    hRejoinAfterKick(&sMsgItem);
                    break;
                case CMD_ONTOPIC:
                    hResetTopic(&sMsgItem);
                    break;
                case CMD_ONWHOIS:
                    hCallback(sMsgItem.pRawLine);
                    break;
                case CMD_ONNONICK:
                    hWhoisFailed(sMsgItem.pRawLine);
                    break;
                case CMD_ONNAMES:
                    hBotNeedOp(&sMsgItem);
                    break;
                case CMD_ONJOIN:
                    hSetModUser(sMsgItem.pRawLine);
                
                /* Command handler */
                case CMD_VIEWGREAT:
                    greeting(&sMsgItem);
                    break;
                case CMD_ONQUIT:
                    sMsgItem.bInteractiveMode=false;
                    logoff(&sMsgItem);
                    break;
                case CMD_LOGOFF:
                    logoff(&sMsgItem);
                    break;
                case CMD_HELP:
                    help(&sMsgItem);
                    break;
                case CMD_VERSION:
                    version(&sMsgItem);
                    break;
                case CMD_HELLO:
                    hello(&sMsgItem);
                    break;
                case CMD_PASS:
                    password(&sMsgItem);
                    break;
                case CMD_IDENT:
                    ident(&sMsgItem);
                    break;
                case CMD_ADDCHANNEL:
                    addChannel(&sMsgItem);
                    break;
                case CMD_RMCHANNEL:
                    rmChannel(&sMsgItem);
                    break;
                case CMD_JOIN:
                    joinChannel(&sMsgItem);
                    break;
                case CMD_PART:
                    partChannel(&sMsgItem);
                    break;
                case CMD_DIE:
                    die(&sMsgItem);
                    break;
                case CMD_NICK:
                    setNick(&sMsgItem);
                    break;
                case CMD_CHANNELS:
                    chanlist(&sMsgItem);
                    break;
                case CMD_SET_GREATING:
                    setGreeting(&sMsgItem);
                    break;
                case CMD_SET_TOPIC:
                    setTopic(&sMsgItem);
                    break;
                case CMD_SAY:
                    say(&sMsgItem);
                    break;
                case CMD_KICK:
                    kickuser(&sMsgItem);
                    break;
                case CMD_ACCOUNTMODE:
                    accountmode(&sMsgItem);
                    break;
                case CMD_RMUSER:
                    rmaccount(&sMsgItem);
                    break;
                case CMD_ACCOUNTLIST:
                    accountlist(&sMsgItem);
                    break;
                case CMD_ALLSAY:
                    allsay(&sMsgItem);
                    break;
                case CMD_CHANMODE:
                    chanmode(&sMsgItem);
                    break;
                case CMD_RESTART:
                    restart(&sMsgItem);
                    break;
                case CMD_BAN:
                    banuser(&sMsgItem);
                    break;
                case CMD_DEBAN:
                    debanuser(&sMsgItem);
                    break;
                case CMD_INVITE:
                    inviteuser(&sMsgItem);
                    break;
                case CMD_CTCPPING:
        		    ctcpping(&sMsgItem);
        		    break;
           		case CMD_CTCPVERSION:
                    ctcpversion(&sMsgItem);
                    break;
           		case CMD_CTCPTIME:
                    ctcptime(&sMsgItem);
                    break;
           		default:
                    logger(LOG_CRIT,_("Unkown command id %d"),pMsg->identify);
		    break;
                }
            } else {
                notice(sMsgItem.pCallingNick,_("Access denied"));
            }


            free (sMsgItem.pAccessChannel);
            free (sMsgItem.pCallingNick);
            free (sMsgItem.pNetmask);
            free (sMsgItem.pRawLine);
            free (pMsg);
            
            free (pCommand);
        }
    }
    logger(LOG_DEBUG,_("Execution thread is stopped"));
    return NULL;
}

/* ############################################################################# */
static int AccessRight(UserLevel_t Level,Cmd_t cmd_id) {
    boolean ret=false;  

    /* check Accesslevel */
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
   logger(LOG_DEBUG,_("Access status for the userlevel %i and the command id %i is %i"),Level,cmd_id,ret);
    return ret;
}
/* ############################################################################# */
RETSIGTYPE stopParser(int sig) {
    if (!stop) {
        quit();
    }

    stop=true;
    logger(LOG_NOTICE,"Stop IRCBot");
}
