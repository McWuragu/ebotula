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

#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

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

ConfigSetup_t sSetup;    // global config structure
volatile boolean stop;       // singal for stop the endless loop
boolean again;

pthread_mutex_t mutexAccount;      // mutex for synchronize the access of the login db 


CallbackDList CallbackList;

int main(int argc,char * const argv[]) {
    int i;
    int msgid;
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
    int iTemp;
    
    uid=geteuid();
    
    // init config
    if (uid==0) {
        // database path
        sSetup.pDatabasePath=(char *)malloc((strlen(DATABASEDIR)+1)*sizeof(char));
        strcpy(sSetup.pDatabasePath,DATABASEDIR);
        
        // config file path
        sSetup.configfile=(char *)malloc((strlen(CONFDIR)+strlen(CONFFILE)+1)*sizeof(char));
        sprintf(sSetup.configfile,"%s%s",CONFDIR,CONFFILE);
    } else {
        User=getpwuid(uid);
        
        // built the path for the  user dir
        sDirDummy=(char*)malloc((strlen(User->pw_dir)+strlen(DEFAULT_USER_CONFDIR)+1)*sizeof(char));
        sprintf(sDirDummy,"%s%s",User->pw_dir,DEFAULT_USER_CONFDIR);

        // create the user config dir
        if (!(pDir=opendir(sDirDummy))) {
            mkdir(sDirDummy,0700);
        }
        closedir(pDir); 

        // database path
        sSetup.pDatabasePath=(char *)malloc((strlen(sDirDummy)+strlen(DEFAULT_USER_DATABASEDIR)+1)*sizeof(char));
        sprintf(sSetup.pDatabasePath,"%s%s",sDirDummy,DEFAULT_USER_DATABASEDIR);

        // config file path
        sSetup.configfile=(char *)malloc((strlen(sDirDummy)+strlen(CONFFILE)+1)*sizeof(char));
        sprintf(sSetup.configfile,"%s%s",sDirDummy,CONFFILE);

        free (sDirDummy);
    }
    
    // set the other default values
    sSetup.pBotname=(char *)malloc((strlen(DEFAULT_BOTNAME)+1)*sizeof(char));
    strcpy(sSetup.pBotname,DEFAULT_BOTNAME);
    sSetup.realname=(char *)malloc((strlen(DEFAULT_REALNAME)+1)*sizeof(char));
    strcpy(sSetup.realname,DEFAULT_REALNAME);
    
    // irc connection
    sSetup.server=(char *)malloc((strlen(DEFAULT_IRC)+1)*sizeof(char));
    strcpy(sSetup.server,DEFAULT_IRC);
    sSetup.port=(char *)malloc((strlen(DEFAULT_PORT)+1)*sizeof(char));
    strcpy(sSetup.port,DEFAULT_PORT);

    // user & group
    sSetup.sExeGroup=(char *)malloc((strlen(DEFAULT_GROUP)+1)*sizeof(char));
    strcpy(sSetup.sExeGroup,DEFAULT_GROUP);
    sSetup.sExeUser=(char *)malloc((strlen(DEFAULT_USER)+1)*sizeof(char));
    strcpy(sSetup.sExeUser,DEFAULT_USER);

    sSetup.newMaster=false;
    sSetup.AccountLiveTime=DEFAULT_ALT;
    sSetup.AutoLoggoff=DEFAULT_LOGOFF;
    
    // send dealy
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

    // versions ausgabe
    printf(VERSIONSTR);
    printf("\n");
    #ifdef HAVE_SYSLOG_H
    openlog(PACKAGE,0,LOG_DAEMON);
    #endif
    
    // Look for simple parameter and  custom config file
    for (i=1;i<argc;i++) {
        if (argv[i][0]==PARAMETER_CHAR) {
            switch (argv[i][1]) {
            case 'D':
            {
                int tmp;
                logger(LOG_INFO,"Found debug level option");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }

                // linie limit  for the first send delay
                tmp=atoi(argv[i]);
                if (tmp<0  || tmp > MAX_LOGLEVEL) {
                    errno=EDOM;
                    perror(getMsgString(ERR_LOGLEVEL_RANGE));
                    exit(errno);
                }
                sSetup.nLogLevel=tmp;
                sSetup.bLogLevelWasSet=1;
                break;
            }
            case 'f':
                if (argv[++i]!=NULL)
        		{
        			free(sSetup.configfile);
        			sSetup.configfile=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                	        strcpy(sSetup.configfile,argv[i]);
        		}
        		else
        		{	
        			errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
        			exit(errno);
        		}
                break;
            case 'v':
                exit(0);
                break;
            case 'h':
            case '?':
                printMsg(getCmdLineHelp());
                exit(0);
                break;
            default:
                break;
            }
        }

    }

    // read config file
    ConfigFileParser();
    
    // check for parameter
    if (argc>1) {
        CommandLineParser(argc,argv);
    }

    // check the automatic times
    if (sSetup.AccountLiveTime<sSetup.AutoLoggoff) {
        sSetup.AutoLoggoff=sSetup.AccountLiveTime;
    }

    // check the  delay timings
    if (sSetup.nSlowSendDelay<sSetup.iSendDelay) {
        sSetup.nSlowSendDelay=sSetup.iSendDelay;
    }

    
    
    // change the uid and the gid for root
    if (uid==0) {
        // group
        if (sSetup.sExeGroup){
            if ((Group=getgrnam(sSetup.sExeGroup)))
               setegid(Group->gr_gid);
            else {
                logger(LOG_ERR,getSyslogString(SYSLOG_GROUP_NOT_FOUND));
            }
        }

        // user
        if (sSetup.sExeUser) {
            if ((User=getpwnam(sSetup.sExeUser)))
               seteuid(User->pw_uid);
            else {
                logger(LOG_ERR,getSyslogString(SYSLOG_USER_NOT_FOUND));
            }
        }
    }
   

    logger(LOG_INFO,"--------------- current setup -------------------");
    logger(LOG_INFO,"Server %s",sSetup.server);
    logger(LOG_INFO,"Port %s",sSetup.port);
    logger(LOG_INFO,"Nickname %s", sSetup.pBotname);
    logger(LOG_INFO,"Realname %s", sSetup.realname);
    logger(LOG_INFO,"Execute as %s.%s",sSetup.sExeUser,sSetup.sExeGroup);
    logger(LOG_INFO,"Threads %d",sSetup.thread_limit);
    logger(LOG_INFO,"Config file %s",sSetup.configfile);
    logger(LOG_INFO,"Database path %s",sSetup.pDatabasePath);
    logger(LOG_INFO,"Ping timeout %ds",sSetup.iTimeout);
    logger(LOG_INFO,"Fast sending delay %dms",sSetup.iSendDelay);
    logger(LOG_INFO,"Slow sending delay %dms",sSetup.nSlowSendDelay);
    logger(LOG_INFO,"Fast sending limit %d",sSetup.nFastSendingCharLimit);
    logger(LOG_INFO,"Startup initialization delay %ds",sSetup.nSettling);
    logger(LOG_INFO,"Account live time %dd",sSetup.AccountLiveTime);
    logger(LOG_INFO,"Autolog of after %dd",sSetup.AutoLoggoff);
    logger(LOG_INFO,"Log level: %i",sSetup.nLogLevel);
    logger(LOG_INFO,"-------------------------------------------------");

    logger(LOG_NOTICE,getSyslogString(SYSLOG_BOT_START));


    // init Database and the mutex for  access to the database
    initDatabases();

    // create master dialog
    if (sSetup.newMaster) {
        if (!dialogMaster()){
            closeDatabase();
            exit(-1);
        }
    }
    
    

    // create the network connection
    if ((sSetup.server!=NULL) && (sSetup.port!=NULL)) {
        #ifdef NDEBUG
        printf("%s\n",getSyslogString(SYSLOG_TRY_CONNECT));
        #endif
        logger(LOG_INFO,getSyslogString(SYSLOG_TRY_CONNECT));
        
        connectServer();
        
        #ifdef NDEBUG
        printf("%s\n",getSyslogString(SYSLOG_IS_CONNECT));
        #endif
        
        logger(LOG_INFO,getSyslogString(SYSLOG_IS_CONNECT));
    } else {
        closeDatabase();
        errno=EINVAL;
        #ifdef NDEBUG
        perror(getSyslogString(SYSLOG_FAILED_NETPARA));
        #endif
        logger(LOG_ERR,getSyslogString(SYSLOG_FAILED_NETPARA));
        exit(errno);
    }

    

    // connect to the server
    ConnectToIrc();
    #ifdef NDEBUG
    printf("%s\n",getSyslogString(SYSLOG_BOT_RUN));
    #endif
    logger(LOG_NOTICE,getSyslogString(SYSLOG_BOT_RUN));

    // redefine the signal handler for to stop the bot
    signal(SIGINT,stopParser);
    signal(SIGTERM,stopParser);
    signal(SIGABRT,stopParser);
    signal(SIGQUIT,stopParser);
    signal(SIGPIPE,stopParser);
    signal(SIGILL,stopParser);
    signal(SIGIO,stopParser);

    #ifdef NDEBUG
    // make a daemon 
    daemon(true,true);
    #endif
	
    pthread_mutex_init(&mutexAccount,NULL);
    
	 
	// init the command queue
	pCommandQueue=initQueue();
    init_extended_CallbackDList(&CallbackList, destroyCallbackItem);

    // Main execution loop
    stop=false;
    again=false;

	// create the threads
    pthread_create(&timeThread,NULL,TimingThread,NULL);

    threads=(pthread_t *)malloc(sSetup.thread_limit*sizeof(pthread_t));
    for (i=0;i<sSetup.thread_limit;i++) {
        pthread_create(&threads[i],NULL,CommandExecutionThread,(void*)pCommandQueue);
    }
    
	// join the channels
    pthread_create(&joinThread,NULL,JoinAllChannelsThread,&sSetup.nSettling);
    pthread_detach(joinThread);

    // init the  buffer  for unparsed string
    pUnparsed=(char*)malloc(sizeof(char));
    *pUnparsed='\0';

    while (!stop) {
        
        // read line from tcp stack
        RecvLine(buffer,RECV_BUFFER_SIZE);

        // added the new string to the  unparsed string
        pCurrStringPos=(char*)malloc((strlen(buffer)+strlen(pUnparsed)+1)*sizeof(char));
        sprintf(pCurrStringPos,"%s%s",pUnparsed,buffer);
        pCurrString=pCurrStringPos;

        // parse all substrings of the  receiving line
        while ((pStrPos=strchr(pCurrStringPos,'\r'))) {
            *pStrPos='\0';

            /* cut out a part of the  complete line */
            pCurrLine=(char *)malloc((strlen(pCurrStringPos)+1)*sizeof(char));
            strcpy(pCurrLine,pCurrStringPos);

            /* parse the part line */
            logger(LOG_DEBUG,"Parse: \"%s\"",pCurrLine);
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
        
        // buffer the  unparsed string
        free(pUnparsed);
        pUnparsed=(char*)malloc((strlen(pCurrStringPos)+1)*sizeof(char));
        strcpy(pUnparsed,pCurrStringPos);

        free(pCurrString);

        

    }
    free (sMsg.pMsgLine);
    free(pUnparsed);

    flushQueue(pCommandQueue);

    logger(LOG_NOTICE,getSyslogString(SYSLOG_BOT_STOP));
    pthread_join(timeThread,NULL);
    

    // wait of  terminat all threads
    for (i=0;i<sSetup.thread_limit;i++) { 
        pthread_join(threads[i],NULL);
    }


    // destroy the mutex
    pthread_mutex_destroy(&mutexAccount);
    

    // clear the wait queue and  callback list
	deleteQueue(pCommandQueue);
    destroyCallbackDList(&CallbackList);

    // disconnect from server
    disconnectServer();
    closeDatabase();
    
    
    //  check for restart option
    if (again) {
        logger(LOG_NOTICE,getSyslogString(SYSLOG_RESTART));
        
        #ifdef HAVE_SYSLOG_H
        closelog();
        #endif
        
        execvp(argv[0],argv);
        #ifdef NDEBUG
        perror(getMsgString(ERR_RESTART));
        #endif
        
        #ifdef HAVE_SYSLOG_H
        openlog(PACKAGE,0,LOG_DAEMON);
        #endif
        
        logger(LOG_ERR,getSyslogString(SYSLOG_RESTART));
        
        #ifdef HAVE_SYSLOG_H
        closelog();
        #endif
    } else {
        logger(LOG_NOTICE,getSyslogString(SYSLOG_STOPPED));
        closelog();
    }

    // clean up thread
    free (threads);

    free (sSetup.pBotname);
    free (sSetup.configfile);
    free (sSetup.pDatabasePath);
    free (sSetup.realname);
    free (sSetup.server);
    free (sSetup.port);
    free (sSetup.sExeGroup);
    free (sSetup.sExeUser);

    return(0);

}


