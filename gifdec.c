#include "gifdec.h"
//TODO DOCUMENTATE THIS SCHEIße 
static uint16_t flags;
//Returns 0 if everything goes well 
uint8_t startgif_init(struct gengetopt_args_info args){
	flags=0; 
	if (args.verbose_given){
		flags^=0x1; 
	}
	
	return 0;
}