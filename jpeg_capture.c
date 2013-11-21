#include "headers.h"

const int Width = 320;
const int Height = 240;
const int SIZE = 153600;

int jpeg_capture(char *filename, unsigned short RGB[]){


	FILE *imgout;
	int i,j,n,cnt,row_stride;
	JSAMPROW row_pointer[1];
	unsigned char image_buffer[SIZE * 3];
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	if(!(imgout = fopen(filename, "wb"))){
		fprintf(stderr, "File Open Error\n");
		exit(1);
	}

	n = cnt = 0;

	for(i = 0 ; i < Height ; i++){
		for(j = Width ; j > 0 ; j--){
			image_buffer[n++] = (((RGB[cnt]) >> 11) << 3);
			image_buffer[n++] = (((RGB[cnt]) << 5) >> 2);
			image_buffer[n++] = (((RGB[cnt]) << 3));
		}
	}
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, imgout);

	cinfo.image_width = Width;
	cinfo.image_height = Height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);

	jpeg_start_compress(&cinfo, TRUE);

	row_stride = cinfo.image_width * 3;

	while(cinfo.next_scanline < cinfo.image_height){
		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	//bzero(path, CH_SZ);
	fclose(imgout);

	//id = getpid();
	//write(dev,&id,4);

	//usleep(85000);
	return 0;
}
