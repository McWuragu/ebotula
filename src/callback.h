/* -------------------------------------------------------------
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
 -------------------------------------------------------------*/

#ifndef _CALLBACK_H
#define _CALLBACK_H

typedef struct CallbackStruct {
	char *nickname;
	void (*CallbackFkt)(char *pNick,void *data);
    void *data;
 } CallbackItem_t;

/* destroxCallbackItem
 *
 * Description:	Destroys Dataelement und sub-item of it
 * 
 * Parameter: 	CallbackItem_t
 * 		-Pointer to CallbackItem_t
 *
 * Return:	void
 *
 */
void destroyCallbackItem(CallbackItem_t *data);

/* ModerResetCb
 * 
 * Description: Callback routine for the  reset of user
 *              access rigths. It is a part of  mode 
 *              protection  featuer
 *
 * Parameter:   
 *      pNetmask
 *      - Pointer to a string of netmask
 *      data
 *      - a pointer to a container of data
 * Return: void
 *
 */
void ModeResetCb(char *pNetmask,void *data);

/* SetBanCb
 * 
 * Description: Callback routine for the activation
 *              of a ban. 
 *
 * Parameter:   
 *      pNetmask
 *      - Pointer to a string of netmask
 *      data
 *      - a pointer to a container of data
 * Return: void
 *
 */
void SetBanCb(char *pNetmask,void *data);

/* RemoveBanCb
 * 
 * Description: Callback routine for the removing
 *              of a ban.
 *
 * Parameter:   
 *      pNetmask
 *      - Pointer to a string of netmask
 *      data
 *      - a pointer to a container of data
 * Return: void
 *
 */
void RemoveBanCb(char *pNetmask,void *data);

/* KickCb
 * 
 * Description: Callback routine for the kicking of
 *              a user.
 *
 * Parameter:   
 *      pNetmask
 *      - Pointer to a string of netmask
 *      data
 *      - a pointer to a container of data
 * Return: void
 *
 */
void KickCb(char *pNetmask, void *data);

#endif

