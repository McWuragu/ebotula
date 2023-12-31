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

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "type.h"

/**
 * the definition of the userlevel
 * 
 * @see getUserLevel()
 */
typedef enum  {
    NoneLevel,
    LoggedLevel,
    FriendLevel,
    OwnerLevel,
    MasterLevel
} UserLevel_t;
                                    


/**
 * get the userlevel for this netmask and  the channel
 * 
 * @param pChannel the environment channel for testing
 * @param pNetmask the netmask for testing
 * @return the userlevel
 * 
 * @see UserLevel_t
 */
UserLevel_t getUserLevel(char *const pChannel, char *const pNetmask);

/**
* This function log on a user. She put the net mask in the  databases usertonick.dbf and
* nicktouser.dbf. If a user in both files then is he status "log on"
*
* @param pNetmask the  complete netmask which receive from the irc server
* @param pLogin the login nameof the user which have logged on
*/
void log_on(char *pNetmask,char *pLogin);
/**
* This function call the  helper __log_out(). This  function is a  mutex 
* protecting version for the  logout
*
* @param pLogin the login name of the user which want logoff
*/
void log_out(char *pLogin);
/**
* This function remove the login name and the appendant  netmask from the databases
* usertonick.dbf and nicktouser.dbf. After this is the user logged off.
*
* @param pLogin the login name of the user which want logoff
*/
static void __log_out(char *pLogin);

/**
* This is for removing of a account. This  remove him form all databases and remove all access rights
*
* @param pLogin the login name of the  user which want remove
*/
void rmAccount(char *pLogin);
/**
* Thois function remove only the all accesss rights of  a user.
*
* @param pLogin pLogin the login name of the  user for which want remove access rights.
*/
void rmAccessRights(char *pLogin);

/**
* This lock for  user which last loging on is later as the  checking time and logoff this user.
*
* @param lCheckTime the limit time
*/
void rmDeadLogins(long lCheckTime);
/**
* This lock for  user which last loging on is later as the  checking time and remove this user.
*
* @param lCheckTimethe limit time
*/ 
void rmDeadAccounts(long lCheckTime);
/**
 * This function check the access right from a user to a other user
 *
 * @param pCmdLogin the login of the executionen user
 * @param pLogin the login of the user in command
 * @param pChannel the channel environment for the command
 * @return the status of the  access right
 */ 
boolean checkUserLevel(char *pCmdLogin, char *pLogin, char const *pChannel);
#endif
