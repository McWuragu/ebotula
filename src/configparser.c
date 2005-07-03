/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 * ############################################################# 
 */

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "configparser.h"
#include "type.h"
#include "ircbot.h"
#include "utilities.h"
extern ConfigSetup_t sSetup;
/* standard checker */
/* unsinged int checker */
int key_uint_checker(const void * value,const void* range1,const void *range2,const char *param_name)
{
	unsigned int tmp=atoi((char*)value);
	unsigned int low_range;
	unsigned int high_range;
/*	NULL ==0 at x86 Architecture!!
 *	if (range1!=NULL)*/
	{
		low_range=(unsigned int)range1;
		if (tmp<low_range)
		{
	            fprintf(stderr,_("Parameter '%s' is invalid: %d\n"),param_name,tmp);
			errno=EDOM;
      	      return (errno);

		}
	}
	if (range2!=NULL)
	{
		high_range=(unsigned int)range2;
		if (tmp>high_range)
		{
	            fprintf(stderr,_("Parameter '%s' is invalid: %d\n"),param_name,tmp);
			errno=EDOM;
      	      return (errno);

		}
	}
	return 0;
}
/* int checker */
int key_int_checker(const void * value,const void* range1,const void *range2,const char *param_name)
{
	int tmp=atoi((char*)value);
	int low_range;
	int high_range;
/*	NULL ==0 at x86 Architecture!!
 *	if (range1!=NULL)*/
	low_range=(int)range1;

	{
		if (tmp<low_range)
		{
	            fprintf(stderr,_("Parameter '%s' is invalid: %d\n"),param_name,tmp);
			errno=EDOM;
      	      return (errno);

		}
	}
	if (range2!=NULL)
	{
		high_range=(int)range2;
		if (tmp>high_range)
		{
	            fprintf(stderr,_("Parameter '%s' is invalid: %d\n"),param_name,tmp);
			errno=EDOM;
      	      return (errno);

		}
	}
	return 0;
} /* check server name*/

int key_server_checker(const void * value,const void* range1,const void *range2,const char *param_name)
{
	if (strpbrk((char*)value,SERVER_NOT_ALLOW_CHAR)) {
           	errno=EINVAL;
            fprintf(stderr,_("The hostname is irregular.\n"));
		return errno;
      }
	return 0;
}
/* check the nickname */
int key_nick_checker(const void * value,const void* range1,const void *range2,const char *param_name)
{
	if (!NickStringCheck((char *)value)) {
      	errno=EINVAL;
            fprintf(stderr,_("The botname is irregular.\n"));
            return (errno);
	}
	return 0;
}                   
 
