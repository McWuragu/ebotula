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

#ifndef TIMING_H
#define TIMING_H

/**
* This is  the function for the timing thread. She handle all task which need a timing
* synchronization. This task are ping the server, remove dead accounts, automatic logoff
* and reset the ban mask after the time ban.
* @param argv is a pointer for any parameters This is unused but necessary for  pthread_create()
* @return a pointer of any return values. This is unused but necessary for  pthread_create()
*/
void *TimingThread(void *argv);

#endif
