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

#include <stdlib.h>
#include <assert.h>
#include "irchelp.h"

static IrcHelp_t   sIrcHelp[CMD_COUNT]={
    {
        CMD_NONE,
        {
            "This is the help system for using this bot.",
            "!help COMMAND for information about a special command",
            "------------------------------------------------------------------------",
            NULL
        },
        "",
        "Syntax:"
    },
    {CMD_ONPING,{NULL},"",""},
    {CMD_ONQUIT,{NULL},"",""},
    {CMD_ONNICKCHG,{NULL},"",""},
    {CMD_ONMODE,{NULL},"",""},
    {CMD_ONKICK,{NULL},"",""},
    {CMD_ONJOIN,{NULL},"",""},
    {CMD_ONNAMES,{NULL},"",""},
    {CMD_ONTOPIC,{NULL},"",""},
    {CMD_ONWHOIS,{NULL},"",""},
    {CMD_ONNONICK,{NULL},"",""},
    {CMD_ONPRIVMSG,{NULL},"",""},
    {CMD_ONNOTICE,{NULL},"",""},
    {CMD_CTCPPING,{NULL},"",""},
    {CMD_CTCPVERSION,{NULL},"",""},
    {CMD_CTCPTIME,{NULL},"",""},
    {
        CMD_VIEWGREAT,
        {
            "Using this command you can inspect the greeting of a",
            "given channel. If no channel is given the greeting of",
            "the current channel will be viewed.",
            NULL
        },
        "shows the greeting line of a channel",
        "!viewgreet <#Channel>"
    },{
        CMD_HELP,
        {
            "Displays general information about the bot and hints for",
            "using the available commands. A list of commands will be",
            "displayed too.",
            NULL
        },
        "give general help for using this bot",
        "!help <Command>"
    },{
        CMD_VERSION,
        {
            "Displays version and compile date of the bot.",
            NULL
        },
        "print the version information",
        "!version"
    },{
        CMD_HELLO,
        {
            "Every user who wants to use the bot needs an account.",
            "Using this command you get one. Your login is the nickname",
            "you had when sending this command. REMEMBER to set a",
            "password with !passwd",
            NULL
        },
        "creates a new account.",
        "!hello"
    },{
        CMD_IDENT,
        {
            "Any user who has got an account, must identify himself",
            "before bot commands are accepted. This is done by this command.",
            NULL
        },
        "identify yourself to the bot",
        "!ident [Login] <Password>"
    },{
        CMD_LOGOFF,
        {
            "If you want to disconnect from the bot,",
            "you should logoff first.",
            NULL
        },
        "logoff from the bot",
        "!logoff"
    },{
        CMD_PASS,
        {
            "Selfexplaining. Change your account password (see also !help hello)",
            "or set it if you don't have one yet.",
            NULL
        },
        "change the password for the current account",
        "!pass <Password>"
    },{
        CMD_INVITE,
        {
            "If a channel is set invite-only, you can only join this channel",
            "if a user in it asks you to (invites you). So if only the bot has",
            "joined the invite-only channel you can use this command to make the",
            "bot invite you so you can join.",
            NULL
        },
        "invite the user in a channel",
        "!invite <#channel> [nick]"
    },{
        CMD_CHANMODE,
        {
            "Change channel modes (e.g. moderated [+m], topic protected +[t] etc.)",
            NULL
        },
        "Set and remove the modes for a channel.",
        "!chanmod <#Channel> [+|-][tnmpsklrqa] <Limit> <Keyword>"
    },{
        CMD_ACCOUNTLIST,
        {
            "If a channel is given as parameter, all users with an account in this",
            "channel are listed. If no channel given, all users with an account in",
            "any channel are listed.",
            NULL
        },
        "print a list of users from the channel or all",
        "!accountlist <#Channel>"
    },{
        CMD_ACCOUNTMODE,
        {
            "User modes are for example Channel-Operator (+o) or Voice (+v)",
            "m...Master",
            "o...Owner",
            "v...Friend",
            NULL
        },
        "set the mode for a user",
        "!accountmode <#Channel> [Login] [+|-][mov]"
    },{
        CMD_SAY,
        {
            "The bot posts the given message into this channel.",
            NULL
        },
        "send a message into a channel",
        "!say <#Channel> [Text]"
    },{
        CMD_KICK,
        {
            "Disconnects a user from the given channel",
            NULL
        },
        "kicks a user out of the channel",
        "!kick <#Channel> [Nickname]"
    },{
        CMD_SET_TOPIC,
        {
            "Topic is the line that can be seen near the channel name",
            "in the channel list.",
            "EXAMPLE: #LUGse -== LUGse - Linux User Group Senftenberg ==-",
            "Used to discripe a channel.",
            NULL
        },
        "sets or removes the  topic of a channel",
        "!topic <#Channel> [Topic]"
    },{
        CMD_SET_GREATING,
        {
            "The greeting is a message the bot sends to the channel",
            "when a user joins the channel.",
            NULL
        },
        "Sets or removes the greeting line for a channel",
        "!greeting <#Channel> [Text]"
    },{
        CMD_BAN,
        {
            "Ban means the user can't join the channel until",
            "he is debanned or changes his nick.",
            NULL
        },
        "bans a user from the channnel",
        "!ban <#channel> [Nickname]"
    },{
        CMD_DEBAN,
        {
            "Removes an existing ban from a user so he can join the",
            "channel again he was banned from.",
            NULL
        },
        "resets the ban from a user",
        "!deban <#channel> [Banmask]"
    },{
        CMD_RESTART,
        {
            "Usefull if configuration is changed. By restarting config is read again.",
            NULL
        },
        "stops and restarts the bot.",
        "!restart"
    },{
        CMD_ALLSAY,
        {
            "Similar to SAY, but message is shown in all channels the bot has joined.",
            NULL
        },
        "sends a message to all channels",
        "!allsay [Text]"
    },{
        CMD_RMUSER,
        {
            "Selfexplaining. Accounts no longer needed can be removed this way.",
            NULL
        },
        "removes a user account of the bot",
        "!rmaccount [Login]"
    },{
        CMD_NICK,
        {
            "The given nickname is only valid until the bot is restarted.",
            NULL
        },
        "changes the nickname of the bot",
        "!nick [Nickname]"
    },{
        CMD_DIE,
        {
            "The bot is stoped but unlike RESTART, DIE doesn't start",
            "the bot again.",
            NULL
        },
        "shutdown the bot.",
        "!die"
    },{
        CMD_CHANNELS,
        {
            "Only channels the bot is within are listed.",
            NULL
        },
        "prints out a list of all channels from the bot",
        "!chanlist"
    },{
        CMD_RMCHANNEL,
        {
            "The bot leaves the given channel and removes it",
            "from its channel list.",
            NULL
        },
        "let the bot part and remove a old channel",
        "!rmchannel <#Channel>"
    },{
        CMD_ADDCHANNEL,
        {
            "The channel must be given as a parameter.",
            "The bot creates the channel, inserts it into his",
            "channel list and joins the channel.",
            NULL
        },
        "let the bot join and add a new channel",
        "!addchannel [#Channel]"
    },{
        CMD_JOIN,
        {
            "The channel must be given as a parameter and must exist",
            "before the bot joins it.",
            NULL
        },
        "let the bot join a channel",
        "!join [#Channel]"
    },{
        CMD_PART,
        {
            "The bot leaves the given channel. The channel is not",
            "removed from the bots channel list.",
            NULL
        },
        "let the bot part a channel",
        "!part <#Channel>"
    },
};


IrcHelp_t * GetIrcHelp(Cmd_t id) {
    assert(sIrcHelp[id].CmdID == id);
    return &sIrcHelp[id];
}
