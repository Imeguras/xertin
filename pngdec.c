#include "pngdec.h"

 /*png_bytepp readPNG(png_bytepp matrix, char *file, size_t* rst_rowbytes, png_uint_32* rst_width, png_uint_32* rst_height){
  static png_structp png_ptr;
  static png_infop info_ptr;
  uint8_t iheader[8];
  png_uint_32 width, height;
  png_byte bit_depth, color_type, interlace_type;
  FILE *fp;
  
  if ((fp = fopen(file, "rb")) == NULL){
    abort_("Welp something got fucked check the IO");
  }
  png_init_io(png_ptr,fp);
  
  const uint32_t cmp_number = 8;
  if (fread(iheader, 1, cmp_number, fp) != cmp_number){
    fprintf(stderr, "read %s file error", file);
    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }  
  
  // check whether magic number matches, and thus it's png file
  if (png_sig_cmp(iheader, 0, cmp_number) != 0)
  {
    // it's not PNG file
    fprintf(stderr, "%s file is not recognized as png file\n", file);
    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }
  
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  png_set_sig_bytes(png_ptr, 8);
  if (png_ptr == NULL){
    // cannot create png structure
      fprintf(stderr, "cannot create png structure\n");
    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL)
  {
    fprintf(stderr, "cannot create png info structure\n");

    // clear png resource
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    
    // close file
    fclose(fp);
    fp = NULL;
    return NULL;
  }
  #define PNG_READ_SETJMP(png_ptr, info_ptr, fp) \
  /* set jmp */ \
 /* if (setjmp(png_jmpbuf(png_ptr)))  \
  { \
    fprintf(stderr, "error png's set jmp for read\n"); \
                                              \
    /* clear png resource */                  \
   /* png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);   \
                                                                      \
    /* close file */ \
 /*   fclose(fp);     \
    fp = NULL;      \
    return NULL;    \
  }
     PNG_READ_SETJMP(png_ptr, info_ptr, fp)

  // set up input code
  png_init_io(png_ptr, fp);
  // let libpng knows that we have read first initial bytes to check whether it's png file
  // thus libpng knows some bytes are missing
 


       
  // read file info up to image data
  png_read_info(png_ptr, info_ptr);
  
  // get info of png image
  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  interlace_type = png_get_interlace_type(png_ptr, info_ptr);
  png_byte channels = png_get_channels(png_ptr, info_ptr);
  size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

   if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_expand(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_expand(png_ptr);
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);
            double  gamma;
      
    /*    if (png_get_gAMA(png_ptr, info_ptr, &gamma))
            png_set_gamma(png_ptr, display_exponent, gamma);
*/


 /* printf("width = %d\n", width);
  printf("height = %d\n", height);
  printf("bit_depth = %d\n", bit_depth);
  switch (color_type)
  {
    case PNG_COLOR_TYPE_GRAY:
      printf("color type = 'PNG_COLOR_TYPE_GRAY' (bit depths 1, 2, 4, 8, 16)\n");
      break;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      printf("color type = 'PNG_COLOR_TYPE_GRAY_ALPHA' (bit depths 8, 16)\n");
      break;
    case PNG_COLOR_TYPE_PALETTE:
      printf("color type = 'PNG_COLOR_TYPE_PALETTE' (bit depths 1, 2, 4, 8)\n");
      break;
    case PNG_COLOR_TYPE_RGB:
      printf("color type = 'PNG_COLOR_TYPE_RGB' (bit depths 8, 16)\n");
      break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
      printf("color type = 'PNG_COLOR_TYPE_RGB_ALPHA' (bit depths 8, 16)\n");
      break;
  }
  switch (interlace_type)
  {
    case PNG_INTERLACE_NONE:
      printf("interlace type = none\n");
      break;
    case PNG_INTERLACE_ADAM7:
      printf("interlace type = ADAM7\n");
      break;
  }
  switch (channels)
  {
    case 1:
      printf("channels = %d (GRAY, PALETTE)\n", channels);
      break;
    case 2:
      printf("channels = %d (GRAY_ALPHA)\n", channels);
      break;
    case 3:
      printf("channels = %d (RGB)\n", channels);
      break;
    case 4:
      printf("channels = %d (RGB_ALPHA or RGB + filter byte)\n", channels);
      break;
  }
  printf("rowbytes = %ld\n", rowbytes);
  /*png_bytep matrixgiant;
  if ((matrixgiant = (png_bytep)malloc(rowbytes*height))!= NULL){
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      return NULL;
  }

  for (size_t i = 0; i < height; i++)
  {
      matrix[i] = matrixgiant + i*rowbytes;
  }*/
  

  // read image data
  //png_read_image(png_ptr, matrix);
  
  // clear png resource
 /* png_read_end(png_ptr,info_ptr);
  // close file
  fclose(fp);
  fp = NULL;

  // return results
  if (rst_rowbytes != NULL)
  {
    *rst_rowbytes = (size_t)rowbytes;
  }
  if (rst_width != NULL)
  {
    *rst_width = width;
  }
  if (rst_height != NULL)
  {
    *rst_height = height;
  }

  return matrix;
}


void free_image_data(png_bytepp data, int height)
{
  for (int y=0; y<height; ++y)
  {
    free(data[y]);
    data[y] = NULL;
  }
  free(data);
}*/


