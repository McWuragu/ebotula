/*************************************************************
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003 Steffen Laube <realebula@gmx.de>
*************************************************************/

#ifndef IRCHELP_ENG_H
#define IRCHELP_ENG_H

static char *irchelp_msg[][15] = {
			// header
			
			{
                "This is the help system for using this bot.",
				"!help COMMAND for information about a special command",
				"------------------------------------------------------------------------",
			},
			// commands
			
			// others
			// VIEWGREAT
			{
				"This give out the greeting line of a channel",
			},
			// HELP
			{
				"give general help for using this bot",
			},
			// VERSION
			{
				"print the version information",
			},
			// HELLO
			{	  
				"create a new account.",
			},
			// IDENT
			{
				"identify yourself to the bot",
			},
		
			// Logged user
			// LOGOFF
			{
				"logoff at the bot",
			},
			// PASS
			{
				"change the password for the current account",
			},
			
			// owner commands
			// CHANMODE
			{
				"Set and remove the modes for a channel.",
			},
			// USERLIST
			{
				"print a list of user from the channel or all",
			},
			// USERMODE
			{
				"This is to set  the mod for a user",
			},
			// SAY
			{
				"This give out a message of a channel",
			},
			// KICK
			{
				"This kick a user out the channel",
			},
			// SET_TOPIC
			{
				"Set or remove the  topic of a channel",
			},
			// SET_GREATING
			{
				"Set or remove the greeting line for a channel",
			},
			// CHANNELS
			{
				"Print out a list of all channels from the bot",
			},
			
			// Master commands
			// RESTART
			{
				"This command stop and  restart the bot.",
			},
			// ALLSAY
			{
				"Send a message to all channels",
			},
			// RMUSER
			{
				"This remove a user account of the bot",
			},
			// NICK
			{
				"change the nickname of the bot",
			},
			// DIE
			{
				"Shutdown the bot.",
			},
			// RMCHANNEL
			{
				"let the bot part and remove a old channel",
			},
			// ADDCHANNEL
			{
				"let the bot join and add a new channel",
			},
			// JOIN
			{
				"let the bot  join a channel",
			},
			// PART
			{
				"let the bot part a channel",
			}
		};

#endif
