/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
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
 * @author Steffen Laube
 * 
 * @param data Pointer to CallbackItem_t
 */
void destroyCallbackItem(CallbackItem_t *data);

/**
 * Callback routine for the  reset of user
 * access rigths. It is a part of  mode 
 * protection  featuer
 * @author Steffen Laube
 *
 * @param pNetmask pointer to a string of netmask
 * @param data a pointer to a container of data
 */
void ModeResetCb(char *pNetmask,void *data);

/**
 * Callback routine for the activation of a ban.
 * @author Steffen Laube 
 *
 * @param pNetmask Pointer to a string of netmask
 * @param data a pointer to a container of data
 */
void SetBanCb(char *pNetmask,void *data);

/**
 * Callback routine for the kicking of a user.
 * @author Steffen Laube
 *
 * @param pNetmask pointer to a string of netmask
 * @param data a pointer to a container of data
 */
void KickCb(char *pNetmask, void *data);

#endif

