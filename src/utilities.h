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
         fprintf(stderr,"%02d:%02d:%02d.%03d ",td->tm_hour,td->tm_min,td->tm_sec,(stamp.tv_nsec/1000000)); \
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

void printMsg(char *const pMsg[]);

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

