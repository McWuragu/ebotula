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
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <locale.h>

#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#ifdef HAVE_SYSLOG_H
    #include <syslog.h>
#endif

#include "messages.h"
#include "callbacklist.h"
#include "utilities.h"
#include "parameter.h"
#include "network.h"
#include "parser.h"
#include "timing.h"
#include "dbaccess.h"
#include "queue.h"
#include "ircbot.h"

ConfigSetup_t sSetup;    /* global config structure */
volatile boolean stop=false;       /* singal for stop the endless loop*/
volatile boolean again=false;
static boolean bRunAsRoot=false;

pthread_mutex_t mutexAccount;      /* mutex for synchronize the access of the login db  */


CallbackDList CallbackList;

/**
 * this  function is the main of the bot. This is  called by the ansi
 * c main. 
 * 
 * @param argc   agrument counter
 * @param argv   agrument vector
 * 
 * @return execution state
 */
int AppMain(int argc, char * const argv[]);

int main(int argc,char * const argv[]) {
    int nRet;

    /* versions ausgabe */
    printf(VERSIONSTR);
    printf("\n");

    /* nls support */
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE,PACKAGE_LOCALE_DIR);
    textdomain(PACKAGE);
 

    do {
        nRet=AppMain(argc,argv);
    } while (again && !nRet);

    return nRet;
}


