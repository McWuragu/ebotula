/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#ifndef TYPE_H
#define TYPE_H


/** This is definetion of boolean datatype */
typedef enum {false,true} boolean;

/** This stuctur is a data container for  all setup parameters.*/
typedef struct ConfStruct{
    /** the nickname of the bot */
    char *botname;
    /** the string  for  the field realname */
    char *realname;
    /** the url of the  irc server */
    char *server;
    /** the port for connection to the server */
    char *port;
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
    int iSendSafeDelay;
    /** number of line whiche send with the normal delay */
    int iSendSafeLine;
    /** the connection timeout in second */
    int iTimeout;
    /** a flag to signal that a new master want create */
    boolean newMaster;
} ConfigSetup_t;


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


/** a structure as container for the channel modes */
typedef struct ChannelModeStruct {
    char pModeStr[_MAX_MODES];
    char *pKeyword;
    char *pLimit;
} ChannelMode_t;

/** This container is used for  the values from a channel */
typedef struct ChannelDataStruct {
    ChannelMode_t* pModes;
    char *pTopic;
    char *pGreeting;
} ChannelData_t;

#endif
