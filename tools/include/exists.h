#ifndef __H_EXISTS
#define __H_EXISTS
#include <gdbm.h>
#include <stdio.h>
#include <stdlib.h>
/**
 * exists dataset in database file, return with success if dataset could be existsd 
 * @param qDbF [in] input db file
 * @param bIgnoreQuoting [in] ignore quoting of dataset
 * @param sKey [in] value key
 * @return return 1 if Key could be found
 */
int exists_gdbm(GDBM_FILE qDbF,int bIgnoreQuoting,const char *sKey);
 #endif //__H_EXISTS
