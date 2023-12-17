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

#ifndef UTILITIES_H
#define UTILITIES_H

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#ifdef HAVE_SYSLOG
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
char * StrToLower(const char *pStr);

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


/**
 * This function convert the verbose level to the syslog level.
 * 
 * @param nVerbose the verbose level
 * 
 * @return the syslog level
 */
int convertVerboseToLogLevel(const int nVerbose);

/**
 * This function disable the echo on the  screen
 */
void HideLogLinesOnScreen();
#endif

