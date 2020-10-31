#ifndef PNGDEC_IMPLEMENTED
#define PNGDEC_IMPLEMENTED
#include <png.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
void readPNG(char *file, uint32_t pChunk);
void abort_(const char * s, ...);
#endif