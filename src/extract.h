/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
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

char *getTopic(char const *pLine);
char *getGreeting(char const *pLine);
char *getChannelMode(char const *pLine);
/**
 * This functions gets the Banmask from a Netmask
 *
 * @param pLine the input string
 * @return the banmask
 */
char *getBanmask(char const *pLine);

char *getFirstPart(char const *pLine,char **pRest);

#endif
