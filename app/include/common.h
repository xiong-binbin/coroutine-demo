#ifndef MDC_COMMON_H
#define MDC_COMMON_H

#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <string.h>


#define     SUCCESS             (0)
#define     FAILED              (-1)

#define SEC2USEC(s) ((s)*1000000LL)

#define SAFE_FREE(x) do\
{\
	if(NULL != (x))\
	{\
		free(x);\
		(x)=NULL;\
	}\
}while(0)

#endif