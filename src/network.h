#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>			 
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define RECV_BUFFER_SIZE		1512
#define HOSTNAME_BUFFER_SIZE	256



#define CMD_PING				1
#define CMD_HELP				2
#define CMD_VERSION				3
#define CMD_HELLO				4
#define CMD_PASS				5
#define CMD_IDENT				6
#define CMD_ADDCHANNEL			7
#define CMD_JOIN				8
#define CMD_PART				9
#define CMD_RMCHANNEL			10
#define CMD_LOGOFF				11
#define CMD_DIE					12

/// connect to the server
void connectServer(void);
/**
* send a command string to the server
* @param line the  commando string whiche  want  to send
*/
void send_line(char *line);

/**
* reveive  oneline from the server
* @param line a pointer of a buffer for the  receiving string
* @param len the  size of the buffer
*/
void recv_line(char *line,unsigned int len);

/**
* This parse a line whiche was receive by the  client
* @param line  a pointer of a string
* @return a struct withe the command id, the type of 
* message and a string as  argument for the  command
*/
struct MSGBUF_DS preParser(char *line);

/**
* the function for  the  thread
* @param argv a pointer  of arguements for  the
* starting of  this thread
*/
void *action_thread(void *argv);



struct MSGBUF_DS {
	long	mtype;
	int identify;
	char	msg_line[RECV_BUFFER_SIZE];
};

#endif
