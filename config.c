#include "config.h"
#include "funcaux.h"
#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <stdint.h>
#include <stdio.h>
struct json_object *parsed_json;
//TODO lets not write that much times 
static uint8_t dirtyBit=0; 
void createjson(){
    //TODO MULTIPLE DISPLAY https://stackoverflow.com/questions/25849045/screen-resolution-in-c-for-linux
    Display* d = XOpenDisplay(NULL);
    Screen*  s = DefaultScreenOfDisplay(d);
    FILE *json; 
    int8_t *dir=NULL; 
    dir=(int8_t *)GetCurrentDir((char *)dir, GENERAL_MAXSIZE_OF_DIRECTORYS);
    if (dir == NULL){
        fprintf(stderr, "TODO, probs the size of dir was to big or some problem ");
        //TODO DOCOMENTATE THIS 
        exit(20); 
    }
    //fprintf(stdout, "\nEHEGJEAN %s\n", dir);
    json=fopen(SPECIFIC_JSON_DIRECTORY, "w");
    //TODO BETTER CHECKING OF THE WRITTING(PERMISSIONS,etc...)
    if (json==NULL){
        abort_(GENERAL_OPENING_READ_FILE_ERROR, "TODO FIX THIS ERROR MESSAGE");
    }
    //TODO do the counting properly stop being lazy 
    fprintf(json, SPECIFIC_JSON_DEFAULTCONFIG,sizeof(SPECIFIC_JSON_DEFAULTCONFIG)+strlen((const char *)dir)+sizeof(s->width)+sizeof(s->height), dir, s->width, s->height);
    //TODO CHECK IF THIS GOES ALRIGHT
    XCloseDisplay(d);
    fclose(json);
}
json_object* readjson_init(const int8_t *filename){
    FILE *fp=NULL;
    uint32_t pos; 
    int8_t *needle= NULL; 
    fp = fopen((const char *)filename,"r");
    if(fp==NULL){
        abort_(GENERAL_OPENING_READ_FILE_ERROR, " ERR01 ");
    }
    needle=malloc(1*sizeof(int8_t));
    if (!needle){
        abort_(GENERAL_ALLOC_ERROR, " ERR02 ");
    }
    *needle='\0';
    fseek(fp, 0, SEEK_END);
    pos=ftell(fp);
    #ifdef SHOW_DEBUG
        fprintf(stdout, "The file size is: %d\n", pos);
    #endif 
    
    fseek(fp, 0, SEEK_SET);
    needle=realloc(needle, pos);
    if(fread(needle, 1, pos, fp)!= pos){
        fprintf(stderr, GENERAL_READ_FILE_ERROR, "ERR10");
    }
    #ifdef SHOW_DEBUG
        fprintf(stdout, "Config is:\n%s\n", needle);
    #endif 
	json_object *parsed_json;
    parsed_json = json_tokener_parse((const char *)needle);
    updatejson_size(parsed_json, pos);
    free(needle);
    needle=NULL;
    if(fclose(fp)){
        //TODO Documentate every othercode
        fprintf(stderr, GENERAL_CLOSING_READ_FILE_ERROR, "ERR10");
    }
    return parsed_json; 
}
void returnjson_resolution(json_object *obs, uint32_t *wid, uint32_t *hei){
    json_object *resolution;
    json_object *settings;
    json_object *setting;
    if(json_object_object_get_ex(obs, "settings",&settings)){
        setting=json_object_array_get_idx(settings, 0);
        if(json_object_object_get_ex(setting, "Resolution",&resolution)){
	        *wid = (uint32_t)json_object_get_int(json_object_array_get_idx(resolution, 0));
	        *hei = (uint32_t)json_object_get_int(json_object_array_get_idx(resolution, 1));
            #ifdef SHOW_DEBUG 
                printf("\nResolution in configs is:%ux%u\n", *wid, *hei);
            #endif
        }
    }
}
uint8_t updatejson_size(json_object *obs, size_t size){
    uint8_t errList=0; 
    json_object *sizeO=NULL;
    json_object_object_get_ex(obs, "size", &sizeO);
    json_object_set_uint64(sizeO, (uint64_t) size);
    return errList;
}
uint8_t writejsonfile(const int8_t *filename, json_object *obs){
    uint8_t errList=0;
    FILE *fp=NULL;
    if ((fp = fopen((const char *)filename, "w+")) == NULL){
        errList+=2;  
        fprintf(stderr, "TODO-Something went wrong");
    }
    if(fprintf(fp, "%s", json_object_get_string(obs))!=json_object_get_string_len(obs)){
        errList++; 
        fprintf(stderr, "TODO-Something went wrong");
    }
    if (fclose(fp) != 0){
        fprintf(stderr, "TODO-Something went wrong");
    }
    
    return errList;
}
/*void returnjson_Background(json_object *obs, uint32_t *size, uint8_t **matrix){
    *size=json_object_get_int(obs);
    matrix=malloc((*size)*sizeof(uint8_t));
    for (size_t y = 0; y < *size; ++y){
        matrix[y]=malloc((*size)*sizeof(uint8_t));
        for (size_t x = 0; x < *size; x++){
            //matrix[y][x]=JSON_get
        }
   }
   
}*/