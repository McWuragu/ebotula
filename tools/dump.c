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
#include "dump.h"
int dump_gdbm(GDBM_FILE qDbF,FILE *fOutStream,int bQuoteOutput)
{
	datum datKey,datNextKey,datContent;
	char *sKeyName=NULL;
	char *sContent=NULL;
	/**
	 * db file is not opened
	 */
	if (qDbF==NULL)
	{
		return 1;
	}
	/**
	 * gets first key
	 */
	datKey=gdbm_firstkey(qDbF);
	/**
	 * loop while key value is not empty
	 */
	while(datKey.dptr)
	{
		/**
		 * get next key
		 */
		datNextKey=gdbm_nextkey(qDbF,datKey);
		/**
		 * fetch content
		 */
		datContent=gdbm_fetch(qDbF,datKey);
		if (datContent.dptr!=NULL)
		{
			/**
			 * quote output
			 */
			if (bQuoteOutput==1)
			{
				sKeyName=quote_string(datKey.dptr);
				sContent=quote_string(datContent.dptr);
			}else
			{
				sKeyName=(char*)malloc((datKey.dsize+1)*sizeof(char));
				sContent=(char*)malloc((datContent.dsize+1)*sizeof(char));
			}//if (bQuoteOutput==1)
			fprintf(fOutStream,"%s %s\n",sKeyName,sContent);
			free(sKeyName);
			free(sContent);
		}// datContent.dptr
                free(datKey.dptr);
                free(datContent.dptr);
                datKey = datNextKey;
	}// while(datKey.dptr)
	return 0;
}
/*void usage(char *sProgName)
{
	fprintf(stderr,"usage: %s <blocksize> <dbfile>\n",sProgName);
}
int main(int argc, char *argv[])
{
	if (argc<3)
	{
		usage(argv[0]);
		exit(1);
	}
	int nKeySize=atoi(argv[1]);
	GDBM_FILE qDbf;
	qDbf=gdbm_open(argv[2],nKeySize,GDBM_READER,S_IREAD|S_IWRITE,0);
	datum key1,nextkey,content1;
	key1= gdbm_firstkey(qDbf);
	char *myName,*value;
        while (key1.dptr) {
                nextkey = gdbm_nextkey(qDbf, key1);
                myName = (char*)malloc(key1.dsize+1);
		memcpy(myName, key1.dptr, key1.dsize);
                content1 = gdbm_fetch(qDbf, key1);
                if (content1.dptr!=0) {
                        value =(char*) malloc(content1.dsize);
                        memcpy(value, content1.dptr, content1.dsize);
		       // fprintf(stdout,"%s %s",key1.dptr,content1.dptr);
			char *sBuf1=quote_string(key1.dptr);
       			char *sBuf2=quote_string(content1.dptr);
       		        fprintf(stdout,"%s %s",sBuf1,sBuf2);
			free(sBuf1);
			free(sBuf2);
                        fflush(stdout);
                        fprintf(stderr,"\n");
                } else {
                        value = (char*)malloc(nKeySize);
                        strncpy(value, "?UNKNOWN?",nKeySize);
                }
                free(key1.dptr);
                free(content1.dptr);
                free(value);
		free(myName);
                key1 = nextkey;
        }
	gdbm_close(qDbf);
	return 0;
}
*/
