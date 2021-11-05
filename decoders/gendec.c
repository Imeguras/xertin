#include "gendec.h"

void decoder_mutex(char *ext, struct gengetopt_args_info args){
	fprintf(stdout,"\nExtension is %s\n",ext+1);
	fprintf(stdout,"[Info] The file extension was translated to %d",strtouint(ext+1));
	size_t rowbytes=0;
	png_uint_32 pwidth=0, pheight=0;
	switch (strtouint(ext+1)) {
		case 1886283520:
			if(startpng_init(args)){
				fprintf(stderr, "[TODO] something went to shit");
			}
			
			pngimp image={NULL, NULL, NULL};
			image=readpng_verificar(args.decode_arg, &rowbytes, &pwidth, &pheight);
			if (args.hex_given){
				image.vetor=RipHexTable((uint8_t **)image.vetor, pwidth,pheight, ',');
			}
			uint8_t *pont=MatrizParaVetor((uint8_t **)image.vetor, pheight, rowbytes);
			pont=displaygrap_winrite(pont,pwidth, pheight, 8, rowbytes, args.decode_arg);
			pngread_destroy(image);
			//free(pont);
			//free_image_data(matrix, pheight);
		break;
		case 1785751296:
		case 1785750887:
			if(startjpeg_init(args)){
				fprintf(stderr, "[TODO] something went to shit");
			}
			uint8_t *vetor=NULL; 
			vetor=readjpeg_verificar(vetor, args.decode_arg, &rowbytes, &pwidth, &pheight);
			if (args.hex_given){
				uint8_t **matriz=NULL;
				matriz=VetorParaMatriz(vetor, rowbytes, pheight);
				matriz=RipHexTable(matriz, rowbytes, pheight,',');
				//free(matriz);
				//matriz=NULL;
			}
			vetor=displaygrap_winrite(vetor,pwidth, pheight, 8, rowbytes,args.decode_arg);
			free(vetor);
			vetor=NULL;
			
		break;
		default:
			fprintf(stderr,"[Error]: The file you are trying to open is not supported");
		break;
	}
}