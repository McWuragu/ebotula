/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
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

#ifndef _CALLBACK_H
#define _CALLBACK_H

typedef struct CallbackStruct {
	char *nickname;
	void (*CallbackFkt)(char *pNick,void *data);
    void *data;
 } CallbackItem_t;

/**
 * Destroys Dataelement und sub-item of it
 * 
 * @param data Pointer to CallbackItem_t
 */
void destroyCallbackItem(CallbackItem_t *data);

/**
 * Callback routine for the  reset of user
 * access rigths. It is a part of  mode 
 * protection  featuer
 *
 * @param pNetmask pointer to a string of netmask
 * @param data a pointer to a container of data
 */
void ModeResetCb(char *pNetmask,void *data);

/**
 * Callback routine for the activation of a ban.
 *
 * @param pNetmask Pointer to a string of netmask
 * @param data a pointer to a container of data
 */
void SetBanCb(char *pNetmask,void *data);

/**
 * Callback routine for the kicking of a user.
 *
 * @param pNetmask pointer to a string of netmask
 * @param data a pointer to a container of data
 */
void KickCb(char *pNetmask, void *data);

#endif

