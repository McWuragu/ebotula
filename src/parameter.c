#include "config.h"
#include "macro.h"
#include "dbaccess.h"
#include "ircbot.h"

#define	CMD_MSG
#include "messages.h"
#undef CMD_MSG

#include "parameter.h"
				  
void cmd_line(int argc,const char *argv[]) {
	extern CONFIG_TYPE setup;
	int i;
	int thread_limit;

    
	for (i=1;i<argc;i++) {
	   	if (argv[i][0]==PARAMETER_CHAR) {
			switch	(argv[i][1]) {
			case 'v':
				print_msg(version_msg);
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
			   
				setup.server=malloc(sizeof(char)*(strlen(argv[i])+1));
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
				if (strpbrk(argv[i],NOT_ALLOW_CHAR)) {
					errno=EINVAL;
					perror(ERR_WRONG_BOTNAME);
					exit(errno);
				}
                
				setup.botname=malloc(sizeof(char)*(strlen(argv[i])+1));
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
				setup.realname=malloc(sizeof(char)*(strlen(argv[i])+1));
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
					errno=ERANGE;
					perror(ERR_PORT_PARAMETER);
					exit(errno);
				}
				
				setup.port=malloc(sizeof(char)*(strlen(argv[i])+1));
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
				thread_limit=atoi(argv[i]);
				if ((thread_limit<1) || (thread_limit>MAX_THREADS_LIMIT)) {
					errno=ERANGE;
					perror(ERR_THREAD_RANGE);
					exit(errno);
				}
				setup.thread_limit=thread_limit;
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
				setup.configfile=malloc(sizeof(char)*(strlen(argv[i])+1));
				strcpy(setup.configfile,argv[i]);
				break;
			case 'm':
				DEBUG("Found bot master option");
				setup.createMaster=true;
				break;
			case 'd':
				DEBUG("Found  database path option");
				if (++i>=argc) {
					errno=EINVAL;
					perror(ERR_MSSING_PARA);
					exit(errno);
				}
				// set database path
				setup.database_path=malloc(sizeof(char)*(strlen(argv[i])+1));
				strcpy(setup.database_path,argv[i]);
				break;
			default:
				print_msg(unknow_parameter);
				break;
			}
		}
	}
}

void read_config_file(void) {
	FILE *fd;
	extern CONFIG_TYPE setup;
	char buffer[MAX_READ_BUFFER_SIZE], *c,*value,*key;
	errno=0;

	if ((fd=fopen(setup.configfile,"r"))==NULL) {
		perror(ERR_CONFIG_FILE);
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
			value=malloc(sizeof(char)*strlen(c));
			strcpy(value,c+1);
			
			// parse the key from the line
			key=malloc(sizeof(char)*(strlen(buffer)-strlen(c)+1));
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
				setup.server=malloc(sizeof(char)*(strlen(value)+1));
				strcpy(setup.server,value);
			} else if ((!strcmp(key,KEY_PORT)) && (setup.port==NULL)) {
				if ((atoi(value)<1) || (atoi(value)>65535)) {
					errno=EINVAL;
					perror(ERR_PORT_PARAMETER);
					exit(errno);
				}
				// set port
				setup.port=malloc(sizeof(char)*(strlen(value)+1));
				strcpy(setup.port,value);
			} else if ((!strcmp(key,KEY_BOTNAME)) && (setup.botname==NULL )) {
				if (strpbrk(value,NOT_ALLOW_CHAR)) {
					errno=EINVAL;
					perror(ERR_WRONG_BOTNAME);
					exit(errno);
				}
				// set botname
				setup.botname=malloc(sizeof(char)*(strlen(value)+1));
				strcpy(setup.botname,value);
			} else if ((!strcmp(key,KEY_REALNAME)) && (setup.realname==NULL)) {
				// ser realname
				setup.realname=malloc(sizeof(char)*(strlen(value)+1));
				strcpy(setup.realname,value);
			} else if ((!strcmp(key,KEY_THREADLIMIT))&& (setup.thread_limit<=0)) {
				int thread_limit=atoi(value);
				if ((thread_limit<=0) || (thread_limit>MAX_THREADS_LIMIT)) {
					errno=ERANGE;
					perror(ERR_THREAD_RANGE);
					exit(errno);
				}	
				// set thread limit
				setup.thread_limit=thread_limit;
			} else  if ((!strcmp(key,KEY_DATABASEPATH))&& (setup.database_path==NULL)) {
				// set database path
				setup.database_path=malloc(sizeof(char)*(strlen(value)+1));
				strcpy(setup.database_path,value);
            }
			free(value);
			free(key);
		}
	}
	fclose(fd);
}



void print_msg(const char *msg[]) {
    int i;
	for (i=0;msg[i][0]!=EOM;i++) {
		printf("%s",msg[i]);
	}
	exit(true);
}
