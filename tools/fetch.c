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
int fetch_gdbm(GDBM_FILE qDbF,int bIgnoreQuoting,const char *sKey,char **Content)
{
	char *sKeyD=NULL;
	if (qDbF==NULL || sKey==NULL|| Content==NULL)
	{
		return 1;
	}
	/**
	 * dequote data and key
	 */
	if (bIgnoreQuoting==0)
	{
		sKeyD=dequote_string((char*)sKey);
	}else
	{
		sKeyD=(char*)(sKey);
	}
	int nRetVal=1;
	datum datKey;
	datKey.dptr=sKeyD;
	datKey.dsize=strlen(sKeyD)+1;
	datum datContent = gdbm_fetch(qDbF, datKey);
	if (datContent.dptr!=NULL)
	{
		nRetVal=0;
		*Content=(char*)malloc(sizeof(char)*(datContent.dsize+1));
		memset(*Content,0,sizeof(char)*(datContent.dsize));
		memcpy(*Content,datContent.dptr,sizeof(char)*(datContent.dsize));
		free(datContent.dptr);
	}
	if (bIgnoreQuoting==0)
	{
		free(sKeyD);
	}
	return nRetVal;
}
/*
void usage(char *sProgName)
{
	fprintf(stderr,"usage: %s <blocksize> <dbfile> <key>\n",sProgName);
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
	char *sKey=argv[3];
//	char *sValue=argv[4];
	GDBM_FILE qDbf;
	qDbf=gdbm_open(sDBFile,nKeySize,GDBM_WRITER,S_IREAD|S_IWRITE,0);
	datum datKey;
	datKey.dptr=sKey;
	int nKeySizeC=strlen(sKey)+1;
	if (nKeySizeC>=nKeySize)
	{
		fprintf(stderr,"Error: key length is larger as allowed keysize\n");
		exit(1);
	}


	datKey.dsize=nKeySizeC;

	datum val = gdbm_fetch(qDbf, datKey);
	if (val.dptr)
	{
		printf("%s\n",val.dptr);
	}
	printf("%d %s\n",nKeySizeC,datKey.dptr);
	int nRetVal=gdbm_exists(qDbf,datKey);
	if (!nRetVal)
	{
	fprintf(stderr,"gdbm_exists: key not found\n");
	exit(0);
	}
	nRetVal = gdbm_delete(qDbf, datKey);
        if (nRetVal!=0) {
                fprintf(stderr,"gdbm_fetch: %s\n",gdbm_strerror(gdbm_errno));
                fflush(stderr);
		exit(1);
        }
	gdbm_close(qDbf);
	return 0;
}*/
