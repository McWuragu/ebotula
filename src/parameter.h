/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef PARAMETER_H
#define PARAMETER_H

#include "type.h"
#define PASSWD_LENGTH	8


void cmd_line(int argc,const char *argv[]);
void read_config_file(void);
boolean dialogMaster(void);
char *getPasswd(void);

#endif
