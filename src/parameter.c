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

#include "config.h"
#include "irc.h"
#include "utilities.h"
#include "dbaccess.h"

#define	CMD_MSG
#include "messages.h"
#undef CMD_MSG

#include "parameter.h"
// ############################################################################# 				  
void cmd_line(int argc,const char *argv[]) {
	extern ConfType setup;
	int i;
	int tmp;

    
	for (i=1;i<argc;i++) {
	   	if (argv[i][0]==PARAMETER_CHAR) {
			switch	(argv[i][1]) {
			case 'v':
				exit(0);
                break;
			case 'h':
			case '?':
				print_msg(help_msg);
				break;
			case 's':
				DEBUG("Found server option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
			   
				// set the server name
				if (strpbrk(argv[i],NOT_ALLOW_CHAR)) {
					errno=EINVAL;
					perror(ERR_WRONG_SERVERNAME);
					exit(errno);
				}
			   
				setup.server=(char *)malloc(strlen((argv[i])+1)*sizeof(char));
				strcpy(setup.server,argv[i]);
                
				break;
			case 'b':
				DEBUG("Found botname option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				
				// set the botname
				if ((strpbrk(argv[i],USER_NOT_ALLOW_CHAR)) && isalpha(argv[i][0])) {
					errno=EINVAL;
					perror(ERR_WRONG_BOTNAME);
					exit(errno);
				}
                
				setup.botname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(setup.botname,argv[i]);
                
				break;
			case 'r':
				DEBUG("Found realname option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
                }
				
				// set realname
				setup.realname=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(setup.realname,argv[i]);
				
				break;
			case 'p':
				DEBUG("Found port option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				
				// set port
				if ((atoi(argv[i])<1) || (atoi(argv[i])>65535)) {
					errno=EDOM;
					perror(ERR_PORT_PARAMETER);
					exit(errno);
				}
				
				setup.port=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(setup.port,argv[i]);
	
				break;
			case 't':
				DEBUG("Found thread limit option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				
				// set threadlimit
				tmp=atoi(argv[i]);
				if ((tmp<1) || (tmp>MAX_THREADS_LIMIT)) {
					errno=EDOM;
					perror(ERR_THREAD_RANGE);
					exit(errno);
				}
				setup.thread_limit=tmp;
				break;
			case 'a':
				DEBUG("Found auto log off time option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				
				// set auto log off time
				tmp=atoi(argv[i]);
				if (tmp<MIN_LOGOFF) {
					errno=EDOM;
					perror(ERR_LOGOFF_RANGE);
					exit(errno);
				}
				setup.AutoLoggoff=tmp;
				break;
			case 'n':
				DEBUG("Found sending delay option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				
				// set sending delay 
				tmp=atoi(argv[i]);
				if (tmp<0) {
					errno=EDOM;
					perror(ERR_SENDDELAY_RANGE);
					exit(errno);
				}
				setup.sendDelay=tmp;
				break;
			case 'k':
				DEBUG("Found account live time option");
											
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				
				// set account live time
				tmp=atoi(argv[i]);
				if (tmp<MIN_ALT) {
					errno=EDOM;
					perror(ERR_ALT_RANGE);
					exit(errno);
				}
				setup.AccountLiveTime=tmp;
				break;
			case 'f':
				DEBUG("Found config file option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
					break;
				}
				
				// set  path to config file
				setup.configfile=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(setup.configfile,argv[i]);
				break;
			case 'm':
				DEBUG("Found bot master option");
				setup.newMaster=true;
				break;
			case 'd':
				DEBUG("Found  database path option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				// set database path
				setup.database_path=(char *)malloc((strlen(argv[i])+1)*sizeof(char));
				strcpy(setup.database_path,argv[i]);
				break;
			default:
				print_msg(unknow_parameter);
				break;
			}
		}
	}
}
// ############################################################################# 
void read_config_file(void) {
	FILE *fd;
	extern ConfType setup;
	int tmp;
	char buffer[MAX_READ_BUFFER_SIZE], *c,*value,*key;
	errno=0;

	if ((fd=fopen(setup.configfile,"r"))==NULL) {
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
			if ((!strcmp(key,KEY_SERVER)) && (setup.server==NULL)) {
				if (strpbrk(value,NOT_ALLOW_CHAR)) {
					errno=EINVAL;
					perror(ERR_WRONG_SERVERNAME);
					exit(errno);
				}
				// set servername
				setup.server=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(setup.server,value);
			} else if ((!strcmp(key,KEY_PORT)) && (setup.port==NULL)) {
				if ((atoi(value)<1) || (atoi(value)>65535)) {
					errno=EINVAL;
					perror(ERR_PORT_PARAMETER);
					exit(errno);
				}
				// set port
				setup.port=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(setup.port,value);
			} else if ((!strcmp(key,KEY_BOTNAME)) && (setup.botname==NULL )) {
				if (strpbrk(value,USER_NOT_ALLOW_CHAR)) {
					errno=EINVAL;
					perror(ERR_WRONG_BOTNAME);
					exit(errno);
				}
				// set botname
				setup.botname=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(setup.botname,value);
			} else if ((!strcmp(key,KEY_REALNAME)) && (setup.realname==NULL)) {
				// ser realname
				setup.realname=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(setup.realname,value);
			} else if ((!strcmp(key,KEY_THREADLIMIT))&& (setup.thread_limit<=0)) {
				tmp=atoi(value);
				if ((tmp<=0) || (tmp>MAX_THREADS_LIMIT)) {
					errno=EDOM;
					perror(ERR_THREAD_RANGE);
					exit(errno);
				}	
				// set thread limit
				setup.thread_limit=tmp;
			} else  if ((!strcmp(key,KEY_DATABASEPATH))&& (setup.database_path==NULL)) {
				// set database path
				setup.database_path=(char *)malloc((strlen(value)+1)*sizeof(char));
				strcpy(setup.database_path,value);
            } else if ((!strcmp(key,KEY_AUTOLOGOFF))&& (setup.AutoLoggoff==MIN_LOGOFF)) {
				tmp=atoi(value);
				if (tmp<MIN_LOGOFF) {
					errno=EDOM;
					perror(ERR_LOGOFF_RANGE);
					exit(errno);
				}	
				// set auto log off time
				setup.AutoLoggoff=tmp;
			} else if ((!strcmp(key,KEY_SENDDELAY))&& (setup.sendDelay<0)) {
				tmp=atoi(value);
				if (tmp<0) {
					errno=EDOM;
					perror(ERR_SENDDELAY_RANGE);
					exit(errno);
				}	
				// set auto log off time
				setup.sendDelay=tmp;
			} else if ((!strcmp(key,KEY_ALT))&& (setup.AccountLiveTime==MIN_ALT)) {
				tmp=atoi(value);
				if (tmp<0) {
					errno=EDOM;
					perror(ERR_ALT_RANGE);
					exit(errno);
				}	
				// set account live time
				setup.AccountLiveTime=tmp;
			}
			free(value);
			free(key);
		}
	}
	fclose(fd);
}

// ############################################################################# 
boolean dialogMaster(void){
	char  name[9],passwd[PASSWD_LENGTH+1],repasswd[PASSWD_LENGTH+1];

	// insert the login name
	printf(MSG_MASTER_TITLE);
	printf(MSG_MASTER_LOGIN);
	fgets(name,8,stdin);

	trim(name);
	StrToLower(name);

	// check loging
	if (strpbrk(name,NOT_ALLOW_CHAR) || !strlen(name)) {
		fprintf(stderr,ERR_NOT_ALLOW_CHAR);
		fprintf(stderr,"\n");
		return false;
	}


	// insert the password
	printf(MSG_MASTER_PASS);
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


	if (!add_db(ACCESS_DB,name,"+ov")) {
		del_db(USER_DB,name);
		fprintf(stderr,MSG_MASTER_ERR);
		fprintf(stderr,"\n");
		return false;
	}

	return true;
}

