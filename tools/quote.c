#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "quote.h"
#define str_quotes " \"\\"
#define sym_quotes "\a\b\f\n\r\t\v"
char *quote_string(const char *in)
{
	if (in==NULL || strlen(in)==0)
	{
		return NULL;
	}
	// 2+2*in+1
	char *sRet=(char*)malloc((2+(2*strlen(in)+1))*sizeof(char));
	memset(sRet,0,(2+(2*strlen(in)+1)));
	char *iter,*iter2;

	sRet[0]='\"';
	iter=(char*)in;
	iter2=(char*)&sRet[1];

	while(iter!=NULL && *iter!='\0')
	{
		if (strchr(str_quotes,*iter)!=NULL)
		{
			*iter2='\\';
			iter2++;
			*iter2=*iter;
		}else if (strchr(sym_quotes,*iter)!=NULL)
		{
			*iter2='\\';
			iter2++;
			switch(*iter)
			{
				case '\a':
					*iter2='a';
					break;
				case '\b':
					*iter2='b';
					break;
				case '\f':
					*iter2='f';
					break;
				case '\n':
					*iter2='n';
					break;
				case '\r':
					*iter2='r';
					break;
				case '\t':
					*iter2='t';
					break;
				case '\v':
					*iter2='v';
					break;
			}
		}else
		{
			*iter2=*iter;
		}
		iter2++;
		iter++;
	}
	*iter2='\"';
	return sRet;
}
char *dequote_string(const char *in)
{
	if (in==NULL || strlen(in)==0)
	{
		return NULL;
	}

	// 2+2*in+1
	char *sRet=(char*)malloc(((strlen(in)+1))*sizeof(char));
	memset(sRet,0,((strlen(in)+1)));
	if (in[0]!='"')
	{
		strcpy(sRet,in);
		return sRet;
	}
	char *iter,*iter2;
	iter=(char*)in;
	iter++;
	iter2=(char*)&sRet[0];
	int bSlash=0;
	while(iter!=NULL && *iter!='\0')
	{
		if (bSlash==1)
		{
			bSlash=2;
			switch(*iter)
			{
				case 'a':
					*iter2='\a';
					break;
				case 'b':
					*iter2='\b';
					break;
				case 'f':
					*iter2='\f';
					break;
				case 'n':
					*iter2='\n';
					break;
				case 'r':
					*iter2='\r';
					break;
				case 't':
					*iter2='\t';
					break;
				case 'v':
					*iter2='\v';
					break;
				default:
					*iter2=*iter;
					break;
			}	
		}
		if (*iter=='\\' && bSlash==0)
		{
			bSlash=1;
			iter++;
			continue;
		}	
		bSlash=0;
		*iter2=*iter;
		iter++;
		if (*iter!='\0')
		{
			iter2++;
		}
	}
	*iter2='\0';
	return sRet;
}
/**
int main()
{
	char *sBuf;
	sBuf=quote_string("test 1 2 3\"");
	printf("%s\n",sBuf);
	sBuf=dequote_string(sBuf);
	printf("%s\n",sBuf);
	free(sBuf);
	return 0;
}
**/
