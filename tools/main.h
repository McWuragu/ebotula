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

static unsigned int GDBM_BLOCKSIZE=512;

enum { UNKNOWN=-1,DUMP,STORE,DEL,FETCH,EXISTS} CmdType;
typedef struct
{
	const char* sCmdName;
	int	nCmdID;
} Cmd_t;
