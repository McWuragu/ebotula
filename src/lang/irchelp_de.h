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
        "Das ist  das Onlinehilfesystem für den Bot.",
        "Mit !help [Kommando] erhält man mehr Hilfe zu einem Kommando",
        "------------------------------------------------------------------------"
    },
    // commands

    // others
    // VIEWGREET
    {
        "gibt die Begrüssung eines Channels aus",
        "Mit diesem Kommando kann man sich nachträglich die Begrüssung",
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
        "Nicknamen als  Login erzeugt. Dieses ist für das Einloggen von",
        "Bedeutung. Auch sollte man gleich danach mit !pass sein",
        "Passwort ändern.",
    },
    // IDENT
    {
        "identifiziert den User gegenüber dem Bot",
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
        "ändert das Passwort des aktuellen Accounts",
        "Das Kommando ermöglicht das setzen und ändern des Passwortes",
        "des aktuell identifizieren Accounts.",
    },

    /* friend commands */
    // INVITE
    {
        "lädt den user in einen Channel ein",
        "Das Kommando ermöglich es sich selbst in einen Channel, der Invite"
        "gesetzt ist, einzuladen", 
    },

    // owner commands
    // CHANMODE
    {
        "setzt oder entfernt Modes vom Channel",
        "Hiermit ist es möglich Rechte und Modi des aktuellen",
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
        "zu setzen und zu löschen",
    },
    // SAY
    {
        "gibt eine Nachricht in einem Channel aus",
        "Damit is es möglich, den Bot anzuweisen eine Nachricht in den",
        "Channel zu posten",
    },
    // KICK
    {
        "wirft einen User aus einen Channel hinaus",
        "Das Kommando ermöglicht es, einen  User aus einem Channel zu werfen",
    },
    // SET_TOPIC
    {
        "setzt und löscht die Topic eines Channels",
        "Durch dies Kommando kann eine Topic für einen Channel gesetzt",
        "oder gelöscht werden",
    },
    // SET_GREETING
    {
        "setzt und löscht die Begrüssung für einen Channel",
        "Dieses Kommando bietet die Möglichkeit eine Begrüssung für einen",
        "Channel zu setzen oder diese auch zulöschen",
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
        "WARRNUNG: Das funktioniert nicht zuverlässig",
    },
    // ALLSAY
    {
        "sendet eine Nachricht in alle Channel des Bots",
        "Dieses Kommando ermöglicht einen Broadcast in alle Channels",
        "die in der Liste des Bots enthalten sind.",
    },
    // RMUSER
    {
        "löscht accounts",
        "Hierrüber kann man manuell Accounts und ihre Rechte löschen.",
    },
    // NICK
    {
        "ändert den Nickname des Bots",
        "Dieses Kommando ändert den Nickname des Bots. Dieser ist aber nur",
        "für die Laufzeit gültig. Bei einem Neustart wird wieder der",
        "vorher konfigurierte gesetzt",
        
    },
    // DIE
    {
        "stopt den bot",
        "Damit läst sich der Bot beenden und sauber remote herunterfahren.",
    },
    // CHANNELS
    {
        "gibt eine Liste von Channels",
        "Damit kann man sich einer Liste der Channels mit den gesetzten",
        "Rechten, Topics und Begrüssungen ausgeben lassen",
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
        "Damit kann man den Bot einen Channel joinen lassen, ohne das er ihn in die",
        "Liste einfügt."
    },
    // PART
    {
        "verlässt einen Channel",
        "Hiermit kann man einen Channel verlassen ohne ihn aus der Liste",
        "zu löschen.",
    }
};
#endif
