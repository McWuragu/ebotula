/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <Laube.Steffen@gmx.de>
 * ############################################################# 
 */

#ifndef MESSAGES_H
#define MESSAGES_H


#define	EOM			'.'

/** container for  message text */



// output of strerr
typedef enum {

    // command acknowledgment
	 OK_BAN,		   
     OK_DEBAN,		    
     OK_USERMODE,	    
	 OK_RMUSER,		    
	 OK_MASTER,		   
	 OK_DIE,		    
	 OK_RESTART,	    
	
	// Informations for invalid input or using of the bot
	 ERR_NOT_MASTER,    
	 ERR_NOT_CHANNELOPT,
     ERR_NOT_PARAMETER,
     ERR_NOT_COMMAND,	  
	 ERR_NOT_ACCOUNT,	  
	 ERR_NOT_CHANNEL,	  
	 ERR_NOT_ACCESS,	  
	 ERR_NOT_SELF,		  
	 ERR_NOT_KICK,       
     ERR_NOT_BAN,         
	
	// Errors
	 ERR_ADDCHANNEL_ALREADY,	  
     ERR_NICK_INVALID,	     
	 ERR_ALREADY_LOGON,	     
	 ERR_UNKNOWN_MODS,	      
	 ERR_NOTSELF_KICK,    
     ERR_NOTSELF_BAN,        
     ERR_WRONG_PW,

	// other informations
	 INFO_HELP_END,	 
	 INFO_HELP_FOR,		    
	 INFO_USERLIST_BEGIN,   
	 INFO_USERLIST_END,	    
	 INFO_CHANNELLIST_BEGIN,  
	 INFO_CHANNELLIST_END,	  
	 INFO_CHANNELLIST_TOPIC,   
	 INFO_CHANNELLIST_GREET,  
	 INFO_DEFAULT_REASON,	  
} eMsgID_t;

typedef struct MsgStruct {
    int MsgID;
    char* MsgString;
} MsgStruct_t;


char * getMsgString(eMsgID_t const MsgID);
void printCmdLineHelp();
#endif
