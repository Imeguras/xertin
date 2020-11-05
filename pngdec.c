#include "pngdec.h"
png_bytepp *readPNG(char *file, int* rst_rowbytes, int* rst_width, int* rst_height){
  png_structp png_ptr;
  png_infop info_ptr;
  uint8_t iheader[8];
  uint32_t sig_read = 0;
  png_uint_32 width, height;
  int bit_depth, color_type, interlace_type;
  FILE *fp;
  png_const_charp pngcharp;
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
  if (setjmp(png_jmpbuf(png_ptr)))  \
  { \
    fprintf(stderr, "error png's set jmp for read\n"); \
                                              \
    /* clear png resource */                  \
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);   \
                                                                      \
    /* close file */ \
    fclose(fp);     \
    fp = NULL;      \
    return NULL;    \
  }
    PNG_READ_SETJMP(png_ptr, info_ptr, fp)

  // set up input code
  png_init_io(png_ptr, fp);
  // let libpng knows that we have read first initial bytes to check whether it's png file
  // thus libpng knows some bytes are missing
  png_set_sig_bytes(png_ptr, cmp_number);

  // read file info up to image data
  png_read_info(png_ptr, info_ptr);
  
  // get info of png image
  width = png_get_image_width(png_ptr, info_ptr);
  height = png_get_image_height(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  interlace_type = png_get_interlace_type(png_ptr, info_ptr);
  int32_t channels = png_get_channels(png_ptr, info_ptr);
  int32_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  printf("width = %d\n", width);
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
  printf("rowbytes = %d\n", rowbytes);

  // allocate enough and continous memory space to whole entire image
  // note: i think we could allocate continous memory space that result in just png_bytep
  // but for some reason it might due to internal libpng's internal implementation that possibly
  // needs some flexibility in row by row pointer, thus we need to allocate memory space this way
  png_bytepp row_ptr = (png_bytepp)malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; ++y)
  {
    row_ptr[y] = (png_bytep)malloc(rowbytes);
  }
  // read image data
  png_read_image(png_ptr, row_ptr);

  // clear png resource
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

  // close file
  fclose(fp);
  fp = NULL;

  // return results
  if (rst_rowbytes != NULL)
  {
    *rst_rowbytes = rowbytes;
  }
  if (rst_width != NULL)
  {
    *rst_width = width;
  }
  if (rst_height != NULL)
  {
    *rst_height = height;
  }

  return row_ptr;
}
