#include "pngdec.h"


void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}
void readPNG(char *file, uint32_t pChunk){
        int x, y;
        int width, height;
        png_byte color_type;
        png_byte bit_depth;

        png_structp png_ptr;
        png_infop info_ptr;
        int number_of_passes;
        png_bytep * row_pointers;

        char header[8];   
        FILE *fp = fopen(file, "r+b");
        if (!fp){
                abort_("[ERROR] File %s could not be opened for reading", file);
        }
        fread(header, 1, 8, fp);
        if (png_sig_cmp(header, 0, 8)){
                abort_("[ERROR] File %s is not recognized as a PNG file", file);
        }

        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        if (!png_ptr){
                abort_("[ERROR] png_create_read_struct failed");
        }
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr){
                abort_("[ERROR] png_create_info_struct failed");
        }
        if (setjmp(png_jmpbuf(png_ptr))){
                abort_("[ERROR] Error during init_io");
        }
        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, info_ptr);

        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);

        number_of_passes = png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, info_ptr);


        /* read file */
        if (setjmp(png_jmpbuf(png_ptr)))
                abort_("[ERROR] Error during read_image");

        row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
        for (y=0; y<height; y++){
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
                
        }
        png_read_image(png_ptr, row_pointers);
        
        fclose(fp);
}
