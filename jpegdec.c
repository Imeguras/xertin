#include "jpegdec.h"
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

//void teste(){
    /*struct jpeg_decompress_struct da;  
    jpeg_create_decompress(&da);
    jpeg_read_header(&da, 0);
    jpeg_read_rawdata();
}*/