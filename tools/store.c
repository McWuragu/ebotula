#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gdbm.h>
#include "quote.h"
int store_gdbm(GDBM_FILE qDbF,int bIgnoreQuoting,int bReplace,const char *sKey,const char *sContent)
{
	char *sKeyD=NULL;
	char *sContentD=NULL;
	if (qDbF==NULL || sKey==NULL || sContent==NULL)
	{
		return 1;
	}
	/**
	 * dequote data and key
	 */
	if (bIgnoreQuoting==0)
	{
		sKeyD=dequote_string((char*)sKey);
		sContentD=dequote_string((char*)sContent);
	}else
	{
		sKeyD=(char*)(sKey);
		sContentD=(char*)(sContent);
	}
	datum datKey,datContent;
	datKey.dptr=sKeyD;
	datKey.dsize=strlen(sKeyD)+1;
	datContent.dptr=sContentD;
	datContent.dsize=strlen(sContentD)+1;
	int nFlags=bReplace==1?GDBM_REPLACE:GDBM_INSERT;
	int nRet=gdbm_store(qDbF, datKey, datContent,nFlags);
	if (bIgnoreQuoting==0)
	{
		free(sKeyD);
		free(sContentD);
	}	
	return nRet;
}
/*
void usage(char *sProgName)
{
	fprintf(stderr,"usage: %s <keysize> <dbfile> <key> <value>\n",sProgName);
}
int main(int argc, char *argv[])
{
	if (argc<4)
	{
		usage(argv[0]);
		exit(1);
	}
	int nKeySize=atoi(argv[1]);
	char *sDBFile=argv[2];
	char *sKey=dequote_string(argv[3]);
	char *sValue=dequote_string(argv[4]);
	GDBM_FILE qDbf;
	qDbf=gdbm_open(sDBFile,nKeySize,GDBM_WRCREAT,S_IREAD|S_IWRITE,0);
	datum datKey,datValue;
	datKey.dptr=sKey;
	int nKeySizeC=strlen(sKey)+1;
	if (nKeySizeC>=nKeySize)
	{
		fprintf(stderr,"Error: key length is larger as allowed keysize\n");
		free(sKey);
		free(sValue);
		exit(1);
	}
	printf("%s %d\n%s %d\n",sKey,nKeySize,sValue,strlen(sValue));
	datKey.dsize=nKeySizeC;
	datValue.dptr=sValue;
	datValue.dsize=strlen(sValue)+1;
	int nRet=gdbm_store(qDbf, datKey, datValue, GDBM_REPLACE);
	gdbm_close(qDbf);
	free(sKey);
	free(sValue);

	return nRet;
}
*/
