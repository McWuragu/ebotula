/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef ACCOUNT_H
#define ACCOUNT_H

void log_on(char *pNetmask,char *pLogin);
void log_out(char *pLogin);

void rmAccount(char *pLogin);
void rmAccessRights(char *pLogin);

void rmDeadLogins(long lCheckTime);
void rmDeadAccounts(long lCheckTime);

#endif
