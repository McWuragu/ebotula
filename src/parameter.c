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
#include <syslog.h>
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
// #############################################################################                  
void CommandLineParser(int argc,char *const argv[]) {
    extern ConfigSetup_t sSetup;
    int i;
    int tmp;

    
    for (i=1;i<argc;i++) {
        if (argv[i][0]==PARAMETER_CHAR) {
            switch  (argv[i][1]) {
            case 's':
                DEBUG("Found server option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
               
                // set the server name
                if (strpbrk(argv[i],SERVER_NOT_ALLOW_CHAR)) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_WRONG_SERVERNAME));
                    exit(errno);
                }
               
                sSetup.server=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.server,argv[i]);
                
                break;
            case 'b':
                DEBUG("Found botname option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set the pBotname
                if (!NickStringCheck(argv[i])) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_WRONG_BOTNAME));
                    exit(errno);
                }
                
                sSetup.pBotname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.pBotname,argv[i]);
                
                break;
            case 'u':
                DEBUG("Found execution user option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set the user
                sSetup.sExeUser=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.sExeUser,argv[i]);
                
                break;
            case 'g':
                DEBUG("Found execution group option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set the group
                sSetup.sExeGroup=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.sExeGroup,argv[i]);
                
                break;
            case 'r':
                DEBUG("Found realname option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set realname
                sSetup.realname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.realname,argv[i]);
                
                break;
            case 'p':
                DEBUG("Found port option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set port
                if ((atoi(argv[i])<1) || (atoi(argv[i])>65535)) {
                    errno=EDOM;
                    perror(getMsgString(ERR_PORT_PARAMETER));
                    exit(errno);
                }
                
                free(sSetup.port);
                sSetup.port=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.port,argv[i]);
    
                break;
            case 't':
                DEBUG("Found thread limit option\n");
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set threadlimit
                tmp=atoi(argv[i]);
                if ((tmp<1) || (tmp>MAX_THREADS_LIMIT)) {
                    errno=EDOM;
                    perror(getMsgString(ERR_THREAD_RANGE));
                    exit(errno);
                }
                sSetup.thread_limit=tmp;
                break;
            case 'a':
                DEBUG("Found auto logoff time option\n");
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set auto logoff time
                tmp=atoi(argv[i]);
                if (tmp<MIN_LOGOFF) {
                    errno=EDOM;
                    perror(getMsgString(ERR_LOGOFF_RANGE));
                    exit(errno);
                }
                sSetup.AutoLoggoff=tmp;
                break;
            case 'n':
                DEBUG("Found first sending delay option\n");
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set sending delay 
                tmp=atoi(argv[i]);
                if (tmp<=0) {
                    errno=EDOM;
                    perror(getMsgString(ERR_SENDDELAY_RANGE));
                    exit(errno);
                }
                sSetup.iSendDelay=tmp;
                break;
            case 'e':
                DEBUG("Found secondary sending delay option\n");
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set sending delay 
                tmp=atoi(argv[i]);
                if (tmp<=0) {
                    errno=EDOM;
                    perror(getMsgString(ERR_SENDDELAY_RANGE));
                    exit(errno);
                }
                sSetup.nSlowSendDelay=tmp;
                break;
            case 'l':
                DEBUG("Found send line limit option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }

                // linie limit  for the first send delay
                tmp=atoi(argv[i]);
                if (tmp<0) {
                    errno=EDOM;
                    perror(getMsgString(ERR_SENDLINE_RANGE));
                    exit(errno);
                }
                sSetup.nFastSendingCharLimit=tmp;
                break;
            case 'k':
                DEBUG("Found account live time option\n");
                                            
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                
                // set account live time
                tmp=atoi(argv[i]);
                if (tmp<MIN_ALT) {
                    errno=EDOM;
                    perror(getMsgString(ERR_ALT_RANGE));
                    exit(errno);
                }
                sSetup.AccountLiveTime=tmp;
                break;
            case 'm':
                DEBUG("Found bot master option\n");
                sSetup.newMaster=true;
                break;
            case 'd':
                DEBUG("Found database path option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }
                // set database path
                sSetup.pDatabasePath=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
                strcpy(sSetup.pDatabasePath,argv[i]);
                break;
            case 'c':
                DEBUG("Found ping timeout option\n");
                if (++i>=argc) {
                    errno=EINVAL;
                    perror(getMsgString(ERR_MISSING_PARAM));
                    exit(errno);
                }

                // set the limit for ping timeout in s
                tmp=atoi(argv[i]);
                if (tmp<MIN_PINGTIME) {
                    errno=EDOM;
                    perror(getMsgString(ERR_PINGTIMEOUT_RANGE));
                    exit(errno);
                }
                sSetup.iTimeout=tmp;
                break;
	    case 'f':
            /* Dummy, Configfile allready parsed */
            i++;
            break;
        default:
                printf("%s ",argv[i]);
                printMsg(getCmdLineError());
                exit(-1);
                break;
            }
        } else {
                printf("%s ",argv[i]);
                printMsg(getCmdLineError());
                exit(-1);
        }
    }
}
// ############################################################################# 
void ConfigFileParser(void) {
    FILE *fd;
    extern ConfigSetup_t sSetup;
    int tmp;
    char buffer[MAX_READ_BUFFER_SIZE], *c,*value,*key;
    //errno=0;

    if (!(fd=fopen(sSetup.configfile,"r"))) {
	    // generating basicconfig for ebotula
	    DEBUG("Creating Configfile: %s\n",sSetup.configfile);
		write_baseconfig(sSetup.configfile);
    }else {
    
        DEBUG("Config file is open\n");
    
        while((fgets(buffer,MAX_READ_BUFFER_SIZE,fd)!=NULL) && (errno==0)){
            // remove newline and leading spaces
            trim(buffer);
            clearspace(buffer);
            
            // ignore space lines and comments
            if ((buffer[0]!=COMMENT_CHAR) && (buffer[0]!='\0')) {
                DEBUG("Found config line %s\n",buffer);
                
    
                c=strchr(buffer,'=');
                
                // parse the value from the line
                value=(char *)malloc(strlen(c)*sizeof(char));
                strcpy(value,c+1);
                
                // parse the key from the line
                key=(char *)malloc((strlen(buffer)-strlen(c)+1)*sizeof(char));
                strtok(buffer,"=");
                strcpy(key,buffer);
                
                // set  the  reading values
                if (!strcmp(key,KEY_SERVER)) {
                    if (strpbrk(value,SERVER_NOT_ALLOW_CHAR)) {
                        errno=EINVAL;
                        perror(getMsgString(ERR_WRONG_SERVERNAME));
                        exit(errno);
                    }
                    // set servername
                    free(sSetup.server);
                    sSetup.server=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.server,value);
                } else if (!strcmp(key,KEY_PORT)) {
                    if ((atoi(value)<1) || (atoi(value)>65535)) {
                        errno=EINVAL;
                        perror(getMsgString(ERR_PORT_PARAMETER));
                        exit(errno);
                    }
                    // set port
                    free(sSetup.port);
                    sSetup.port=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.port,value);
                } else if (!strcmp(key,KEY_BOTNAME)) {
                    if (!NickStringCheck(value)) {
                        errno=EINVAL;
                        perror(getMsgString(ERR_WRONG_BOTNAME));
                        exit(errno);
                    }
                    // set pBotname
                    free(sSetup.pBotname);
                    sSetup.pBotname=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.pBotname,value);
                } else if (!strcmp(key,KEY_REALNAME)) {
                    // ser realname
                    free(sSetup.realname);
                    sSetup.realname=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.realname,value);
                } else if (!strcmp(key,KEY_THREADLIMIT)) {
                    tmp=atoi(value);
                    if ((tmp<=0) || (tmp>MAX_THREADS_LIMIT)) {
                        errno=EDOM;
                        perror(getMsgString(ERR_THREAD_RANGE));
                        exit(errno);
                    }   
                    // set thread limit
                    sSetup.thread_limit=tmp;
                } else  if (!strcmp(key,KEY_DATABASEPATH)) {
                    // set database path
                    free(sSetup.pDatabasePath);
                    sSetup.pDatabasePath=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.pDatabasePath,value);
                } else if (!strcmp(key,KEY_AUTOLOGOFF)) {
                    tmp=atoi(value);
                    if (tmp<=0) {
                        errno=EDOM;
                        perror(getMsgString(ERR_LOGOFF_RANGE));
                        exit(errno);
                    }   
                    // set auto logoff time
                    sSetup.AutoLoggoff=tmp;
                } else if (!strcmp(key,KEY_FASTSENDDELAY)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        perror(getMsgString(ERR_SENDDELAY_RANGE));
                        exit(errno);
                    }   
                    // set first delay time
                    sSetup.iSendDelay=tmp;
                } else if (!strcmp(key,KEY_SLOWSENDDELAY)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        perror(getMsgString(ERR_SENDDELAY_RANGE));
                        exit(errno);
                    }   
                    // set second delay time
                    sSetup.nSlowSendDelay=tmp;
                } else if (!strcmp(key,KEY_FASTSENDLIMIT)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        perror(getMsgString(ERR_SENDLINE_RANGE));
                        exit(errno);
                    }   
                    // set auto logoff time
                    sSetup.nFastSendingCharLimit=tmp;
                } else if (!strcmp(key,KEY_ALT)) {
                    tmp=atoi(value);
                    if (tmp<0) {
                        errno=EDOM;
                        perror(getMsgString(ERR_ALT_RANGE));
                        exit(errno);
                    }   
                    // set account live time
                    sSetup.AccountLiveTime=tmp;
                } else if (!strcmp(key,KEY_PINGTIMEOUT)) {
                    tmp=atoi(value);
                    if (tmp<=0) {
                        errno=EDOM;
                        perror(getMsgString(ERR_PINGTIMEOUT_RANGE));
                        exit(errno);
                    }   
                    // set account live time
                    sSetup.iTimeout=tmp;
                }else if (!strcmp(key,KEY_EXEUSER)) {
                    // set execution rights
                    if (sSetup.sExeUser)free(sSetup.sExeUser);
                    sSetup.sExeUser=(char *)malloc((strlen(value)+1)*sizeof(char));
                    strcpy(sSetup.sExeUser,value);
                }else if (!strcmp(key,KEY_EXEGROUP)) {
                    // set execution rights
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
    printf("%s\n\n",getMsgString(INFO_MASTER_TITLE));
    printf("%s",getMsgString(INFO_MASTER_LOGIN));
    fgets(name,LOGIN_LENGTH,stdin);

    trim(name);
    StrToLower(name);

    /* check loging */
    if (!NickStringCheck(name) || !strlen(name)) {
        fprintf(stderr,"%s\n",getMsgString(ERR_NOT_ALLOW_CHAR));
        return false;
    }
    /**************************************/
    /* getting the password		  */
    /* get term-attribut */
    if (tcgetattr(0, &stBuf) == -1)
	perror("tcgetattr");
    /* save option */
    stBufsave=stBuf;
    /* disable echo */
    stBuf.c_lflag &= ~(ECHO);
    /* set attributs */
    if (tcsetattr(0, TCSADRAIN, &stBuf) == -1)
        perror("tcsetattr");
    /* insert the password */
    printf("%s",getMsgString(INFO_MASTER_PASS));
    fgets(passwd,PASSWD_LENGTH+1,stdin);
    printf("\n%s",getMsgString(INFO_MASTER_REPASS));
    fgets(repasswd,PASSWD_LENGTH+1,stdin);
    /* set save attributs to terminal */
    if (tcsetattr(0, TCSADRAIN, &stBufsave) == -1)
        perror("tcsetattr");
    /* empty lines to screen */
    printf("\n\n");
		      
    
    /* check the password */
    if (strcmp(passwd,repasswd)) {
        fprintf(stderr,"%s\n",getMsgString(ERR_MASTER_PASS));
        return false;
    } else if (strpbrk(passwd," \t")) {
        fprintf(stderr,"%s\n",getMsgString(ERR_NOT_ALLOW_CHAR));
        return false;
    }

    trim(passwd);

    // create account
    if (!add_db(USER_DB,name,passwd)) {
        fprintf(stderr,"%s\n",getMsgString(ERR_NICK_EXIST));
        return false;
    }

    // make him to the master
    if (!add_db(ACCESS_DB,name,"+m")) {
        del_db(USER_DB,name);
        fprintf(stderr,"%s\n",getMsgString(ERR_MASTER));
        return false;
    }

    return true;
}

