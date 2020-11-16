#ifndef JPEGDEC_IMPLEMENTED
#define JPEGDEC_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "args.h"
#include "funcaux.h" 
#include "config.h"
uint8_t startjpeg_init(struct gengetopt_args_info args);

/*struct my_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;	
};

typedef struct my_error_mgr * my_error_ptr;


METHODDEF(void) my_error_exit (j_common_ptr cinfo);
JSAMPLE* readjpeg_verificar (char *file,size_t *rsw, uint32_t *wid, uint32_t *hei);**/
#endif