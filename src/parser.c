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
#include <assert.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "extract.h"
#include "irc.h"
#include "dbaccess.h"
#include "cmdlist.h"
#include "queue.h"
#include "utilities.h"
#include "messages.h"
#include "parser.h"
#include "network.h"
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
    if (!strncmp(pPos,CmdHandleField[CMD_ONPING].pCmdString,strlen(CmdHandleField[CMD_ONPING].pCmdString))) {
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
	} else if (!strncmp(pPos,CmdHandleField[CMD_ONQUIT].pCmdString,strlen(CmdHandleField[CMD_ONQUIT].pCmdString))) {
        pMsg->identify=CMD_ONQUIT;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONJOIN].pCmdString,strlen(CmdHandleField[CMD_ONJOIN].pCmdString))) {
        pMsg->identify=CMD_ONJOIN;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONNICKCHG].pCmdString,strlen(CmdHandleField[CMD_ONNICKCHG].pCmdString))) {
        pMsg->identify= CMD_ONNICKCHG;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONMODE].pCmdString,strlen(CmdHandleField[CMD_ONMODE].pCmdString))) {
        pMsg->identify= CMD_ONMODE;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONKICK].pCmdString,strlen(CmdHandleField[CMD_ONKICK].pCmdString))) {
        pMsg->identify= CMD_ONKICK;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONTOPIC].pCmdString,strlen(CmdHandleField[CMD_ONTOPIC].pCmdString))) {
        pMsg->identify= CMD_ONTOPIC;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONNAMES].pCmdString,strlen(CmdHandleField[CMD_ONNAMES].pCmdString))) {
        pMsg->identify=CMD_ONNAMES;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONWHOIS].pCmdString,strlen(CmdHandleField[CMD_ONWHOIS].pCmdString))) {
        pMsg->identify=CMD_ONWHOIS;
    } else if (!strncmp(pPos,CmdHandleField[CMD_ONNONICK].pCmdString,strlen(CmdHandleField[CMD_ONNONICK].pCmdString))) {
        pMsg->identify=CMD_ONNONICK;
    } else if (!strncmp(pPos,"PRIVMSG",strlen("PRIVMSG")) ||
               !strncmp(pPos,"NOTICE",strlen("NOTICE"))) {
        if ((pStr=strstr(pLine," :!"))!=NULL) {
            /* bot command parser */
            if (strlen(pStr)>=3) {
                pStr+=3;
                
                for (i=CMD_FIRST;i<CMD_COUNT;i++) {
                    
                    /* check the correctness of the command  array */
                    assert(i==CmdHandleField[i].CmdId);


                    if (!strncmp(pStr,CmdHandleField[i].pCmdString,strlen(CmdHandleField[i].pCmdString))){
                        pMsg->identify=i;
                        logger(LOG_DEBUG,_("Found command %s"),CmdHandleField[i].pCmdString);
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
               if (!strncmp(pStr,CmdHandleField[CMD_CTCPPING].pCmdString,strlen(CmdHandleField[CMD_CTCPPING].pCmdString))){
                         pMsg->identify=CMD_CTCPPING;
                     logger(LOG_DEBUG,_("Found ctcp command %s"),CmdHandleField[CMD_CTCPPING].pCmdString);
                } else if (!strncmp(pStr,CmdHandleField[CMD_CTCPVERSION].pCmdString,strlen(CmdHandleField[CMD_CTCPVERSION].pCmdString))){
                         pMsg->identify=CMD_CTCPVERSION;
                     logger(LOG_DEBUG,_("Found ctcp command %s"),CmdHandleField[CMD_CTCPVERSION].pCmdString);
                } else if (!strncmp(pStr,CmdHandleField[CMD_CTCPTIME].pCmdString,strlen(CmdHandleField[CMD_CTCPTIME].pCmdString))){
                            pMsg->identify=CMD_CTCPTIME;
                     logger(LOG_DEBUG,_("Found ctcp command %s"),CmdHandleField[CMD_CTCPTIME].pCmdString);
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
                if (pMsg->identify> CMD_NONE && pMsg->identify<CMD_COUNT) {
                    /* check the  correctness of  command id */
                    assert(pMsg->identify==CmdHandleField[pMsg->identify].CmdId);

                    sMsgItem.bInteractiveMode=CmdHandleField[pMsg->identify].bIsCommand;
                    CmdHandleField[pMsg->identify].CommandHandler(&sMsgItem);
                }else{
                    logger(LOG_CRIT,_("Unkown command id %d"),pMsg->identify);
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
