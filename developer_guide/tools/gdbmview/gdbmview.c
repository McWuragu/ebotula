#include <gdbm.h>
#include <stdio.h>
#include <stdlib.h>

#define TAB	16

int main(int argc, char *argv[]) {
	GDBM_FILE  dbf;
	datum value,key,nextkey;
	int i,c=0;

	if (!(dbf=gdbm_open(argv[1],512,GDBM_READER,0,NULL))) {
		perror("Can't open the file");
		exit(-1);
	}

	// header
	printf("Key");
	for (i=0;i<(TAB-4);i++){
		printf(" ");
	}
	printf("| Value\n");
	printf("----------------------------------------------------------------\n");
	
	key=gdbm_firstkey(dbf);
	
	// lines
	while (key.dptr) {
		c++;
		value=gdbm_fetch(dbf,key);
		printf("%s",key.dptr);
		
		for (i=0;i<(TAB-key.dsize);i++){
			printf(" ");
		}
		printf("| %s\n",value.dptr);
		nextkey=gdbm_nextkey(dbf,key);
		key=nextkey;
	}
	printf("----------------------------------------------------------------\n");
	printf("Found %d itmes\n",c);
	
	gdbm_close(dbf);
	return 0;
}
