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
    
};


#endif
