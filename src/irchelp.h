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

#ifndef		EOM
	#define	EOM		"."
#endif

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
	"log off at the bot",
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
	
	"!channels",
	"Print out a list of all channels from the bot",
	"Only the bot master can use this command.",
	"",
	"",
	"",
	"",
	"",
	"",
	"",

	"!greating",
	"Set or remove the greating line for a channel",
	"Only the bot master or channel owner can do this",
	"!greating TEXT          -> Set itfor the current channel",
	"!greating #CHANNEL TEXT -> Set it for the giving channel",
	"!greating               -> Remove it for the current channel",
	"!greating #CHANNEL      -> Remove it for the giving channel",
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
