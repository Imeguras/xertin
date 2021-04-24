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
#include "grapdec.h"
#include "config.h"
#include "decoders/pngdec.h"
#include "decoders/jpegdec.h"
#include "decoders/gifdec.h"
json_object * config;
uint8_t vreadable(char *string,uint32_t *favouredsize);
//uint8_t ** (*JanelaEescrevePTR)(uint8_t **, uint32_t, uint32_t, const int8_t *);
int32_t main(int argc, char *argv[]){
    struct gengetopt_args_info args; 
    
    if (cmdline_parser(argc, argv, &args)!= 0 ){
        return 1;
    }
    uint32_t favblock;
    if(!vreadable(SPECIFIC_JSON_DIRECTORY,&favblock)){
        createjson();
    }
    config=readjson_init((const int8_t*)SPECIFIC_JSON_DIRECTORY);
    gfx_start(config);
    if (args.decode_given&&vreadable(args.decode_arg,&favblock)){
        int8_t *ext;
        //TODO well i tried finding a fuction and it seems that this one is pretty good it just a extra r in code too
        ext = (int8_t *)strrchr(args.decode_arg, '.');
        if(!ext){
            fprintf(stderr,"[Error]: no file extension found, are you opening a text file?");
        }else{
            fprintf(stdout,"\nExtension is %s\n",ext+1);
            //todo this is a dozie maybe i can just do some basic "hash" or something to be faster in a case instead of doing the calculations over and over again
            if (!strcmp((char *)ext+1, "png")){
                
                if(startpng_init(args)){
                    fprintf(stderr, "[TODO] something went to shit");
                }
                size_t rowbytes=0;
                png_uint_32 pwidth=0, pheight=0;
                pngimp image={NULL, NULL, NULL};
                image=readpng_verificar(args.decode_arg, &rowbytes, &pwidth, &pheight);
                //RipHexTable((uint8_t **)image.vetor, pwidth,pheight, ',');
                /*uint8_t *pont=MatrizParaVetor((uint8_t **)image.vetor, pheight, rowbytes);
                pont=displaygrap_winrite(pont,pwidth, pheight, 8, rowbytes, args.decode_arg);*/
                pngread_destroy(image);
                //free(pont);
                //free_image_data(matrix, pheight);
                
            }else if (!strcmp((char *)ext+1, "jpg") || !strcmp((char *)ext+1, "jpeg")){
                if(startjpeg_init(args)){
                    fprintf(stderr, "[TODO] something went to shit");
                }
                size_t rowbytes=0;
                uint32_t pwidth=0, pheight=0;
               
                uint8_t *vetor=NULL; 
                uint8_t **matriz=NULL; 
                vetor=readjpeg_verificar(vetor, args.decode_arg, &rowbytes, &pwidth, &pheight);
                matriz=VetorParaMatriz(vetor, rowbytes, pheight);
                matriz=RipHexTable(matriz, rowbytes, pheight,',');
                //vetor=displaygrap_winrite(vetor,pwidth, pheight, 8, rowbytes,args.decode_arg);

                //vetor=displaygrap_winrite(vetor,pwidth, pheight, 8, rowbytes, args.decode_arg)
                //matriz=JanelaEescreve(matriz, pwidth, pheight, args.decode_arg);
                //uint8_t *(*JanelaEescrevePTR)(png_bytepp, uint32_t, uint32_t, const int8_t *);
                //JanelaEescrevePTR=&JanelaEescreve;
                //vetor=JanelaEescreve(vetor, pwidth, pheight, (const int8_t *)args.decode_arg);
                //
                free(vetor);
                vetor=NULL;
                //pngread_destroy(image);
                //free(matriz);
                //free_image_data(matrix, pheight);
                
            }else if (!strcmp((char *)ext+1, "gif")){
                
            }else{
                fprintf(stderr,"[Error]: The file you are trying to open is not supported");
            }
        }
        
    }
    
    cmdline_parser_free(&args);
    return 0;
}

uint8_t vreadable(char *string,uint32_t *favouredsize){
    struct stat fs;
    if (stat(string, &fs) == -1) {
        perror("There was a failure in reading the file status");
        return 0;
        exit(2);
    }
    if (fs.st_mode & R_OK){
        *favouredsize=fs.st_blksize;
        return 1;
    }else{
        return 0;
    }   
}