/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

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
    // VIEWGREET
    {
        "This shows the greeting line of a channel",
	"Using this command you can inspect the greeting of a",
	"given channel. If no channel is given the greeting of",
	"the current channel will be viewed."
    },
    // HELP
    {
        "give general help for using this bot",
	"Displays general information about the bot and hints for",
	"using the available commands. A list of commands will be",
	"displayed too."
    },
    // VERSION
    {
        "print the version information",
	"Displays version and compile date of the bot."
    },
    // HELLO
    {
        "creates a new account.",
	"Every user who wants to use the bot needs an account.",
	"Using this command you get one. Your login is the nickname",
	"you had when sending this command. REMEMBER to set a",
	"password with !passwd",
    },
    // IDENT
    {
        "identify yourself to the bot",
	"Any user who has got an account, must identify himself",
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
    
    /* friend commands */
    /* INVITE */
    {
        "invite the user in a channel",
	"If a channel is set invite-only, you can only join this channel",
	"if a user in it asks you to (invites you). So if only the bot has",
	"joined the invite-only channel you can use this command to make the",
	"bot invite you so you can join."
    },


    // owner commands
    // CHANMODE
    {
        "Set and remove the modes for a channel.",
	"Change channel modes (e.g. moderated [+m], topic protected +[t] etc.)",
    },
    // USERLIST
    {
        "print a list of users from the channel or all",
	"If a channel is given as parameter, all users with an account in this",
	"channel are listed. If no channel given, all users with an account in",
	"any channel are listed.",
    },
    // USERMODE
    {
        "This is used to set the mod for a user",
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
        "This kicks a user out of the channel",
	"Disconnects a user from the given channel",
    },
    // SET_TOPIC
    {
        "Sets or removes the  topic of a channel",
	"Topic is the line that can be seen near the channel name",
	"in the channel list.",
	"EXAMPLE: #LUGse -== LUGse - Linux User Group Senftenberg ==-",
	"Used to discripe a channel.",
    },
    // SET_GREETING
    {
        "Sets or removes the greeting line for a channel",
	"The greeting is a message the bot sends to the channel",
	"when a user joins the channel.",
    },
    // CHANNELS
    {
        "Prints out a list of all channels from the bot",
	"Only channels the bot is within are listed.",
    },
    // BAN
    {
        "bans a user from the channnel",
	"Ban means the user can't join the channel until",
	"he is debanned or changes his nick."
    },
    // DEBAN
    {
        "resets the ban from a user",
	"Removes an existing ban from a user so he can join the",
	"channel again he was banned from."
    },

    // Master commands
    // RESTART
    {
        "This command stops and restarts the bot.",
	"Usefull if configuration is changed. By restarting config is read again.",
    },
    // ALLSAY
    {
        "Sends a message to all channels",
	"Similar to SAY, but message is shown in all channels the bot has joined.",
    },
    // RMUSER
    {
        "This removes a user account of the bot",
	"Selfexplaining. Accounts no longer needed can be removed this way.",
    },
    // NICK
    {
        "changes the nickname of the bot",
	"The given nickname is only valid until the bot is restarted.",
    },
    // DIE
    {
        "Shutdown the bot.",
	"The bot is stoped but unlike RESTART, DIE doesn't start",
	"the bot again."
    },
    // RMCHANNEL
    {
        "let the bot part and remove a old channel",
	"The bot leaves the given channel and removes it",
	"from its channel list."
    },
    // ADDCHANNEL
    {
        "let the bot join and add a new channel",
	"The channel must be given as a parameter."
	"The bot creates the channel, inserts it into his",
	"channel list and joins the channel."
    },
    // JOIN
    {
        "let the bot join a channel",
	"The channel must be given as a parameter and must exist",
	"before the bot joins it."
    },
    // PART
    {
        "let the bot part a channel",
	"The bot leaves the given channel. The channel is not",
	"removed from the bots channel list."
    }
};

#endif
