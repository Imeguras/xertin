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

METHODDEF(void) my_error_exit (j_common_ptr cinfo){
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

uint8_t* readjpeg_verificar(uint8_t** matriz, const char *file, size_t *rwb, uint32_t *wid, uint32_t *hei){
	//FILE *da;
	//da=fopen("testeda.txt","w");
	struct jpeg_decompress_struct info;
	struct readjpeg_errorstruct jerr;
	info.err = jpeg_std_error(&jerr.pub);
	FILE *fp=fopen(file, "rb"); 
	if (fp== NULL){
		abort_(GENERAL_OPENING_READ_FILE_ERROR, " ERR01 ");
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
	info.out_color_space=JCS_EXT_BGR;
	jpeg_calc_output_dimensions(&info);
	
	jpeg_start_decompress(&info);
	
	int numChannels = info.num_components;
	*rwb = info.output_width * numChannels;
	*wid = info.output_width; 
	*hei = info.output_height;
	/* Make a one-row-high sample array that will go away when done with image */
	uint8_t *vetor=calloc((*rwb)*(*hei), sizeof(uint8_t));
	matriz = (uint8_t **)(*info.mem->alloc_sarray)((j_common_ptr) &info, JPOOL_IMAGE, *rwb, 1);
	size_t needle; 
	while (info.output_scanline < *hei) {
		jpeg_read_scanlines(&info, matriz, 1);
		memcpy(vetor+(needle),matriz[0], (*rwb));
		needle+=*rwb;
	}
	//printf("%s",vetor);
	//fprintf(da, "%s", matriz[1]);
	
	//unsigned long dataSize = (*rwb) * (*hei);
	//unsigned char *data = (unsigned char *)malloc(dataSize);
    //unsigned char* rowptr;
	//jpeg_start_output(&info)
	//unsigned char **data=malloc(*hei);

    (void) jpeg_finish_decompress(&info);
	jpeg_destroy_decompress(&info);
	fclose(fp);
	return vetor;
}


// uint8_t ** readjpeg_verificar(char *file,size_t *rsw, uint32_t *wid, uint32_t *hei){
//   /* This struct contains the JPEG decompression parameters and pointers to
//    * working space (which is allocated as needed by the JPEG library).
//    */
//   struct jpeg_decompress_struct cinfo;
//   /* We use our private extension JPEG error handler.
//    * Note that this struct must live as long as the main JPEG parameter
//    * struct, to avoid dangling-pointer problems.
//    */
//   struct my_error_mgr jerr;
//   /* More stuff */
//   FILE * infile;		
//   JSAMPARRAY buffer;		
//   if ((infile = fopen(file, "rb")) == NULL) {
//     fprintf(stderr, "TODO %s\n", file);
//     return 0;
//   }
  
//   /* Step 1: allocate and initialize JPEG decompression object */

//   /* We set up the normal JPEG error routines, then override error_exit. */
//   cinfo.err = jpeg_std_error(&jerr.pub);
//   jerr.pub.error_exit = my_error_exit;
//   //TODO maybe use the set jump we already have on PNG? too lazy to do for now
//   /* Establish the setjmp return context for my_error_exit to use. */
//   if (setjmp(jerr.setjmp_buffer)) {
//     /* If we get here, the JPEG code has signaled an error.
//      * We need to clean up the JPEG object, close the input file, and return.
//      */
//     jpeg_destroy_decompress(&cinfo);
//     fclose(infile);
//     return 0;
//   }
//   /* Now we can initialize the JPEG decompression object. */
//   jpeg_create_decompress(&cinfo);

//   /* Step 2: specify data source (eg, a file) */
//   jpeg_stdio_src(&cinfo, infile);

//   /* Step 3: read file parameters with jpeg_read_header() */
//   (void) jpeg_read_header(&cinfo, TRUE);
//   /* We can ignore the return value from jpeg_read_header since
//    *   (a) suspension is not possible with the stdio data source, and
//    *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
//    * See libjpeg.txt for more info.
//    */

//   /* Step 4: set parameters for decompression */

//   /* In this example, we don't need to change any of the defaults set by
//    * jpeg_read_header(), so we do nothing here.
//    */

//   /* Step 5: Start decompressor */

//   (void) jpeg_start_decompress(&cinfo);
//   /* We can ignore the return value since suspension is not possible
//    * with the stdio data source.
//    */

//   /* We may need to do some setup of our own at this point before reading
//    * the data.  After jpeg_start_decompress() we have the correct scaled
//    * output image dimensions available, as well as the output colormap
//    * if we asked for color quantization.
//    * In this example, we need to make an output work buffer of the right size.
//    */ 
//   /* JSAMPLEs per row in output buffer */
//   *rsw = cinfo.output_width * cinfo.output_components;
//   *wid = cinfo.output_width; 
//   *hei = cinfo.output_height;
//   /* Make a one-row-high sample array that will go away when done with image */
//   buffer = (*cinfo.mem->alloc_sarray)
// 		((j_common_ptr) &cinfo, JPOOL_IMAGE, *rsw, 1);

//   /* Step 6: while (scan lines remain to be read) */
//   /*           jpeg_read_scanlines(...); */

//   /* Here we use the library's state variable cinfo.output_scanline as the
//    * loop counter, so that we don't have to keep track ourselves.
//    */
//   while (cinfo.output_scanline < cinfo.output_height) {
//     /* jpeg_read_scanlines expects an array of pointers to scanlines.
//      * Here the array is only one element long, but you could ask for
//      * more than one scanline at a time if that's more convenient.
//      */
//     (void) jpeg_read_scanlines(&cinfo, buffer, 1);
//     /* Assume put_scanline_someplace wants a pointer and sample count. */
//     put_scanline_someplace(buffer[0], *rsw);
//   }

//   /* Step 7: Finish decompression */

//   (void) jpeg_finish_decompress(&cinfo);
//   /* We can ignore the return value since suspension is not possible
//    * with the stdio data source.
//    */

//   /* Step 8: Release JPEG decompression object */

//   /* This is an important step since it will release a good deal of memory. */
//   jpeg_destroy_decompress(&cinfo);

//   /* After finish_decompress, we can close the input file.
//    * Here we postpone it until after no more JPEG errors are possible,
//    * so as to simplify the setjmp error logic above.  (Actually, I don't
//    * think that jpeg_destroy can do an error exit, but why assume anything...)
//    */
//   fclose(infile);

//   /* At this point you may want to check to see whether any corrupt-data
//    * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
//    */
//   return buffer;
// }
