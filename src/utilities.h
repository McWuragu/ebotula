/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#ifdef HAVE_SYSLOG_H
    #include <syslog.h>
#else
    /* fixed the missing header file syslog.h*/
    #define LOG_EMERG   0
    #define LOG_ALERT   1
    #define LOG_CRIT    2
    #define LOG_ERR     3
    #define LOG_WARNING 4
    #define LOG_NOTICE  5
    #define LOG_INFO    6
    #define LOG_DEBUG   7
#endif

#include "type.h"


#define msleep(m)   usleep((m)*1000)

#define COMMON_NOT_ALLOW_CHAR       "\t\r\n, \a\0"
#define USER_ALLOW_CHAR             "-[]\\^{}|_`"

#define CommonStringCheck(m) ((strpbrk((m),COMMON_NOT_ALLOW_CHAR))?false:true)

#ifdef HAVE_GETTEXT
    #include<libintl.h>
#endif

#ifdef HAVE_GETTEXT
    #define _(STRING)    gettext(STRING)
#else
    #define _(STRING)   (STRING)
#endif
/**
* This function trim a string. It remove leading, folling and
* multi spaces and  CR LF at the end.
* @param pStr is a pointer of a string to modify
*/
void trim(char *pStr);

/**
* This function replace all lrge captitals to small letters
*
* @param pStr is the string  for modification
*/
void StrToLower(char *pStr);

/**
* This convert the structure for the mode to a string
*
* @param pMode a pointer of a struct for convert
* @return the accuring string
*/
char *ChannelModeToStr(ChannelMode_t *pMode);

/**
 * This convert a string to a structure for the channel mode
 * 
 * @param pModeStr input string
 * @param pMode    pointer of return value
 */
void StrToChannelMode(char *pModeStr,ChannelMode_t * pMode);

/**
 * This convert a string to a structure for the channel data
 * 
 * @param pChannelSet
 *               a pointer  of the input  data as string
 * @param pChannelData
 *               this is a pointer of  a struct for the return values
 */
void StrToChannelData(char *pChannelSet,ChannelData_t * pChannelData);

/**
* This convert the struct for a channel to a string
*
* @param pChannelData a pointer of a struct for convert
* @return the accuring string
*/
char *ChannelDataToStr(ChannelData_t *pChannelData);

/**
* This function remove all spaces in a string until from double quote include spaces.
* This space in double quote untouched.
* @param pStr is a pointer  of a string to modify
*/
void clearspace(char *pStr);

boolean ChannelStringCheck(char *const pStr);
boolean NickStringCheck(char *const pStr);
/**
* logging all data to syslog or debug
*
* @param syslog-prirorty code
* 	 format string
* 	 ....
* @return -1 if error 
*/
int logger(int priority, char *format, ...);
#endif