void readpng_version_info(){
        fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
          PNG_LIBPNG_VER_STRING, png_libpng_ver);
        fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
          ZLIB_VERSION, zlib_version);
}
void readpng_verificar(fich *file, size_t* rwb, uint32_t wid, uint32_t hei){
    //RARE TO USE
    png_voidp per_chunck_ptr; 
    //
    FILE *fp=file->filep;
    if (fp==NULL){
      if ((fp = fopen(file, "rb")) == NULL){
        fp = NULL;
        abort_(GENERAL_OPENING_READ_FILE_ERROR, " ERR01 ");
      }
    }
    uint8_t *pop;
    
    if(fread(pop,sizeof(uint8_t),GENERAL_PNG_SIG_SIZE,fp)!=GENERAL_PNG_SIG_SIZE){
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
    png_structp png_ptr;
    if(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)==NULL){
      fclose(fp);
      fp= NULL;
      fprintf(stderr, "TODO");
      exit(7);
    }
    png_infop info_ptr;
    if (info_ptr= png_create_info_struct(info_ptr)){
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      fclose(fp);
      fp=NULL;
      fprintf(stderr, "TODO");
      exit(7);
    }
    PNG_READ_SETJMP(png_ptr, info_ptr, fp)
    png_init_io(png_ptr,fp);
    png_set_sig_bytes(png_ptr,GENERAL_PNG_SIG_SIZE);
    png_set_compression_buffer_size(png_ptr, SPECIFIC_LIBPNG_ZLIB_BUFFER_COMPRESSION_BYTE_SIZE);
    //TODO HANDLING DE ERROS DO FICHEIRO
    png_set_crc_action(png_ptr, PNG_CRC_DEFAULT, PNG_CRC_DEFAULT);
    //TODO CHUNKS CUSTOM?
    png_get_user_chunk_ptr(png_ptr);
    png_set_read_user_chunk_fn(png_ptr, per_chunck_ptr,readpng_chunk_callback);
    png_set_read_status_fn(png_ptr, pngread_whilerow);
    //TODO HANDLING DE CHUNKS COMPLETAMENTE DESCONHECIDOS
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
    #if (PNG_LIBPNG_VER >= 10504)
      png_set_gamma(png_ptr, PNG_DEFAULT_sRGB, SPECIFIC_LIBPNG_SRGB_GAMA_MULTIPLIER); 
      //TODO CHECK THIS LATER
      png_set_alpha_mode(png_ptr, PNG_ALPHA_STANDARD, PNG_DEFAULT_sRGB);
   
     
    #endif
   

}

//TODO
int16_t readpng_chunk_callback(png_structp png_ptr,png_unknown_chunkp chunk){
              /* The unknown chunk structure contains your
                 chunk data, along with similar data for any other
                 unknown chunks: */
                  png_byte name[5];
                  png_byte *data;
                  size_t size;

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

