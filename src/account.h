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

/**
* This function log on a user. She put the net mask in the  databases usertonick.dbf and
* nicktouser.dbf. If a user in both files then is he status "log on"
*
* @author Steffen Laube
* @param pNetmask the  complete netmask which receive from the irc server
* @param pLogin the login nameof the user which have logged on
*/
void log_on(char *pNetmask,char *pLogin);
/**
* This function remove the login name and the appendant  netmask from the databases
* usertonick.dbf and nicktouser.dbf. After this is the user logged off.
*
* @author Steffen Laube
* @param pLogin the login name of the user which want log off
*/
void log_out(char *pLogin);

/**
* This is for removing of a account. This  remove him form all databases and remove all access rights
*
* @author Steffen Laube
* @param pLogin the login name of the  user which want remove
*/
void rmAccount(char *pLogin);
/**
* Thois function remove only the all accesss rights of  a user.
*
* @author Steffen Laube
* @param pLogin pLogin the login name of the  user for which want remove access rights.
*/
void rmAccessRights(char *pLogin);

/**
* This lock for  user which last loging on is later as the  checking time and log off this user.
*
* @author Steffen Laube
* @param lCheckTime the limit time
*/
void rmDeadLogins(long lCheckTime);
/**
* This lock for  user which last loging on is later as the  checking time and remove this user.
*
* @author Steffen Laube
* @param lCheckTimethe limit time
*/ 
void rmDeadAccounts(long lCheckTime);

#endif
