/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <termios.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "ircbot.h"
#include "irc.h"
#include "utilities.h"
#include "dbaccess.h"
#include "baseconfig.h" 

#define CMD_MSG
    #include "messages.h"
#undef CMD_MSG

#include "parameter.h"
/* #############################################################################                   */
boolean CommandLineParser(int argc,char *const argv[]) {
    extern ConfigSetup_t sSetup;
    int i;
    int tmp;

    logger(LOG_NOTICE,_("Command line parameters found and read"));
    
    for (i=1;i<argc;i++) {
        if (argv[i][0]==PARAMETER_CHAR) {
            logger(LOG_INFO,_("Check option \"%s\""),argv[i]);
            switch  (argv[i][1]) {
            case 's':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
               
                /* set the server name*/
                if (strpbrk(argv[i],SERVER_NOT_ALLOW_CHAR)) {
                    errno=EINVAL;
                    fprintf(stderr,_("The hostname is irregular.\n"));
                    exit(errno);
                }
               
                sSetup.sHostname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.sHostname,argv[i]);
                
                break;
            case 'b':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set the pBotname */
                if (!NickStringCheck(argv[i])) {
                    errno=EINVAL;
                    fprintf(stderr,_("The botname is irregular.\n"));
                    exit(errno);
                }
                
                sSetup.pBotname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.pBotname,argv[i]);
                
                break;
            case 'u':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set the user */
                sSetup.sExeUser=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.sExeUser,argv[i]);
                
                break;
            case 'g':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set the group */
                sSetup.sExeGroup=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.sExeGroup,argv[i]);
                
                break;
            case 'r':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set realname */
                sSetup.realname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.realname,argv[i]);
                
                break;
            case 'p':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set portnumber */
                if ((atoi(argv[i])<1) || (atoi(argv[i])>65535)) {
                    errno=EDOM;
                    fprintf(stderr,_("The portnumber %i value is out of range.\n"),atoi(argv[i]));
                    exit(errno);
                }
                
                free(sSetup.sPort);
                sSetup.sPort=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.sPort,argv[i]);
    
                break;
            case 't':
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set threadlimit */
                tmp=atoi(argv[i]);
                if ((tmp<1) || (tmp>MAX_THREADS_LIMIT)) {
                    errno=EDOM;
                    fprintf(stderr,_("The thread limit is out of range.\n"));
                    exit(errno);
                }
                sSetup.thread_limit=tmp;
                break;
            case 'a':
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set auto logoff time */
                tmp=atoi(argv[i]);
                if (tmp<MIN_LOGOFF) {
                    errno=EDOM;
                    fprintf(stderr,_("The automatic logoff time is invalid.\n"));
                    exit(errno);
                }
                sSetup.AutoLoggoff=tmp;
                break;
            case 'n':
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set sending delay */
                tmp=atoi(argv[i]);
                if (tmp<=0) {
                    errno=EDOM;
                    fprintf(stderr,_("The send delay time is invalid.\n"));
                    exit(errno);
                }
                sSetup.iSendDelay=tmp;
                break;
            case 'e':
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set sending delay */
                tmp=atoi(argv[i]);
                if (tmp<=0) {
                    errno=EDOM;
                    fprintf(stderr,_("The send delay time is invalid.\n"));
                    exit(errno);
                }
                sSetup.nSlowSendDelay=tmp;
                break;
            case 'l':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }

                /* linie limit  for the first send delay */
                tmp=atoi(argv[i]);
                if (tmp<0) {
                    errno=EDOM;
                    fprintf(stderr,_("The limit for sending with the fast send delay is invalid.\n"));
                    exit(errno);
                }
                sSetup.nFastSendingCharLimit=tmp;
                break;
            case 'i':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }

                /* linie limit  for the first send delay */
                tmp=atoi(argv[i]);
                if (tmp<0) {
                    errno=EDOM;
                    fprintf(stderr,_("The delay of the startup initialization is invalid.\n"));
                    exit(errno);
                }
                sSetup.nSettling=tmp;
                break;
            case 'k':
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                
                /* set account live time */
                tmp=atoi(argv[i]);
                if (tmp<MIN_ALT) {
                    errno=EDOM;
                    fprintf(stderr,_("The account living time is invalid.\n"));
                    exit(errno);
                }
                sSetup.AccountLiveTime=tmp;
                break;
            case 'm':
                sSetup.newMaster=true;
                break;
            case 'd':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }
                /* set database path */
                sSetup.pDatabasePath=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.pDatabasePath,argv[i]);
                break;
            case 'c':
                if (++i>=argc) {
                    errno=EINVAL;
                    fprintf(stderr,_("\"%s\" need a parameter value.\n"),argv[i-1]);
                    exit(errno);
                }

                /* set the limit for ping timeout in s */
                tmp=atoi(argv[i]);
                if (tmp<MIN_PINGTIME) {
                    errno=EDOM;
                    fprintf(stderr,_("The ping timeout is invalid.\n"));
                    exit(errno);
                }
                sSetup.iTimeout=tmp;
                break;
            case 'D':
            case 'f':
                /* Dummy, Configfile allready parsed */
                i++;
                break;
            default:
                fprintf(stderr,_("Unkown option \"%s\"\n"),argv[i]);
                fprintf(stderr,_("Type %s -h for more help.\n"),PACKAGE);
                errno=EINVAL;
                return false;
                break;
            }
        } else {
                fprintf(stderr,_("Invalid option \"%s\"\n"),argv[i]);
                fprintf(stderr,_("Type %s -h for more help.\n"),PACKAGE);
                errno=ENOSYS;
                return false;
        }
    }
    return true;
}
/*############################################################################# */
void ConfigFileParser(void) {
    FILE *fd;
    extern ConfigSetup_t sSetup;
    int tmp;
    char buffer[MAX_READ_BUFFER_SIZE], *c,*value,*key;
    /*errno=0; */

    logger(LOG_NOTICE,_("Read configuration file %s"),sSetup.configfile);

    if (!(fd=fopen(sSetup.configfile,"r"))) {
	    /* generating basicconfig for ebotula */
        write_baseconfig(sSetup.configfile);
    }else {
        logger(LOG_DEBUG,_("File is open"),sSetup.configfile);
    
        while((fgets(buffer,MAX_READ_BUFFER_SIZE,fd)!=NULL)){
            /* remove newline and leading spaces */
            trim(buffer);
            clearspace(buffer);
            
            /* ignore space lines and comments */
            if ((buffer[0]!=COMMENT_CHAR) && (buffer[0]!='\0')) {
                logger(LOG_INFO,_("Found config line %s"),buffer);
                
    
                c=strchr(buffer,'=');
                
                /* parse the value from the line */
                value=(char *)malloc((strlen(c)+1)*sizeof(char));
                strcpy(value,c+1);
                
                /* parse the key from the line */
                key=(char *)malloc((strlen(buffer)-strlen(c)+1)*sizeof(char));
                strtok(buffer,"=");
                strcpy(key,buffer);
                
                /* set  the  reading values */
                if (!strcmp(key,KEY_SERVER)) {
                    if (strpbrk(value,SERVER_NOT_ALLOW_CHAR)) {
                        errno=EINVAL;
                        fprintf(stderr,_("The hostname is irregular.\n"));
                        exit(errno);
                    }
                    /* set hostname */
                    free(sSetup.sHostname);
                    sSetup.sHostname=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.sHostname,value);
                } else if (!strcmp(key,KEY_PORT)) {
                    if ((atoi(value)<1) || (atoi(value)>65535)) {
                        errno=EINVAL;
                        fprintf(stderr,_("The portnumber %i value is out of range.\n"),atoi(value));
                        exit(errno);
                    }
                    /* set port */
                    free(sSetup.sPort);
                    sSetup.sPort=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.sPort,value);
                } else if (!strcmp(key,KEY_BOTNAME)) {
                    if (!NickStringCheck(value)) {
                        errno=EINVAL;
                        fprintf(stderr,_("The botname is irregular.\n"));
                        exit(errno);
                    }
                    /* set pBotname */
                    free(sSetup.pBotname);
                    sSetup.pBotname=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.pBotname,value);
                } else if (!strcmp(key,KEY_REALNAME)) {
                    /* ser realname */
                    free(sSetup.realname);
                    sSetup.realname=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.realname,value);
                } else if (!strcmp(key,KEY_THREADLIMIT)) {
                    tmp=atoi(value);
                    if ((tmp<=0) || (tmp>MAX_THREADS_LIMIT)) {
                        errno=EDOM;
                        fprintf(stderr,_("The thread limit is out of range.\n"));
                        exit(errno);
                    }   
                    /* set thread limit */
                    sSetup.thread_limit=tmp;
                } else  if (!strcmp(key,KEY_DATABASEPATH)) {
                    /* set database path */
                    free(sSetup.pDatabasePath);
                    sSetup.pDatabasePath=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.pDatabasePath,value);
                } else if (!strcmp(key,KEY_AUTOLOGOFF)) {
                    tmp=atoi(value);
                    if (tmp<=0) {
                        errno=EDOM;
                        fprintf(stderr,_("The automatic logoff time is invalid.\n"));
                        exit(errno);
                    }   
                    /* set auto logoff time */
                    sSetup.AutoLoggoff=tmp;
                } else if (!strcmp(key,KEY_FASTSENDDELAY)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        fprintf(stderr,_("The send delay time is invalid.\n"));
                        exit(errno);
                    }   
                    /* set first delay time */
                    sSetup.iSendDelay=tmp;
                } else if (!strcmp(key,KEY_SLOWSENDDELAY)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        fprintf(stderr,_("The send delay time is invalid.\n"));
                        exit(errno);
                    }   
                    /* set second delay time */
                    sSetup.nSlowSendDelay=tmp;
                } else if (!strcmp(key,KEY_FASTSENDLIMIT)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        fprintf(stderr,_("The limit for sending with the fast send delay is invalid.\n"));
                        exit(errno);
                    }   
                    /* set auto logoff time */
                    sSetup.nFastSendingCharLimit=tmp;
                } else if (!strcmp(key,KEY_INITDELAY)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        fprintf(stderr,_("The delay of the startup initialization is invalid.\n"));
                        exit(errno);
                    }   
                    /* set auto logoff time */
                    sSetup.nSettling=tmp;
                } else if (!strcmp(key,KEY_ALT)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        fprintf(stderr,_("The account living time is invalid.\n"));
                        exit(errno);
                    }   
                    /* set account live time */
                    sSetup.AccountLiveTime=tmp;
                } else if (!strcmp(key,KEY_PINGTIMEOUT)) {
                    tmp=atoi(value);
                    if (tmp<=0) {
                        errno=EDOM;
                        fprintf(stderr,_("The ping timeout is invalid.\n"));
                        exit(errno);
                    }   
                    /* set account live time */
                    sSetup.iTimeout=tmp;
                } else if (!strcmp(key,KEY_LOGLEVEL)) {
                    /* cmd line has change  the log level */
                    if (!sSetup.bLogLevelWasSet) {
                        tmp=atoi(value);
                        if (tmp<=0 || tmp > MAX_LOGLEVEL) {
                            errno=EDOM;
                            fprintf(stderr,_("The log level %i is invalid.\n"),tmp);
                            exit(errno);
                        }
                        sSetup.nLogLevel=tmp;
                    }
                }else if (!strcmp(key,KEY_EXEUSER)) {
                    /* set execution rights */
                    if (sSetup.sExeUser)free(sSetup.sExeUser);
                    sSetup.sExeUser=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.sExeUser,value);
                }else if (!strcmp(key,KEY_EXEGROUP)) {
                    /* set execution rights */
                    if (sSetup.sExeGroup) free(sSetup.sExeGroup);
                    sSetup.sExeGroup=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.sExeGroup,value);
                }
                free(value);
                free(key);
            }
        }
        fclose(fd);
    }
}

