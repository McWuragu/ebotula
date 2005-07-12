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

#include <time.h>
#include <sys/utsname.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "ctcp.h"

/* #########################################################################
   Bot comand: \001PING <ID>\001
   ######################################################################### */
void ctcpping(MsgItem_t *pMsg) {
    char *pPing;
    char pPong[64];
   if ((pPing=strstr(pMsg->pRawLine," :\001"))==NULL) {
 	/* ignoring errors */
        return;
    }
   pPing+=2;
   sprintf(pPong,"%s",pPing);
   notice(pMsg->pCallingNick,pPong);
}
/* #########################################################################
   Bot comand: \001VERSION\001
   ######################################################################### */
void ctcpversion(MsgItem_t *pMsg) {
    char pMsgStr[256];
    /*char pVerStr[256];*/
    struct utsname env;

    uname(&env);
    /* creat Versions String */
    /* sprintf(pVerStr,VERSIONSTR); */
    sprintf(pMsgStr,"\001VERSION %s:v%s:%s %s %s\001",PACKAGE,PACKAGE_VERSION,env.sysname,env.release,env.machine);
    notice(pMsg->pCallingNick,pMsgStr);
    return;
}
/* #########################################################################
   Bot comand: \001TIME\001
   ######################################################################### */
void ctcptime(MsgItem_t *pMsg) {
	char pMsgStr[64];
	time_t t;
	struct tm *pTm;
	time(&t);
	/* UTC-TIME */
	/* pTm=gmtime(&t);*/
	/* localtime */
	pTm=localtime(&t);
	strftime(pMsgStr,63,"\001TIME %a %b %d %H:%M:%S\001",pTm);
	notice(pMsg->pCallingNick,pMsgStr);
	return;
}
