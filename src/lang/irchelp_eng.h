/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef IRCHELP_ENG_H
#define IRCHELP_ENG_H

static char *pIrcHelp[][15] = {
    // header
    
    {
        "This is the help system for using this bot.",
        "!help COMMAND for information about a special command",
        "------------------------------------------------------------------------",
    },
    // commands
    
    // others
    // VIEWGREAT
    {
        "This shows the greeting line of a channel",
    },
    // HELP
    {
        "give general help for using this bot",
    },
    // VERSION
    {
        "print the version information",
    },
    // HELLO
    {     
        "create a new account.",
	"Every user who wants to use the bot needs an account.",
	"Using this command you get one. Your login is the nickname,",
	"you had when sending this command. REMEMBER to set a",
	"password with !passwd",
    },
    // IDENT
    {
        "identify yourself to the bot",
	"Any user who has got an account, must identify himself,",
	"before bot commands are accepted. This is done by this command.",
    },

    // Logged user
    // LOGOFF
    {
        "logoff from the bot",
	"If you want to disconnect from the bot,",
	"you should logoff first.",
	
    },
    // PASS
    {
        "change the password for the current account",
	"Selfexplaining. Change your account password (see also !help hello)",
	"or set it if you don't have one yet.",
    },
    
    // owner commands
    // CHANMODE
    {
        "Set and remove the modes for a channel.",
	"Change channel modes (e.g moderated [+m], topic protected +[t] etc.)",
    },
    // USERLIST
    {
        "print a list of user from the channel or all",
	"If a channel is given as parameter, all users with an account in this",
	"channel are listed. If no channel given, all users with an account in",
	"any channel are listed.",
    },
    // USERMODE
    {
        "This is to set  the mod for a user",
	"User modes are for example Channel-Operator (+o) or Voice (+v)",
    },
    // SAY
    {
        "This posts a message into a channel",
	"Needs a channel as paramater. The bot posts the given",
	"message into this channel.",
    },
    // KICK
    {
        "This kicks a user out the channel",
	"Disconnects a user from the given channel",
    },
    // SET_TOPIC
    {
        "Set or remove the  topic of a channel",
	"Topic is the line that can be seen near the channel name",
	"in the channel list.",
	"EXAMPLE: #LUGse -== LUGse - Linux User Group Senftenberg ==-",
	"Used to discripe a channel.",
    },
    // SET_GREATING
    {
        "Set or remove the greeting line for a channel",
	"The greeting is a message the bot sends to the channel",
	"when a user joins the channel.",
    },
    // CHANNELS
    {
        "Print out a list of all channels from the bot",
	"Only channels the bot is within are listed.",
    },
    
    // Master commands
    // RESTART
    {
        "This command stop and restart the bot.",
	"Usefull if configuration is changed. By restarting config is read again.",
    },
    // ALLSAY
    {
        "Send a message to all channels",
	"Similar to SAY, but message is shown in all channels the bot is within.",
    },
    // RMUSER
    {
        "This remove a user account of the bot",
	"Selfexplaining. Accounts no longer needed can be removed this way.",
    },
    // NICK
    {
        "change the nickname of the bot",
	"The given nickname is only valid untill the bot is restarted.",
    },
    // DIE
    {
        "Shutdown the bot.",
    },
    // RMCHANNEL
    {
        "let the bot part and remove a old channel",
    },
    // ADDCHANNEL
    {
        "let the bot join and add a new channel",
    },
    // JOIN
    {
        "let the bot join a channel",
    },
    // PART
    {
        "let the bot part a channel",
    }
};

#endif
