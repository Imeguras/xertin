#include "grapdec.h"
uint8_t ** VetorParaMatriz(uint8_t * vetor, size_t rwb, uint32_t hei){
    uint8_t **matriz;
    matriz=calloc(hei,sizeof(uint8_t *));
    for (size_t y = 0; y < sizeof(vetor); y++){
        for (size_t x= 0; x < rwb; x++){
            matriz[y]=malloc(rwb);
            matriz[y][x]=vetor[x*y];
        }
    }
    return matriz;
}





uint8_t ** JanelaEescreve(uint8_t ** matrix, uint32_t width, uint32_t height, const int8_t *title){
    gfx_open(width, height, (const char *)title);
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
    
    if(waitForKey((uint8_t)XK_Escape,&width, &height)==1){
        
    } 
    gfx_close();

    return matrix;
}
