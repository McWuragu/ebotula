/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef TIMING_H
#define TIMING_H

/**
* This is  the function for the timing thread. She handle all task which need a timing
* synchronization. This task are ping the server, remove dead accounts, automatic log off
* and reset the ban mask after the time ban.
* @param argv is a pointer for any parameters This is unused but necessary for  pthread_create()
* @return a pointer of any return values. This is unused but necessary for  pthread_create()
*/
void *synchron(void *argv);

#endif
