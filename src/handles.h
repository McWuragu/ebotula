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
*	@param pLine is the complete line which was received from the  irc server
*/
void hNickChange(char *pLine);
/** 
*	This is a handle function for JOIN.
*   This check whether the bot have  operater access rigths. If  the bot havn't operator rights
*   then send this function a message  to the channel.
*   @param pLine is the complete line which was received from the irc server
*/
void hBotNeedOp(char *pLine);
/**
*	This is a handle function for JOIN.
*   This set the mod for the joined user. This function can call manuel to set or reset the
*   mod after modify of this by other user in the channel or by the bot in the database.
*   @param pLine is the complete line which was received from the irc server
*/
void hSetModUser(char *pLine);
 /**
 * This is a handle function for MODE
 * This reset the user mod if this  change not from the bot self
 */
void hResetModUser(char *pLine);
#endif
