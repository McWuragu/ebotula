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
	KEY_SLOWSENDDELAY"=",
	KEY_FASTSENDLIMIT"=",
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
        syslog(LOG_ERR,getSyslogString(SYSLOG_CONFIG_FILE));
        exit(errno);
	}
		/* writingd data to file */
		/* Version string */
		sprintf(tmpstr,VERSIONSTR);
		strcat(tmpstr,"\n");
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"# %s\n",tmpstr);
		/* borname */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%s\n",bstr[i++],sSetup.botname);
		/* realname */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s\"%s\"\n",bstr[i++],sSetup.realname);
		/* user & group */
        fprintf(fd,"%s\n",bstr[i++]);
        fprintf(fd,"%s%s\n",bstr[i++],sSetup.sExeUser);
        fprintf(fd,"%s%s\n",bstr[i++],sSetup.sExeGroup);
        /* Thread limit */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%d\n",bstr[i++],sSetup.thread_limit);
		/* Servername */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%s\n",bstr[i++],sSetup.server);
		/* Serverport */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%s\n",bstr[i++],sSetup.port);
		/* databasepath */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%s\n",bstr[i++],sSetup.pDatabasePath);
		/* delays */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%d\n",bstr[i++],sSetup.iSendDelay);
		fprintf(fd,"%s%d\n",bstr[i++],sSetup.nSlowSendDelay);
		fprintf(fd,"%s%d\n",bstr[i++],sSetup.nFastSendingCharLimit);
		/* Accountlivtime */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%d\n",bstr[i++],sSetup.AccountLiveTime);
		/* Autologoff */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%d\n",bstr[i++],sSetup.AutoLoggoff);
		/* Pingtimeout */
		fprintf(fd,"%s\n",bstr[i++]);
		fprintf(fd,"%s%d\n",bstr[i++],sSetup.iTimeout);

		/* closing file */
		fclose(fd);
}

