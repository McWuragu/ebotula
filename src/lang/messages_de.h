/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * #############################################################
 */

#ifndef MESSAGE_GER_H
#define MESSAGE_GER_H

#include "../messages.h"

	// Here insert all message in german


// excute error
static MsgStruct_t Msg[] ={

	// Informations for invalid input or using of the bot
	{ ERR_NOT_MASTER,	"Du bist kein Botmaster" },
	{ ERR_NOT_CHANNELOPT,	"Dieses Kommando benötigt einen Channel" },
    { ERR_NOT_PARAMETER,	"Es konnnten keine gültigen Parameter gefunden werden."},
    { ERR_NOT_COMMAND,		"Es wurde keine Hilfe für das Kommando gefunden" },
	{ ERR_NOT_ACCOUNT,	"Der Account wurde nicht gefunden" },
	{ ERR_NOT_CHANNEL,	"Dieser Channel ist nicht auf der Channelliste" },
	{ ERR_NOT_ACCESS,	"Zugriff verweigert" },
	{ ERR_NOT_SELF,		"Du kannst dich nicht selbst modifizieren" },
	{ ERR_NOT_KICK,		"Kann leider diesen User nicht kicken" },
    { ERR_NOT_BAN,		"Kann leider diesen User nicht bannen" },

	// Errors
	{ ERR_ADDCHANNEL_ALREADY,	"Dieser Channel ist schon auf der Channelliste" },
	{ ERR_NICK_INVALID,	"Der Nickname ist ungültig" },
	{ ERR_ALREADY_LOGON,	"Du bist schon identifiziert" },
	{ ERR_UNKNOWN_MODS,	"Dieser Modus ist ungültig"  },
	{ ERR_NOTSELF_KICK,	"Kann mich nicht selbst kicken."  },
    { ERR_NOTSELF_BAN,		"Kann mich nicht selbst bannen."  },
    { ERR_WRONG_PW,     "Falsches Passwort"},

	// other informations
	{ INFO_HELP_END,	"Ende der Hilfe." },
	{ INFO_HELP_FOR,	"Hilfe für" },
	{ INFO_USERLIST_BEGIN,	"Benutzerliste:" },
	{ INFO_USERLIST_END,	"Ende der Benutzerliste." },
	{ INFO_CHANNELLIST_BEGIN,	"Channelliste:" },
	{ INFO_CHANNELLIST_END,		"Ende der Channelliste." },
	{ INFO_CHANNELLIST_TOPIC,	"Topic:"  },
	{ INFO_CHANNELLIST_GREET,	"Begrüssung:" },
};



#endif
