/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * #############################################################
 */

#ifndef MESSAGES_ENG_H
#define MESSAGES_ENG_H

#include "../messages.h"


static MsgStruct_t Msg[] = {
  
	// command acknowledgment
    { OK_JOIN,			"The bot has joined the channel."},
	{ OK_PART,			"The bot has left the channel."},
	{ OK_RMCHANNEL,			"The channel is removed from the channel list."},
	{ OK_ADDCHANNEL,		"The channel is added to the channel list"},
	{ OK_NICK_SET,			"The bot tries to set the new nickname." },
	{ OK_LOGOFF,			"You're now logged off."},
	{ OK_SET_GREETING,		"The new greeting is set" },
	{ OK_RM_GREETING,		"The greeting is removed"},
	{ OK_SET_TOPIC,			"The new topic is set"},
	{ OK_RM_TOPIC,			"The topic is removed"},
	{ OK_KICK,			"The user is kicked by the bot" },
	{ OK_BAN,			"The user is banned by the bot" },
    	{ OK_DEBAN,			"The ban is removed by the bot" },
	{ OK_USERMODE,			"The user mod are changed" },
	{ OK_RMUSER,			"The account is removed"},
	{ OK_MASTER,			"You are a bot master"},
	{ OK_DIE,			"The bot is going  down"},
	{ OK_RESTART,			"The bot is going to restart"},

	// Informations for invalid input or using of the bot
	{ ERR_NOT_MASTER,			"You're not a irc bot master."},
	{ ERR_NOT_CHANNELOPT,		"This command requieres a channel name."},
    	{ ERR_NOT_PARAMETER,       	"I couldn't found regulare parameters."},
	{ ERR_NOT_COMMAND,		"No help found for this command."},
	{ ERR_NOT_ACCOUNT,		"Account was not found." },
	{ ERR_NOT_CHANNEL,		"This channel isn't in the channel list."},
	{ ERR_NOT_ACCESS,		"Access denied" },
	{ ERR_NOT_SELF,			"You can't modify yourself"},
    	{ ERR_NOT_KICK,			"Can't kick this user"},
    	{ ERR_NOT_BAN,			"Can't ban this user"},

    // Errors
	{ ERR_ADDCHANNEL_ALREADY,	"This channel is already in the channel list." },
	{ ERR_NICK_INVALID,		"Nickname is invalid."},
	{ ERR_ALREADY_LOGON,		"You're already identified." },
	{ ERR_UNKNOWN_MODS,		"This mod is invalid."},
    { ERR_NOTSELF_KICK,			"Can't kick myself"},
    { ERR_NOTSELF_BAN,			"Can't ban myself"},
    { ERR_WRONG_PW,             "Invalid password"},

	// other informations
	{ INFO_HELP_END,		"End of help."},
	{ INFO_HELP_FOR,		"Help for"},
	{ INFO_USERLIST_BEGIN,		"User list:"},
	{ INFO_USERLIST_END,		"End of user list."},
	{ INFO_CHANNELLIST_BEGIN,	"Channel list:"},
	{ INFO_CHANNELLIST_END,		"End of channel list."},
	{ INFO_CHANNELLIST_TOPIC,	"Topic:"},
	{ INFO_CHANNELLIST_GREET,	"Greeting:"},
	{ INFO_DEFAULT_REASON,		"You are kicked from the bot."},
	{ INFO_INIT_CHANNEL,		"has initialized this channnel"},
	{ INFO_NEED_OP,			"needs operator access rights." },
    { INFO_NOT_PASS,			"You've set a empty password."},


};


#endif
