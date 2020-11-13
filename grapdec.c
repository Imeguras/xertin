#include "grapdec.h"

png_bytepp JanelaEescreve(png_bytepp matrix, uint32_t width, uint32_t height, const int8_t *title){
    int8_t a;
    gfx_open(width, height, (const char *)title);
    FILE *hey;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < (width*4); x++){
            uint8_t red = matrix[y][x];
            uint8_t green = matrix[y][x+1];
            uint8_t blue = matrix[y][x+2];
            gfx_color(red, green, blue);
            size_t realx= x/4; 
            gfx_point(realx, y);
            x=x+3;
        }
    }
    
    if(waitForKey(XK_Escape,&width, &height)==1){
        //JanelaEescreve()    
    } 
    gfx_close();
    //fclose(hey);
    return matrix;
}
