/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef MESSAGE_GER_H
#define MESSAGE_GER_H

	// Here insert all message in german
	// syslog
	#define SYSLOG_READ_CMD			"Kommandozeilenparameter gefunden und eingelesen"
	#define SYSLOG_READ_CONFFILE		"Konfigurationsdatei gefunden und eingelesen"
	#define SYSLOG_INIT_DB			"Initialisierung der Datenbank"
	#define SYSLOG_TRY_CONNECT		"Versuche an %s zu verbinden:%s"
	#define SYSLOG_IS_CONNECT		"Mit Server verbunden"
	#define SYSLOG_BOT_RUN			"Wird ausgeführt...."
	#define SYSLOG_BOT_STOP			"Stoppe...."
	#define SYSLOG_THREAD_RUN		"Thread %d gestartet"
	#define SYSLOG_BOT_START		"Starte..."
	#define SYSLOG_UNKNOWN_CMDID		"Unbekannte Kommando ID %d"
	#define SYSLOG_LOGIN_RM			"Der Account %s wurde automatisch abgemeldet"
	#define SYSLOG_ACCOUNT_RM       	"Der Account %s wurde von der Liste entfernt"
	#define SYSLOG_FAILED_NETPARA		"Der Servername oder die Portnummer wurden nicht gesetzt"
	#define SYSLOG_MSG_QUEUE		"Kann keine Messagequeue anlegen"
	#define SYSLOG_RESOLVE_HOSTNAME		"Kann Servernamen nicht auflösen"
	#define SYSLOG_RESOLVE_PROTOCOL		"TCP ist ein unbekanntes Protokoll"
	#define SYSLOG_SOCKET			"Kann Socket nicht anlegen"
	#define SYSLOG_CONNECT			"Verbindung fehlgeschlagen"
	#define SYSLOG_CONFIG_FILE		"Kann Konfigurationsdatei nicht öffen"
	#define SYSLOG_SEND			"Kann nicht senden"
	#define SYSLOG_RECV			"Kann nichts empfangen"
	#define SYSLOG_TIMEOUT			"Timeout beim Empfang"
	#define SYSLOG_CREATE_DIR		"Lege Verzeichnis für die Datenbank an"
	#define SYSLOG_CREAT_DIR_ERR 		"Kann kein Verzeichnis für die Datenbank anlegen"
	#define SYSLOG_DATABASE_ERR		"Kann Datenbank nicht öffen"
	#define SYSLOG_RESTART			"Neustart..."
	#define SYSLOG_STOPPED			"Angehalten..."
	
	// excute error
	// output of strerr
	#define ERR_RESTART			"Kann nicht neustarten"
	#define ERR_MISSING_PARAM		"Parameter fehlt"
	#define ERR_PORT_PARAMETER		"Portnummer liegt ausserhalb  des Bereiches"
	#define ERR_WRONG_SERVERNAME		"Servername ist unzulässig"
	#define ERR_WRONG_BOTNAME		"Botname ist unzulässig"
	#define ERR_ALT_RANGE			"Die Accountlebenserwartung ist ungültig"
	#define ERR_PINGTIMEOUT_RANGE		"Die Pingtimeoutzeit ist ungültig"
	#define ERR_SENDDELAY_RANGE     	"Die Sendeverzögerung ist ungültig"
    #define ERR_SENDLINE_RANGE      "Das Limit für das Senden mit der 1. Verzögerung ist ungültig"
	#define ERR_LOGOFF_RANGE		"Die Auslogzeit ist ungültig"
	#define ERR_THREAD_RANGE		"Das Threadlimit liegt ausserhalb des Bereiches"
	#define ERR_NICK			"Kann Nicknamen nicht setzen"
	#define ERR_MUTEX			"Synchronisation der Threads nicht möglich"
	#define ERR_NOT_ALLOW_CHAR		"Nicht erlaubte Zeichen wurden verwendet"
	
	
	// command acknowledgment
	#define MSG_HELLO			"Schön dich zu treffen"
	#define MSG_HELLO2			"Du hast einen neuen Account erhalten"
	#define MSG_PASSWD			"Password wurde geändert"
	#define MSG_IDENT_OK			"Du bist nun identifiziert"
	#define MSG_JOIN_OK			"Der Bot ist in den Channel gejoint"
	#define MSG_PART_OK			"Der Bot hat den Channel verlassen"
	#define MSG_RMCHANNEL_OK		"Der Channel wurde von der Liste entfernt"
	#define MSG_ADDCHANNEL_OK		"Der Channel wurde der Liste hinzugefügt"
	#define MSG_NICK_SET			"Der bot versucht den neuen Nickname zusetzen"
	#define MSG_LOGOFF			"Du bist ausgeloggt"
	#define MSG_SET_GREATING		"Die neue Begrüssung wurde gesetzt"
	#define MSG_RM_GREATING			"Die Begrüssung wurde entfernt"
	#define MSG_SET_TOPIC			"Die neue Topic wurde gesetzt"
	#define MSG_RM_TOPIC			"Die Topic wurde entfernt"
	#define MSG_KICK_OK			"Der Benutzer wurde vom Bot gekickt"
	#define MSG_USERMODE_OK			"Die Benutzermodus wurde geändert "
	#define MSG_RMUSER_OK			"Der Account wurde entfernt"
	#define MSG_MASTER			"Du bist ein Bot Master"
	#define MSG_DIE_OK			"Der Bot beendet sich"
	#define MSG_RESTART_OK			"Der Bot startet neu"
	
	// Informations for invalid input or using of the bot
	#define MSG_NOT_LOGON			"Du bist nicht identifiziert"
	#define MSG_NOT_MASTER			"Du bist kein Botmaster"
	#define MSG_NOT_OWNER			"Du bist nicht der Channeleigentümer"
	#define MSG_NOT_CHANNELOPT		"Dieses Kommando benötigt einen Channel"
	#define MSG_NOT_PASS			"Du hast ein leeres Passwort gesetzt"
	#define MSG_NOT_COMMAND			"Es wurde keine Hilfe für das Kommado gefunden"
	#define MSG_NOT_ACCOUNT			"Der Account wurde nicht gefunden"
	#define MSG_NOT_CHANNEL			"Dieser Channel ist nicht auf der Channelliste"
	#define MSG_NOT_ACCESS			"Zugriff verweigert"
	#define MSG_NOT_SELF			"Du kannst dich nicht selbst modifizieren"
	
	// Errors
	#define MSG_ADDCHANNEL_ALREADY		"Dieser Channel ist schon auf der Channelliste"
	#define MSG_NICK_INVALID		"Der Nickname ist ungültig"
	#define MSG_NICK_EXIST			"Es gibt schon ein Account mit diesem Nicknamen"
	#define MSG_ALREADY_LOGON		"Du bist schon identifiziert"
	#define MSG_NEED_OP			"Ich benötige Operatorrechte"
	#define MSG_UNKNOWN_MODS		"Dieser Modus ist ungültig"
	#define MSG_MASTER_PASS_ERR		"Passwörter nicht identisch"
	#define MSG_MASTER_ERR			"Kann den Account nicht als Master anlegen"
	#define MSG_USERLIST_EMPTY		"Die Benutzerliste ist leer"
	#define MSG_UNKNOW_MODE			"Unbekannter Parameter"
	
	// other informations
	#define MSG_HELP_END			"Ende der Hilfe."
	#define MSG_HELP_FOR			"Hilfe für"
	#define MSG_USERLIST_BEGIN		"Bentuzerliste:"
	#define MSG_USERLIST_END		"Ende der Benutzerliste."
	#define MSG_CHANNELLIST_BEGIN		"Channelliste:"
	#define MSG_CHANNELLIST_END		"Ende der Channelliste."
	#define MSG_CHANNELLIST_MODE    	"Channelmodus:"
	#define MSG_CHANNELLIST_TOPIC		"Topic:"
	#define MSG_CHANNELLIST_GREAT		"Begrüssung:"
	#define MSG_DEFAULT_REASON		"Du wurdest vom bot gekickt"
	#define MSG_INIT_CHANNEL		"Ich habe den Channel eingerichtet"
	
	// command  format erros
	#define MSG_ADDCHANNEL_ERR		"Ungültiges Kommando: !addchannel <#channel>"
	#define MSG_JOIN_ERR			"Ungültiges Kommando: !join <#channel>"
	#define MSG_IDENT_ERR			"Ungültiges Kommando: !ident <login password>"
	#define MSG_NICK_ERR			"Ungültiges Kommando: !nick <nickname>"			
	#define MSG_SAY_ERR			"Ungültiges Kommando: !say [#channel] <message>"			
	#define MSG_KICK_ERR			"Ungültiges Kommando: !kick [#channel] <nickname> [reason]"
	#define MSG_USERMODE_ERR		"Ungültiges Kommando: !usermode [#channel] <login> <+/-><mod>"
	#define MSG_CHANMODE_ERR		"Ungültiges Kommando: !chamode [#channel] <+/-><mods>"
	
	// Messages for the master dialog
	#define MSG_MASTER_TITLE		"Masteraccount anlegen\n\n"
	#define MSG_MASTER_LOGIN		"Loginname (max %d): "
	#define MSG_MASTER_PASS			"\nPasswort(max. %d): "
	#define MSG_MASTER_REPASS		"Passwort wiederholen: "
	
	
	
	static char *const unknow_parameter[]=
	{
		"Unbekannte Option\n",
		"Gib \"",PACKAGE," -h\" für mehr Information ein.\n",
		"."
	};
	
	
	
	static char *const help_msg[]=
	{
		"\n",
		PACKAGE," [Optionen]\n",
		"\n\tOptionen:\n",
		"\t-a Tage\t\tSetzt die maximale  Loginzeit.\n",
		"\t-b Name\t\tSetzt den Namen des Bots.\n",
		"\t-c Sek\t\tSetzt das Timeoutlimit.\n",
		"\t-d Pfad\t\tsetzt den Pfad zu Datenbank an\n",
		"\t-f Datei\ttSetzt Pfad zur Konfigurationdatei\n",
		"\t-h\t\tGibt diese Hilfe aus.\n",
		"\t-k Tage\t\tSetzt die maximal Lebenszeit eines Account bei.\n",
		"\t-m\t\tLegt neuen Master an.\n",
		"\t-n ms\t\tSetzt die Verzögerung  für das Senden.\n",
		"\t-p Port\t\tSetzt den Prot des IRC-Servers.\n"
		"\t-r String\tSetzt den String für Realnamefeld.\n",
		"\t-s Server\tSetzt den IRC Server.\n",
		"\t-t Anzahl\tSetzt die Anzaahl der zu erzeugenden Threads.\n"
		"\t-v\t\tGibt die Version aus.\n",
		"."
	};
#endif
