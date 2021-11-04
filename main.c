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
#include "decoders/pngdec.h"
#include "decoders/jpegdec.h"
#include "decoders/gifdec.h"
json_object * config = NULL;
uint8_t vreadable(char *string);
//uint8_t ** (*JanelaEescrevePTR)(uint8_t **, uint32_t, uint32_t, const int8_t *);
int32_t main(int argc, char *argv[]){

    struct gengetopt_args_info args; 
    
    if (cmdline_parser(argc, argv, &args)!= 0 ){
        return 1;
    }
    if(!vreadable(SPECIFIC_JSON_DIRECTORY)){
		DEBUG("Config.json not detected creating it from scratch!");
        createjson();
    }
	config=readjson_init((const int8_t*)SPECIFIC_JSON_DIRECTORY);
	int width=0, heigh=0; 
	returnjson_general(config, 4, sizeof("->settings-|0->resolution-="), "->settings-|0->resolution-*", &width, &heigh);
	DEBUG("done: %d %d", width, heigh);
    gfx_start(config);

    if (args.decode_given&&vreadable(args.decode_arg)){
        int8_t *ext;
        //TODO well i tried finding a fuction and it seems that this one is pretty good it just a extra r in code too
        ext = (int8_t *)strrchr(args.decode_arg, '.');
        if(!ext){
            	(stderr,"[Error]: no file extension found, are you opening a text file?");
        }else{
			fprintf(stdout,"\nExtension is %s\n",ext+1);
			//todo this is a dozie maybe i can just do some basic "hash" or something to be faster in a case instead of doing the calculations over and over again
			fprintf(stdout,"[Info] The file extension was translated to %d",strtouint((uint8_t *)ext+1));
			size_t rowbytes=0;
			png_uint_32 pwidth=0, pheight=0;
			switch (strtouint((uint8_t *)ext+1)) {
				case 1886283520:
					if(startpng_init(args)){
						fprintf(stderr, "[TODO] something went to shit");
					}
					pngimp image={NULL, NULL, NULL};
					image=readpng_verificar(args.decode_arg, &rowbytes, &pwidth, &pheight);
					if (args.hex_given){
						image.vetor=RipHexTable((uint8_t **)image.vetor, pwidth,pheight, ',');
					}
					uint8_t *pont=MatrizParaVetor((uint8_t **)image.vetor, pheight, rowbytes);
					pont=displaygrap_winrite(pont,pwidth, pheight, 8, rowbytes, args.decode_arg);
					pngread_destroy(image);
					//free(pont);
					//free_image_data(matrix, pheight);
				break;
				case 1785751296:
				case 1785750887:
					if(startjpeg_init(args)){
						fprintf(stderr, "[TODO] something went to shit");
					}
					uint8_t *vetor=NULL; 
					vetor=readjpeg_verificar(vetor, args.decode_arg, &rowbytes, &pwidth, &pheight);
					if (args.hex_given){
						uint8_t **matriz=NULL;
						matriz=VetorParaMatriz(vetor, rowbytes, pheight);
						matriz=RipHexTable(matriz, rowbytes, pheight,',');
						//free(matriz);
						//matriz=NULL;
					}
					vetor=displaygrap_winrite(vetor,pwidth, pheight, 8, rowbytes,args.decode_arg);
					free(vetor);
					vetor=NULL;
					
				break;
				default:
					fprintf(stderr,"[Error]: The file you are trying to open is not supported");
				break;
			}
		}	
	}
	/*if(writejson_scratch((const int8_t*)SPECIFIC_JSON_DIRECTORY, config)){
		fprintf(stderr, "TODO somethingWentWrong" );
	}*/
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