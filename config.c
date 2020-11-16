#include "config.h"
struct json_object *parsed_json;
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
    fprintf(json, SPECIFIC_JSON_DEFAULTCONFIG, dir, s->width, s->height);
    //TODO CHECK IF THIS GOES ALRIGHT
    XCloseDisplay(d);
    fclose(json);
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