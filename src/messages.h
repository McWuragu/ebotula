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


typedef enum   {
     SYSLOG_READ_CMD,		
	 SYSLOG_READ_CONFFILE,	
	 SYSLOG_INIT_DB,		
	 SYSLOG_TRY_CONNECT,	
	 SYSLOG_IS_CONNECT,		
	 SYSLOG_BOT_RUN,		
	 SYSLOG_BOT_STOP,		
     SYSLOG_BOT_START,		
	 SYSLOG_UNKNOWN_CMDID,	
	 SYSLOG_ACCOUNT_RM,     
	 SYSLOG_FAILED_NETPARA,	
	 SYSLOG_MSG_QUEUE,		 
	 SYSLOG_RESOLVE_HOSTNAME,
	 SYSLOG_RESOLVE_PROTOCOL,
	 SYSLOG_SOCKET,			
	 SYSLOG_CONNECT,		
	 SYSLOG_CONFIG_FILE,	
	 SYSLOG_SEND,			
	 SYSLOG_RECV,		    
	 SYSLOG_TIMEOUT,		
	 SYSLOG_CREATE_DIR,		
	 SYSLOG_CREATE_DIR_ERR,	
     SYSLOG_DATABASE_ERR,	
     SYSLOG_RESTART,
	 SYSLOG_STOPPED
}eSyslogID_t;

// output of strerr
typedef enum {
	 INFO_MASTER_TITLE,  
	 INFO_MASTER_LOGIN,  
	 INFO_MASTER_PASS,  
	 INFO_MASTER_REPASS,  
	 ERR_MASTER_PASS,  
	 ERR_MASTER,   

    // output of strerr
     ERR_RESTART,  
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
	 ERR_NICK,			   
	 ERR_MUTEX,			   
	 ERR_NOT_ALLOW_CHAR,    
    

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
	 ERR_NICK_EXIST,	     
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
char * getSyslogString(eSyslogID_t const MsgID);
char ** getCmdLineHelp();
char ** getCmdLineError();
#endif