InputValueStruct_t vsInputConfig[VAL_COUNT]={
	{KEY_BOTNAME,PARAM_STRING,(void *) (&sSetup.pBotname),false,key_nick_checker,NULL,NULL},
	{KEY_REALNAME,PARAM_STRING,(void *) (&sSetup.realname),false,NULL,NULL,NULL},
	{KEY_EXEUSER,PARAM_STRING,(void *) (&sSetup.sExeUser),false,NULL,NULL,NULL},
	{KEY_EXEGROUP,PARAM_STRING,(void *) (&sSetup.sExeGroup),false,NULL,NULL,NULL},
	{KEY_THREADLIMIT,PARAM_SHORT,(void *) (&sSetup.thread_limit),false,key_int_checker,(void*)1,(void *)MAX_THREADS_LIMIT},
	{KEY_SERVER,PARAM_STRING,(void *) (&sSetup.sHostname),false,key_server_checker,NULL,NULL},
	{KEY_PORT,PARAM_STRING,(void *) (&sSetup.sPort),false,key_int_checker,(void *)1,(void*)(65536)},
	{KEY_DATABASEPATH,PARAM_STRING,(void *) (&sSetup.pDatabasePath),false,NULL,NULL,NULL},
	{KEY_FASTSENDDELAY,PARAM_INT,(void *) (&sSetup.iSendDelay),false,key_int_checker,(void*)0,NULL},
	{KEY_SLOWSENDDELAY,PARAM_INT,(void *) (&sSetup.nSlowSendDelay),false,key_int_checker,(void*)0,NULL},
	{KEY_FASTSENDLIMIT,PARAM_INT,(void *) (&sSetup.nFastSendingCharLimit),false,key_int_checker,(void*)0,NULL},
	{KEY_INITDELAY,PARAM_INT,(void *) (&sSetup.nSettling),false,key_int_checker,(void*)0,NULL},
	{KEY_ALT,PARAM_INT,(void *) (&sSetup.AccountLiveTime),false,key_int_checker,(void*)0,NULL},
	{KEY_AUTOLOGOFF,PARAM_INT,(void *) (&sSetup.AutoLoggoff),false,key_int_checker,(void *)1,NULL},
	{KEY_PINGTIMEOUT,PARAM_INT,(void *) (&sSetup.iTimeout),false,key_int_checker,(void*)1,NULL},
	{KEY_LOGLEVEL,PARAM_INT,(void *) (&sSetup.nLogLevel),false,key_int_checker,(void *)0,(void*)(MAX_LOGLEVEL)}
};
/*############################################################################# */
int ConfigFileParser(void){
    FILE *fd;
    int counter;
    int elem=0;
    int err=0;
    char **retString;
    char buffer[MAX_READ_BUFFER_SIZE], *c,*value,*key;
    /*errno=0; */
     char *pChar;
     short *pShort;
     int *pInt;
	long *pLong;
	float *pFloat;
	double *pDouble;	
     
    logger(LOG_NOTICE,_("Read configuration file %s"),sSetup.configfile);

    if (!(fd=fopen(sSetup.configfile,"r"))) {
	    /* generating basicconfig for ebotula */
        /* write_baseconfig(sSetup.configfile);*/
    }else {
        logger(LOG_DEBUG,_("File is open"),sSetup.configfile);
    
        while((fgets(buffer,MAX_READ_BUFFER_SIZE,fd)!=NULL)){
            /* remove newline and leading spaces */
            trim(buffer);
            clearspace(buffer);
            
            /* ignore space lines and comments */
            if ((buffer[0]!=COMMENT_CHAR) && (buffer[0]!='\0')) {
                logger(LOG_INFO,_("Found config line %s"),buffer);
		}else
			  continue;
    
                c=strchr(buffer,'=');
                
                /* parse the value from the line */
                value=(char *)malloc((strlen(c)+1)*sizeof(char));
                strcpy(value,c+1);
                
                /* parse the key from the line */
                key=(char *)malloc((strlen(buffer)-strlen(c)+1)*sizeof(char));
                strtok(buffer,"=");
                strcpy(key,buffer);
                
		    for(counter=0;counter<VAL_COUNT;counter++)
		    {
				/* search for param */
				if (!strcmp(vsInputConfig[counter].sParamName,key))
				{
					  /* param was already set */
					  if (vsInputConfig[counter].isFound)
					  {
		  		                fprintf(stderr,_("Value already set: %s\n"),key);
						    logger(LOG_INFO,_("Value already set: %s\n"),key);
						    elem=0;
						    break;
					  }else
					  {	
						    elem=1;
						    break;
					  }
				}
		    }
		    if (!elem)
				continue;
		    else
		    {
		    		/* check for checker function */
	  	 		if (vsInputConfig[counter].check_param!=NULL)
				{
				  	if ((err=vsInputConfig[counter].check_param(value,vsInputConfig[counter].pRange1,vsInputConfig[counter].pRange2,vsInputConfig[counter].sParamName))!=0)
				   	{
				     		/*error found */
							fclose(fd);
						     exit(err);
			   		}
			 	}
				switch(vsInputConfig[counter].iParamType)
				{
					case PARAM_CHAR:
						  
/*						(char)(*(char *)vsInputConfig[counter].pParam)=(char)value[0];*/
						pChar=((char *)vsInputConfig[counter].pParam);
					  	*pChar=(char)value[0];
						break;
					case PARAM_SHORT:
					/*	(short)(*(short *)vsInputConfig[counter].pParam)=(short)atoi(value);*/
						pShort=((short *)vsInputConfig[counter].pParam);
						*pShort=(short)atoi(value);
						
					 break;
					case PARAM_INT:
						/*(int)(*(int *)vsInputConfig[counter].pParam)=(int)atoi(value);
*/
						pInt=((int *)vsInputConfig[counter].pParam);
						*pInt=(int)atoi(value);
					break;
					case PARAM_LONG:
/*						(long)(*(long *)vsInputConfig[counter].pParam)=(long)atol(value);*/
						pLong=((long *)vsInputConfig[counter].pParam);
						*pLong=(long)atol(value);
						break;
					case PARAM_FLOAT:
						/*(float)(*(float *)vsInputConfig[counter].pParam)=(float)atof(value);*/
						pFloat=((float *)vsInputConfig[counter].pParam);
						*pFloat=(float)atof(value);
						break;
					case PARAM_DOUBLE:
						/*(double)(*(double *)vsInputConfig[counter].pParam)=(double)atof(value);*/
						pDouble=((double *)vsInputConfig[counter].pParam);
						*pDouble=(double)atof(value);
						break;
					case PARAM_HEX:
					case PARAM_STRING:
						retString=(char**)vsInputConfig[counter].pParam;
						if (*retString!=NULL)
							  free(*retString);
						if ((*retString=(char*)malloc(strlen(value)+1))==NULL)
						{
							exit(1);
						}
						strcpy(*retString,(char*)value);
						break;
					default:
						logger(LOG_EMERG,_("Unknown parse type '%d'! Check your maschine"),vsInputConfig[counter].iParamType);
							exit(-1);
						break;
					}
					vsInputConfig[counter].isFound=true;
			     
				 
			}
				
                free(value);
                free(key);
        }
        fclose(fd);
    }
    return 0;
}


