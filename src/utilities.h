/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include "type.h"



#define msleep(m)   usleep((m)*1000)

#define COMMON_NOT_ALLOW_CHAR       "\t\r\n, \a\0"
#define USER_ALLOW_CHAR             "-[]\\^{}|_`"

#define CommonStringCheck(m) ((strpbrk((m),COMMON_NOT_ALLOW_CHAR))?false:true)


#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#ifndef NDEBUG
    #include <time.h>
    #include <stdio.h>
    
    /** Macro for debugging messages */
    #define DEBUG(str...){ \
         struct timespec stamp; \
         struct tm *td; \
         clock_gettime(CLOCK_REALTIME,&stamp); \
         td=localtime(&stamp.tv_sec);\
         fprintf(stderr,"%2d:%2d:%2d.%3d ",td->tm_hour,td->tm_min,td->tm_sec,(stamp.tv_nsec/1000000)); \
         fprintf(stderr,str); \
         }
#else
    /** Macro for debugging messages */
    #define DEBUG(str...)
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
* @return pointer of the accuring structure
*/
ChannelMode_t * StrToChannelMode(char *pModeStr);

/**
* This convert a string to a structure for the channel data
*
* @param pModeStr input string
* @return pointer of the accuring structure
*/
ChannelData_t *StrToChannelData(char *pChannelSet);

/**
* This convert the struct for a channel to a string
*
* @param pChannelData a pointer of a struct for convert
* @return the accuring string
*/
char *ChannelDataToStr(ChannelData_t *pChannelData);

void printMsg(char *const pMsg[]);

/**
* This function remove all spaces in a string until from double quote include spaces.
* This space in double quote untouched.
* @param pStr is a pointer  of a string to modify
*/
void clearspace(char *pStr);

boolean ChannelStringCheck(char *const pStr);
boolean NickStringCheck(char *const pStr);

#endif

