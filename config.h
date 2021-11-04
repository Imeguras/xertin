#ifndef CONFIG_IMPLEMENTED
#define CONFIG_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h> 
#include <json-c/json.h> 
#include <string.h>
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
json_object* readjson_init(const int8_t *filename);
void returnjson_resolution(json_object *obs, uint32_t *wid, uint32_t *hei);
uint8_t writejsonfile(const int8_t *filename, json_object *obs);
void returnjson_general(json_object *obs, int va_argc, ...);
//void returnjson_Background(json_object *obs, uint32_t *size, uint8_t **matrix);
#endif