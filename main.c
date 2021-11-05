#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <png.h>
#include "args.h"
#include "funcaux.h"
#include "grapdec.h"
#include "config.h"
#include "decoders/gendec.h"

json_object * config = NULL;
uint8_t vreadable(char *string);
//uint8_t ** (*JanelaEescrevePTR)(uint8_t **, uint32_t, uint32_t, const int8_t *);
int main(int argc, char *argv[]){

    struct gengetopt_args_info args; 
    
    if (cmdline_parser(argc, argv, &args)!= 0 ){
        return 1;
    }
    if(!vreadable(SPECIFIC_JSON_DIRECTORY)){
		DEBUG("Config.json not detected creating it from scratch!");
        createjson();
    }
	config=readjson_init(SPECIFIC_JSON_DIRECTORY);
    gfx_start(config);

    if (args.decode_given&&vreadable(args.decode_arg)){
        char *ext=NULL;
        ext = strrchr(args.decode_arg, '.');
        if(ext){
			decoder_mutex(ext, args);
        }else{
			//TODO
			ERROR(1,"no file extension found, are you opening a text file?");
		}	
	}
	writejson_close();
    return 0; 
}
uint8_t vreadable(char *string){
    struct stat fs;
    if (stat(string, &fs) == -1) {
        perror("There was a failure in reading the file status");
        return 0;
        exit(2);
    }
    if (fs.st_mode & R_OK){
        return 1;
    }else{
        return 0;
    }   
}