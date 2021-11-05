#ifndef GENDEC_IMPLEMENTED
#define GENDEC_IMPLEMENTED
#include "../config.h"
#include "../funcaux.h"
#include <stdint.h>
#include "../grapdec.h"
#include "pngdec.h"
#include "jpegdec.h"
#include "gifdec.h"
void decoder_mutex(char *ext, struct gengetopt_args_info args);
#endif