#include "pngdec.h"

void readpng_version_info(){
        fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
          PNG_LIBPNG_VER_STRING, png_libpng_ver);
        fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
          ZLIB_VERSION, zlib_version);
}
png_bytepp readpng_verificar(char *file, size_t* rwb, uint32_t* wid, uint32_t* hei){
    //RARE TO USE
	
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
    if(png_sig_cmp(pop, 0, GENERAL_PNG_SIG_SIZE)!= 0){
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
    //TODO CHUNKS CUSTOM?
    per_chunck_ptr=png_get_user_chunk_ptr(png_ptr);
    png_set_read_user_chunk_fn(png_ptr, per_chunck_ptr,(png_user_chunk_ptr)readpng_chunk_callback);
    png_set_read_status_fn(png_ptr, pngread_whilerow);
    //TODO HANDLING DE CHUNKS COMPLETAMlENTE DESCONHECIDOS
    png_set_keep_unknown_chunks(png_ptr, PNG_HANDLE_CHUNK_IF_SAFE, NULL, 0);
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
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_SCALE_16, NULL); 
    *rwb=png_get_rowbytes(png_ptr, info_ptr);
	*wid=png_get_image_width(png_ptr, info_ptr);
	*hei=png_get_image_height(png_ptr, info_ptr);
	png_bytepp row_pointers = png_malloc(png_ptr,(*hei)*(sizeof (png_bytep)));
	row_pointers = png_get_rows(png_ptr, info_ptr);
	for (uint32_t i=0; i<(*hei); ++i){
		row_pointers[i]=NULL;
		row_pointers[i]=png_malloc(png_ptr, *wid);
	}
	png_set_rows(png_ptr, info_ptr, row_pointers);
	//png_read_end(png_ptr, info_ptr);
	
    fclose(fp);
	return row_pointers;
}
png_bytepp readpng_archverificar(char *file, size_t* rwb, uint32_t* wid, uint32_t* hei){
  FILE *fp;
  if ((fp = fopen(file, "rb")) == NULL){
        fp = NULL;
        abort_(GENERAL_OPENING_READ_FILE_ERROR, " ERR01 ");
  }
    int8_t *pop;
    //pop=malloc(GENERAL_PNG_SIG_SIZE * sizeof(char));
    size_t readnum; 
    readnum=fread(pop,GENERAL_PNG_SIG_SIZE,sizeof(char),fp);
    printf("%s",pop);
    fclose(fp);
    return NULL;
} 
//TODO
int16_t readpng_chunk_callback(png_structp png_ptr,png_unknown_chunkp chunk){
        /* The unknown chunk structure contains your
                 chunk data, along with similar data for any other
                 unknown chunks: */
              //    png_byte name;
            //      png_byte *data;
          //        size_t size;
				//size=chunk->size;
				//data=chunk->data;
				//name=strcpy(name, (char *)chunk->name);
				//free(name);
				//printf("DATA %s , %ld",data, size);
              /* Note that libpng has already taken care of
                 the CRC handling */

              //TODO COOD

              //return -n; /* chunk had an error */
              return 0; /* did not recognize */
              //return n; /* success */
}
//TODO
void pngread_whilerow(png_structp png_ptr, png_uint_32 row, int pass){
             /* put your code here */
             /*
             When  this  function  is called the row has already been completely processed and the 'row' and 'pass' refer to the next row to be handled.  For the non-inter‐
       laced case the row that was just handled is simply one less than the passed in row number, and pass will always be 0.  For the interlaced case the same applies
       unless  the row value is 0, in which case the row just handled was the last one from one of the preceding passes.  Because interlacing may skip a pass you can‐
       not be sure that the preceding pass is just 'pass-1'; if you really need to know what the last pass is record (row,pass) from the callback  and  use  the  last
       recorded value each time.*/
}
void pngread_destroy(png_bytepp matrix, uint32_t hei){
	for (uint32_t i = 0; i < hei; ++i){
		free(matrix[hei]);
		matrix[hei]=NULL;
	}
	free(matrix);
	matrix=NULL;

	
}