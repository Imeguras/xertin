#include "config.h"
#include "funcaux.h"
#include <X11/Xlib.h>
#include <json-c/json_object.h>
#include <json-c/json_types.h>
#include <json-c/json_util.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
struct json_object *parsed_json;

void createjson(){
    //TODO MULTIPLE DISPLAY https://stackoverflow.com/questions/25849045/screen-resolution-in-c-for-linux
    Display* d = XOpenDisplay(NULL);
    Screen*  s = DefaultScreenOfDisplay(d);
    FILE *json; 
    int8_t *dir=NULL; 
    dir=(int8_t *)GetCurrentDir((char *)dir, GENERAL_MAXSIZE_OF_DIRECTORYS);
    if (dir == NULL){
        //TODO documentate the proper error code
        ERROR(1, GENERAL_DIRING_ERROR, "ERR20");        
    }
    
    parsed_json = json_object_new_object();
    struct json_object *settingsObject = json_object_new_object();
    struct json_object *settingsArray=json_object_new_array();
    struct json_object *resolution=json_object_new_array();
    struct json_object *width=json_object_new_int64(s->width);
    struct json_object *height=json_object_new_int64(s->height); 
    json_object_array_add(resolution, width);
    json_object_array_add(resolution, height);
    json_object_object_add(settingsObject, "resolution", resolution);
    json_object_array_add(settingsArray, settingsObject);
    json_object_object_add(parsed_json, "settings", settingsArray);
    json_object_to_file(SPECIFIC_JSON_DIRECTORY, parsed_json);
    XCloseDisplay(d);
}

json_object* readjson_init(const int8_t *filename){
    FILE *fp=NULL;
    uint32_t pos; 
    int8_t *needle= NULL; 
    fp = fopen((const char *)filename,"r");
    if(fp==NULL){
        ERROR(1, GENERAL_OPENING_FILE_ERROR, "ERRX");
    }
    needle=malloc(1*sizeof(int8_t));
    if (!needle){
        ERROR(1, GENERAL_ALLOC_ERROR, "ERRX");    
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
    free(needle);
    needle=NULL;
    if(fclose(fp)){
		//TODO Documentate every othercode
		fprintf(stderr, GENERAL_CLOSING_FILE_ERROR, "ERR10");
    }
    return parsed_json; 
}
void returnjson_general(json_object *obs, int va_argc, ...){
	va_list valist;
	if (va_argc<=1){
		WARNING("To Lurk one needs to know the size of the instructions and the instructions themselves, at least two parameters must be sent");
		return; 
	}
	va_start(valist, va_argc);
	
	int32_t bufferSize=va_arg(valist, int);
	int8_t *buf = NULL;
	buf = malloc(bufferSize*sizeof(int8_t));
	if (buf){
		strcpy((char *) buf,(char *)va_arg(valist,int*));
		struct json_object *temp_obj=obs; 
		char *tkn; 
		tkn=strtok((char *) buf, "-");
		while (tkn) {
			/*int8_t delimeter=buf[0]; 
			buf=buf[1];*/ 
			switch (tkn[0]) {
				case '>':
					if(!json_object_object_get_ex(temp_obj, tkn+1, &temp_obj)){
						WARNING("Failed to find a object by the name of %s", tkn+1);
						return;
					}
				break;
				case '|':
					if(!(temp_obj=json_object_array_get_idx(temp_obj, atoi(tkn+1)))){
						WARNING("Failed to grab the %s'th element of the array",tkn+1);
						return;
					}
				break;
				case '*':
					DEBUG(tkn);
					for (uint32_t i = 0; i < (va_argc-2); i++){
						struct json_object *loc_obj;
						if(!(loc_obj=json_object_array_get_idx(temp_obj, i))){
							WARNING("Failed to grab the %s'th element of the array",tkn+1);
							return;
						}
						int * input_parameter=va_arg(valist,int*); 
						
						int result=json_object_get_int(loc_obj); 
						DEBUG(" %d, result: %d, index: %d", *input_parameter, result, i);
						*input_parameter=result; 
					}
				break;
				default:
					
					WARNING("Theres an incorrect field on the config");
				break; 
				
			}
			tkn=strtok(NULL, "-");
		}
		
	}else{
		ERROR(1, GENERAL_ALLOC_ERROR, "ERRX");
	}

	 
	DEBUG((char *)buf);
	free(buf);
	buf=NULL;
	
   /* clean memory reserved for valist */
   va_end(valist);
}
void returnjson_resolution(json_object *obs, uint32_t *wid, uint32_t *hei){
    json_object *resolution;
    json_object *settings;
    json_object *setting;
    if(json_object_object_get_ex(obs, "settings",&settings)){
        setting=json_object_array_get_idx(settings, 0);
		if(json_object_object_get_ex(setting, "resolution",&resolution)){
			*wid = (uint32_t)json_object_get_int(json_object_array_get_idx(resolution, 0));
			*hei = (uint32_t)json_object_get_int(json_object_array_get_idx(resolution, 1));
			#ifdef SHOW_DEBUG 
				printf("\nResolution in configs is:%ux%u\n", *wid, *hei);
            #endif
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