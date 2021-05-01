#include "funcaux.h"
#include <stdio.h>

/**
 * This function shouldnt be called directly instead use the all caps macro(ABORT) version of it.
 * @param file the name of the file, normally __FILE__ 
 * @param line the line where the warning was called, normally __LINE__
 * @param fmt formatable string like in fprinf
 * @param ... its related to va_list and variable parameters
 * @return the return of the function is void
 * @see DEBUG
 */
void abort_(const char *file, const int line , const char * s, ...)
{
        va_list args;
        va_start(args, s);
        fprintf(stderr, "[%s@%d] [CRITICAL] - ", file, line);
        vfprintf(stderr, s, args);
        va_end(args);
        fprintf(stderr, "\n");
        fflush(stderr);
        abort();
}
/**
 * This function shouldnt be called directly instead use the all caps macro version of it.
 * @param file the name of the file, normally __FILE__ 
 * @param line the line where the warning was called, normally __LINE__
 * @param fmt formatable string like in fprinf
 * @param ... its related to va_list and variable parameters
 * @return the return of the function is void
 * @see DEBUG
 */
void debug(const char *file, const int line, char *fmt, ...)
{
    
    va_list ap;
    va_start(ap, fmt);
    fprintf(stdout, "[%s@%d] [DEBUG] - ", file, line);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fprintf(stdout, "\n");
    fflush(stdout);
    return; 
}


/**
 * This function shouldnt be called directly instead use the all caps macro version of it.
 * @param file the name of the file, normally __FILE__ 
 * @param line the line where the warning was called, normally __LINE__
 * @param fmt formatable string like in fprinf
 * @param ... its related to va_list and variable parameters
 * @return the return of the function is void
 * @see WARNING
 */
void warning(const char *file, const int line, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] [WARNING] - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", strerror(errno));
    fflush(stderr);
    return; 
}
/**
 * This function shouldnt be called directly instead use the all caps macro version of it.
 * @param file the name of the file, normally __FILE__ 
 * @param line the line where the warning was called, normally __LINE__
 * @param exitCode the number thats provided to end the run of the program the most common are: 0 for sucess, 1 for a minor problem, 2 for a major one
 * @param fmt formatable string like in fprinf
 * @param ... its related to va_list and variable parameters
 * @return the return of the function is void
 * @see ERROR
 */
void error(const char *file, const int line, int exitCode, char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "[%s@%d] [ERROR] - ", file, line);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, ": %s\n", strerror(errno));
    fflush(stderr);
    exit(exitCode);
}
//TOOPTIMIZE maybe macro it? 
uint32_t strtouint(uint8_t c[4]){
    uint32_t result=0;
    result= (uint32_t)c[0]<< 24 | (uint32_t)c[1]<< 16 | (uint32_t)c[2]<< 8 | (uint32_t)c[3];
    return result;
}