int AppMain(int argc, char * const argv[]) {
    int i,nRet;
    char buffer[RECV_BUFFER_SIZE],*pStrPos,*pCurrLine,*pCurrString,*pCurrStringPos,*pUnparsed;
    pthread_t *threads;
    pthread_t timeThread;
    
    pthread_t joinThread;
    MsgBuf_t sMsg;
    QueueData Command;	
    PQueue pCommandQueue;
    uid_t uid;
    struct passwd *User;
    struct group *Group;
	char *sDirDummy;
    DIR *pDir;
    struct stat attribut;
    
    uid=geteuid();
    bRunAsRoot=(uid==0 && !bRunAsRoot)?true:bRunAsRoot;
   
    
    
    /* init config */
    if (bRunAsRoot) {
        /* database path */
        sSetup.pDatabasePath=(char *)malloc((strlen(DATABASEDIR)+1)*sizeof(char));
        strcpy(sSetup.pDatabasePath,DATABASEDIR);
        
        /* config file path */
        sSetup.configfile=(char *)malloc((strlen(CONFDIR)+strlen(CONFFILE)+1)*sizeof(char));
        sprintf(sSetup.configfile,"%s%s",CONFDIR,CONFFILE);
    } else {
        User=getpwuid(uid);
        
        /* built the path for the  user dir */
        sDirDummy=(char*)malloc((strlen(User->pw_dir)+strlen(DEFAULT_USER_CONFDIR)+1)*sizeof(char));
        sprintf(sDirDummy,"%s%s",User->pw_dir,DEFAULT_USER_CONFDIR);

        /* create the user config dir */
        if (!(pDir=opendir(sDirDummy))) {
            mkdir(sDirDummy,0700);
        }
        closedir(pDir); 

        /* database path */
        sSetup.pDatabasePath=(char *)malloc((strlen(sDirDummy)+strlen(DEFAULT_USER_DATABASEDIR)+1)*sizeof(char));
        sprintf(sSetup.pDatabasePath,"%s%s",sDirDummy,DEFAULT_USER_DATABASEDIR);

        /* config file path */
        sSetup.configfile=(char *)malloc((strlen(sDirDummy)+strlen(CONFFILE)+1)*sizeof(char));
        sprintf(sSetup.configfile,"%s%s",sDirDummy,CONFFILE);

        free (sDirDummy);
    }
    
    /* set the other default values */
    sSetup.pBotname=(char *)malloc((strlen(DEFAULT_BOTNAME)+1)*sizeof(char));
    strcpy(sSetup.pBotname,DEFAULT_BOTNAME);
    sSetup.realname=(char *)malloc((strlen(DEFAULT_REALNAME)+1)*sizeof(char));
    strcpy(sSetup.realname,DEFAULT_REALNAME);
    
    /* irc connection */
    sSetup.sHostname=(char *)malloc((strlen(DEFAULT_IRC)+1)*sizeof(char));
    strcpy(sSetup.sHostname,DEFAULT_IRC);
    sSetup.sPort=(char *)malloc((strlen(DEFAULT_PORT)+1)*sizeof(char));
    strcpy(sSetup.sPort,DEFAULT_PORT);

    /* user & group */
    sSetup.sExeGroup=(char *)malloc((strlen(DEFAULT_GROUP)+1)*sizeof(char));
    strcpy(sSetup.sExeGroup,DEFAULT_GROUP);
    sSetup.sExeUser=(char *)malloc((strlen(DEFAULT_USER)+1)*sizeof(char));
    strcpy(sSetup.sExeUser,DEFAULT_USER);

    sSetup.newMaster=false;
    sSetup.AccountLiveTime=DEFAULT_ALT;
    sSetup.AutoLoggoff=DEFAULT_LOGOFF;
    
    /* send dealy */
    sSetup.iSendDelay=DEFAULT_FAST_SEND_DELAY;
    sSetup.nSlowSendDelay=DEFAULT_SLOW_SEND_DELAY;
    sSetup.nFastSendingCharLimit=DEFAULT_FAST_SEND_LIMIT;

    sSetup.iTimeout=DEFAULT_PING_TIMEOUT;
    sSetup.thread_limit=DEFAULT_THREAD_LIMIT;
    
    sSetup.nSettling=DEFAULT_INIT_DELAY;

    #ifdef NDEBUG
        sSetup.nLogLevel=DEFAULT_LOGLEVEL;
    #else
        sSetup.nLogLevel=DEFAULT_DEBUG_LOGLEVEL;
    #endif
    sSetup.bLogLevelWasSet=0;

    
    
    #ifdef HAVE_SYSLOG_H
    openlog(PACKAGE,0,LOG_DAEMON);
    #endif
    
    /* Look for simple parameter and  custom config file */
    for (i=1;i<argc;i++) {
        if (argv[i][0]==PARAMETER_CHAR) {
            switch (argv[i][1]) {
            case 'V':
            {
                int tmp;
                logger(LOG_INFO,_("Found debug level option."));
                if (++i>=argc) {
                    errno=EINVAL;
                    logger(LOG_ERR,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    return(errno);
                }

                /* linie limit  for the first send delay */
                tmp=atoi(argv[i]);
                if (tmp<0  || tmp > MAX_LOGLEVEL) {
                    errno=EDOM;
                    logger(LOG_ERR,_("The log level %i is invalid.\n"),tmp);
                    return(errno);
                }
                sSetup.nLogLevel=convertVerboseToLogLevel(tmp);
                sSetup.bLogLevelWasSet=1;
                break;
            }
            case 'f':
                if (argv[++i]!=NULL)
        		{
    				if (stat(argv[i],&attribut))
    				{
    					logger(LOG_ERR,"Couldn't access to %s - %s\n",argv[i],strerror(errno));
    					return(errno);
    				}else
    				{
    					if (S_ISREG(attribut.st_mode))
    					{
    				      		free(sSetup.configfile);
            					sSetup.configfile=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
    	        	        	        strcpy(sSetup.configfile,argv[i]);
    					}else
    					{
    						logger(LOG_ERR,"%s is not a regulare file\n",argv[i]);
    						errno=EINVAL;
    						return(errno);
    					}
    				}
        		}
        		else
        		{	
        			errno=EINVAL;
                    logger(LOG_ERR,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    return(errno);
        		}
                break;
            case 'v':
                stop=false;
                return(0);
                break;
            case 'h':
            case '?':
                printCmdLineHelp();
                stop=false;
                return(0);
                break;
            default:
                break;
            }
        }

    }

    /* read config file*/
    if (nRet=ConfigFileParser())
        return nRet;
    
    /* check for parameter */
    if (argc>1) {
        if (!CommandLineParser(argc,argv))
            return errno;
    }

    /* check the automatic times */
    if (sSetup.AccountLiveTime<sSetup.AutoLoggoff) {
        sSetup.AutoLoggoff=sSetup.AccountLiveTime;
    }

    /* check the  delay timings */
    if (sSetup.nSlowSendDelay<sSetup.iSendDelay) {
        sSetup.nSlowSendDelay=sSetup.iSendDelay;
    }

    
    
    /* change the uid and the gid for root */
    if (bRunAsRoot) {
        /* group */
        if (sSetup.sExeGroup){
            if ((Group=getgrnam(sSetup.sExeGroup)))
               setegid(Group->gr_gid);
            else {
                logger(LOG_ERR,_("The group %s couldn't found"),sSetup.sExeGroup);
            }
        }

        /* user */
        if (sSetup.sExeUser) {
            if ((User=getpwnam(sSetup.sExeUser)))
               seteuid(User->pw_uid);
            else {
                logger(LOG_ERR,_("The user %s couldn't found"),sSetup.sExeUser);
            }
        }
    }
   

    logger(LOG_INFO,_("--------------- current setup -------------------"));
    logger(LOG_INFO,_("Hostname %s"),sSetup.sHostname);
    logger(LOG_INFO,_("Portnumber %s"),sSetup.sPort);
    logger(LOG_INFO,_("Nickname %s"), sSetup.pBotname);
    logger(LOG_INFO,_("Realname %s"), sSetup.realname);
    logger(LOG_INFO,_("Execute as %s.%s"),sSetup.sExeUser,sSetup.sExeGroup);
    logger(LOG_INFO,_("Threads %d"),sSetup.thread_limit);
    logger(LOG_INFO,_("Configuration file %s"),sSetup.configfile);
    logger(LOG_INFO,_("Database path %s"),sSetup.pDatabasePath);
    logger(LOG_INFO,_("Ping timeout %ds"),sSetup.iTimeout);
    logger(LOG_INFO,_("Fast sending delay %dms"),sSetup.iSendDelay);
    logger(LOG_INFO,_("Slow sending delay %dms"),sSetup.nSlowSendDelay);
    logger(LOG_INFO,_("Fast sending limit %d"),sSetup.nFastSendingCharLimit);
    logger(LOG_INFO,_("Startup initialization delay %ds"),sSetup.nSettling);
    logger(LOG_INFO,_("Account live time %dd"),sSetup.AccountLiveTime);
    logger(LOG_INFO,_("Autolog of after %dd"),sSetup.AutoLoggoff);
    logger(LOG_INFO,_("Log level: %i"),sSetup.nLogLevel);
    logger(LOG_INFO,_("----------------- setup end ---------------------"));

    logger(LOG_NOTICE,_("Start..."));


    /* init Database and the mutex for  access to the database */
    if (!initDatabases())
        return(errno);

    /* create master dialog */
    if (sSetup.newMaster) {
        if (!dialogMaster()){
            closeDatabase();
            return(-1);
        }
    }
    
    

    /* create the network connection */
    if (!connectServer()) {
        closeDatabase();
        closelog();
        return(errno);
    }
    
    /* connect to the irc service */
    if (!ConnectToIrc()) {
        disconnectServer();
        closeDatabase();
        closelog();
        return(errno);
    }

    logger(LOG_NOTICE,_("Running..."));

    /* redefine the signal handler for to stop the bot */
    signal(SIGINT,stopParser);
    signal(SIGTERM,stopParser);
    signal(SIGABRT,stopParser);
    signal(SIGQUIT,stopParser);
    signal(SIGPIPE,stopParser);
    signal(SIGILL,stopParser);
    signal(SIGIO,stopParser);

    #ifdef NDEBUG
    /* make a daemon  */
    daemon(true,true);
    HideLogLinesOnScreen();
    #endif
	
    pthread_mutex_init(&mutexAccount,NULL);
    
	 
	/* init the command queue */
	pCommandQueue=initQueue();
    init_extended_CallbackDList(&CallbackList, destroyCallbackItem);

    /* Main execution loop */
    stop=false;
    again=false;

	/* create the threads */
    pthread_create(&timeThread,NULL,TimingThread,NULL);

    threads=(pthread_t *)malloc(sSetup.thread_limit*sizeof(pthread_t));
    for (i=0;i<sSetup.thread_limit;i++) {
        pthread_create(&threads[i],NULL,CommandExecutionThread,(void*)pCommandQueue);
    }
    
	/* join the channels*/
    pthread_create(&joinThread,NULL,JoinAllChannelsThread,&sSetup.nSettling);
    pthread_detach(joinThread);

    /* init the  buffer  for unparsed string */
    pUnparsed=(char*)malloc(sizeof(char));
    *pUnparsed='\0';

    while (!stop) {
        
        /* read line from tcp stack */
        RecvLine(buffer,RECV_BUFFER_SIZE);

        /* added the new string to the  unparsed string */
        pCurrStringPos=(char*)malloc((strlen(buffer)+strlen(pUnparsed)+1)*sizeof(char));
        sprintf(pCurrStringPos,"%s%s",pUnparsed,buffer);
        pCurrString=pCurrStringPos;

        /* parse all substrings of the  receiving line */
        while ((pStrPos=strchr(pCurrStringPos,'\r'))) {
            *pStrPos='\0';

            /* cut out a part of the  complete line */
            pCurrLine=(char *)malloc((strlen(pCurrStringPos)+1)*sizeof(char));
            strcpy(pCurrLine,pCurrStringPos);

            /* parse the part line */
	    if ((strstr(pCurrLine,CmdList[CMD_IDENT]))!=NULL)
	    {
	            logger(LOG_DEBUG,_("Receive: \"%s\""),"!ident <nickname> <password>\0");
	    }else{
		    
		logger(LOG_DEBUG,_("Receive: \"%s\""),pCurrLine);
	    }
            preParser(pCurrLine,&sMsg);

            /* put the identified line  on the  queue */
            if (sMsg.identify!=CMD_NONE) {
                Command.t_size=sizeof(MsgBuf_t);
				Command.data=&sMsg;
				pushQueue(pCommandQueue,Command);


            }
            free(pCurrLine);
            
            /* 
             * checking the length of the next substring
             * a irc answer line can't are ten charakter 
             * it is to small for a complete line
             */
            if (strlen(++pStrPos)>0) {
                /* set the begin of new substring of the old end
                 * and plus one for the NL */
                pCurrStringPos=pStrPos+1;
            }
            
        }
        
        /* buffer the  unparsed string */
        free(pUnparsed);
        pUnparsed=(char*)malloc((strlen(pCurrStringPos)+1)*sizeof(char));
        strcpy(pUnparsed,pCurrStringPos);

        free(pCurrString);

        

    }
    free (sMsg.pMsgLine);
    free(pUnparsed);

    flushQueue(pCommandQueue);

    logger(LOG_NOTICE,_("The bot is stopping."));
    pthread_join(timeThread,NULL);
    

    /* wait of  terminat all threads */
    for (i=0;i<sSetup.thread_limit;i++) { 
        pthread_join(threads[i],NULL);
    }


    /* destroy the mutex */
    pthread_mutex_destroy(&mutexAccount);
    

    /* clear the wait queue and  callback list */
	deleteQueue(pCommandQueue);
    destroyCallbackDList(&CallbackList);

    /* disconnect from server */
    disconnectServer();
    closeDatabase();
    
    /* clean up thread */
    free (threads);

    free (sSetup.pBotname);
    free (sSetup.configfile);
    free (sSetup.pDatabasePath);
    free (sSetup.realname);
    free (sSetup.sHostname);
    free (sSetup.sPort);
    free (sSetup.sExeGroup);
    free (sSetup.sExeUser);



    closelog();

    return(0);
}

