#ifndef UTILITIES_H
#define UTILITIES_H


#define		bool	int
#define		true	1
#define		false	0

// Macro for debugging msg
#ifdef _DEBUG
	#include <stdio.h>
    #define DEBUG(str...)  fprintf(stderr,"Debug: "); \
						fprintf(stderr,str); 	\
						fprintf(stderr,"\n");	 
#else
    #define DEBUG(str...)  
#endif

	  
/**
* This function trim a string. It remove leading, folling and 
* multi spaces and  CR LF at the end.
* @param line a pointer of a string for modify
*/	  
void trim(char *line);

void stopParser(int sig);

void dialog_master(void);
/**
* clearspace() remove all spaces in a string until from double quote include spaces.
* This space in double quote untouched.
* @param line a pointer  of a string for modify
*/ 
void clearspace(char *line);

/** 
* This is  a sleep call. The time for sleeping is giving in millesecond
* @param ms the sleeping time in millisecond
*/
void millisleep(unsigned long ms);

#endif

