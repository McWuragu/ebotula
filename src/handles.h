/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 * ############################################################# 
 */

#ifndef HANDLES_H
#define HANDLES_H

/**
 * This  fiunction is a  helper for the initialization of a channel
 *
 * @param pChannel the name  of the channel
 */
static void channelInit(char *pChannel);

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
void hResetModes(char *pLine);


/**
 * This is the handle for rest the topic. he is call by chang the topic
 * by other user as the bot self.
 *
 * @param pline the  pointer of the  receiving line  from the server
 */
void hResetTopic(char *pLine);

/**
 * This is the handle for  the rejoining after  the  kick out the channel
 * @param pLine a pointer  of the receiving line from the smerver
 */
void hRejoinAfterKick(char *pLine);

/**
 * This is the handle for  initialize of a channel after get
 * operator rights. He read the information about this channel out the
 * database and set the  topic and the mode oft the channel.
 *
 * @param pLine a pointer  of the receiving line from the smerver
 */
void hInitAfterOp(char *pLine);

/**
 * This handle is for the answer  of the  command /who. He look in the
 * list of  callback function and search the right  item. Then excute the
 * function form this item.
 */
void hCallback(char *pLine);


void hWhoisFailed(char *pLine);

#endif
