/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

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
    // VIEWGREET
    {
        "gibt die Begr�ssung eines Channels aus",
        "Mit diesem Kommando kann man sich nachtr�glich die Begr�ssung",
        "des aktuellen oder angegeben Channels ausgeben lassen.",
    },
    // HELP
    {
        "ruft die Onlinehilfe des Bots auf",
        "Das Kommando gibt generelle Informationen zum Bot und Hinweise",
        "zur Benutzung einzelner Kommandos.",
    },
    // VERSION
    {
        "gibt die Version aus",
        "Damit kann man sich die Version und das Kompilierdatum des Bots",
        "anezeigen lassen",
    },
    // HELLO
    {
        "legt neuen Account an",
        "Damit wird ein neuer Account beim Bot mit dem aktuellen",
        "Nicknamen als  Login erzeugt. Dieses ist f�r das Einloggen von",
        "Bedeutung. Auch sollte man gleich danach mit !pass sein",
        "Passwort �ndern.",
    },
    // IDENT
    {
        "identifiziert den User gegen�ber dem Bot",
        "Durch aufrufen dieses Kommandos loggt man sich beim Bot ein",
        "und kann dann weitere Funktionen nutzen. Desweiteren werden",
        "die Rechte des Account in allen Channel vom bot gesetzt",
    },

    // Logged user
    // LOGOFF
    {
        "ausloggen beim Bot",
        "Hiermit kann man sich manuell beim Bot ausloggen",
    },
    // PASS
    {
        "�ndert das Passwort des aktuellen Accounts",
        "Das Kommando erm�glicht das setzen und �ndern des Passwortes",
        "des aktuell identifizieren Accounts.",
    },

    /* friend commands */
    // INVITE
    {
        "l�dt den user in einen Channel ein",
        "Das Kommando erm�glich es sich selbst in einen Channel, der Invite"
        "gesetzt ist, einzuladen", 
    },

    // owner commands
    // CHANMODE
    {
        "setzt oder entfernt Modes vom Channel",
        "Hiermit ist es m�glich Rechte und Modi des aktuellen",
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
        "zu setzen und zu l�schen",
    },
    // SAY
    {
        "gibt eine Nachricht in einem Channel aus",
        "Damit is es m�glich, den Bot anzuweisen eine Nachricht in den",
        "Channel zu posten",
    },
    // KICK
    {
        "wirft einen User aus einen Channel hinaus",
        "Das Kommando erm�glicht es, einen  User aus einem Channel zu werfen",
    },
    // SET_TOPIC
    {
        "setzt und l�scht die Topic eines Channels",
        "Durch dies Kommando kann eine Topic f�r einen Channel gesetzt",
        "oder gel�scht werden",
    },
    // SET_GREETING
    {
        "setzt und l�scht die Begr�ssung f�r einen Channel",
        "Dieses Kommando bietet die M�glichkeit eine Begr�ssung f�r einen",
        "Channel zu setzen oder diese auch zul�schen",
    },
    // BAN
    {
        "bannt einen User aus dem  Channel",
    },
    // DEBAN
    {
        "hebt einen Ban wieder auf",
    },

    // Master commands
    // RESTART
    {
        "startet den Bot neu",
        "Durch aufrufen diese Kommandos startet der Bot neu und",
        "liest die Konfigurationsdaten neu ein",
        "WARRNUNG: Das funktioniert nicht zuverl�ssig",
    },
    // ALLSAY
    {
        "sendet eine Nachricht in alle Channel des Bots",
        "Dieses Kommando erm�glicht einen Broadcast in alle Channels",
        "die in der Liste des Bots enthalten sind.",
    },
    // RMUSER
    {
        "l�scht accounts",
        "Hierr�ber kann man manuell Accounts und ihre Rechte l�schen.",
    },
    // NICK
    {
        "�ndert den Nickname des Bots",
        "Dieses Kommando �ndert den Nickname des Bots. Dieser ist aber nur",
        "f�r die Laufzeit g�ltig. Bei einem Neustart wird wieder der",
        "vorher konfigurierte gesetzt",
        
    },
    // DIE
    {
        "stopt den bot",
        "Damit l�st sich der Bot beenden und sauber remote herunterfahren.",
    },
    // CHANNELS
    {
        "gibt eine Liste von Channels",
        "Damit kann man sich einer Liste der Channels mit den gesetzten",
        "Rechten, Topics und Begr�ssungen ausgeben lassen",
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
        "Damit kann man den Bot einen Channel joinen lassen, ohne das er ihn in die",
        "Liste einf�gt."
    },
    // PART
    {
        "verl�sst einen Channel",
        "Hiermit kann man einen Channel verlassen ohne ihn aus der Liste",
        "zu l�schen.",
    }
};
#endif
