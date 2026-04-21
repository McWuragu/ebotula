/* #############################################################
 *
 *  This file is a part of ebotula testsuit.
 *
 * #############################################################
 */

#include <stdlib.h>
#include <string.h>
#include "parser_pipeline_test.h"
#include "ircbot.h"

boolean stop = false;

static int gPongCallCount = 0;
static char gPongArg[256];

int init_parser_pipeline(void) {
    gPongCallCount = 0;
    gPongArg[0] = '\0';
    return 0;
}

int clean_parser_pipeline(void) {
    gPongCallCount = 0;
    gPongArg[0] = '\0';
    return 0;
}

void test_preParser_ping_calls_pong_direct(void) {
    MsgBuf_t msg;
    char line[] = "PING :irc.example.org";

    preParser(line, &msg);

    CU_ASSERT_EQUAL(msg.identify, CMD_NONE);
    CU_ASSERT_PTR_NULL(msg.pMsgLine);
    CU_ASSERT_EQUAL(gPongCallCount, 1);
    CU_ASSERT_STRING_EQUAL(gPongArg, ":irc.example.org");
}

void test_preParser_identifies_bot_command(void) {
    MsgBuf_t msg;
    char line[] = ":nick!user@host PRIVMSG #chan :!help";

    preParser(line, &msg);

    CU_ASSERT_EQUAL(msg.identify, CMD_HELP);
    CU_ASSERT_PTR_NOT_NULL(msg.pMsgLine);
    if (msg.pMsgLine) {
        CU_ASSERT_STRING_EQUAL(msg.pMsgLine, line);
        free(msg.pMsgLine);
    }
}

void test_preParser_identifies_ctcp_command(void) {
    MsgBuf_t msg;
    char line[] = ":nick!user@host PRIVMSG ebotula :\001VERSION\001";

    preParser(line, &msg);

    CU_ASSERT_EQUAL(msg.identify, CMD_CTCPVERSION);
    CU_ASSERT_PTR_NOT_NULL(msg.pMsgLine);
    free(msg.pMsgLine);
}

void test_preParser_unknown_message_no_dispatch(void) {
    MsgBuf_t msg;
    char line[] = ":server NOTICE user :plain notice without bot command";

    preParser(line, &msg);

    CU_ASSERT_EQUAL(msg.identify, CMD_NONE);
    CU_ASSERT_PTR_NULL(msg.pMsgLine);
}

/* ---------- Test doubles to isolate parser/cmdlist ---------- */
void pong(char *pPong) {
    gPongCallCount++;
    if (pPong == NULL) {
        gPongArg[0] = '\0';
        return;
    }
    strncpy(gPongArg, pPong, sizeof(gPongArg) - 1);
    gPongArg[sizeof(gPongArg) - 1] = '\0';
}

void SendLine(char *pMsg) {(void)pMsg;}
void notice(char *pNick, char *pMsgStr) {(void)pNick; (void)pMsgStr;}
UserLevel_t getUserLevel(char *const pChannel, char *const pNetmask) {
    (void)pChannel;
    (void)pNetmask;
    return MasterLevel;
}

/* Event and command handlers referenced by CmdHandleField */
void hPong(MsgItem_t *pMsg) {(void)pMsg;}
void hNickChange(MsgItem_t *pMsg) {(void)pMsg;}
void hResetModes(MsgItem_t *pMsg) {(void)pMsg;}
void hRejoinAfterKick(MsgItem_t *pMsg) {(void)pMsg;}
void hUserJoin(MsgItem_t *pMsg) {(void)pMsg;}
void hBotNeedOp(MsgItem_t *pMsg) {(void)pMsg;}
void hResetTopic(MsgItem_t *pMsg) {(void)pMsg;}
void hCallback(MsgItem_t *pMsg) {(void)pMsg;}
void hWhoisFailed(MsgItem_t *pMsg) {(void)pMsg;}

void ctcpping(MsgItem_t *pMsg) {(void)pMsg;}
void ctcpversion(MsgItem_t *pMsg) {(void)pMsg;}
void ctcptime(MsgItem_t *pMsg) {(void)pMsg;}

void logoff(MsgItem_t *pMsg) {(void)pMsg;}
void greeting(MsgItem_t *pMsg) {(void)pMsg;}
void help(MsgItem_t *pMsg) {(void)pMsg;}
void version(MsgItem_t *pMsg) {(void)pMsg;}
void hello(MsgItem_t *pMsg) {(void)pMsg;}
void ident(MsgItem_t *pMsg) {(void)pMsg;}
void password(MsgItem_t *pMsg) {(void)pMsg;}
void inviteuser(MsgItem_t *pMsg) {(void)pMsg;}
void chanmode(MsgItem_t *pMsg) {(void)pMsg;}
void accountlist(MsgItem_t *pMsg) {(void)pMsg;}
void accountmode(MsgItem_t *pMsg) {(void)pMsg;}
void say(MsgItem_t *pMsg) {(void)pMsg;}
void kickuser(MsgItem_t *pMsg) {(void)pMsg;}
void setTopic(MsgItem_t *pMsg) {(void)pMsg;}
void setGreeting(MsgItem_t *pMsg) {(void)pMsg;}
void banuser(MsgItem_t *pMsg) {(void)pMsg;}
void debanuser(MsgItem_t *pMsg) {(void)pMsg;}
void restart(MsgItem_t *pMsg) {(void)pMsg;}
void allsay(MsgItem_t *pMsg) {(void)pMsg;}
void rmaccount(MsgItem_t *pMsg) {(void)pMsg;}
void setNick(MsgItem_t *pMsg) {(void)pMsg;}
void die(MsgItem_t *pMsg) {(void)pMsg;}
void chanlist(MsgItem_t *pMsg) {(void)pMsg;}
void rmChannel(MsgItem_t *pMsg) {(void)pMsg;}
void addChannel(MsgItem_t *pMsg) {(void)pMsg;}
void joinChannel(MsgItem_t *pMsg) {(void)pMsg;}
void partChannel(MsgItem_t *pMsg) {(void)pMsg;}
