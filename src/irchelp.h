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

#if HAVE_CONFIG_H
    #include "config.h"
#endif


#ifdef LANG_DE
    #include "lang/irchelp_de.h"
#else 
    #include "lang/irchelp_eng.h"
#endif

static char *pIrcSyntax[][5] = {
    // header
    
    {
        "Syntax:",
        ".",
    },
    // commands
    
    // others
    // VIEWGREAT
    {
        "!viewgreet <#Channel>",
        ".",
    },
    // HELP
    {
        "!help <Command>",
        ".",
    },
    // VERSION
    {
        "!version",
        ".",
    },
    // HELLO
    {     
        "!hello",
        ".",
    },
    // IDENT
    {
        "!ident [Login] <Password>",
        ".",
    },

    // Logged user
    // LOGOFF
    {
        "!logoff",
        ".",
    },
    // PASS
    {
        "!pass <Password>",
        ".",
    },
    
    /* friend commands */
    // INVITE
    {
        "!invite <#channel> [nick]",
        ".",
    },

    // owner commands
    // CHANMODE
    {
        "!chanmod <#Channel> [+|-][tnmpsklrqa] <Limit> <Keyword>",
        ".",
    },
    // USERLIST
    {
        "!userlist <#Channel>",
        ".",
    },
    // USERMODE
    {
        "!usermode <#Channel> [Login] [+|-][mov]",
        "m...Master",
        "o...Owner",
        "v...Friend",
        ".",
    },
    // SAY
    {
        "!say <#Channel> [Text]",
        ".",
    },
    // KICK
    {
        "!kick <#Channel> [Nickname]",
        ".",
    },
    // SET_TOPIC
    {
        "!topic <#Channel> [Topic]",
        ".",
    },
    // SET_GREATING
    {
        "!greeting <#Channel> [Text]",
        ".",
    },
    // BAN
    {
        "!ban <#channel> [Nickname]",
        ".",
    },
    // DEBAN
    {
        "!deban <#channel> [Banmask]",
        ".",
    },
    
    // Master commands
    // RESTART
    {
        "!restart",
        ".",
    },
    // ALLSAY
    {
        "!allsay [Text]",
        ".",
    },
    // RMUSER
    {
        "!rmuser [Login]",
        ".",
    },
    // NICK
    {
        "!nick [Nickname]",
        ".",
        
    },
    // DIE
    {
        "!die",
        ".",
    },
    // CHANNELS
    {
        "!chanlist",
        ".",
    },
    // RMCHANNEL
    {
        "!rmchannel <#Channel>",
        ".",
    },
    // ADDCHANNEL
    {
        "!addchannel [#Channel]",
        ".",
    },
    // JOIN
    {
        "!join [#Channel]",
        ".",
    },
    // PART
    {
        "!part <#Channel>",
        ".",
    }
};
#endif
