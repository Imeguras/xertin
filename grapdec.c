#include "grapdec.h"

png_bytepp JanelaEescreve(png_bytepp matrix, uint32_t width, uint32_t height, const int8_t *title){
    gfx_open(width, height, (const char *)title);
    FILE *hey;
    hey=fopen("new.txt", "wb");
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < (width*3); x++){
            uint8_t red = matrix[y][x];
            uint8_t green = matrix[y][x+1];
            uint8_t blue = matrix[y][x+2];
            fprintf(hey, "\nCOLOR %d, %d %d", red, green, blue);
            
            gfx_color(red, green, blue);
            size_t realx= x/3; 
            gfx_point(realx, y);
            x=x+3;
        }
    }
    
    gfx_flush();
    gfx_wait();
    fclose(hey);
    return matrix;
}