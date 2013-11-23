#include "headers.h"
#include "camera.h"
#include "jpeg_capture.h"
#include "mutex.h"
#define XPOS 240
#define YPOS 320

int camera_stop = 0;

void* camera(void *data) {
        int camera_dev;
        int fd;
        unsigned short rgb[XPOS*YPOS];

        if((camera_dev = open("/dev/camera", O_RDWR)) == -1 ) {
                printf("Can't open dev_cad(camera_dev, rgb, 153600); camera %d\n", camera_dev);
                exit(1);
        }

        fd = open("/dev/fb", O_RDWR);
        if (fd < 0) {
                printf("Error fbdev open\n");
                exit(1);
        }
        while(1) {
		if (camera_stop) {
			pthread_mutex_lock(&write_mutex);
			printf("camera stop!\n");
			jpeg_capture("temp.jpg", rgb);
			printf("picture saved!\n");
			pthread_mutex_unlock(&write_mutex);
        		sleep(1);
			camera_stop = 0;
		}
        	read(camera_dev, rgb, 153600);


		// 이렇게해보기
                if (lseek(fd, 0, SEEK_SET) < 0) {
                        printf("ERROR fbdev lseek!\n");
                        exit(1);
                }
		write(fd, rgb, 153600);
        }

        close(camera_dev);
        close(fd);
	
	
}
