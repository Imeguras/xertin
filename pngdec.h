#ifndef PNGDEC_IMPLEMENTED
#define PNGDEC_IMPLEMENTED
#include <png.h>
#include <funcaux.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
png_bytepp *readPNG(char *file, int* rst_rowbytes, int* rst_width, int* rst_height);

#endif