/* #############################################################
 *
 * This is a part of ebotula.
 * It is distributed under the GNU General Public License
 * See the file COPYING for details.
 *
 * (c)2003 Steffen Laube <realebula@gmx.de>
 * ############################################################# 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include "utilities.h"
#include "messages.h"
#include "network.h"
#include "dbaccess.h"
#include "irc.h"



// #############################################################################
void user(void) {
    extern ConfigSetup_t sSetup;
    struct passwd *pw;
    char hostname[HOSTNAME_BUFFER_SIZE];
    char *buffer;
    int buffer_size;

    // get the username  and hostname
    pw = getpwuid(geteuid());
    gethostname(hostname,HOSTNAME_BUFFER_SIZE);

    // calculat the command size
    buffer_size=strlen("USER")+strlen(pw->pw_name)+strlen(hostname)+strlen(sSetup.server)+strlen(sSetup.realname)+8;
    buffer=(char *)malloc(sizeof(char)*buffer_size);

    // create the  commando string
    sprintf(buffer,"USER %s %s %s :%s\r\n",pw->pw_name,hostname,sSetup.server,sSetup.realname);

    // send commando
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void action(char *pTarget, char *pMsgStr){
    char *buffer;
    buffer=(char *)malloc((strlen("ACTION ")+strlen(pMsgStr)+1)*sizeof(char));
    sprintf(buffer,"ACTION %s",pMsgStr);
    privmsg(pTarget,buffer);
}
// #############################################################################
void privmsg(char *pTarget, char *pMsgStr){
    char *buffer;
    buffer=(char *)malloc((strlen("PRIVMSG ")+strlen(pTarget)+strlen(pMsgStr)+5)*sizeof(char));
    sprintf(buffer,"PRIVMSG %s :%s\r\n",pTarget,pMsgStr);
    send_line(buffer);
}
// #############################################################################
void notice(char *pNick,char *pMsgStr) {
    char *buffer;
    buffer=(char *)malloc((strlen("NOTICE ")+strlen(pNick)+strlen(pMsgStr)+5)*sizeof(char));
    sprintf(buffer,"NOTICE %s :%s\r\n",pNick,pMsgStr);
    send_line(buffer);
    free (buffer);
}

// #############################################################################
//void quit(void) {
//    send_direct("QUIT\r\n");
//}
// #############################################################################
void join(char *pChannel) {
    char *buffer;
    buffer=(char *)malloc((strlen("JOIN ")+strlen(pChannel)+3)*sizeof(char));
    sprintf(buffer,"JOIN %s\r\n",pChannel);
    send_line(buffer);
    free (buffer);
}
// #############################################################################
void part(char *pChannel) {
    char *buffer;
    buffer=(char *)malloc((strlen("PART ")+strlen(pChannel)+3)*sizeof(char));
    sprintf(buffer,"PART %s\r\n",pChannel);
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void pong(void) {
    char *buffer,hostname[HOSTNAME_BUFFER_SIZE];

    // get  hostname
    gethostname(hostname,HOSTNAME_BUFFER_SIZE);

    buffer=(char *)malloc((strlen("PONG ")+strlen(hostname)+3)*sizeof(char));
    sprintf(buffer,"PONG %s\r\n",hostname);
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void ping(char *pTarget) {
    char *buffer;
    buffer=(char *)malloc((strlen("PING ")+strlen(pTarget)+3)*sizeof(char));
    sprintf(buffer,"PING %s\r\n",pTarget);
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void invite(char *pChannel,char *pNick) {
    char *buffer;
    buffer=(char*)malloc((strlen("INVITE")+strlen(pNick)+strlen(pChannel)+4)*sizeof(char));
    sprintf(buffer,"INVITE %s %s\r\n",pNick,pChannel);
    send_line(buffer);
    free(buffer);
}
// #############################################################################
void nick(char *pNick) {
    char *buffer;
    buffer=(char *)malloc((strlen("NICK ")+strlen(pNick)+3)*sizeof(char));
    sprintf(buffer,"NICK %s\r\n",pNick);
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void topic(char *pChannel, char* pMsgStr) {
    char *buffer;
    buffer=(char*)malloc((strlen("TOPIC ")+strlen(pChannel)+strlen(pMsgStr)+5)*sizeof(char));
    sprintf(buffer,"TOPIC %s :%s\r\n",pChannel,pMsgStr);
    send_line(buffer);
    free (buffer);
}
// #############################################################################
void kick(char *pChannel, char *pNick, char *pMsgStr) {
    char *buffer;

    // check  optional parameters and  set  it of default values
    if (pMsgStr==NULL) {
        pMsgStr=getMsgString(INFO_DEFAULT_REASON);;
    }
    buffer=(char*)malloc((strlen("KICK ")+strlen(pChannel)+strlen(pNick)+strlen(pMsgStr)+6)*sizeof(char));
    sprintf(buffer,"KICK %s %s :%s\r\n",pChannel,pNick,pMsgStr);
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void ban(char *pChannel,char *pMask){
    char *buffer;
    buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMask)+6)*sizeof(char));
    sprintf(buffer,"MODE %s +b %s\r\n",pChannel,pMask);
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void deban(char *pChannel,char *pMask){
    char *buffer;
    buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMask)+6)*sizeof(char));
    sprintf(buffer,"MODE %s -b %s\r\n",pChannel,pMask);
    send_direct(buffer);
    free (buffer);
}
// #############################################################################
void mode(char *pChannel, char *pMod, char *pModParameter) {
    char *buffer;

    // check  optional parameters and  set  it of default values
    if (pModParameter==NULL) {
        pModParameter="";
    }
    buffer=(char*)malloc((strlen("MODE ")+strlen(pChannel)+strlen(pMod)+strlen(pModParameter)+5)*sizeof(char));
    sprintf(buffer,"MODE %s %s %s\r\n",pChannel,pMod,pModParameter);
    send_line(buffer);
    free (buffer);
}
// #############################################################################
void whois(char *pNickname) {
    char *buffer;
    buffer=(char*)malloc((strlen("WHOIS ")+strlen(pNickname)+3)*sizeof(char));
    sprintf(buffer,"WHOIS %s\r\n",pNickname);
    send_direct(buffer);
    free (buffer);
}




