/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef EXTRACT_H
#define EXTRACT_H

char *getNickname(char *pLine);
char *getNetmask(char *pLine);
char *getArgument(char *pLine);
char *getParameters(char *pLine);
char *getCommand(char *pLine);
char *getChannel(char *pLine);
char *getAccessChannel(char *pLine);
char *getTopic(char *pChannelData);
char *getGreating(char *pChannelData);
char *getMode(char *pChannelData);

#endif
