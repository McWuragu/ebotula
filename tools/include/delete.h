#ifndef __H_DELETE
#define __H_DELETE
#include <gdbm.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * delete dataset in database file, return with success if dataset could be deleted 
 * @param qDbF [in] input db file
 * @param bIgnoreQuoting [in] ignore quoting of dataset
 * @param sKey [in] value key
 * @return success of access or nothing
 */
int delete_gdbm(GDBM_FILE qDbF,int bIgnoreQuoting,const char *sKey);
 #endif //__H_DELETE
