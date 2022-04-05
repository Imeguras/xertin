#include "gendec.h"
#include <X11/Xutil.h>
#include <stdint.h>

void decoder_mutex(char *ext, struct gengetopt_args_info args){
	fprintf(stdout,"\nExtension is %s\n",ext+1);
	fprintf(stdout,"[Info] The file extension was translated to %d",strtouint(ext+1));
	size_t rowbytes=0;
	uint32_t pwidth=0, pheight=0;
	int pixel_depth=0; 
	uint8_t *pont=NULL;
	switch (strtouint(ext+1)) {
		case 1886283520:
			if(startpng_init(args)){
				fprintf(stderr, "[TODO] something went to shit");
			}
			
			pngimp image={NULL, NULL, NULL};
			image=readpng_verificar(args.decode_arg, &rowbytes, &pwidth, &pheight, &pixel_depth);
			if (args.hex_given){
				image.vetor=RipHexTable((uint8_t **)image.vetor, pwidth,pheight, ',');
			}
			if (args.decode_given){
				pont=MatrizParaVetor((uint8_t **)image.vetor, pheight, rowbytes);
				pont=displaygrap_winrite(pont,pwidth, pheight, pixel_depth, rowbytes, args.decode_arg);
			}
			
			pngread_destroy(image, pheight);
			free(pont);
			//free_image_data(matrix, pheight);
		break;
		case 1785751296:
		case 1785750887:
			if(startjpeg_init(args)){
				fprintf(stderr, "[TODO] something went to shit");
			}
			 
			pont=readjpeg_verificar(pont, args.decode_arg, &rowbytes, &pwidth, &pheight);
			if (args.hex_given){
				uint8_t **matriz=NULL;
				matriz=VetorParaMatriz(pont, rowbytes, pheight);
				matriz=RipHexTable(matriz, rowbytes, pheight,',');
				
				//free(matriz);
			}
			if (args.decode_given){
				pont=displaygrap_winrite(pont,pwidth, pheight, 8, rowbytes,args.decode_arg);
			}
			free(pont);
			//pont=NULL;
			
		break;
		default:
			fprintf(stderr,"[Error]: The file you are trying to open is not supported");
		break;
	}
}