#ifndef PNGDEC_IMPLEMENTED
#define PNGDEC_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <png.h>
#include <zlib.h>
#include "../funcaux.h"
#include "../args.h"
#include "../config.h"
typedef struct 
{
    char *filen;
    FILE *filep;
}fich;
typedef struct{
    png_bytepp vetor;
    png_structp png_ptr;
    png_infop info_ptr;
}pngimp;
//TODO
#define PNG_READ_SETJMP(png_ptr, info_ptr, fp) \
  if (setjmp(png_jmpbuf(png_ptr))){\
    fprintf(stderr, "error png's set jmp for read\n");\
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);\
    fclose(fp);     \
    fp = NULL;      \
    return NULL;    \
  }


uint8_t startpng_init(struct gengetopt_args_info args);
void readpng_version_info(void);
pngimp readpng_verificar(char *file, size_t* rwb, uint32_t* wid, uint32_t* hei, int* pixel_depth);
int32_t readpng_chunk_callback(png_structp png_ptr,png_unknown_chunkp chunk);
void pngread_whilerow(png_structp png_ptr, png_uint_32 row, int pass);
void pngread_destroy(pngimp matrix, uint32_t hei);
#endif