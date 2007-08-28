#ifndef __H_DUMP
#define __H_DUMP
#include <stdio.h>
#include <stdlib.h>
#include <gdbm.h>
/**
 * dump dbbase to stream
 * @param qDbF [in] input db file
 * @param fOutStream [in] pointer to output stream
 * @param bQuoteOutput [in] enable or disable output quoting
 * @return success of access or nothing
 */
int dump_gdbm(GDBM_FILE qDbF,FILE *fOutStream,int bQuoteOutput);
#endif //__H_DUMP
