/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * #############################################################
 */

#ifndef MESSAGE_GER_H
#define MESSAGE_GER_H

#include "../messages.h"

	// Here insert all message in german

// syslog
static MsgStruct_t Syslog[]= {
	{ SYSLOG_READ_CMD,		"Kommandozeilenparameter gefunden und eingelesen"},
	{ SYSLOG_READ_CONFFILE,		"Konfigurationsdatei gefunden und eingelesen"},
	{ SYSLOG_INIT_DB,		"Initialisierung der Datenbank"},
	{ SYSLOG_TRY_CONNECT,		"Versuche zu verbinden"},
	{ SYSLOG_IS_CONNECT,		"Mit Server verbunden"},
	{ SYSLOG_BOT_RUN,		"Wird ausgeführt...."},
	{ SYSLOG_BOT_STOP,		"Stoppe...."},
	{ SYSLOG_BOT_START,		"Starte..."},
	{ SYSLOG_UNKNOWN_CMDID,		"Unbekannte Kommando ID"},
	{ SYSLOG_ACCOUNT_RM,		"Ein Account wurde von der Liste entfernt"},
	{ SYSLOG_FAILED_NETPARA,	"Der Servername oder die Portnummer wurden nicht gesetzt"},
	{ SYSLOG_MSG_QUEUE,		"Kann keine Messagequeue anlegen"},
	{ SYSLOG_RESOLVE_HOSTNAME,	"Kann Servernamen nicht auflösen"},
	{ SYSLOG_RESOLVE_PROTOCOL,	"TCP ist ein unbekanntes Protokoll"},
	{ SYSLOG_SOCKET,		"Kann Socket nicht anlegen"},
	{ SYSLOG_CONNECT,		"Verbindung fehlgeschlagen"},
	{ SYSLOG_CONFIG_FILE,		"Kann Konfigurationsdatei nicht öffen"},
	{ SYSLOG_SEND,			"Kann nicht senden"},
	{ SYSLOG_RECV,			"Kann nichts empfangen"},
	{ SYSLOG_TIMEOUT,		"Timeout beim Empfang"},
	{ SYSLOG_CREATE_DIR,		"Lege Verzeichnis für die Datenbank an"},
	{ SYSLOG_CREATE_DIR_ERR,	"Kann kein Verzeichnis für die Datenbank anlegen"},
	{ SYSLOG_DATABASE_ERR,		"Kann Datenbank nicht öffen"},
	{ SYSLOG_RESTART,		"Neustart..."},
	{ SYSLOG_STOPPED,		"Angehalten..."}
};

