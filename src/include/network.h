/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2023 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
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
