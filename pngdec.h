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
#include "funcaux.h"
typedef struct 
{
    char *filen;
    FILE *filep;
}fich;
//TODO
#define PNG_READ_SETJMP(png_ptr, info_ptr, fp) \
  if (setjmp(png_jmpbuf(png_ptr))){\
    fprintf(stderr, "error png's set jmp for read\n");\
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);\
    fclose(fp);     \
    fp = NULL;      \
    return NULL;    \
  }

void readpng_version_info(void);
png_bytepp readpng_verificar(char *file, size_t* rwb, uint32_t* wid, uint32_t* hei);
png_bytepp readpng_archverificar(char *file, size_t* rwb, uint32_t* wid, uint32_t* hei);
int16_t readpng_chunk_callback(png_structp png_ptr,png_unknown_chunkp chunk);
void pngread_whilerow(png_structp png_ptr, png_uint_32 row, int pass);
void pngread_destroy(png_bytepp matrix, uint32_t hei);
#endif