/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef IRCHELP_H
#define IRCHELP_H

#define HELP_ITEM_SIZE	10

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifndef		EOM
	#define	EOM		"."
#endif


/**
* This array of char pointers is a container for  the help information. It is a part of the
* help system. Here are the gernal and the detial information about the irc commands.
* The size for a command are ten lines. The first ten lines contains the  header for the
* command list. The following lines are the detail information for a command. Each tenth Line
* represent the name of the  command and the  following line the smart describtion. 
* This two first lines from the  detail commando help build the  command list. The dot at the end
* is a sentinel for the  help text. After this comes a dummy for new command help
*/

#ifdef LANG_DE
	// Here insert all help messages in german
#else
	static const char *irchelp_msg[]={
		"This is the help system for using this bot.",
		"!help COMMAND for information about a special command",
		"------------------------------------------------------------------------",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		
		"!help",
		"give general help for using this bot",
		"!help         -> command list",
		"!help COMMAND -> information about this command",
		"",
		"",
		"",
		"",
		"",
		"",
		
		"!version",
		"print the version information",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		
		"!ident",
		"identify yourself to the bot",
		"You must have an account to use this command!(see !hello)",
		"!ident LOGIN PASS -> identify at the bot with password",
		"!ident LOGIN      -> identify at the bot with empty password",
		"",
		"",
		"",
		"",
		"",
		
		"!logoff",
		"logoff at the bot",
		"You must have be identified before",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		
		"!die",
		"Shutdown the bot.",
		"Only the bot master can use this command.",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		
		"!hello",
		"create a new account.",
		"Any user can use this to create an account",
		"The loginname of this new account is the current nick",
		"",
		"",
		"",
		"",
		"",
		"",
	
		"!nick",
		"change the nickname of the bot",
		"Only the bot master can use this command.",
		"!nick NICKNAME -> set the new nickname",
		"",
		"",
		"",
		"",
		"",
		"",
	
		"!addchannel",
		"let the bot join and add a new channel",
		"Only the bot master can use this command.",
		"!addchannel #CHANNEL ->Add and join this channel",
		"",
		"",
		"",
		"",
		"",  
		"",
	
		"!rmchannel",
		"let the bot part and remove a old channel",
		"Only the bot master can use this command.",
		"!rmchannel #CHANNEL ->Remove and part this channel",
		"",
		"",
		"",
		"",
		"",
		"",
	
		"!join",
		"let the bot  join a channel",
		"Only the bot master can use this command.",
		"!join #CHANNEL ->Join this channel",
		"",
		"",
		"",
		"",
		"",
		"",
	
		"!part",
		"let the bot part a channel",
		"Only the bot master can use this command.",
		"!part #CHANNEL ->Part this channel",
		"",
		"",
		"",
		"",
		"",
		"",
	
		"!pass",
		"change the password for the current account",
		"!pass PASSWORD ->Set a new password",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		
		"!chanlist",
		"Print out a list of all channels from the bot",
		"Only the bot master can use this command.",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
	
		"!greeting",
		"Set or remove the greeting line for a channel",
		"Only the bot master or channel owner can do this",
		"!greeting TEXT          -> Set it for the current channel",
		"!greeting #CHANNEL TEXT -> Set it for the giving channel",
		"!greeting               -> Remove it for the current channel",
		"!greeting #CHANNEL      -> Remove it for the giving channel",
		"",
		"",
		"",
	
		"!topic",
		"Set or remove the  topic of a channel",
		"Only the bot master or a channel owner can use this command.",
		"!topic TEXT          -> Set it for the cuttent channel",
		"!topic #CHANNEL TEXT -> Set it for the giving channel",
		"!topic               -> Remove it for the current channel",
		"!topic #CHANNEL      -> Remove it for the giving channel",
		"",
		"",
		"",
	
		"!say",
		"This give out a message of a channel",
		"Only the bot master or a channel owner can use this command.",
		"!say TEXT          -> Type a message of the current channel",
		"!say #CHANNEL TEXT -> Type a message of the giving channel",
		"",
		"",
		"",
		"",
		"",
	
		"!viewgreet",
		"This give out the greeting line of a channel",
		"!viewgreet          -> Type the greeting line of the current channel",
		"!viewgreet #CHANNEL -> Type the greeting line of the selected channel",
		"",
		"",
		"",
		"",
		"",
		"",
			
		"!kick",
		"This kick a user out the channel",
		"Only the bot master or a channel owner can use this command.",
		"!kick NICK          -> Kick the user out the current channel",
		"!kick #CHANNEL NICK -> kick the user out the selected channel",
		"",
		"",
		"",
		"",
		"",
		
		"!usermode",
		"This is to set  the mod for a user",
		"Only the bot master or a channel owner can use this command.",
		"!usermode LOGIN +/-MODS          -> Set the mod for the user in the current channel",
		"!usermode #CHANNEL LOGIN +/-MODS -> Set the mod for the user in the selected channel",
		"Mods:",
		"+v  Voice",
		"+o  Operator/channel owner",
		"+m  Operator/bot master",
		"",
	
		"!rmuser",
		"This remove a user account of the bot",
		"Only the bot master can use this command.",
		"!rmuser LOGIN   -> Remove this user inclusive all access rights",
		"",
		"",
		"",
		"",
		"",
		"",
	
		"!userlist",
		"This print out a list of user for the channel or all",
		"Only the bot master or a channel owner can use this command.",
		"A channel owner become a list of user from his channel",
		"But  a master can get  complete list or  list of a channel",
		"!userlist          -> user list from the current channel or all",
		"!userlist #CHANNEL -> user lsit from the selected channel",
		"",
		"",
		"",
	
		"!allsay",
		"Send a message to all channels",
		"Only the bot master can use this command.",
		"The bot must be a irc operator",
		"!allsay TEXT -> send text in all channels",
		"",
		"",
		"",
		"",
		"",
	
		"!chanmode",
		"Set and remove the modes for a channel.",
		"Only the bot master or a channel owner can use this command.",
		"!chanmode +/-MODES          -> set or remove the modes for the current channel",
		"!chanmode #CHANNEL +/-MODES -> set or remove the modes for the selected channel",
		"Supported Modes:",
		"t,n,m,s,p,r,q,a,k,l -> equal the irc modes",
		"",
		"",
		"",
			
		"!restart",
		"This command stop and  restart the bot.",
		"Only the bot master can use this  command",
        "",
		"",
		"",
		"",
		"",
		"",
		"",
		
		// end mark of the complete array
		"."
	
		"!KOMMAND",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
	};
#endif

#endif
