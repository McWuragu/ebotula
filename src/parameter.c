/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <syslog.h>

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include "ircbot.h"
#include "irc.h"
#include "utilities.h"
#include "dbaccess.h"

#define	CMD_MSG
#include "messages.h"
#undef CMD_MSG

#include "parameter.h"
// ############################################################################# 				  
void ComandLineParser(int argc,char *const argv[]) {
	extern ConfType sSetup;
	int i;
	int tmp;

    
	for (i=1;i<argc;i++) {
	   	if (argv[i][0]==PARAMETER_CHAR) {
			switch	(argv[i][1]) {
			case 's':
				DEBUG("Found server option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
			   
				// set the server name
				if (strpbrk(argv[i],SERVER_NOT_ALLOW_CHAR)) {
					errno=EINVAL;
					perror(ERR_WRONG_SERVERNAME);
					exit(errno);
				}
			   
				sSetup.server=(char *)malloc(strlen((argv[i])+1)*sizeof(char));
				strcpy(sSetup.server,argv[i]);
                
				break;
			case 'b':
				DEBUG("Found botname option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
				
				// set the botname
				if (!NickStringCheck(argv[i])) {
					errno=EINVAL;
					perror(ERR_WRONG_BOTNAME);
					exit(errno);
				}
                
				sSetup.botname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(sSetup.botname,argv[i]);
                
				break;
			case 'r':
				DEBUG("Found realname option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
                }
				
				// set realname
				sSetup.realname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(sSetup.realname,argv[i]);
				
				break;
			case 'p':
				DEBUG("Found port option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
				
				// set port
				if ((atoi(argv[i])<1) || (atoi(argv[i])>65535)) {
					errno=EDOM;
					perror(ERR_PORT_PARAMETER);
					exit(errno);
				}
				
				sSetup.port=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(sSetup.port,argv[i]);
	
				break;
			case 't':
				DEBUG("Found thread limit option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
				
				// set threadlimit
				tmp=atoi(argv[i]);
				if ((tmp<1) || (tmp>MAX_THREADS_LIMIT)) {
					errno=EDOM;
					perror(ERR_THREAD_RANGE);
					exit(errno);
				}
				sSetup.thread_limit=tmp;
				break;
			case 'a':
				DEBUG("Found auto logoff time option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
				
				// set auto logoff time
				tmp=atoi(argv[i]);
				if (tmp<MIN_LOGOFF) {
					errno=EDOM;
					perror(ERR_LOGOFF_RANGE);
					exit(errno);
				}
				sSetup.AutoLoggoff=tmp;
				break;
			case 'n':
				DEBUG("Found sending delay option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
				
				// set sending delay 
				tmp=atoi(argv[i]);
				if (tmp<=0) {
					errno=EDOM;
					perror(ERR_SENDDELAY_RANGE);
					exit(errno);
				}
				sSetup.sendDelay=tmp;
				break;
			case 'k':
				DEBUG("Found account live time option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
				
				// set account live time
				tmp=atoi(argv[i]);
				if (tmp<MIN_ALT) {
					errno=EDOM;
					perror(ERR_ALT_RANGE);
					exit(errno);
				}
				sSetup.AccountLiveTime=tmp;
				break;
			case 'm':
				DEBUG("Found bot master option");
				sSetup.newMaster=true;
				break;
			case 'd':
				DEBUG("Found  database path option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}
				// set database path
				sSetup.pDatabasePath=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(sSetup.pDatabasePath,argv[i]);
				break;
			case 'c':
				DEBUG("Found ping timeout option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MISSING_PARAM);
					exit(errno);
				}

				// set the limit for ping timeout in s
				tmp=atoi(argv[i]);
				if (tmp<MIN_PINGTIME) {
					errno=EDOM;
					perror(ERR_PINGTIMEOUT_RANGE);
					exit(errno);
				}
				sSetup.iTimeout=tmp;
				break;
			default:
				printMsg(unknow_parameter);
				break;
			}
		}
	}
}
// ############################################################################# 
void ConfigFileParser(void) {
	FILE *fd;
	extern ConfType sSetup;
	int tmp;
	char buffer[MAX_READ_BUFFER_SIZE], *c,*value,*key;
	errno=0;

	if ((fd=fopen(sSetup.configfile,"r"))==NULL) {
		perror(SYSLOG_CONFIG_FILE);
		syslog(LOG_ERR,SYSLOG_CONFIG_FILE);
        exit(errno);
	}

	DEBUG("Config file is open");

	while((fgets(buffer,MAX_READ_BUFFER_SIZE,fd)!=NULL) && (errno==0)){
		// remove newline and leading spaces
		trim(buffer);
		clearspace(buffer);
		
		// ignore space lines and comments
		if ((buffer[0]!=COMMENT_CHAR) && (buffer[0]!='\0')) {
			DEBUG("Found config line %s",buffer);
			

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
					perror(ERR_WRONG_SERVERNAME);
					exit(errno);
				}
				// set servername
				sSetup.server=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(sSetup.server,value);
			} else if (!strcmp(key,KEY_PORT)) {
				if ((atoi(value)<1) || (atoi(value)>65535)) {
					errno=EINVAL;
					perror(ERR_PORT_PARAMETER);
					exit(errno);
				}
				// set port
				sSetup.port=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(sSetup.port,value);
			} else if (!strcmp(key,KEY_BOTNAME)) {
				if (!NickStringCheck(value)) {
					errno=EINVAL;
					perror(ERR_WRONG_BOTNAME);
					exit(errno);
				}
				// set botname
				free(sSetup.botname);
				sSetup.botname=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(sSetup.botname,value);
			} else if (!strcmp(key,KEY_REALNAME)) {
				// ser realname
				free(sSetup.realname);
				sSetup.realname=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(sSetup.realname,value);
			} else if (!strcmp(key,KEY_THREADLIMIT)) {
				tmp=atoi(value);
				if ((tmp<=0) || (tmp>MAX_THREADS_LIMIT)) {
					errno=EDOM;
					perror(ERR_THREAD_RANGE);
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
					perror(ERR_LOGOFF_RANGE);
					exit(errno);
				}	
				// set auto logoff time
				sSetup.AutoLoggoff=tmp;
			} else if (!strcmp(key,KEY_SENDDELAY)) {
				tmp=atoi(value);
				if (tmp<=0) {
					errno=EDOM;
					perror(ERR_SENDDELAY_RANGE);
					exit(errno);
				}	
				// set auto logoff time
				sSetup.sendDelay=tmp;
			} else if (!strcmp(key,KEY_ALT)) {
				tmp=atoi(value);
				if (tmp<0) {
					errno=EDOM;
					perror(ERR_ALT_RANGE);
					exit(errno);
				}	
				// set account live time
				sSetup.AccountLiveTime=tmp;
			} else if (!strcmp(key,KEY_PINGTIMEOUT)) {
				tmp=atoi(value);
				if (tmp<=0) {
					errno=EDOM;
					perror(ERR_PINGTIMEOUT_RANGE);
					exit(errno);
				}	
				// set account live time
				sSetup.iTimeout=tmp;
			}
			free(value);
			free(key);
		}
	}
	fclose(fd);
}

// ############################################################################# 
boolean dialogMaster(void){
	char  name[LOGIN_LENGTH+1],passwd[PASSWD_LENGTH+1],repasswd[PASSWD_LENGTH+1];

	// insert the login name
	printf(MSG_MASTER_TITLE);
	printf(MSG_MASTER_LOGIN,LOGIN_LENGTH);
	fgets(name,LOGIN_LENGTH,stdin);

	trim(name);
	StrToLower(name);

	// check loging
	if (NickStringCheck(name) || !strlen(name)) {
		fprintf(stderr,ERR_NOT_ALLOW_CHAR);
		fprintf(stderr,"\n");
		return false;
	}


	// insert the password
	printf(MSG_MASTER_PASS,PASSWD_LENGTH);
	fgets(passwd,PASSWD_LENGTH+1,stdin);
	printf(MSG_MASTER_REPASS);
    fgets(repasswd,PASSWD_LENGTH+1,stdin);

	// check the password
	if (strcmp(passwd,repasswd)) {
		fprintf(stderr,MSG_MASTER_PASS_ERR);
		fprintf(stderr,"\n");
        return false;
	} else if (strpbrk(passwd," \t")) {
		fprintf(stderr,ERR_NOT_ALLOW_CHAR);
		fprintf(stderr,"\n");
		return false;
	}

	trim(passwd);

	// create account
	if (!add_db(USER_DB,name,passwd)) {
		fprintf(stderr,MSG_NICK_EXIST);
		fprintf(stderr,"\n");
		return false;
	}


	if (!add_db(ACCESS_DB,name,"+m")) {
		del_db(USER_DB,name);
		fprintf(stderr,MSG_MASTER_ERR);
		fprintf(stderr,"\n");
		return false;
	}

	return true;
}

