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



#define RECV_BUFFER_SIZE		0x1000
#define HOSTNAME_BUFFER_SIZE	0xFF



#define CMD_PING				0x01
#define CMD_HELP				0x02
#define CMD_VERSION				0x03
#define CMD_HELLO				0x04
#define CMD_PASS				0x05
#define CMD_IDENT				0x06
#define CMD_ADDCHANNEL			0x07
#define CMD_JOIN				0x08
#define CMD_PART				0x09
#define CMD_RMCHANNEL			0x0A
#define CMD_LOGOFF				0x0B
#define CMD_DIE					0x0C
#define CMD_NICK				0x0D
#define CMD_CHANNELS			0x0E
#define CMD_NAMES				0x0F
#define CMD_JOIN_GREATING		0x10
#define CMD_GREATING			0x11

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
