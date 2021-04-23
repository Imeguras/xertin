#ifndef GIFDEC_IMPLEMENTED
#define GIFDEC_IMPLEMENTED
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <gif_lib.h>
#include "../args.h"
#include "../funcaux.h" 
#include "../config.h"
uint8_t startgif_init(struct gengetopt_args_info args);
#endif