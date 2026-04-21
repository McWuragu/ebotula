/* #############################################################
 *
 *  This file is a part of ebotula testsuit.
 *
 * #############################################################
 */

#ifndef TESTSUIT_INCLUDE_PARSER_PIPELINE_TEST_H_
#define TESTSUIT_INCLUDE_PARSER_PIPELINE_TEST_H_

#include "testsuit.h"
#include "parser.h"

int init_parser_pipeline(void);
int clean_parser_pipeline(void);

void test_preParser_ping_calls_pong_direct(void);
void test_preParser_identifies_bot_command(void);
void test_preParser_identifies_ctcp_command(void);
void test_preParser_unknown_message_no_dispatch(void);

#define NUMBER_OF_PARSER_PIPELINE_TESTS 4

static strTestDesc_t pstrParserPipelineTestSet[NUMBER_OF_PARSER_PIPELINE_TESTS] = {
    {test_preParser_ping_calls_pong_direct,        "preParser(): handles PING synchronously"},
    {test_preParser_identifies_bot_command,        "preParser(): identifies !help command"},
    {test_preParser_identifies_ctcp_command,       "preParser(): identifies CTCP VERSION"},
    {test_preParser_unknown_message_no_dispatch,   "preParser(): ignores unknown message"}
};

#endif /* TESTSUIT_INCLUDE_PARSER_PIPELINE_TEST_H_ */
