#ifndef PARAMETER_H
#define PARAMETER_H


#define	PARAMETER_CHAR	'-'
#define COMMENT_CHAR	'#'
#define MAX_READ_BUFFER_SIZE 	256

#define KEY_SERVER			"servername"
#define KEY_PORT			"port"
#define KEY_BOTNAME			"botname"
#define KEY_REALNAME		"realname"
#define KEY_THREADLIMIT		"threadlimit"
#define	KEY_DATABASEPATH	"databasepath"


void cmd_line(int argc,const char *argv[]);
void read_config_file(void);
void print_msg(const char *msg[]);

#endif