// excute error
static MsgStruct_t Msg[] ={
   	// Messages for the master dialog
	{ INFO_MASTER_TITLE,	"Masteraccount anlegen\n\n" },
	{ INFO_MASTER_LOGIN,	"Loginname : " },
	{ INFO_MASTER_PASS,	"\nPasswort  : " },
	{ INFO_MASTER_REPASS,	"Passwort wiederholen: " },
	{ ERR_MASTER_PASS,	"Passwörter nicht identisch" },
	{ ERR_MASTER,		"Kann den Account nicht als Master anlegen"  },

    // output of strerr
    { ERR_RESTART,		"Kann nicht neustarten" },
	{ ERR_MISSING_PARAM,	"Parameter fehlt" },
	{ ERR_PORT_PARAMETER,	"Portnummer liegt ausserhalb des Bereiches" },
	{ ERR_WRONG_SERVERNAME,	"Servername ist unzulässig" },
	{ ERR_WRONG_BOTNAME,	"Botname ist unzulässig" },
	{ ERR_ALT_RANGE,	"Die Accountlebenserwartung ist ungültig" },
	{ ERR_PINGTIMEOUT_RANGE,"Die Pingtimeoutzeit ist ungültig" },
	{ ERR_SENDDELAY_RANGE,	"Die Sendeverzögerung ist ungültig" },
    { ERR_SENDLINE_RANGE,	"Das Limit für das Senden mit der 1. Verzögerung ist ungültig" },
	{ ERR_LOGOFF_RANGE,	"Die Auslogzeit ist ungültig" },
	{ ERR_THREAD_RANGE,	"Das Threadlimit liegt ausserhalb des Bereiches" },
	{ ERR_NICK,		"Kann Nicknamen nicht setzen" },
	{ ERR_MUTEX,		"Synchronisation der Threads nicht möglich" },
	{ ERR_NOT_ALLOW_CHAR,	"Nicht erlaubte Zeichen wurden verwendet" },


    // command acknowledgment
	{ OK_HELLO,		"Schön dich zu treffen" },
	{ OK_HELLO2,		"Du hast einen neuen Account erhalten" },
	{ OK_PASSWD,		"Password wurde geändert" },
	{ OK_IDENT,		"Du bist nun identifiziert" },
	{ OK_JOIN,		"Der Bot ist in den Channel gejoint" },
	{ OK_PART,		"Der Bot hat den Channel verlassen" },
	{ OK_RMCHANNEL,		"Der Channel wurde von der Liste entfernt" },
	{ OK_ADDCHANNEL,	"Der Channel wurde der Liste hinzugefügt" },
	{ OK_NICK_SET,		"Der Bot versucht den neuen Nickname zusetzen" },
	{ OK_LOGOFF,		"Du bist ausgeloggt" },
	{ OK_SET_GREETING,	"Die neue Begrüssung wurde gesetzt" },
	{ OK_RM_GREETING,	"Die Begrüssung wurde entfernt" },
	{ OK_SET_TOPIC,		"Die neue Topic wurde gesetzt" },
	{ OK_RM_TOPIC,		"Die Topic wurde entfernt" },
	{ OK_KICK,		"Der Benutzer wurde vom Bot gekickt" },
	{ OK_BAN,		"Der Benutzer wurde vom Bot gebannt" },
    { OK_DEBAN,			"Der Ban wurde vom Bot aufgehoben" },
    { OK_USERMODE,		"Die Benutzermodus wurde geändert " },
	{ OK_RMUSER,		"Der Account wurde entfernt" },
	{ OK_MASTER,		"Du bist ein Bot Master" },
	{ OK_DIE,		"Der Bot beendet sich" },
	{ OK_RESTART,		"Der Bot startet neu" },

	// Informations for invalid input or using of the bot
	{ ERR_NOT_LOGON,	"Du bist nicht identifiziert" },
	{ ERR_NOT_MASTER,	"Du bist kein Botmaster" },
	{ ERR_NOT_OWNER,	"Du bist nicht der Channeleigentümer" },
	{ ERR_NOT_FRIEND,		"Du bist kein Freund des Channels" },
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
	{ ERR_NICK_EXIST,	"Es gibt schon ein Account mit diesem Nicknamen" },
	{ ERR_ALREADY_LOGON,	"Du bist schon identifiziert" },
	{ ERR_UNKNOWN_MODS,	"Dieser Modus ist ungültig"  },
	{ ERR_USERLIST_EMPTY,	"Die Benutzerliste ist leer"  },
	{ ERR_UNKNOW_MODE,	"Unbekannter Parameter" },
	{ ERR_NOTSELF_KICK,	"Kann mich nicht selbst kicken."  },
    { ERR_NOTSELF_BAN,		"Kann mich nicht selbst bannen."  },

	// other informations
	{ INFO_HELP_END,	"Ende der Hilfe." },
	{ INFO_HELP_FOR,	"Hilfe für" },
	{ INFO_USERLIST_BEGIN,	"Bentuzerliste:" },
	{ INFO_USERLIST_END,	"Ende der Benutzerliste." },
	{ INFO_CHANNELLIST_BEGIN,	"Channelliste:" },
	{ INFO_CHANNELLIST_END,		"Ende der Channelliste." },
	{ INFO_CHANNELLIST_MODE,    "Channelmodus:"  },
	{ INFO_CHANNELLIST_TOPIC,	"Topic:"  },
	{ INFO_CHANNELLIST_GREET,	"Begrüssung:" },
	{ INFO_DEFAULT_REASON,	"Du wurdest vom bot gekickt" },
	{ INFO_INIT_CHANNEL,	"Ich habe den Channel eingerichtet" },
	{ INFO_NEED_OP,		"Ich benötige Operatorrechte" },
	{ INFO_NOT_PASS,	"Du hast ein leeres Passwort gesetzt" }
};



	static char * unknow_parameter[]=
	{
		"Unbekannte Option\n",
		"Gib \"",PACKAGE," -h\" für mehr Information ein.\n",
		"."
	};



	static char  * help_msg[]=
	{
		"\n",
		PACKAGE," [Optionen]\n",
		"\n\tOptionen:\n",
		"\t-a Tage\t\tSetzt die maximale Loginzeit.\n",
		"\t-b Name\t\tSetzt den Namen des Bots.\n",
		"\t-c Sek\t\tSetzt das Timeoutlimit.\n",
		"\t-d Pfad\t\tsetzt den Pfad zu Datenbank an\n",
		"\t-f Datei\tSetzt Pfad zur Konfigurationdatei\n",
		"\t-h\t\tGibt diese Hilfe aus.\n",
		"\t-k Tage\t\tSetzt die maximal Lebenszeit eines Account bei.\n",
		"\t-m\t\tLegt neuen Master an.\n",
		"\t-n ms\t\tSetzt die Verzögerung  für das Senden.\n",
		"\t-p Port\t\tSetzt den Prot des IRC-Servers.\n"
		"\t-r String\tSetzt den String für Realnamefeld.\n",
		"\t-s Server\tSetzt den IRC Server.\n",
		"\t-t Anzahl\tSetzt die Anzaahl der zu erzeugenden Threads.\n",
        "\t-u user\t\tSetzt den User unter dem gestartet werden soll (nur root)\n",
		"\t-v\t\tGibt die Version aus.\n",
		"."
	};
#endif
