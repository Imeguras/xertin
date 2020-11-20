#ifndef GRAPDEC_IMPLEMENTED
#define GRAPDEC_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include "gfx.h"
#include <png.h>
#include "config.h"
uint8_t ** JanelaEescreve(uint8_t** matrix, uint32_t width, uint32_t height, const int8_t *title);
uint8_t ** VetorParaMatriz(uint8_t * vetor, size_t rwb, uint32_t hei);
uint8_t * MatrizParaVetor(uint8_t** matriz, uint32_t hei, size_t rwb);
uint8_t *displaygrap_winrite(uint8_t *vetor, uint32_t wid, uint32_t hei, uint8_t bitdepth,size_t rwb, const char *title);
#endif