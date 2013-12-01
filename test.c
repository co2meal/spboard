#include "headers.h"
#include "EasyBMP.h"

typedef unsigned char ubyte;

unsigned short makepixel(unsigned char r, unsigned char g, unsigned char b) {
	return (unsigned short) (((r >> 3) << 11)|((g >> 2) << 5) | (b >> 3));
}


void write_rgb(BMP Image) {
	int fd = open("/dev/fb", O_RDWR);
	if (fd < 0) {
		printf("Error fbdev open\n");
		exit(1);
	} int count = 0;
	for(int t = 1; t < 240; t++) {
		for (int tt = 1; tt <320; tt++) {
			int offset = (t * 320 * 2) + (tt * 2);
			unsigned char r = Image(tt, t)->Red;
			unsigned char g = Image(tt, t)->Green;
			unsigned char b = Image(tt, t)->Blue;
			unsigned short pixel = makepixel(r, g, b);
			if (lseek(fd, offset, SEEK_SET) < 0) {
				printf("ERROR fbdev lseek\n");
				exit(1);
			}

			write(fd, &pixel, 2);
		}
	}	
	close(fd);
}

int main( ){
	BMP Image;
	Image.ReadFromFile("/root/pressimage.bmp");
	cout << "width : " << Image.TellWidth() << endl;
	cout << "height : " << Image.TellHeight() << endl;
	write_rgb(Image);
}
