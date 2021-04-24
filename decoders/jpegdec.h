#ifndef JPEGDEC_IMPLEMENTED
#define JPEGDEC_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "../args.h"
#include "../funcaux.h" 
#include "../config.h"

uint8_t startjpeg_init(struct gengetopt_args_info args);
struct readjpeg_errorstruct {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;	
};


typedef struct readjpeg_errorstruct * my_error_ptr;
//METHODDEF(void) my_error_exit (j_common_ptr cinfo);
uint8_t* readjpeg_defragment(uint8_t* DefragVetor, uint8_t* vetor, size_t rwb, size_t lastindex);
uint8_t* readjpeg_verificar(uint8_t* matriz, const char *file, size_t *rwb, uint32_t *wid, uint32_t *hei); 

#endif