#ifndef CONFIG_IMPLEMENTED
#define CONFIG_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h> 
#include <json-c/json.h> 
#include "funcaux.h"
#include <X11/Xlib.h>
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif
void createjson(void);
//void returnjson_Background(json_object *obs, uint32_t *size, uint8_t **matrix);
#endif