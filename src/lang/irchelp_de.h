/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef IRCHELP_GER_H
#define IRCHELP_GER_H

static char *pIrcHelp[][15] = {
			// header
			
			{
           		"Das ist  das Onlinehilfesystem für den Bot.",
				"Mit !help [Kommando] erhält man mehr Hilfe zu einem Kommando",
				"------------------------------------------------------------------------"
			},
			// commands
			
			// others
			// VIEWGREAT
			{
				"gibt die Begrüssung eines Channels aus",
				"Mit diesem Kommando kann man nachträglich sich Begrüssung",
				"des aktuellen oder angegeben Channels ausgeben lassen.",
			},
			// HELP
			{
				"ruft die Onlinehilfe des Bots auf",
				"Das Kommando gibt generale Informationen zum Bot und Hinweis",
				"zur Benutzung  einzelner Kommandos",
			},
			// VERSION
			{
				"gibt die Version aus",
				"Damit kann man sich die  Version und Kompilierdatum des Bots",
				"anezeigen lassen",
			},
			// HELLO
			{	  
				"legt neuen Account an",
				"Damit wir ein neuer Account beim Bot mit dem aktuellen",
				"Nicknamen als  Login. Dieses ist für das Einloggen von",
				"Bedeutung. Auch solle man gleich dach nach mit !pass sein",
				"Passwort ändern.",
			},
			// IDENT
			{
				"indifiziert den User gegenüber dem Bot",
				"Durch aufrufen dieses Kommando logt man sich beim Bot ein",
				"und kann dann weitere Funktionen nutzen. Des Weiteren werden",
				"die Rechte des Account in allen Channel vom bot gesetzt",
			},
		
			// Logged user
			// LOGOFF
			{
				"auloggen beim Bot",
				"Hier mit kann man sich manuell beim Bot ausloggen",
			},
			// PASS
			{
				"ändert das Passwort des aktuellen Accounts",
				"duDas Kommando ermöglichdas setzen unde änder des Passwortes",
				"des aktuell identifizieren Accounts.",
			},
			
			// owner commands
			// CHANMODE
			{
				"setzt oder entfernt Modes vom Channel",
				"Hier mit ist es möglich Rechte und Modien des aktuellen",
				"oder einem angebenen Channnel zu setzen oder zu löschen",
			},
			// USERLIST
			{
				"gibt die Liste aller Accounts oder für einen Channel",
				"Durch diese Kommando kann man sich eine Liste aller Accounts",
				"anzeigen lassen. Diese Liste gibt Information über die Rechte",
				"und dem Status der Accounts",
			},
			// USERMODE
			{
				"setzt und entfernt die Rechte eines Accounts",
				"Hiermit ist es möglich die Rechte eines Account in einem Channel",
				"zusetzen und zulöschen",
			},
			// SAY
			{
				"gibt eine Nachricht in einem Channel aus",
				"Damit is es möglich dem Bot anzuweisen einen  Nachricht in den",
				"Channel zuposten",
			},
			// KICK
			{
				"wirft einen User aus einen Channel hinaus",
				"Das Kommando ermöglicht es einen  User aus einem Channel zuwerfen",
			},
			// SET_TOPIC
			{
				"setzt und löscht die Topic eines Channels",
				"Durch dies Kommando kann eine Topic für einen Channel gesetzt",
				"oder gelöscht werden",
			},
			// SET_GREATING
			{
				"setzt und löscht die Begrüssung für einen Channel",
				"Dieses Kommando bietet die Möglichkeit eine Begrüssung für einen",
				"Channel zu setzen oder diese auch zulöschen",
			},
			// CHANNELS
			{
				"gibt eine Liste von Channels",
				"Damit kann man sich einer Liste der Channels mit den gesetzten",
			    "Rechten, Topics und Begrüssung augaben lassen",
			},
			
			// Master commands
			// RESTART
			{
				"startet den Bot neu",
				"Durch aufrufen diese Kommandos startet der Bot neu und",
				"liest  die Konfigurationsdaten neu ein",
				"WARRNING: Das funktioniert nicht  zuverlässig",
			},
			// ALLSAY
			{
				"sendet eine Nachricht in alle Channel des Bots",
				"Dieses Kommando ermöglicht einen Broadcast in alle Channels",
				"in in der Liste des Bots enthalten sind.",
			},
			// RMUSER
			{
				"löscht accounts",
				"Hier drüber kann man manuell Accounts und ihre Rechte löschen.",
			},
			// NICK
			{
				"ändert den Nickname des Bots",
				"Dieses Kommando ändert den Nickname des Bots. Dieser ist aber nur",
				"für die Laufzeit gültig. Bei einem Neustart wird wieder der",
			    "vorher Konfigurierte gesetzt",
				
			},
			// DIE
			{
				"stopt den bot",
				"Damit läst sich der Bot beenden und sauber remote herunterfahren.",
			},
			// RMCHANNEL
			{
				"löscht ein Channel von der Channeliste",
				"Mit diesem Kommando kann man einen Channel aus der Liste des Bot",
				"entfernen und der Bot verlässt diesen Channel auch.",
			},
			// ADDCHANNEL
			{
				"fügt ein Channel in die Channelliste ein",
				"Durch dieses Kommando kann man einen neuen Channel zu Liste des",
				"Bots hinzufügen und er joint diesen auch selbstständig.",
			},
			// JOIN
			{
				"joint einem Channel",
				"Damit kann man den Bot einem Channel join ohne das er ihne in die",
				"Liste mit einfügt."
			},
			// PART
			{
				"verläst einen Channel",
				"Hier mit kann man einen Channel verlassen ohne ihn aus der Liste",
				"zulöschen.",
			}
		};
#endif
