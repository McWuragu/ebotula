/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#ifndef PARAMETER_H
#define PARAMETER_H

#include "type.h"
#include "configparser.h"

/** the  maximum length which read form the configuration file */

#define PASSWD_LENGTH   8
#define LOGIN_LENGTH    8

boolean CommandLineParser(int argc,char *const argv[]);
boolean dialogMaster(void);

#endif
