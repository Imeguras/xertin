#ifndef PNGDEC_IMPLEMENTED
#define PNGDEC_IMPLEMENTED
#include <png.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include "funcaux.h"
png_bytepp readPNG(char *file, uint32_t* rst_rowbytes, uint32_t* rst_width, uint32_t* rst_height);

#endif