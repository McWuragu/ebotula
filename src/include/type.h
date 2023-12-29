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

#ifndef TYPE_H
#define TYPE_H

#define MAX_READ_BUFFER_SIZE    256
#define SERVER_NOT_ALLOW_CHAR           " :;<>*#/\"\'\\{}[]()=?§$"

/** This is definetion of boolean datatype */
typedef enum {false,true} boolean;

/** This stuctur is a data container for  all setup parameters.*/
typedef struct ConfStruct{
    /** the nickname of the bot */
    char *pBotname;
    /** the string  for  the field realname */
    char *realname;
    /** the url of the  irc server */
    char *sHostname;
    /** the portnumber for connection to the server */
    char *sPort;
    /** the path to a not default configuration file */
    char *configfile;
    /** the path to the database */
    char *pDatabasePath;
    /** number of threads which at the begin was create */
    unsigned int thread_limit;
    /** the limit for the existenz of a account in days */
    unsigned int AccountLiveTime;
    /** the limit for login in days */
    unsigned int AutoLoggoff;
    /** the delay time for sending in  millisecond */
    int iSendDelay;
    /** the secondary delay for sending in millisecond */
    int nSlowSendDelay;
    /** number of line whiche send with the normal delay */
    int nFastSendingCharLimit;
    /** the connection timeout in second */
    int iTimeout;
    /** a flag to signal that a new master want create */
    boolean newMaster;
    /** the name of the user for execution in the root mode */
    char * sExeUser;
    /** the name of the group for execution in the root mode */
    char *sExeGroup;
    /** a delay of initialization of the channels */
    unsigned int nSettling;
    /** log level is define as syslog level */
    unsigned int nLogLevel;
    int bLogLevelWasSet;

	/** nick serv ident extension to allow identification to nickserv */
	char * sNickServIdent;
	int nEnableNickServIdent;
	int nNickServIdentTimeout;
} ConfigSetup_t;


typedef enum {
    NoticeMode,
    PrvMsgMode
} AnswerMode_t;


/** the number of channel mode options */
#define MAX_MODES               11
#define _MAX_MODES              MAX_MODES+2

/** This  enum is  used as index for value pModeStr from the struct ChannelModeStruct */
enum ChanModeEnum {
    MOD_TYPE,
    MOD_KEYWORD,
    MOD_LIMIT,
    MOD_TOPIC,
    MOD_MESSAGE,
    MOD_MODERAT,
    MOD_PRIVAT,
    MOD_SECRET,
    MOD_ANONYM,
    MOD_QUIET,
    MOD_REOP,
    MOD_INVITE
};
/* 
 * modiefier for param list
 */
typedef enum {
	  PARAM_CHAR,
	  PARAM_SHORT,
	  PARAM_INT,
	  PARAM_LONG,
	  PARAM_HEX,
	  PARAM_FLOAT,
	  PARAM_DOUBLE,
	  PARAM_STRING
} Input_t,Output_t; 
typedef enum {
	MOD_NONE=0,
	MOD_QUOTED
	} Modefier_t;
/*
 * Value Structure
 */
enum Values_e{
	VAL_BOTNAME=0,
	VAL_REALNAME,
	VAL_EXEUSER,
	VAL_EXEGROUP,
	VAL_THREADLIMIT,
	VAL_SERVER,
	VAL_PORT,
	VAL_DATABASEPATH,
	VAL_FASTSENDDELAY,
	VAL_SLOWSENDDELAY,
	VAL_FASTSENDLIMIT,
	VAL_INITDELAY,
	VAL_ALT,
	VAL_AUTOLOGOFF,
	VAL_PINGTIMEOUT,
	VAL_LOGLEVEL,
	VAL_NICKSERV_IDENT,
	VAL_ENABLE_NICKSERV_IDENT,
	VAL_NICKSERV_IDENT_TIMEOUT,
	VAL_COUNT
};


/** a structure as container for the channel modes */
typedef struct ChannelModeStruct {
    char pModeStr[_MAX_MODES];
    char *pKeyword;
    char *pLimit;
} ChannelMode_t;

/** This container is used for  the values from a channel */
typedef struct ChannelDataStruct {
    ChannelMode_t sModes;
    char *pTopic;
    char *pGreeting;
} ChannelData_t;

#endif
