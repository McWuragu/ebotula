/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef HANDLES_H
#define HANDLES_H

/**	
*	This is  the handle  for setting  a new nickname by the user.
*   The function change the netmask inclusive  the nickname in the login databases.
*   This is necessary for holding the login consistent
*	@param line is the complete line which was received from the  irc server
*/
void hNickChange(char *line);
/** 
*	This is a handle function for JOIN.
*   This check whether the bot have  operater access rigths. If  the bot havn't operator rights
*   then send this function a message  to the channel.
*   @param line is the complete line which was received from the irc server
*/
void hBotNeedOp(char *line);
/**
*	This is a handle function for JOIN.
*   This set the mods for the joined user. This function can call manuel to set or reset the
*   mods after modify of this by other user in the channel or by the bot in the database.
*   @param line is the complete line which was received from the irc server
*/
void hSetMods(char *line);

#endif
