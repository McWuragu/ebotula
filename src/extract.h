/* -------------------------------------------------------------
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
 -------------------------------------------------------------*/

#ifndef EXTRACT_H
#define EXTRACT_H

#include "type.h"

/**
* This function extract the nickname from the network identify or complete
* string from the irc server
*.
* @author Steffen Laube
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


char *getTopic(char const *pLine);
char *getGreeting(char const *pLine);
char *getChannelMode(char const *pLine);

/**
* This function splits a giving string  in seprate strings.
* @author Steffen Laube
*
* @param pString the input string
* @param nRetArraySize the number of  wished parts
* @return a array of char pointers with the giving size
*/
char **splitString(char const *pString,int nRetArraySize);
/**
 * This functions gets the Banmask from a Netmask
 * @author Uwe Strempel
 *
 * @param pLine the input string
 * @return the banmask
 */
char *getBanmask(char const *pLine);
#endif
