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

void log_on(char *netmask,char *login);
void log_out(char *login);

void rmAccount(char *login);
void rmAccessRights(char *login);

void rmDeadLogins(long checkTime);
void rmDeadAccounts(long chackTime);

#endif
