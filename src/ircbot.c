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
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <syslog.h>
#include <pwd.h>
#include <dirent.h>


#include <sys/types.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
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

pthread_mutex_t account_mutex;      // mutex for synchronize the access of the login db 


CallbackDList CallbackList;
PQueue pSendingQueue;

int main(int argc,char * const argv[]) {
    int i;
    int msgid;
    char buffer[RECV_BUFFER_SIZE],*pos,*str,*tmp;
    pthread_t *threads;
    pthread_t timeThread;
    pthread_t sendThread;
    MsgBuf_t *pMsg;
    QueueData Command;	
    PQueue pCommandQueue;
    uid_t uid;
    struct passwd *User;
	char *sDirDummy;
    DIR *pDir;

    uid=geteuid();
    User=getpwuid(uid);

    // init config
    if (uid==0) {
        // database path
        sSetup.pDatabasePath=(char *)malloc((strlen(DATABASEDIR)+1)*sizeof(char));
        strcpy(sSetup.pDatabasePath,DATABASEDIR);
        
        // config file path
        sSetup.configfile=(char *)malloc((strlen(CONFDIR)+strlen(CONFFILE)+1)*sizeof(char));
        sprintf(sSetup.configfile,"%s%s",CONFDIR,CONFFILE);
    
        // change the user
        User=getpwnam(sSetup.sExeUser);
        setreuid(User->pw_uid,User->pw_uid);
        uid=User->pw_uid;

    } else {
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
    }
    
    // set the other default values
    sSetup.botname=(char *)malloc((strlen(DEFAULT_BOTNAME)+1)*sizeof(char));
    strcpy(sSetup.botname,DEFAULT_BOTNAME);
    sSetup.realname=(char *)malloc((strlen(DEFAULT_REALNAME)+1)*sizeof(char));
    strcpy(sSetup.realname,DEFAULT_REALNAME);
    sSetup.newMaster=false;
    sSetup.AccountLiveTime=MIN_ALT;
    sSetup.AutoLoggoff=MIN_LOGOFF;
    sSetup.iSendDelay=DEFAULT_SEND_DELAY;
    sSetup.iSendSafeDelay=DEFAULT_SEND_SAFE_DELAY;
    sSetup.iSendSafeLine=DEFAULT_SEND_SAFE_LINE;
    sSetup.iTimeout=DEFAULT_PING_TIMEOUT;
    sSetup.thread_limit=DEFAULT_THREAD_LIMIT;
    sSetup.sExeUser=DEFAULT_USER;

    

    
    // versions ausgabe
    printf(VERSIONSTR);
    openlog(PACKAGE,0,LOG_DAEMON);
    
    // Look for simple parameter and  custom config file
    for (i=1;i<argc;i++) {
        if (argv[i][0]==PARAMETER_CHAR) {
            switch (argv[i][1]) {
            case 'f':
                free(sSetup.configfile);
                sSetup.configfile=(char *)malloc((strlen(argv[++i])+1)*sizeof(char));
                strcpy(sSetup.configfile,argv[i]);
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
    DEBUG("File %s\n",sSetup.configfile);

    // read config file
    ConfigFileParser();
    syslog(LOG_NOTICE,getSyslogString(SYSLOG_READ_CONFFILE));
    
    // check for parameter
    if (argc>1) {
        ComandLineParser(argc,argv);
        syslog(LOG_NOTICE,SYSLOG_READ_CMD);
    }

    // check the automatic times
    if (sSetup.AccountLiveTime<sSetup.AutoLoggoff) {
        sSetup.AutoLoggoff=sSetup.AccountLiveTime;
    }

    // check the  delay timings
    if (sSetup.iSendSafeDelay<sSetup.iSendDelay) {
        sSetup.iSendSafeDelay=sSetup.iSendDelay;
    }

    
    
    // change the uid for root
    if (uid==0) {
    
    }
   
    DEBUG("----------------------------------------------\n");
    DEBUG("Server %s\n",sSetup.server);
    DEBUG("Port %s\n",sSetup.port);
    DEBUG("Nickname %s\n", sSetup.botname);
    DEBUG("Realname %s\n", sSetup.realname);
    DEBUG("Threads %d\n",sSetup.thread_limit);
    DEBUG("Config file %s\n",sSetup.configfile);
    DEBUG("Database path %s\n",sSetup.pDatabasePath);
    DEBUG("Ping timeout %ds\n",sSetup.iTimeout);
    DEBUG("Sending delay %dms\n",sSetup.iSendDelay);
    DEBUG("Sending safe delay %dms\n",sSetup.iSendSafeDelay);
    DEBUG("Sending  line limit %d\n",sSetup.iSendSafeLine);
    DEBUG("Account live time %dd\n",sSetup.AccountLiveTime);
    DEBUG("Autolog of after %dd\n",sSetup.AutoLoggoff);
    DEBUG("-----------------------------------------------\n");

    syslog(LOG_NOTICE,getSyslogString(SYSLOG_BOT_START));

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
        printf("%s\n",getSyslogString(SYSLOG_TRY_CONNECT));
        
        syslog(LOG_INFO,getSyslogString(SYSLOG_TRY_CONNECT));
        
        connectServer();
        
        syslog(LOG_INFO,getSyslogString(SYSLOG_IS_CONNECT));
        
        printf("%s\n",getSyslogString(SYSLOG_IS_CONNECT));
    } else {
        closeDatabase();
        errno=EINVAL;
        perror(getSyslogString(SYSLOG_FAILED_NETPARA));
        syslog(LOG_ERR,getSyslogString(SYSLOG_FAILED_NETPARA));
        exit(errno);
    }

    

    // connect to the server
    ConnectToIrc();
    printf("%s\n",getSyslogString(SYSLOG_BOT_RUN));
    syslog(LOG_NOTICE,getSyslogString(SYSLOG_BOT_RUN));


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
   
	pthread_mutex_init(&account_mutex,NULL);
	 
	// init the command queue
	pCommandQueue=initQueue();
    pSendingQueue=initQueue();
    init_extended_CallbackDList(&CallbackList, destroyCallbackItem);

    // Main execution loop
    stop=false;
    again=false;

	// create the threads
    pthread_create(&timeThread,NULL,TimingThread,NULL);
    pthread_create(&sendThread,NULL,SendingThread,NULL);

    threads=(pthread_t *)malloc(sSetup.thread_limit*sizeof(pthread_t));
    for (i=0;i<sSetup.thread_limit;i++) {
        pthread_create(&threads[i],NULL,ComandExecutionThread,(void*)pCommandQueue);
    }
    
	// join the channels
    join_all_channels();

    while (!stop) {

        // read line from tcp stack
        recv_line(buffer,RECV_BUFFER_SIZE);

        // parse all substrings of the  receiving line
        tmp=buffer;
        while ((pos=strchr(tmp,'\r'))) {
            *pos='\0';

            /* cut out a part of the  complete line */
            str=(char *)calloc(strlen(tmp)+1,sizeof(char));
            strcpy(str,tmp);

            /* parse the part line */
            DEBUG("Parse: \"%s\"\n",str);
            pMsg=preParser(str);

            /* put the identified line  on the  queue */
            if (pMsg->identify!=CMD_NONE) {
				Command.t_size=sizeof(MsgBuf_t)+strlen(pMsg->pMsgLine)+1;
				Command.data=pMsg;
				pushQueue(pCommandQueue,Command);
            }
            free(str);
            
            /* 
             * checking the length of the next substring
             * a irc answer line can't are ten charakter 
             * it is to small for a complete line
             */
            if (strlen(++pos)>0) {
                /* set the begin of new substring of the old end
                 * and plus one for the NL */
                tmp=pos+1;
            }
            
        }

        

    }

    flushQueue(pCommandQueue);
    flushQueue(pSendingQueue);

    syslog(LOG_NOTICE,getSyslogString(SYSLOG_BOT_STOP));
    pthread_join(timeThread,NULL);
    
    pthread_join(sendThread,NULL);

    // wait of  terminat all threads
    for (i=0;i<sSetup.thread_limit;i++) { 
        pthread_join(threads[i],NULL);
    }


    // destroy the mutex
    pthread_mutex_destroy(&account_mutex);

    // clear the wait queue and  callback list
	deleteQueue(pCommandQueue);
    deleteQueue(pSendingQueue);
    destroyCallbackDList(&CallbackList);

    // disconnect from server
    disconnectServer();
    closeDatabase();
    
    
    //  check for restart option
    if (again) {
        syslog(LOG_NOTICE,getSyslogString(SYSLOG_RESTART));
        closelog();
        execvp(argv[0],argv);
        perror(getMsgString(ERR_RESTART));
    } else {
        syslog(LOG_NOTICE,getSyslogString(SYSLOG_STOPPED));
        closelog();
    }
    return(0);

}


