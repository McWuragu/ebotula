/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H


#define msleep(m)   usleep((m)*1000)

#ifdef _DEBUG
	#include <syslog.h>
	/** Macro for debugging messages */
    #define DEBUG(str...) syslog(LOG_DEBUG,str);
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

void StrToLower(char *pStr);							 

void printMsg(const char **pMsg);

/**
* This function remove all spaces in a string until from double quote include spaces.
* This space in double quote untouched.
* @param pStr is a pointer  of a string to modify
*/ 
void clearspace(char *pStr);

#endif

