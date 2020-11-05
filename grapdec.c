#include "grapdec.h"
uint8_t **JanelaEescreve(uint8_t **matrix, int32_t width, int32_t height, const int8_t *title){
    gfx_open(width, height, title);
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            uint8_t compressed = matrix[y] + x*sizeof(uint8_t);
            gfx_color(compressed & 0xFF, compressed>>8 & 0xFF, compressed>>16 &  0xFF);
            gfx_point(x, y);
            
        }
    }
    gfx_flush();
    gfx_wait();
    
    return matrix;
}
