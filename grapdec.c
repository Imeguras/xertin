#include "grapdec.h"
#include <stdio.h>

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
uint8_t * MatrizParaVetor(uint8_t** matriz, uint32_t hei, size_t rwb){
    uint8_t *vetor = calloc(hei*rwb,sizeof(uint8_t));
    uint8_t *pCurrent = vetor;
    for (uint32_t row = 0; row < hei; row++){
        uint8_t *pRow = matriz[row];
        memcpy(pCurrent, pRow, rwb);
        pCurrent += rwb;
    }
    return vetor;
}



uint8_t * MatrizParaVetorJPEG(uint8_t** matriz, uint32_t hei, size_t rwb){
    //printf("\n\tALTURA|%d",hei);
    //printf("\n\tSIZEBYTEROW|%lu\n",rwb);
    uint8_t *vetor = calloc(hei*rwb,sizeof(uint8_t));
    //uint8_t *pCurrent = vetor;
    for (uint32_t row = 0; row < hei; row++)
    {
        for (size_t i = 0; i < rwb; i++)
        {
            vetor[row*i]=matriz[row][i];
        }
            
    }
    return vetor;
}
//A function that prints every decoded character onto stdout
uint8_t  **RipHexTable(uint8_t ** matrix, uint32_t width, uint32_t height, int8_t delimeter){
    size_t i;
    fprintf(stdout,"[Output]: Heres the hex table:\n");
    for (i = 0; i < height; i++){
        size_t x;
        
        for (x = 0; x < width; x++){
            printf("%x%c",matrix[i][x], delimeter);
        }
        printf("\n");
    }
    
    return matrix;
}
//OBSOLETE
uint8_t ** JanelaEescreve(uint8_t ** matrix, uint32_t width, uint32_t height, const int8_t *title){
    gfx_open(&width, &height, (const char *)title);
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
    gfx_flush();
    //gfx_wait();
    if(waitForKey((uint8_t)XK_Escape,&width, &height)==1){
        
    }
    gfx_close();
    return matrix;
}

uint8_t *displaygrap_winrite(uint8_t *vetor, uint32_t wid, uint32_t hei, uint8_t bitdepth,size_t rwb, const char *title){
    XImage *img;
    gfx_open(&wid, &hei, (const char *)title); 
    img=gfxvetor_image(vetor, bitdepth, wid, hei, rwb);
    gfx_image(img, wid, hei);
    //gfx_flush();
    gfx_color(255,10,20);
    gfx_line(0,0,40,60);
    uint32_t oldwidth=wid;
    uint32_t oldheight=hei;
    //TODO fix this! 
    if(waitForKey((uint8_t)XK_Escape,&wid, &hei)==1){
        oldwidth-=wid;
        oldheight-=hei; 
        
    }
    
    gfx_close();
    return vetor;
}