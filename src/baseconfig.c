/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
         
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif


#include "messages.h"
#include "ircbot.h"
#include "type.h"
#include "baseconfig.h"

char	* bstr[]={
    "# Config file for the ebotula", 	
    "# The name of the bot",		
    KEY_BOTNAME,"=",
    "\n# String for the realname",
    KEY_REALNAME,"=",
    "\n# execute the bot as this user and this group", 
    KEY_EXEUSER,"=",
    KEY_EXEGROUP,"=",
    "\n# Number of the created threads",
	KEY_THREADLIMIT,"= ",
	"\n# The server the bot connects to",
	KEY_SERVER,"=",
	"\n# The port the bot connects to",
	KEY_PORT,"=",
	"\n# The location of the database for the bot",
	KEY_DATABASEPATH,"=",
	"\n# The delay time in millisecond for sending",
	"# It is used for the prevention of excess flooding",
	KEY_FASTSENDDELAY,"=",
	KEY_SLOWSENDDELAY,"=",
	KEY_FASTSENDLIMIT,"=",
	"\n# The time in days an account exists.",
	"# Unused accounts will be removed after this time.",
	KEY_ALT,"=",
    "\n# The time limit in days for maximum login time",
	KEY_AUTOLOGOFF,"=",
	"\n# The time limit in seconds for ping timeout.",
	KEY_PINGTIMEOUT,"="
    };

/**
  * gernerates baseconfig string for baseconfigfile and writes it 
  * to given filename
  * 
  * @parm    filename with absolut path
  * @return  pointer to baseconfigfile
 */

void write_baseconfig()
{
	/* filediskriptor for output file*/
	FILE * fd;
    char tmpstr[128];
    int i=0;
	extern  ConfigSetup_t sSetup;

    /* Create & Openfile*/
	if ((fd=fopen(sSetup.configfile,"wb"))==NULL)
	{
        	perror(getSyslogString(SYSLOG_CONFIG_FILE));
		/* syslog(LOG_ERR,getSyslogString(SYSLOG_CONFIG_FILE));**/
		logger(LOG_ERR,getSyslogString(SYSLOG_CONFIG_FILE)); 
		exit(errno);
	}
		/* writingd data to file */
		/* Version string */
		sprintf(tmpstr,VERSIONSTR);
		strcat(tmpstr,"\n");
		/*    "# Config file for the ebotula", 	*/
		i=0;
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		fprintf(fd,"# %s\n",tmpstr);
		/* Botname */
		/*  "# The name of the bot",*/
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/*    KEY_BOTNAME,"=",*/
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%s\n",bstr[i],sSetup.pBotname);
		i++;
		/* realname */
		/*    "\n# String for the realname",*/
 		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/*   KEY_REALNAME,"=",*/
 		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s\"%s\"\n",bstr[i],sSetup.realname);
		i++;
		/* user & group */
	        /* "\n# execute the bot as this user and this group", */
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* KEY_EXEUSER,"=",*/
        	fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%s\n",bstr[i],sSetup.sExeUser);
		i++;
	        /* KEY_EXEGROUP,"=", */
        	fprintf(fd,"%s",bstr[i]);
		i++;
	        fprintf(fd,"%s%s\n",bstr[i],sSetup.sExeGroup);
		i++;
	        /* Thread limit */
		/*    "\n# Number of the created threads",*/
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/*KEY_THREADLIMIT,"= ",*/
        	fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%d\n",bstr[i],sSetup.thread_limit);
		i++;
		/* Servername */
		/*"\n# The server the bot connects to",*/
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* KEY_SERVER,"=", */
        	fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%s\n",bstr[i],sSetup.server);
		i++;
		/* Serverport */
		/* "\n# The port the bot connects to", */
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/*KEY_PORT,"=",*/
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%s\n",bstr[i],sSetup.port);
		i++;
		/* databasepath */
		/* "\n# The location of the database for the bot", */
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* KEY_DATABASEPATH,"=", */
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%s\n",bstr[i],sSetup.pDatabasePath);
		i++;
		/* delays */
		/* "\n# The delay time in millisecond for sending",  */
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* "# It is used for the prevention of excess flooding", */
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* KEY_FASTSENDDELAY,"=",*/
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%d\n",bstr[i],sSetup.iSendDelay);
		i++;
		/* KEY_SLOWSENDDELAY"=", */
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%d\n",bstr[i],sSetup.nSlowSendDelay);
		i++;
		/* KEY_FASTSENDLIMIT"=", */
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%d\n",bstr[i],sSetup.nFastSendingCharLimit);
		i++;
		/* Accountlivtime */
		/* "\n# The time in days an account exists.", */
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* "# Unused accounts will be removed after this time.",*/
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* KEY_ALT,"=",*/
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%d\n",bstr[i],sSetup.AccountLiveTime);
		i++;
		/* Autologoff */
	    	/* "\n# The time limit in days for maximum login time",*/
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		/* KEY_AUTOLOGOFF,"=",*/
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%d\n",bstr[i],sSetup.AutoLoggoff);
		i++;
		/* Pingtimeout */
		/*"\n# The time limit in seconds for ping timeout.",*/
		fprintf(fd,"%s\n",bstr[i]);
		i++;
		fflush(fd);
		/* KEY_PINGTIMEOUT,"="*/
		fprintf(fd,"%s",bstr[i]);
		i++;
		fprintf(fd,"%s%d\n",bstr[i],sSetup.iTimeout);
		i++;

		/* closing file */
		fclose(fd);
}

