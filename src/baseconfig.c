/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003-2004 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
         
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif


#include "utilities.h"
#include "messages.h"
#include "ircbot.h"
#include "type.h"
#include "baseconfig.h"
extern ConfigSetup_t sSetup;
ValueStruct_t vsConfig[VAL_COUNT]={
	{ {"#",
       "# The name of the bot",
       "#",
	    NULL},
	    KEY_BOTNAME,PARAM_STRING,MOD_NONE,(void*)&sSetup.pBotname},
	{ {"#",
       "# String for the realname",
       "#",
           NULL},
	   KEY_REALNAME,PARAM_STRING,MOD_QUOTED,(void*)&sSetup.realname},
	{ {"#",
       "# Execute the bot as this user and this group",
       "#",
	    NULL},
    	KEY_EXEUSER,PARAM_STRING,MOD_NONE,(void*)&sSetup.sExeUser},
	{ {NULL},
        KEY_EXEGROUP,PARAM_STRING,MOD_NONE,(void*)&sSetup.sExeGroup},
	{ {"#",
       "# Number of the created threads",
       "#",
        NULL},
        KEY_THREADLIMIT,PARAM_INT,MOD_NONE,(void*)&sSetup.thread_limit},
	{ {"#",
       "# The hostname and the portnumber of the irc server for the connecting of the bot",
       "#",
        NULL},
        KEY_SERVER,PARAM_STRING,MOD_NONE,(void*)&sSetup.sHostname},
	{ {NULL},
        KEY_PORT,PARAM_STRING,MOD_NONE,(void*)&sSetup.sPort},
	{ {"#",
       "# The location of the database for the bot",
       "#",
        NULL},
        KEY_DATABASEPATH,PARAM_STRING,MOD_NONE,(void*)&sSetup.pDatabasePath},
	{ {"#",
       "# The delay time in millisecond for seding",
       "# It is used for prevention of excess flooding",
       "#",
        NULL},		
        KEY_FASTSENDDELAY,PARAM_INT,MOD_NONE,(void*)&sSetup.iSendDelay},
	{ { NULL},
        KEY_SLOWSENDDELAY,PARAM_INT,MOD_NONE,(void*)&sSetup.nSlowSendDelay},
	{ { NULL},
        KEY_FASTSENDLIMIT,PARAM_INT,MOD_NONE,(void*)&sSetup.nFastSendingCharLimit},
	{ {"#",
       "# The delay of the start from the startup initialization",
       "#", 
        NULL},
        KEY_INITDELAY,PARAM_INT,MOD_NONE,(void*)&sSetup.nSettling},
	{ {"#",
       "# The time in days an account exists.",
	   "# Unused accounts will be removed after this time.",
	   "#",
        NULL},
	   KEY_ALT,PARAM_INT,MOD_NONE,(void*)&sSetup.AccountLiveTime},
	{ {"#",
       "# The time limit in days for maximum login time",
       "#", 
        NULL},
        KEY_AUTOLOGOFF,PARAM_INT,MOD_NONE,(void*)&sSetup.AutoLoggoff},
	{ {"#",
       "# The time limit in seconds for ping timeout.",
       "#", 
        NULL},
        KEY_PINGTIMEOUT,PARAM_INT,MOD_NONE,(void*)&sSetup.iTimeout},
	{ {"#",
       "# The level of the output (0-7)",
       "#", 
        NULL},
        KEY_LOGLEVEL,PARAM_INT,MOD_NONE,(void*)&sSetup.nLogLevel}
};

void write_baseconfig()
{
	int i,t;
	char *pC;
	int *pI;
	long *pL;
	float *pF;
	double *pD;
	char **pS;
		
        FILE * fd;

       logger(LOG_NOTICE,gettext("Creating Configfile %s"),sSetup.configfile);

      /* Create & Openfile*/
      if ((fd=fopen(sSetup.configfile,"wb"))==NULL)
      {
          #ifdef NDEBUG
          fprintf(stderr,gettext("Couldn't open the configuration file %s."),sSetup.configfile);
          fprintf(stderr,"\n");
          #endif
              
          logger(LOG_ERR,gettext("Couldn't open the configuration file %s."),sSetup.configfile);
              
          exit(errno);
      }
					  
	
	for(i=0;i<VAL_COUNT;i++)
	{
		/* print Description */
		for(t=0;vsConfig[i].sDescription[t] ;t++)
		{
            if (strlen(vsConfig[i].sDescription[t]))
                fprintf(fd,"%s\n",vsConfig[i].sDescription[t]);
		}

		/* print Name */
		fprintf(fd,"%s=",vsConfig[i].sParamName);
		switch(vsConfig[i].iParamType)
		{
			case PARAM_CHAR:
				pC=(vsConfig[i].pParam);
				fprintf(fd,"%c\n",*pC);
				break;
			case PARAM_INT:
				pI=(vsConfig[i].pParam);
				fprintf(fd,"%d\n",*pI);
				break;
			case PARAM_LONG:
				pL=(vsConfig[i].pParam);
				fprintf(fd,"%ld\n",*pL);
				break;
			case PARAM_HEX:
				pI=(vsConfig[i].pParam);
				fprintf(fd,"%x\n",*pI);
				break;
			case PARAM_FLOAT:
				pF=(vsConfig[i].pParam);
				fprintf(fd,"%f\n",*pF);
				break;
			case PARAM_DOUBLE:
				pD=(vsConfig[i].pParam);
				fprintf(fd,"%G\n",*pD);
				break;
			case PARAM_STRING:
				pS=(vsConfig[i].pParam);
				if (vsConfig[i].iModefier==MOD_NONE)
					fprintf(fd,"%s\n",*pS);
				else if (vsConfig[i].iModefier==MOD_QUOTED)
					fprintf(fd,"\"%s\"\n",*pS);

				break;
			default:
				break;
		}
	}
	fclose(fd);
}
