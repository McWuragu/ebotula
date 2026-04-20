/* #############################################################
 *
 *  This file is a part of ebotula testsuit.
 *
 *  Coypright (C)2023-2024 Steffen Laube <Laube.Steffen@gmx.de>
 *
 * #############################################################
 */

#include "configparser_test.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "type.h"

extern ConfigSetup_t sSetup;
extern InputValueStruct_t vsInputConfig[VAL_COUNT];

extern int key_int_checker(const void * value,const void* range1,const void *range2,const char *param_name);
extern int key_server_checker(const void * value,const void* range1,const void *range2,const char *param_name);
extern int key_nick_checker(const void * value,const void* range1,const void *range2,const char *param_name);

static void reset_parser_state(void) {
    int i;

    for (i = 0; i < VAL_COUNT; i++) {
        vsInputConfig[i].isFound = false;
    }

    free(sSetup.pBotname);
    free(sSetup.realname);
    free(sSetup.sHostname);
    free(sSetup.sPort);
    free(sSetup.configfile);
    free(sSetup.pDatabasePath);
    free(sSetup.sExeUser);
    free(sSetup.sExeGroup);
    free(sSetup.sNickServIdent);

    memset(&sSetup, 0, sizeof(sSetup));
}

static char *write_temp_config(const char *content) {
    static const char tmpl[] = "/tmp/ebotula-configparser-XXXXXX";
    char *path = strdup(tmpl);
    int fd;

    if (!path) {
        return NULL;
    }

    fd = mkstemp(path);
    if (fd < 0) {
        free(path);
        return NULL;
    }

    if (write(fd, content, strlen(content)) < 0) {
        close(fd);
        unlink(path);
        free(path);
        return NULL;
    }

    close(fd);
    return path;
}

int init_configparser(void) {
    reset_parser_state();
    return 0;
}

int clean_configparser(void) {
    reset_parser_state();
    return 0;
}

void test_key_int_checker_valid_boundary(void) {
    errno = 0;
    CU_ASSERT_EQUAL(key_int_checker("1", (void*)1, (void*)10, "threadlimit"), 0);
    CU_ASSERT_EQUAL(errno, 0);
}

void test_key_int_checker_below_min(void) {
    errno = 0;
    CU_ASSERT_EQUAL(key_int_checker("0", (void*)1, (void*)10, "threadlimit"), EDOM);
    CU_ASSERT_EQUAL(errno, EDOM);
}

void test_key_server_checker_invalid(void) {
    errno = 0;
    CU_ASSERT_EQUAL(key_server_checker("bad host", NULL, NULL, "hostname"), EINVAL);
    CU_ASSERT_EQUAL(errno, EINVAL);
}

void test_key_nick_checker_valid(void) {
    errno = 0;
    CU_ASSERT_EQUAL(key_nick_checker("MyBot_1", NULL, NULL, "botname"), 0);
    CU_ASSERT_EQUAL(errno, 0);
}

void test_key_nick_checker_invalid(void) {
    errno = 0;
    CU_ASSERT_EQUAL(key_nick_checker("My Bot", NULL, NULL, "botname"), EINVAL);
    CU_ASSERT_EQUAL(errno, EINVAL);
}

void test_ConfigFileParser_valid_config(void) {
    char *path = write_temp_config(
        "# comment line\n"
        "botname = UnitBot\n"
        "hostname=irc.example.org\n"
        "port=6667\n"
        "threadlimit=4\n"
        "databasepath=/tmp/ebotula-db\n"
        "loglevel=4\n");

    CU_ASSERT_PTR_NOT_NULL_FATAL(path);

    sSetup.configfile = strdup(path);
    CU_ASSERT_PTR_NOT_NULL_FATAL(sSetup.configfile);

    CU_ASSERT_EQUAL(ConfigFileParser(), 0);
    CU_ASSERT_STRING_EQUAL(sSetup.pBotname, "UnitBot");
    CU_ASSERT_STRING_EQUAL(sSetup.sHostname, "irc.example.org");
    CU_ASSERT_STRING_EQUAL(sSetup.sPort, "6667");
    CU_ASSERT_EQUAL(sSetup.thread_limit, 4);
    CU_ASSERT_STRING_EQUAL(sSetup.pDatabasePath, "/tmp/ebotula-db");
    CU_ASSERT_EQUAL(sSetup.nLogLevel, 4);

    unlink(path);
    free(path);
}

void test_ConfigFileParser_duplicate_key_uses_first_value(void) {
    char *path = write_temp_config(
        "botname=FirstBot\n"
        "botname=SecondBot\n");

    CU_ASSERT_PTR_NOT_NULL_FATAL(path);

    sSetup.configfile = strdup(path);
    CU_ASSERT_PTR_NOT_NULL_FATAL(sSetup.configfile);

    CU_ASSERT_EQUAL(ConfigFileParser(), 0);
    CU_ASSERT_STRING_EQUAL(sSetup.pBotname, "FirstBot");

    unlink(path);
    free(path);
}
