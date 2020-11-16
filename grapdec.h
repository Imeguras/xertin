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
#endif