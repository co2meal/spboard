#include "headers.h"
#include "camera.h"
#define XPOS 240
#define YPOS 320

int camera_stop = 0;

void* camera(void *data) {
        int camera_dev;
        int fd;
        int t, tt;
        unsigned short rgb[XPOS*YPOS];
        unsigned int offset;
        unsigned short pixel;

        if((camera_dev = open("/dev/camera", O_RDWR)) == -1 ) {
                printf("Can't open dev_cad(camera_dev, rgb, 153600); camera %s\n",                      camera_dev);
                exit(1);
        }

        fd = open("/dev/fb", O_RDWR);
        while(1) {
		if (camera_stop) {
        		sleep(1);
			printf("camera stop!\n");
			camera_stop = 0;
		}
        	read(camera_dev, rgb, 153600);

                if (fd < 0) {
                        printf("Error fbdev open\n");
                        exit(1);
                }

                for(t= 0; t < YPOS; t++) {
                        for (tt = 0; tt < XPOS; tt++) {
                                offset = (t * XPOS * 2) + (tt * 2);
                                pixel = rgb[t * XPOS + tt];
                                if (lseek(fd, offset, SEEK_SET) < 0) {
                                        printf("ERROR fbdev lseek!\n");
                                        exit(1);
                                }
                                write(fd, &pixel, 2);
                        }
                }
        }

        close(camera_dev);
        close(fd);
	
	
}
