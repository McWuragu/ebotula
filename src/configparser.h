#ifndef __H_CONFIG_FILE_PARSER
#define __H_CONFIG_FILE_PARSER
#define MAX_COMMENT_LINES 25
typedef struct 
{
	char *sParamName;
	int iParamType;
	void *pParam;
	int isFound;
	int (*check_param)(const void *param,const void *range1,const void *range2,const char *param_name);
	void *pRange1;
	void *pRange2;
}InputValueStruct_t;
enum ErrCodes{
	  No_Error=0,
	  EndOfFile=1,
	  ErrUnkownParam=-1,
	  ErrInvalidParam=-2,
	  ErrNoParam=-3,
        ErrOnMalloc=-4

};
/**
 * read configfile
 *
 */
int ConfigFileParser(void);
#endif
