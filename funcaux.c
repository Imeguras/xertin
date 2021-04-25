#include "funcaux.h"
#include <stdint.h>
void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}
//TOOPTIMIZE maybe macro it? 
uint32_t strtouint(uint8_t c[4]){
    uint32_t result=0;
    result= (uint32_t)c[0]<< 24 | (uint32_t)c[1]<< 16 | (uint32_t)c[2]<< 8 | (uint32_t)c[3];
    return result;
}