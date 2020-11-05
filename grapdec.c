#include "grapdec.h"
png_bytepp (*JanelaEescrevePTR)(png_bytepp matrix, uint32_t width, uint32_t height, const int8_t *title);
png_bytepp JanelaEescreve(png_bytepp matrix, uint32_t width, uint32_t height, const int8_t *title){
    gfx_open(width, height, (const char *)title);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            uint8_t compressed = (uint8_t)*(matrix[y] + x*sizeof(uint8_t));
            gfx_color(compressed & 0xFF, compressed>>8 & 0xFF, compressed>>16 &  0xFF);
            gfx_point(x, y);
            
        }
    }
    gfx_flush();
    gfx_wait();
    
    return matrix;
}
