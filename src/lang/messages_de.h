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

    // output of strerr
	{ ERR_MISSING_PARAM,	"Parameter fehlt" },
	{ ERR_PORT_PARAMETER,	"Portnummer liegt ausserhalb des Bereiches" },
	{ ERR_WRONG_SERVERNAME,	"Servername ist unzul�ssig" },
	{ ERR_WRONG_BOTNAME,	"Botname ist unzul�ssig" },
	{ ERR_ALT_RANGE,	"Die Accountlebenserwartung ist ung�ltig" },
	{ ERR_PINGTIMEOUT_RANGE,"Die Pingtimeoutzeit ist ung�ltig" },
	{ ERR_LOGLEVEL_RANGE,    "Das Loglevel ist ung�ltig"},
    { ERR_SENDDELAY_RANGE,	"Die Sendeverz�gerung ist ung�ltig" },
    { ERR_SENDLINE_RANGE,	"Das Limit f�r das Senden mit der 1. Verz�gerung ist ung�ltig" },
	{ ERR_INIT_DELAY,       "Die Verz�gerung f�r das Einrichten der Channels ist ung�ltig"},
    { ERR_LOGOFF_RANGE,	"Die Auslogzeit ist ung�ltig" },
	{ ERR_THREAD_RANGE,	"Das Threadlimit liegt ausserhalb des Bereiches" },
	{ ERR_MUTEX,		"Synchronisation der Threads nicht m�glich" },


    // command acknowledgment
	{ OK_HELLO,		"Sch�n dich zu treffen" },
	{ OK_HELLO2,		"Du hast einen neuen Account erhalten" },
	{ OK_PASSWD,		"Password wurde ge�ndert" },
	{ OK_IDENT,		"Du bist nun identifiziert" },
	{ OK_JOIN,		"Der Bot ist in den Channel gejoint" },
	{ OK_PART,		"Der Bot hat den Channel verlassen" },
	{ OK_RMCHANNEL,		"Der Channel wurde von der Liste entfernt" },
	{ OK_ADDCHANNEL,	"Der Channel wurde der Liste hinzugef�gt" },
	{ OK_NICK_SET,		"Der Bot versucht den neuen Nickname zusetzen" },
	{ OK_LOGOFF,		"Du bist ausgeloggt" },
	{ OK_SET_GREETING,	"Die neue Begr�ssung wurde gesetzt" },
	{ OK_RM_GREETING,	"Die Begr�ssung wurde entfernt" },
	{ OK_SET_TOPIC,		"Die neue Topic wurde gesetzt" },
	{ OK_RM_TOPIC,		"Die Topic wurde entfernt" },
	{ OK_KICK,		"Der Benutzer wurde vom Bot gekickt" },
	{ OK_BAN,		"Der Benutzer wurde vom Bot gebannt" },
    { OK_DEBAN,			"Der Ban wurde vom Bot aufgehoben" },
    { OK_USERMODE,		"Die Benutzermodus wurde ge�ndert" },
	{ OK_RMUSER,		"Der Account wurde entfernt" },
	{ OK_MASTER,		"Du bist ein Bot Master" },
	{ OK_DIE,		"Der Bot beendet sich" },
	{ OK_RESTART,		"Der Bot startet neu" },

	// Informations for invalid input or using of the bot
	{ ERR_NOT_LOGON,	"Du bist nicht identifiziert" },
	{ ERR_NOT_MASTER,	"Du bist kein Botmaster" },
	{ ERR_NOT_OWNER,	"Du bist nicht der Channeleigent�mer" },
	{ ERR_NOT_FRIEND,	"Du bist kein Freund des Channels" },
	{ ERR_NOT_CHANNELOPT,	"Dieses Kommando ben�tigt einen Channel" },
    { ERR_NOT_PARAMETER,	"Es konnnten keine g�ltigen Parameter gefunden werden."},
    { ERR_NOT_COMMAND,		"Es wurde keine Hilfe f�r das Kommando gefunden" },
	{ ERR_NOT_ACCOUNT,	"Der Account wurde nicht gefunden" },
	{ ERR_NOT_CHANNEL,	"Dieser Channel ist nicht auf der Channelliste" },
	{ ERR_NOT_ACCESS,	"Zugriff verweigert" },
	{ ERR_NOT_SELF,		"Du kannst dich nicht selbst modifizieren" },
	{ ERR_NOT_KICK,		"Kann leider diesen User nicht kicken" },
    { ERR_NOT_BAN,		"Kann leider diesen User nicht bannen" },

	// Errors
	{ ERR_ADDCHANNEL_ALREADY,	"Dieser Channel ist schon auf der Channelliste" },
	{ ERR_NICK_INVALID,	"Der Nickname ist ung�ltig" },
	{ ERR_ALREADY_LOGON,	"Du bist schon identifiziert" },
	{ ERR_UNKNOWN_MODS,	"Dieser Modus ist ung�ltig"  },
	{ ERR_USERLIST_EMPTY,	"Die Benutzerliste ist leer"  },
	{ ERR_UNKNOW_MODE,	"Unbekannter Parameter" },
	{ ERR_NOTSELF_KICK,	"Kann mich nicht selbst kicken."  },
    { ERR_NOTSELF_BAN,		"Kann mich nicht selbst bannen."  },
    { ERR_WRONG_PW,     "Falsches Passwort"},

	// other informations
	{ INFO_HELP_END,	"Ende der Hilfe." },
	{ INFO_HELP_FOR,	"Hilfe f�r" },
	{ INFO_USERLIST_BEGIN,	"Benutzerliste:" },
	{ INFO_USERLIST_END,	"Ende der Benutzerliste." },
	{ INFO_CHANNELLIST_BEGIN,	"Channelliste:" },
	{ INFO_CHANNELLIST_END,		"Ende der Channelliste." },
	{ INFO_CHANNELLIST_MODE,    "Channelmodus:"  },
	{ INFO_CHANNELLIST_TOPIC,	"Topic:"  },
	{ INFO_CHANNELLIST_GREET,	"Begr�ssung:" },
	{ INFO_DEFAULT_REASON,	"Du wurdest vom bot gekickt" },
	{ INFO_INIT_CHANNEL,	"hat den Channel eingerichtet" },
	{ INFO_NEED_OP,		"ben�tigt Operatorrechte" },
	{ INFO_NOT_PASS,	"Du hast ein leeres Passwort gesetzt" }
};



#endif
