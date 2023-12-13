#ifndef __H_STORE
#define __H_STORE
#include <gdbm.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * store dataset to database file
 * @param qDbF [in] input db file
 * @param bIgnoreQuoting [in] ignore quoting of dataset
 * @param bReplace [in] replace or just only insert dataset, insert returns an error if its already exists
 * @param sKey [in] value key
 * @param sContent [in] content data
 * @return success of access or nothing
 */
int store_gdbm(GDBM_FILE qDbF,int bIgnoreQuoting,int bReplace,const char *sKey,const char *sContent);
 #endif //__H_STORE
