/* -------------------------------------------------------------
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
 -------------------------------------------------------------*/

#ifndef PARAMETER_H
#define PARAMETER_H

#include "type.h"

/** the  maximum length which read form the configuration file */
#define MAX_READ_BUFFER_SIZE    256

#define PASSWD_LENGTH   8
#define LOGIN_LENGTH    8

#define SERVER_NOT_ALLOW_CHAR           " :;<>*#/\"\'\\{}[]()=?§$"

void ComandLineParser(int argc,char *const argv[]);
void ConfigFileParser(void);
boolean dialogMaster(void);

#endif
