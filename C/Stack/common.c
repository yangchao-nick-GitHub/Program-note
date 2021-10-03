#include <stdio.h>
#include <stdarg.h>


void log_cmd(char *fmt,...)
{
    va_list args;
    va_start(args,fmt);
    char buf[256] = {0};
    vsnprintf(buf,sizeof(buf),fmt,args);
    va_end(args);
    perror(buf);
}