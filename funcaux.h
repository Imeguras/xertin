#ifndef FUNCAUX_IMPLEMENTED
#define FUNCAUX_IMPLEMENTED
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
//PNG RELATED MACROS 
#define GENERAL_PNG_SIG_SIZE 8
#define SPECIFIC_LIBPNG_ZLIB_BUFFER_COMPRESSION_BYTE_SIZE 8192
#define SPECIFIC_LIBPNG_READ_WIDTH_MAX 65535
#define SPECIFIC_LIBPNG_READ_HEIGHT_MAX 65535
#define SPECIFIC_LIBPNG_SRGB_GAMA_MULTIPLIER 2.2

#define GENERAL_OPENING_READ_FILE_ERROR "[Error]||Welp while opening the file to read something got fucked, try restarting the program? %s"
#define GENERAL_READ_FILE_ERROR "[Error]||Welp while reading the file something got fucked, try restarting the program? %s"
#define GENERAL_ALLOC_ERROR "[ERROR]||Welp while allocating space in memory something got fucked, try restarting the program? %s"



 void abort_(const char * s, ...);
#endif