#include <gdbm.h>
#include <stdio.h>


int main(int argc, char *argv[]) {
	GDBM_FILE  dbf;
	datum value,key,nextkey;

	dbf=gdbm_open(argv[1],512,GDBM_READER,0,NULL);

	key=gdbm_firstkey(dbf);

	while (key.dptr) {
		value=gdbm_fetch(dbf,key);
		printf("%s\t\t| %s\n",key.dptr,value.dptr);
		nextkey=gdbm_nextkey(dbf,key);
		key=nextkey;
	}
	gdbm_close(dbf);

	return 0;
}
