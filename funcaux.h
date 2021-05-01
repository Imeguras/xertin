#ifndef FUNCAUX_IMPLEMENTED
#define FUNCAUX_IMPLEMENTED
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
//PNG RELATED MACROS 
#define GENERAL_PNG_SIG_SIZE 8
#define SPECIFIC_LIBPNG_ZLIB_BUFFER_COMPRESSION_BYTE_SIZE 8192
#define SPECIFIC_LIBPNG_READ_WIDTH_MAX 65535
#define SPECIFIC_LIBPNG_READ_HEIGHT_MAX 65535
#define SPECIFIC_LIBPNG_SRGB_GAMA_MULTIPLIER 2.2
//JPEG RELATED MACROS
#define SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA 1

#define GENERAL_CLOSING_READ_FILE_ERROR "[Error]||Welp while closing the file to read something got fucked, try restarting the program? %s"
#define GENERAL_OPENING_READ_FILE_ERROR "[Error]||Welp while opening the file to read something got fucked, try restarting the program? %s"
#define GENERAL_READ_FILE_ERROR "[Error]||Welp while reading the file something got fucked, try restarting the program? %s"
#define GENERAL_ALLOC_ERROR "[ERROR]||Welp while allocating space in memory something got fucked, try restarting the program? %s"
#define GENERAL_MAXSIZE_OF_DIRECTORYS 255
#define SPECIFIC_JSON_DIRECTORY "config.json"
#define SPECIFIC_JSON_DEFAULTCONFIG "{\n\t\"size\":%lu,\n\t\"directories\":[\n\t\t{\n\t\t\t\"Workspace\": \"%s\",\n\t\t\t\"Objects\": \"/obj\",\n\t\t\t\"Coredumps\": \"/coredump\"\n\t\t}\n\t],\n\t\"settings\":[\n\t\t{\n\t\t\t\"Resolution\": [%d,%d]\n\t\t}\n\t],\n\t\"images\":[\n\t\t{\n\t\t\t\"TransparencyBackground\":[\n\t\t\t\t{\n\t\t\t\t\t\"Size\":\"1\", \n\t\t\t\t\t\"Pixels\": \"#FFFFFF\"\n\t\t\t\t}\n\t\t\t],\n\t\t\t\"FrameBackground\":[\n\t\t\t\t{\n\t\t\t\t\t\"Size\":\"1\",\n\t\t\t\t\t\"Pixels\":\"#FFFFFF\"\n\t\t\t\t}\n\t\t\t]\n\t\t}\n\t]\n}"
void abort_(const char *file, const int line , const char * s, ...);
void debug(const char *file, const int line, char *fmt, ...);
void warning(const char *file, const int line, char *fmt, ...);
void error(const char *file, const int line, int exitCode, char *fmt, ...);

#define ABORT(...) abort_(__FILE__, __LINE__, __VA_ARGS__)
/**
 * A simple macro to print in stdout some debug messages
 *
 * @param __FILE__ this is filled by the compiler, its a pointer to the name of the file where the debug was issued
 * @param __LINE__ this is filled by the compiler, the line where it was called
 * @param __VA_ARGS__ this is a list that makes it so multiple parameters can be printed and is the only parameter we should care about 
 * @return the return of the function is void                                           
 * @see debug()
 */
#define DEBUG(...) debug(__FILE__, __LINE__, __VA_ARGS__)


/**
 * A simple macro to print in stderr, insucessfull calls to functions,
 * unexpected behaviour or otherwise non critical errors as such it 
 * doesnt stop the program all together
 * and should be used to, as the function label denotes,
 * warn the user of something being wrong
 *
 * @param __FILE__ this is filled by the compiler, its a pointer to the name of the file where the warning was issued
 * @param __LINE__ this is filled by the compiler, the line where it was called 
 * @param __VA_ARGS__ this is a list that makes it so multiple parameters can be printed and is the only parameter we should care about
 * @return the return of the function is void  
 * @see warning()
 */
#define WARNING(...) warning(__FILE__, __LINE__, __VA_ARGS__)


/**
 * A simple macro to print in stderr, insucessfull calls to critical functions,
 * lacking or insuficient behaviour or otherwise critical errors to the fuction of the program
 * If the program can't continue properly without said behaviour ERROR should be called
 *
 * @param __FILE__ this is filled by the compiler, its a pointer to the name of the file where the error was issued
 * @param __LINE__ this is filled by the compiler, the line where it was called 
 * @param __VA_ARGS__ this is a list that makes it so multiple parameters can be printed and is the only parameter we should care about
 * @return the return of the function is void  
 * @see error()
 */
#define ERROR(exitCode, ...) \
	error(__FILE__, __LINE__, (exitCode), __VA_ARGS__)

uint32_t strtouint(uint8_t c[4]);
#endif

