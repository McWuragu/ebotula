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
           		"Das ist  das Onlinehilfesystem f�r den Bot.",
				"Mit !help [Kommando] erh�lt man mehr Hilfe zu einem Kommando",
				"------------------------------------------------------------------------"
			},
			// commands
			
			// others
			// VIEWGREAT
			{
				"gibt die Begr�ssung eines Channels aus",
				"Mit diesem Kommando kann man nachtr�glich sich Begr�ssung",
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
				"Nicknamen als  Login. Dieses ist f�r das Einloggen von",
				"Bedeutung. Auch solle man gleich dach nach mit !pass sein",
				"Passwort �ndern.",
			},
			// IDENT
			{
				"indifiziert den User gegen�ber dem Bot",
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
				"�ndert das Passwort des aktuellen Accounts",
				"duDas Kommando erm�glichdas setzen unde �nder des Passwortes",
				"des aktuell identifizieren Accounts.",
			},
			
			// owner commands
			// CHANMODE
			{
				"setzt oder entfernt Modes vom Channel",
				"Hier mit ist es m�glich Rechte und Modien des aktuellen",
				"oder einem angebenen Channnel zu setzen oder zu l�schen",
			},
			// USERLIST
			{
				"gibt die Liste aller Accounts oder f�r einen Channel",
				"Durch diese Kommando kann man sich eine Liste aller Accounts",
				"anzeigen lassen. Diese Liste gibt Information �ber die Rechte",
				"und dem Status der Accounts",
			},
			// USERMODE
			{
				"setzt und entfernt die Rechte eines Accounts",
				"Hiermit ist es m�glich die Rechte eines Account in einem Channel",
				"zusetzen und zul�schen",
			},
			// SAY
			{
				"gibt eine Nachricht in einem Channel aus",
				"Damit is es m�glich dem Bot anzuweisen einen  Nachricht in den",
				"Channel zuposten",
			},
			// KICK
			{
				"wirft einen User aus einen Channel hinaus",
				"Das Kommando erm�glicht es einen  User aus einem Channel zuwerfen",
			},
			// SET_TOPIC
			{
				"setzt und l�scht die Topic eines Channels",
				"Durch dies Kommando kann eine Topic f�r einen Channel gesetzt",
				"oder gel�scht werden",
			},
			// SET_GREATING
			{
				"setzt und l�scht die Begr�ssung f�r einen Channel",
				"Dieses Kommando bietet die M�glichkeit eine Begr�ssung f�r einen",
				"Channel zu setzen oder diese auch zul�schen",
			},
			// CHANNELS
			{
				"gibt eine Liste von Channels",
				"Damit kann man sich einer Liste der Channels mit den gesetzten",
			    "Rechten, Topics und Begr�ssung augaben lassen",
			},
			
			// Master commands
			// RESTART
			{
				"startet den Bot neu",
				"Durch aufrufen diese Kommandos startet der Bot neu und",
				"liest  die Konfigurationsdaten neu ein",
				"WARRNING: Das funktioniert nicht  zuverl�ssig",
			},
			// ALLSAY
			{
				"sendet eine Nachricht in alle Channel des Bots",
				"Dieses Kommando erm�glicht einen Broadcast in alle Channels",
				"in in der Liste des Bots enthalten sind.",
			},
			// RMUSER
			{
				"l�scht accounts",
				"Hier dr�ber kann man manuell Accounts und ihre Rechte l�schen.",
			},
			// NICK
			{
				"�ndert den Nickname des Bots",
				"Dieses Kommando �ndert den Nickname des Bots. Dieser ist aber nur",
				"f�r die Laufzeit g�ltig. Bei einem Neustart wird wieder der",
			    "vorher Konfigurierte gesetzt",
				
			},
			// DIE
			{
				"stopt den bot",
				"Damit l�st sich der Bot beenden und sauber remote herunterfahren.",
			},
			// RMCHANNEL
			{
				"l�scht ein Channel von der Channeliste",
				"Mit diesem Kommando kann man einen Channel aus der Liste des Bot",
				"entfernen und der Bot verl�sst diesen Channel auch.",
			},
			// ADDCHANNEL
			{
				"f�gt ein Channel in die Channelliste ein",
				"Durch dieses Kommando kann man einen neuen Channel zu Liste des",
				"Bots hinzuf�gen und er joint diesen auch selbstst�ndig.",
			},
			// JOIN
			{
				"joint einem Channel",
				"Damit kann man den Bot einem Channel join ohne das er ihne in die",
				"Liste mit einf�gt."
			},
			// PART
			{
				"verl�st einen Channel",
				"Hier mit kann man einen Channel verlassen ohne ihn aus der Liste",
				"zul�schen.",
			}
		};
#endif
