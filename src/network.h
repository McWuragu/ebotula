/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#ifndef NETWORK_H
#define NETWORK_H

#include "type.h"
/** the size of  the receive buffer */
#define RECV_BUFFER_SIZE        1<<10
/** the size of the buffer for the  hostname */
#define HOSTNAME_BUFFER_SIZE    0xFF

boolean ConnectToIrc(void);
void * JoinAllChannelsThread(void*);



/* connect to the server */
boolean connectServer(void);
/* close connection to the server */
void disconnectServer(void);

/**
* send a command string to the server
* @param pLine the  commando string whiche  want  to send
*/
void SendLine(char* pMsg);

/**
* reveive  oneline from the server
* @param pLine a pointer of a buffer for the  receiving string
* @param len the  size of the buffer
*/
void RecvLine(char *pLine,unsigned int len);





#endif
