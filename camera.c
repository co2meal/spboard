#include "headers.h"
#include "camera.h"
#include "keypad.h"
#include "jpeg_capture.h"
#include "mutex.h"
#include "attendance.h"
#include "dotmatrix.h"
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
			execl("/usr/local/bin/madplay", "/usr/local/bin/madplay", "/root/mpthreetest.mp3", NULL);
			cout << attendance(sid,"1","temp.jpg") << endl;
			Json::Value res = attendance(sid,"1","temp.jpg");
			if(res["status"]== "sucess")
				printf("OK!!!!\n");
			else if(res["status"]== "late")
				printf("half OK!!!!\n");
			else
				printf("NO OK!!!!!\n");

			if(res["status"]=="sucess"||res["status"]=="late")
			{
				if(res["authenticated"]=="warning")
					printf("you die!\n");

				else
					printf("Good Student!!\n");
			}


			if (1) {
				dotmatrix(23);
			} else {
				dotmatrix(10);
			}
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
