#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <gdbm.h>
#include "dump.h"
#include "store.h"
#include "delete.h"
#include "fetch.h"
#include "exists.h"
void usage(char *sProgName)
{
	fprintf(stderr,"usage: %s <blocksize> <dbfile> <command> [<cmd-parameter>]\n",sProgName);
	fprintf(stderr,"   dump   - dumps database to stdout (default) or file\n");
	fprintf(stderr,"   store  - store (replace) dataset (key,value) in database\n");
	fprintf(stderr,"   del	  - delete dataset referenced by key in database\n");
	fprintf(stderr,"   fetch  - fetch dataset referenced by key from database\n");
	fprintf(stderr,"   exists - check if dataset exisits referenced by key in database\n");
	fprintf(stderr," example: %s 512 user.dbf dump user.dbf.dump\n",sProgName);
}
enum { UNKNOWN=-1,DUMP,STORE,DEL,FETCH,EXISTS} CmdType;
typedef struct
{
	const char* sCmdName;
	int	nCmdID;
} Cmd_t;
Cmd_t g_cmds[]={	
		{"dump",DUMP},
		{"store",STORE}	,
		{"del",DEL},
		{"fetch",FETCH}
		,{"exists",EXISTS}
		,{NULL,UNKNOWN}};
char *strtolower(const char* sIn)
{
	char *sRet=NULL;
	if (sIn==NULL)
	{
		return sRet;
	}
	sRet=malloc((strlen(sIn)+1)*sizeof(char));
	memset(sRet,0,(strlen(sIn)+1)*sizeof(char));
	char *sIter,*sIter2;
	sIter=(char*)sIn;
	sIter2=(char*)sRet;
	while((*sIter)!='\0')
	{
		*sIter2=tolower(*sIter);
		sIter++;
		sIter2++;
	}
	return sRet;
}
int main(int argc,char *argv[])
{
	if (argc<4)
	{
		usage(argv[0]);
		exit(2);
	}
	int nBlockSize=atoi(argv[1]);
	if (nBlockSize==0)
	{
		fprintf(stderr,"Error: Invalid blocksize\n");
		exit(2);
	}
	char *sDBFileName=argv[2];
	char *sCmd=strtolower(argv[3]);
	Cmd_t *pCmds=g_cmds;
	int nType=UNKNOWN;
	for(;pCmds->sCmdName!=NULL;pCmds++)
	{	
		if (!strcmp(pCmds->sCmdName,sCmd))
		{
			nType=pCmds->nCmdID;
			break;
		}
	}
	free(sCmd);
	if (nType==UNKNOWN)
	{
		fprintf(stderr,"Error: Unknown command \"%s\"\n",argv[3]);
		exit(2);
	}
	GDBM_FILE qDbF;
	qDbF=gdbm_open(sDBFileName,nBlockSize,GDBM_WRCREAT,S_IREAD|S_IWRITE,0);
	if (qDbF==NULL)
	{
		fprintf(stderr,"Error: gdbm_open error \"%s\"\n",gdbm_strerror(errno));
		exit(2);
	}
	FILE* fOutputFile=stdout;
	int nRet;
	char *sValue;
	switch(nType)
	{
		case DUMP:
			if (argc>=5)
			{
				/**
				 * open output file
				 */
				if ((fOutputFile=fopen(argv[4],"w"))==NULL)
				{
					fprintf(stderr,"Error: Failed to open \"%s\": \"%s\"\n",argv[4],strerror(errno));
					gdbm_close(qDbF);
					exit(2);
				}
				
			}
			nRet=dump_gdbm(qDbF,fOutputFile,1);
			if (fOutputFile!=stdout)
			{
					fclose(fOutputFile);
			}
			if (nRet!=0)
			{
				fprintf(stderr,"Error: dump error \"%s\"\n",gdbm_strerror(errno));
				gdbm_close(qDbF);
				exit(2);
			}
		break;
		case STORE:
			if (argc<6)
			{
				fprintf(stderr,"Error: To less parameter for command store. Command store requires Key Value pair\n");
				gdbm_close(qDbF);
				exit(2);
			}
			nRet=store_gdbm(qDbF,0,1,argv[4],argv[5]);
			if (nRet!=0)
			{
				fprintf(stderr,"Error: store error \"%s\"\n",gdbm_strerror(errno));
				gdbm_close(qDbF);
				exit(2);
			}

		break;
		case DEL:
			if (argc<5)
			{
				fprintf(stderr,"Error: To less parameter for command del. Command store requires a key.\n");
				gdbm_close(qDbF);
				exit(2);
			}
			nRet=delete_gdbm(qDbF,0,argv[4]);
			if (nRet!=0)
			{
				fprintf(stderr,"Error: delete error \"%s\"\n",nRet==-1?"Key not present":gdbm_strerror(errno));
				gdbm_close(qDbF);
				exit(2);
			}

		break;
		case FETCH:
			if (argc<5)
			{
				fprintf(stderr,"Error: To less parameter for command fetch. Command store requires a key.\n");
				gdbm_close(qDbF);
				exit(2);
			}
			nRet=fetch_gdbm(qDbF,0,argv[4],&sValue);
			if (nRet!=0)
			{
				fprintf(stderr,"Error: fetch error \"%s\"\n",gdbm_strerror(errno));
				free(sValue);
				gdbm_close(qDbF);
				exit(2);
			}
			fprintf(stdout,"%s\n",sValue);
			free(sValue);

		break;
		case EXISTS:
			if (argc<5)
			{
				gdbm_close(qDbF);
				exit(0);
			}
			nRet=exists_gdbm(qDbF,0,argv[4]);
			fprintf(stderr,"Key %sfound\n",nRet==1?"":"not ");

		break;
	}
	gdbm_close(qDbF);
	return nRet;
}
