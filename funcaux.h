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
//JPEG RELATED MACROS
#define SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA 1


#define GENERAL_OPENING_READ_FILE_ERROR "[Error]||Welp while opening the file to read something got fucked, try restarting the program? %s"
#define GENERAL_READ_FILE_ERROR "[Error]||Welp while reading the file something got fucked, try restarting the program? %s"
#define GENERAL_ALLOC_ERROR "[ERROR]||Welp while allocating space in memory something got fucked, try restarting the program? %s"
#define GENERAL_MAXSIZE_OF_DIRECTORYS 255
#define SPECIFIC_JSON_DIRECTORY "config.json"
#define SPECIFIC_JSON_DEFAULTCONFIG "{\n\t\"size\":%lu,\n\t\"directories\":[\n\t\t{\n\t\t\t\"Workspace\": \"%s\",\n\t\t\t\"Objects\": \"/obj\",\n\t\t\t\"Coredumps\": \"/coredump\"\n\t\t}\n\t],\n\t\"settings\":[\n\t\t{\n\t\t\t\"Resolution\": [%d,%d]\n\t\t}\n\t],\n\t\"images\":[\n\t\t{\n\t\t\t\"TransparencyBackground\":[\n\t\t\t\t{\n\t\t\t\t\t\"Size\":\"1\", \n\t\t\t\t\t\"Pixels\": \"#FFFFFF\"\n\t\t\t\t}\n\t\t\t],\n\t\t\t\"FrameBackground\":[\n\t\t\t\t{\n\t\t\t\t\t\"Size\":\"1\",\n\t\t\t\t\t\"Pixels\":\"#FFFFFF\"\n\t\t\t\t}\n\t\t\t]\n\t\t}\n\t]\n}"
 void abort_(const char * s, ...);
#endif

