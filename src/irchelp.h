/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#ifndef IRCHELP_H
#define IRCHELP_H

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "command.h"

typedef struct {
    Cmd_t   CmdID;
    char *pIrcHelp[15];
    char *pShort;
    char *pIrcSyntax;
}IrcHelp_t ;


IrcHelp_t * GetIrcHelp(Cmd_t id);



        
#endif
