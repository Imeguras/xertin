#include "grapdec.h"

png_bytepp JanelaEescreve(png_bytepp matrix, uint32_t width, uint32_t height, const int8_t *title){
    gfx_open(width, height, (const char *)title);
    FILE *hey;
    hey=fopen("new.txt", "wb");
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            uint8_t compressed = matrix[y][x];
            //fprintf(hey, "\nCOLOR %d, %d %d", compressed & 0xFF, compressed>>8 & 0xFF, compressed>>16 &  0xFF);
            
            gfx_color(compressed & 0xFF, compressed>>8 & 0xFF, compressed>>16 &  0xFF);
            gfx_point(x, y);
            
        }
    }
    
    gfx_flush();
    gfx_wait();
    fclose(hey);
    return matrix;
}