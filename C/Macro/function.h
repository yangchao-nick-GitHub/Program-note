#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <stdio.h>
#include <stdint.h>

#define ERROR_LOG(log) fprintf(stderr,"error: "#log"\n")

#define PRINTF1(a,b)            \
    {                           \
        printf("print %d\n",a);    \
        printf("print %d\n",b);    \
    }                   

#define PRINTF2(a,b)            \
    do{                         \
        printf("print %d\n",a);    \
        printf("print %d\n",b);    \
    }while(0)

#define PRINTF3(a,b)            \
    do{                         \
        printf("a:%s %d\n",#a,b);\
    }while(0)


#endif