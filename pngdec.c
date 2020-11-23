#include "pngdec.h"
png_byte unused_chunks[]={
104,  73,  83,  84, (png_byte) ' ',
105,  84,  88, 116, (png_byte) ' ',
112,  67,  65,  76, (png_byte) ' ',
115,  67,  65,  76, (png_byte) ' ',
115,  80,  76,  84, (png_byte) ' ',
116,  73,  77,  69, (png_byte) ' '
};
//TODO DOCUMENTATE THIS SCHEIße 
static uint16_t flags;
//Returns 0 if everything goes well 
uint8_t startpng_init(struct gengetopt_args_info args){
	flags=0; 
	if (args.verbose_given){
		flags^=0x1; 
	}
	
	return 0;
}
void readpng_version_info(){
				fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
					PNG_LIBPNG_VER_STRING, png_libpng_ver);
				fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
					ZLIB_VERSION, zlib_version);
}
pngimp readpng_verificar(char *file, size_t* rwb, uint32_t* wid, uint32_t* hei){
		png_voidp per_chunck_ptr; 
		size_t readnum; 
		FILE *fp;
		if ((fp = fopen(file, "rb")) == NULL){
			fp = NULL;
			abort_(GENERAL_OPENING_READ_FILE_ERROR, " ERR01 ");
		}
		int8_t *pop;
		pop=malloc(GENERAL_PNG_SIG_SIZE * sizeof(char));
		readnum=fread(pop,sizeof(char),GENERAL_PNG_SIG_SIZE,fp);
		if(readnum!=GENERAL_PNG_SIG_SIZE){
			fclose(fp);
			//ERR 5
			fp = NULL;
			fprintf(stderr, GENERAL_READ_FILE_ERROR, "ERR02");
			exit(5);
		}
		if(png_sig_cmp((png_const_bytep)pop, 0, GENERAL_PNG_SIG_SIZE)!= 0){
			fclose(fp);
			fp = NULL;
			fprintf(stderr, "File is not recognized as png file\n");
			//err 6
			exit(6);
		}
		free(pop);
		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(png_ptr==NULL){
			fclose(fp);
			fp= NULL;
			fprintf(stderr, "TODO");
			exit(7);
		}
		 png_infop info_ptr= png_create_info_struct(png_ptr);
		if (info_ptr== NULL){
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(fp);
			fp=NULL;
			fprintf(stderr, "TODO");
			exit(7);
		}
		
		//PNG_READ_SETJMP(png_ptr, info_ptr, fp)
		png_init_io(png_ptr,fp);
		png_set_sig_bytes(png_ptr,GENERAL_PNG_SIG_SIZE);
		png_set_compression_buffer_size(png_ptr, SPECIFIC_LIBPNG_ZLIB_BUFFER_COMPRESSION_BYTE_SIZE);
		//TODO HANDLING DE ERROS DO FICHEIRO
		png_set_crc_action(png_ptr, PNG_CRC_DEFAULT, PNG_CRC_DEFAULT);
		//png_read_info(png_ptr, info_ptr);
		//png_read_update_info(png_ptr,info_ptr);

		//TODO CHUNKS CUSTOM?
		per_chunck_ptr=png_get_user_chunk_ptr(png_ptr);
		png_set_read_user_chunk_fn(png_ptr, per_chunck_ptr,(png_user_chunk_ptr)readpng_chunk_callback);
		png_set_read_status_fn(png_ptr, pngread_whilerow);
		//TODO HANDLING DE CHUNKS COMPLETAMlENTE DESCONHECIDOS
		//png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_NEVER, unused_chunks, sizeof(unused_chunks)/5);
		png_set_keep_unknown_chunks(png_ptr,PNG_HANDLE_CHUNK_IF_SAFE, NULL, 0);
		png_set_user_limits(png_ptr, SPECIFIC_LIBPNG_READ_WIDTH_MAX, SPECIFIC_LIBPNG_READ_HEIGHT_MAX);
		//TODO png_set_chunk_cache_max(png_ptr, user_chunk_cache_max);  0x7fffffffL unlimited 
		//png_set_chunk_malloc_max(png_ptr, user_chunk_malloc_max);
		/*TODO If  you  intend  to  display the PNG or to incorporate it in other image data you need to tell libpng information about your display or drawing surface so that
			 libpng can convert the values in the image to match the display.

			 From libpng-1.5.4 this information can be set before reading the PNG file header.  In earlier versions  png_set_gamma()  existed  but  behaved  incorrectly  if
			 called before the PNG file header had been read and png_set_alpha_mode() did not exist.

			 If  you  need  to support versions prior to libpng-1.5.4 test the version number as illustrated below using "PNG_LIBPNG_VER >= 10504" and follow the procedures
			 described in the appropriate manual page.
		*/
	 //TODO THIS IS DIFFERENTE FOR ENCODE
	#if PNG_LIBPNG_VER >= 10504
		png_set_alpha_mode(png_ptr, PNG_ALPHA_STANDARD, PNG_DEFAULT_sRGB);
		#else
		png_set_gamma(png_ptr, PNG_DEFAULT_sRGB, 1.0/PNG_DEFAULT_sRGB);
		#endif
	png_set_bgr(png_ptr);
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL); 
	
	*rwb=png_get_rowbytes(png_ptr, info_ptr);
	*wid=png_get_image_width(png_ptr, info_ptr);
	*hei=png_get_image_height(png_ptr, info_ptr);
	png_bytepp row_pointers = png_malloc(png_ptr,(*hei)*(sizeof (png_bytep)));
	
	for (uint32_t i=0; i<(*hei); ++i){
		row_pointers[i]=NULL;
		row_pointers[i]=png_malloc(png_ptr, *wid);
	}
	row_pointers = png_get_rows(png_ptr, info_ptr);
	png_set_rows(png_ptr, info_ptr, row_pointers);
	//png_read_end(png_ptr, info_ptr);
		fclose(fp);
	pngimp returnvalue={row_pointers,png_ptr, info_ptr};
	
	return returnvalue;
}

