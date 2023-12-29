/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2023 Steffen Laube <Laube.Steffen@gmx.de>
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

#ifndef EXTRACT_H
#define EXTRACT_H

#include "type.h"


#define rmFirstPart(STRING,RETURN) {char *pTmp=getFirstPart(STRING,RETURN); free(pTmp);}

/**
* This function extract the nickname from the network identify or complete
* string from the irc server
*.
* @param pLine the input string
* @return the nickname
*/			 
char *getNickname(char const *pLine);

char *getNetmask(char const *pLine);
char *getArgument(char const *pLine);
char *getParameters(char const *pLine);
char *getCommand(char const *pLine);
char *getChannel(char const *pLine);
char *getAccessChannel(char const *pLine);
AnswerMode_t getAnswerMode(char const *pLine);

char *getTopic(char const *pChannelSet);
char *getGreeting(char const *pChannelSet);
char *getChannelMode(char const *pChannelSet);
/**
 * This functions gets the Banmask from a Netmask
 *
 * @param pLine the input string
 * @return the banmask
 */
char *getBanmask(char const *pLine);

char *getFirstPart(char const *pLine,char **pRest);

#endif