/* ############################################################################# */
boolean dialogMaster(void){
    char  name[LOGIN_LENGTH+1],passwd[PASSWD_LENGTH+1],repasswd[PASSWD_LENGTH+1];
     struct termios stBuf, stBufsave;
    /* insert the login name */
    printf(_("Create a new master account\n\n"));
    printf(_("Type the login (max. 8): "));
    fgets(name,LOGIN_LENGTH,stdin);

    trim(name);
    StrToLower(name);

    /* check loging */
    if (!NickStringCheck(name) || !strlen(name)) {
        fprintf(stderr,_("Forbidden characters used.\n"));
        return false;
    }
    /**************************************/
    /* getting the password		  */
    /* get term-attribut */
    if (tcgetattr(0, &stBuf) == -1)
	fprintf(stderr,"tcgetattr");
    /* save option */
    stBufsave=stBuf;
    /* disable echo */
    stBuf.c_lflag &= ~(ECHO);
    
    /* set attributs */
    if (tcsetattr(0, TCSADRAIN, &stBuf) == -1)
        fprintf(stderr,"tcsetattr");
    
    /* insert the password */
    printf(_("Enter the password: "));
    fgets(passwd,PASSWD_LENGTH+1,stdin);
    printf(_("\nReenter the password: "));
    fgets(repasswd,PASSWD_LENGTH+1,stdin);
    
    /* set save attributs to terminal */
    if (tcsetattr(0, TCSADRAIN, &stBufsave) == -1)
        fprintf(stderr,"tcsetattr");
    /* empty lines to screen */
    printf("\n\n");
		      
    
    /* check the password */
    if (strcmp(passwd,repasswd)) {
        fprintf(stderr,_("Typing error... passwords not equal\n"));
        return false;
    } else if (strpbrk(passwd," \t")) {
        fprintf(stderr,_("Forbidden characters used.\n"));
        return false;
    }

    trim(passwd);

    /* create account */
    if (!add_db(USER_DB,name,passwd)) {
        fprintf(stderr,_("A account %s already exists."),name);
        fprintf(stderr,"\n");
        return false;
    }

    /* make him to the master */
    if (!add_db(ACCESS_DB,name,"+m")) {
        del_db(USER_DB,name);
        fprintf(stderr,_("Couldn't set this account to master."));
        return false;
    }

    return true;
}