//TODO
int32_t readpng_chunk_callback(png_structp png_ptr,png_unknown_chunkp chunk){
	/* The unknown chunk structure contains your
	chunk data, along with similar data for any other
	unknown chunks: */
	//png_byte name;
	//png_byte *data;
	//size_t size;
	//size=chunk->size;
	//data=chunk->data;
	//name=strcpy(name, (char *)chunk->name);
	//free(name);
	//printf("DATA %s , %ld",data, size);
	/* Note that libpng has already taken care of
	the CRC handling */
	//TODO COOD
	png_get_chunk_malloc_max(png_ptr);	
	if(flags==1){ 
		printf("[INFO]: Spotted a chunck named %s\n", chunk->name);
	}
	//return -n; /* chunk had an error */
	return 0; /* did not recognize */
	//return n; /* success */
}
//TODO
void pngread_whilerow(png_structp png_ptr, png_uint_32 row, int pass){
	png_get_copyright(png_ptr);	
	if(flags==1){ 
		printf("Reading row:%d, pass:%d\n", row, pass);
	}
}
void pngread_destroy(pngimp matrix){
	png_destroy_info_struct(matrix.png_ptr, &matrix.info_ptr);
	png_destroy_read_struct(&matrix.png_ptr,&matrix.info_ptr, NULL);
	//TODO VERIFY IF EVERYTHINGS FREE'D PROPERLY 
	/*for (uint32_t i = 0; i < hei; ++i){
		if(matrix.matrix[hei]!=NULL){
			free(matrix.matrix[hei]);
			matrix.matrix[hei]=NULL;
		}else{
			fprintf(stderr, "ItS ALREADY CLEANED");
		}
		
	}
	if(matrix.matrix!=NULL){
		free(matrix.matrix);
		matrix.matrix=NULL;
	}else
	{
		fprintf(stderr, "ItS ALREADY CLEANED");
	}*/
	
}