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


void irc_connect(void);
void join_all_channels(void);

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






#endif
