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
    //TODO do the counting properly stop being lazy 
    fprintf(json, SPECIFIC_JSON_DEFAULTCONFIG,sizeof(SPECIFIC_JSON_DEFAULTCONFIG)+strlen((const char *)dir)+sizeof(s->width)+sizeof(s->height), dir, s->width, s->height);
    //TODO CHECK IF THIS GOES ALRIGHT
    XCloseDisplay(d);
    fclose(json);
}
json_object* readjson_init(const int8_t *filename){
    FILE *fp;
	size_t bytespan;
    uint32_t pos; 
    uint8_t pos2;
    //char buffer[1024];
    //TODO PROPER CHECKING 
    int8_t *needle; 
    json_object *hey;
    fp = fopen((const char *)filename,"r");
    if(fp==NULL){
        abort_(GENERAL_OPENING_READ_FILE_ERROR, " ERR01 ");
    }
    //TOOPTIMISE is this feasable? or might as well just have a giant buffer?  
    needle=malloc(1);
    do
    {
        if(*needle==':'){
            pos2=ftell(fp);
        }
        fread(needle, 1, 1,fp);
    } while (*needle!=',');
    pos=ftell(fp)-1;
    fseek(fp, pos2, SEEK_SET);
    needle=realloc(needle, pos-pos2);
    fread(needle, 1, pos-pos2, fp);
    printf("\n%s\n",needle);
	json_object *parsed_json;
    bytespan=atoi((char *)needle);
    needle=realloc(needle,bytespan);
    fseek(fp, 0, SEEK_SET);
    fread(needle, sizeof(uint8_t), bytespan, fp);
    //printf("\n%s\n", needle);
    parsed_json = json_tokener_parse((const char *)needle);
    //TODO PROPER CLOSING 
    fclose(fp);
    return parsed_json; 
}
void returnjson_resolution(json_object *obs, uint32_t *wid, uint32_t *hei){
    json_object *resolution;
    json_object *settings;
    json_object *setting;
    json_object_iter het;
    if(json_object_object_get_ex(obs, "settings",&settings)){
        setting=json_object_array_get_idx(settings, 0);
        if(json_object_object_get_ex(setting, "Resolution",&resolution)){
	        *wid = (uint32_t)json_object_get_int(json_object_array_get_idx(resolution, 0));
	        *hei = (uint32_t)json_object_get_int(json_object_array_get_idx(resolution, 1));
            printf("\n%u %u\n", *wid, *hei);
        }
    }
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