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

char *getNickname(char *line);
char *getNetmask(char *line);
char *getArgument(char *line);
char *getParameters(char *line);
char *getCommand(char *line);
char *getChannel(char *line);
char *getAccessChannel(char *line);
char *getTopic(char *channelstr);
char *getGreating(char *channelstr);
char *getMode(char *channelstr);

#endif
