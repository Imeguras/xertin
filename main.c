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
#include "grapdec.h"

uint8_t vreadable(char *string,uint32_t *favouredsize);
uint8_t standartout=STDOUT_FILENO;
uint8_t standarterr=STDERR_FILENO;
int32_t main(int argc, char *argv[]){
    struct gengetopt_args_info args; 
    cmdline_parser(argc,argv, &args);
    uint32_t favblock;
    if (args.decode_given&&vreadable(args.decode_arg,&favblock)){
        int8_t *ext;
        ext = (int8_t *)strchr(args.decode_arg, '.');
        if(!ext){
            dprintf(standarterr,"[Error]: no file extension found, are you opening a text file?");
        }else{
            dprintf(standartout,"\nExtension is %s\n",ext+1);
            if (!strcmp((const char *)ext+1, "png")){
                //uint8_t *done;
                 
                int32_t rowbytes=0;
                uint32_t pwidth=0, pheight=0;
                png_bytepp matrix = readPNG(args.decode_arg,&rowbytes,&pwidth, &pheight);
                //free_image_data(matrix,pheight);
                png_bytepp (*JanelaEescrevePTR)(png_bytepp, uint32_t, uint32_t, const int8_t *);
                JanelaEescrevePTR=&JanelaEescreve;
                JanelaEescreve(matrix, pwidth, pheight, args.decode_arg);
                free_image_data(matrix, pheight);

            }else if (!strcmp((const char *)ext+1, "jpg")){
                
            }else if (!strcmp((const char *)ext+1, "gif")){
                
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
        exit(2);
    }
    if (fs.st_mode & R_OK){
        *favouredsize=fs.st_blksize;
        return 1;
    }else{
        return 0;
    }
    
}

