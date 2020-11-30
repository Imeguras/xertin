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
#include "pngdec.h"
#include "jpegdec.h"
#include "gifdec.h"
#include "grapdec.h"
#include "config.h"
json_object * config;
uint8_t vreadable(char *string,uint32_t *favouredsize);
uint8_t standartout=STDOUT_FILENO;
uint8_t standarterr=STDERR_FILENO;
//uint8_t ** (*JanelaEescrevePTR)(uint8_t **, uint32_t, uint32_t, const int8_t *);
int32_t main(int argc, char *argv[]){
    struct gengetopt_args_info args; 
    //TODO VERIFY THAT THIS DOESNT BREAK MAGICALLY!
    cmdline_parser(argc,argv, &args);
    uint32_t favblock;
    if(!vreadable(SPECIFIC_JSON_DIRECTORY,&favblock)){
        createjson();
    }
    config=readjson_init((const int8_t*)SPECIFIC_JSON_DIRECTORY);
    gfx_start(config);
    if (args.decode_given&&vreadable(args.decode_arg,&favblock)){
        int8_t *ext;
        //TODO seriously this fuction god im so lazy anyway a better way of rapidly checking if its a png will be needed
        ext = (int8_t *)strchr(args.decode_arg, '.');
        if(!ext){
            dprintf(standarterr,"[Error]: no file extension found, are you opening a text file?");
        }else{
            dprintf(standartout,"\nExtension is %s\n",ext+1);
            if (!strcmp((char *)ext+1, "png")){
                //uint8_t *done;
                if(startpng_init(args)){
                    fprintf(stderr, "[TODO] something went to shit");
                }
                size_t rowbytes=0;
                png_uint_32 pwidth=0, pheight=0;
                
                pngimp image={NULL, NULL, NULL};
                //matrix=readpng_verificar(&nome,&rowbytes, &pwidth, &pheight);
                image=readpng_verificar(args.decode_arg, &rowbytes, &pwidth, &pheight);
                uint8_t *pont=MatrizParaVetor((uint8_t **)image.vetor, pheight, rowbytes);
                pont=displaygrap_winrite(pont,pwidth, pheight, 8, rowbytes, args.decode_arg);
                pngread_destroy(image);
                free(pont);
                //free_image_data(matrix, pheight);
                
            }else if (!strcmp((char *)ext+1, "jpg") || !strcmp((char *)ext+1, "jpeg")){
                if(startjpeg_init(args)){
                    fprintf(stderr, "[TODO] something went to shit");
                }
                size_t rowbytes=0;
                uint32_t pwidth=0, pheight=0;
               
                uint8_t *vetor=NULL; 
                uint8_t **matriz=NULL;
                vetor=readjpeg_verificar(matriz, args.decode_arg, &rowbytes, &pwidth, &pheight);
                //vetor=MatrizParaVetor(matriz, pheight, rowbytes);
                vetor=displaygrap_winrite(vetor,pwidth, pheight, 8, rowbytes,args.decode_arg);
                //matriz=JanelaEescreve(matriz, pwidth, pheight, args.decode_arg);
                //uint8_t *(*JanelaEescrevePTR)(png_bytepp, uint32_t, uint32_t, const int8_t *);
                //JanelaEescrevePTR=&JanelaEescreve;
                //vetor=JanelaEescreve(vetor, pwidth, pheight, (const int8_t *)args.decode_arg);
                //
                //free(vetor);
                //pngread_destroy(image);
                //free(matriz);
                //free_image_data(matrix, pheight);
                
            }else if (!strcmp((char *)ext+1, "gif")){
                
            }else{
                dprintf(standarterr,"[Error]: the file you are trying to open is not supported");
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