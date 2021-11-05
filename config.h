#ifndef CONFIG_IMPLEMENTED
#define CONFIG_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h> 
#include <json-c/json.h> 
#include <string.h>
#include "funcaux.h"
#include <X11/Xlib.h>
//#define RETURNJSON_GENERAL(obs,...) (returnjson_general(obs,strlen()))
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

void writejson_close(void);
void createjson(void);
json_object* readjson_init(const int8_t *filename);
void returnjson_general(json_object *obs, int va_argc, ...);

//void returnjson_Background(json_object *obs, uint32_t *size, uint8_t **matrix);
#endif