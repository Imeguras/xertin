#ifndef JPEGDEC_IMPLEMENTED
#define JPEGDEC_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <jpeglib.h>
#include "args.h"
#include "funcaux.h" 
#include "config.h"
uint8_t startjpeg_init(struct gengetopt_args_info args);
#endif