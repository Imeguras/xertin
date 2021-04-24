#include "bmpdec.h"
/*void ReadImage(const char *fileName,int8_t **pixels, int32_t *width, int32_t *height, int32_t *bytesPerPixel){
        FILE *imageFile = fopen(fileName, "rb");
        int32_t dataOffset;
        fseek(imageFile, DATA_OFFSET_OFFSET, SEEK_SET);
        fread(&dataOffset, 4, 1, imageFile);
        fseek(imageFile, WIDTH_OFFSET, SEEK_SET);
        fread(width, 4, 1, imageFile);
        fseek(imageFile, HEIGHT_OFFSET, SEEK_SET);
        fread(height, 4, 1, imageFile);
        int16_t bitsPerPixel;
        fseek(imageFile, BITS_PER_PIXEL_OFFSET, SEEK_SET);
        fread(&bitsPerPixel, 2, 1, imageFile);
        *bytesPerPixel = ((int32_t)bitsPerPixel) / 8;
        int32_t paddedRowSize = (int32_t)(4 * ceil((float)(*width) / 4.0f))*(*bytesPerPixel);
        int32_t unpaddedRowSize = (*width)*(*bytesPerPixel);
        int32_t totalSize = unpaddedRowSize*(*height);
        *pixels = (int8_t*)malloc(totalSize);
        int32_t i = 0;
        int8_t *currentRowPointer = *pixels+((*height-1)*unpaddedRowSize);
        for (i = 0; i < *height; i++)
        {
            fseek(imageFile, dataOffset+(i*paddedRowSize), SEEK_SET);
            fread(currentRowPointer, 1, unpaddedRowSize, imageFile);
            currentRowPointer -= unpaddedRowSize;
        }
 
        fclose(imageFile);
}*/