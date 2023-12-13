#ifndef __H_FETCH
#define __H_FETCH
#include <gdbm.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * fetch dataset in database file, return with success if dataset could be fetched
 * @param qDbF [in] input db file
 * @param bIgnoreQuoting [in] ignore quoting of dataset
 * @param sKey [in] value key
 * @param sContent [in] content data
 * @return success of access or nothing
 */
int fetch_gdbm(GDBM_FILE qDbF,int bIgnoreQuoting,const char *sKey,char **sContent);
 #endif //__H_FETCH
