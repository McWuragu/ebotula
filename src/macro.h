#ifndef MACRO_H
#define MACRO_H

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

#endif
