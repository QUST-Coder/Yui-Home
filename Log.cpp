#include "Log.h"
#include <cstdio>
#include <cstdarg>
using namespace std;

FILE* _fp;
bool log_init()
{
    return ( _fp=fopen("log.txt","w") ) != nullptr;
}

void log(const char* fmt,...)
{
    va_list va;
    va_start(va,fmt);
    vfprintf(_fp,fmt,va);
    fprintf(_fp,"\n");
    fflush(_fp);
    va_end(va);

    #ifdef DEBUG /// If is in debug mode, also print to screen.
    va_start(va,fmt);
    printf("[Log] ");
    vprintf(fmt,va);
    printf("\n");
    va_end(va);
    #endif
}
