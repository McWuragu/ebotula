/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "baseconfig.h"
#include "ircbot.h"

char	* bstr[26]={"# Config file for the ebotula", 	
/*2*/		"# copyright 2003 by Steffen Laube",	
/*3*/		"# The name of the bot",		
/*4*/		"botname=",
/*5*/		"# String for the realname",
/*6*/		"realname=",
/*7*/		"# Number of the created threads",
/*8*/		"threadlimit= ",
/*9*/		"# The server the bot connects to",
/*10*/		"servername=",
/*11*/		"# The port the bot connects to",
/*12*/		"port =",
/*13*/		"# The location of the database for the bot",
/*14*/		"#databasepath=/usr/local/var/ebotula",
/*15*/		"# The delay time in millisecond for sending",
/*16*/		"# It is used for the prevention of excess flooding",
/*17*/		"firstsenddelay=",
/*18*/		"secsenddelay=",
/*19*/		"sendlinelimit=",
/*20*/		"# The time in days an account exists.",
/*21*/		"# Unused accounts will be removed after this time.",
/*22*/		"accountlivetime=",
/*23*/		"# The time limit in days for maximum login time",
/*24*/		"autologoff=",
/*25*/		"# The time limit in seconds for ping timeout.",
/*26*/		"pingtimeout="};

/**
  * gernerates baseconfig string for baseconfigfile and writes it 
  * to given filename
  * 
  * @parm    filename with absolut path
  * @return  pointer to baseconfigfile
 */

int write_baseconfig(char *filename)
{
	/* filediskriptor for output file*/
	FILE * fd;
	char tempstr[1024];
	int tempint;
	/* Create & Openfile*/
	if ((fd=fopen(filename,"wb"))==NULL)
	{
		return -1;
	}
		/* writingd data to file */
		/* Version string */
		sprintf(tempstr,VERSIONSTR);
		fprintf(fd,"%s\n",bstr[0]);
		fprintf(fd,"%s\n",bstr[1]);
		fprintf(fd,"# %s\n",tempstr);
		/* borname */
		sprintf(tempstr,DEFAULT_BOTNAME);
		fprintf(fd,"%s\n",bstr[2]);
		fprintf(fd,"%s%s\n",bstr[3],tempstr);
		/* realname */
		sprintf(tempstr,DEFAULT_REALNAME);
		fprintf(fd,"%s\n",bstr[4]);
		fprintf(fd,"%s\"%s\"\n",bstr[5],tempstr);
		/* Thread limit */
		tempint=DEFAULT_THREAD_LIMIT;
		fprintf(fd,"%s\n",bstr[6]);
		fprintf(fd,"%s%d\n",bstr[7],tempint);
		/* Servername */
		fprintf(fd,"%s\n",bstr[8]);
		fprintf(fd,"%sirc.fh-lausitz.de\n",bstr[9]);
		/* Serverport */
		fprintf(fd,"%s\n",bstr[10]);
		fprintf(fd,"%s 6667\n",bstr[11]);
		/* databasepath */
		fprintf(fd,"%s\n",bstr[12]);
		fprintf(fd,"%s\n",bstr[13]);
		/* delays */
		fprintf(fd,"%s\n",bstr[14]);
		fprintf(fd,"%s\n",bstr[15]);
		tempint=DEFAULT_SEND_DELAY;
		fprintf(fd,"%s%d\n",bstr[16],tempint);
		tempint=DEFAULT_SEND_SAFE_DELAY;
		fprintf(fd,"%s%d\n",bstr[17],tempint);
		tempint=DEFAULT_SEND_SAFE_LINE;
		fprintf(fd,"%s%d\n",bstr[18],tempint);
		/* Accountlivtime */
		fprintf(fd,"%s\n",bstr[19]);
		fprintf(fd,"%s\n",bstr[20]);
		fprintf(fd,"%s 90\n",bstr[21]);
		/* Autologoff */
		fprintf(fd,"%s\n",bstr[22]);
		fprintf(fd,"%s 3\n",bstr[23]);
		/* Pingtimeout */
		fprintf(fd,"%s\n",bstr[24]);
		fprintf(fd,"%s 120\n",bstr[25]);

		/* closing file */
		fclose(fd);
	return 0;
}

