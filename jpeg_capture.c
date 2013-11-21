#include "headers.h"

int jpeg_capture(char *filename){
	FILE *imgout;
	int i,j,n,cnt,row_stride;
	JSAMPROW row_pointer[1];
	unsigned char image_buffer[SIZE * 3];

	if(!(imgout = fopen(filename, "wb"))){
		fprintf(stderr, "File Open Error\n");
		exit(1);
	}

	n = cnt = 0;

	for(i = 0 ; i < Height ; i++){
		for(j = Width ; j > 0 ; j--){
			image_buffer[n++] = (((RGB[cnt] & 0xf800) >> 11) << 3);
			image_buffer[n++] = (((RGB[cnt] & 0x07e0) >> 15) << 2);
			image_buffer[n++] = (((RGB[cnt] & 0x001f) << 3));
		}
	}
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_creat_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, imgout);

	cinfo.image_width = Width;
	cinfo.image_height = Height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_default(&cinfo);

	jpeg_start_compress(&cinfo, TRUE);

	row_stride = cinfo.image_width * 3;

	while(cinfo.next_scanline < cinfo.image_height){
		row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanline(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	bzero(path, CH_SZ);
	fclose(imgout);

	id = getpid();
	write(dev,&id,4);

	usleep(85000);
	return 0;
}
