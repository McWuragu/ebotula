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

    // output of strerr
	 ERR_MISSING_PARAM,  
	 ERR_PORT_PARAMETER,  
	 ERR_WRONG_SERVERNAME,  
	 ERR_WRONG_BOTNAME,	  
	 ERR_ALT_RANGE,		  
	 ERR_PINGTIMEOUT_RANGE,  
	 ERR_LOGLEVEL_RANGE,
     ERR_SENDDELAY_RANGE,   
     ERR_SENDLINE_RANGE,
     ERR_INIT_DELAY,
	 ERR_LOGOFF_RANGE,	    
	 ERR_THREAD_RANGE,	    
	 ERR_MUTEX,			   
    

    // command acknowledgment
	 OK_HELLO,	  
	 OK_HELLO2,	 
	 OK_PASSWD,	 
	 OK_IDENT,	  
	 OK_JOIN,	  
	 OK_PART,	  
	 OK_RMCHANNEL,	   
	 OK_ADDCHANNEL,	   
	 OK_NICK_SET,	   
	 OK_LOGOFF,		 
	 OK_SET_GREETING,   
	 OK_RM_GREETING,    
	 OK_SET_TOPIC,	    
	 OK_RM_TOPIC,	   
	 OK_KICK,		    
	 OK_BAN,		   
     OK_DEBAN,		    
     OK_USERMODE,	    
	 OK_RMUSER,		    
	 OK_MASTER,		   
	 OK_DIE,		    
	 OK_RESTART,	    
	
	// Informations for invalid input or using of the bot
	 ERR_NOT_LOGON,	  
	 ERR_NOT_MASTER,    
	 ERR_NOT_OWNER,
     ERR_NOT_FRIEND,
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
	 ERR_USERLIST_EMPTY,      
	 ERR_UNKNOW_MODE,	    
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
	 INFO_CHANNELLIST_MODE,    
	 INFO_CHANNELLIST_TOPIC,   
	 INFO_CHANNELLIST_GREET,  
	 INFO_DEFAULT_REASON,	  
	 INFO_INIT_CHANNEL,		  
	 INFO_NEED_OP,
     INFO_NOT_PASS
} eMsgID_t;

typedef struct MsgStruct {
    int MsgID;
    char* MsgString;
} MsgStruct_t;


char * getMsgString(eMsgID_t const MsgID);
void printCmdLineHelp();
#endif
