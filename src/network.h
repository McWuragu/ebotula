/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef NETWORK_H
#define NETWORK_H

#include "command.h"

#define RECV_BUFFER_SIZE		0x1000
#define HOSTNAME_BUFFER_SIZE	0xFF

typedef struct MsgBufStruct {
	long	mtype;
	CmdType	identify;
	char	msg_line[RECV_BUFFER_SIZE];
} MsgBufType; 


/// connect to the server
void connectServer(void);
/// close connection to the server
void disconnectServer(void);
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
MsgBufType preParser(char *line);

/**
* the function for  the  thread
* @param argv a pointer  of arguements for  the
* starting of  this thread
*/
void *action_thread(void *argv);





#endif
