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

#include <assert.h>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include"utilities.h"

#include "messages.h"

void printCmdLineHelp() {

		printf(_("\n%s [Options]\n"),PACKAGE);
		printf(_("\n\tOptions:\n"));
		printf(_("\t-a days\t\tSet the time in days for the automatic logoff.\n"));
		printf(_("\t-b name\t\tSet the name of the bot.\n"));
		printf(_("\t-c sec\t\tSet the ping timeout of the network connection.\n"));
		printf(_("\t-d path\t\tSet the path of the database direcotry\n"));
        printf(_("\t-L Level\tSet the logging level (0-3)\n"));
		printf(_("\t-e ms\t\tSet the delay time in millisecond for the slow sending.\n"));
       	printf(_("\t-f file\t\tSet the config file.\n"));
	    printf(_("\t-g group\tSet the group under wich the bot run (only root).\n"));
        printf(_("\t-h\t\tPrint this help information.\n"));
		printf(_("\t-i sec\t\tSet the delay of the channel initialization.\n"));
        printf(_("\t-k days\t\tSet the account live time in days.\n"));
        printf(_("\t-l char/min\tSet the charakter limit for the first sending delay level.\n"));
		printf(_("\t-m\t\tStart the dialog for creating of a new master.\n"));
		printf(_("\t-n ms\t\tSet the delay time in millisecond for the fast sending.\n"));
		printf(_("\t-p port\t\tSet the portnumber of the irc server.\n"));
		printf(_("\t-r string\tSet the output for a request of the real name.\n"));
		printf(_("\t-s server\tSet the irc server name.\n"));
		printf(_("\t-t count\tSet the number of created  threads.\n"));
		printf(_("\t-u user\t\tSet the user under which the bot run (root only).\n"));
        printf(_("\t-v\t\tPrint version.\n"));
}
