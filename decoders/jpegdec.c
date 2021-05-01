#include "jpegdec.h"
#include <jpeglib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//TODO DOCUMENTATE THIS SCHEIße 
static uint16_t flags;

//Returns 0 if everything goes well 
uint8_t startjpeg_init(struct gengetopt_args_info args){
	flags=0; 
	if (args.verbose_given){
		flags^=0x1; 
	}
	
	return 0;
}

METHODDEF(void) my_error_exit (j_common_ptr cinfo){
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

uint8_t* readjpeg_verificar(uint8_t* vetor, const char *file, size_t *rwb, uint32_t *wid, uint32_t *hei){
	size_t check=0;
	struct jpeg_decompress_struct info;
	struct readjpeg_errorstruct jerr;
	info.err = jpeg_std_error(&jerr.pub);
	FILE *fp=fopen(file, "rb"); 
	if (fp== NULL){
		//abort_(GENERAL_OPENING_READ_FILE_ERROR, " ERR01 ");
		//TODO
	}
	jerr.pub.error_exit = my_error_exit;
	//TODO
	/*if (setjmp(jerr.setjmp_buffer)) {
    	jpeg_destroy_decompress(&info);
    	fclose(fp);
    	return 0;
  	}*/
	
	jpeg_create_decompress(&info);
	jpeg_stdio_src(&info, fp);
	jpeg_read_header(&info, TRUE);
	info.out_color_space=JCS_EXT_BGRA;
	jpeg_calc_output_dimensions(&info);
	
	jpeg_start_decompress(&info);
	fprintf(stdout, "[Debug] Data precision: %d\n", info.data_precision);
	int numChannels = info.output_components;
	
	fprintf(stdout, "[Debug] Number of Color Component(R,G,B,A etc...): %d\n", numChannels);
	
	//fprintf(stdout, "[Debug] Data precision: %d\n", info.data_precision);
	*rwb = info.output_width * numChannels*info.data_precision/8;//;//*info.data_precision;
	*wid = info.output_width; 
	*hei = info.output_height;
  	JSAMPARRAY buffer; 
  
  //TODO MAKE MULTITHREADING FOR BIGGER IMAGES
  	buffer = (info.mem->alloc_sarray) ((j_common_ptr)&info, JPOOL_IMAGE, *rwb, SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA);
	while (info.output_scanline < info.output_height) {
	//TODO SOMEONE(AS IN ME) FIX THE UGGO CODE AND ADD AN ABORTION SYSTEM OR SOMETHING? DID SETJMP WORK FOR THIS? ALSO ENDOFFILE EXISTS 
		forceretry:
			check=jpeg_read_scanlines(&info, buffer, SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA);
				if(check!=SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA){
					fprintf(stderr, "[ERROR] Failed to read %lu line(s) forcing a retry", SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA-check);
					goto forceretry;
				}
		vetor=readjpeg_defragment(vetor,(uint8_t *)buffer[0],*rwb, info.output_scanline);	
  	}

    (void) jpeg_finish_decompress(&info);
	jpeg_destroy_decompress(&info);
	fclose(fp);
	return vetor;
}
//TOOPTMIZE LASTINDEX ONLY FEASIBLY GOES TO UNSIGNED INT
uint8_t* readjpeg_defragment(uint8_t* DefragVetor, uint8_t* vetor, size_t rwb, size_t lastindex){
	DefragVetor=realloc(DefragVetor,(rwb*lastindex+rwb*SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA)*sizeof(uint8_t));
	uint8_t *ref=DefragVetor+(rwb*(lastindex-1));
	memcpy(ref, vetor, rwb*SPECIFIC_LIBJPEG_SCANROWS_EMPREITADA);
	return DefragVetor;
}
