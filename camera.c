#include "headers.h"
#include "camera.h"
#include "keypad.h"
#include "jpeg_capture.h"
#include "mutex.h"
#include "attendance.h"
#include "dotmatrix.h"
#define XPOS 240
#define YPOS 320

int captured;

void play_mp3(string filename) {

	if (fork() == 0)
		execl("/usr/local/bin/madplay", "/usr/local/bin/madplay", filename.c_str(), NULL);
}

void camera() {

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
	while(captured == 0) {

		read(camera_dev, rgb, 153600);


		// 이렇게해보기
		if (lseek(fd, 0, SEEK_SET) < 0) {
			printf("ERROR fbdev lseek!\n");
			exit(1);
		}
		write(fd, rgb, 153600);
	}
	pthread_mutex_lock(&write_mutex);
	printf("camera stop!\n");
	jpeg_capture("temp.jpg", rgb);
	printf("picture saved!\n");
	pthread_mutex_unlock(&write_mutex);
	Json::Value res = attendance(sid,"1","temp.jpg");
	cout << res << endl;

	if(res["error"]=="Server error!")
	{
		printf("Sorry. Server is failed!\n");

	}	
	else if(res["error"]=="No current lecture error")
	{
		printf("No lecture\n");

	}	
	else if(res["authenticated"]=="warning")//대리 출석 인증 시
	{
		play_mp3("/root/broken.mp3");
		dotmatrix(10); 
		printf("you are lying\n");
	}
	else                               //정상적으로 출석인증성공시
	{
		if(res["status"]== "success") //정상출석 시간에 출석
		{
			play_mp3("/root/verygood.mp3");
			printf("you are very good !\n");
			dotmatrix(23);
		}
		else if(res["status"]=="late")//지각 출석 시간에 출석
		{ 
			play_mp3("/root/good.mp3");
			printf("good but need more eariler\n");
			dotmatrix(23);
		}
		else
		{
			play_mp3("/root/broken.mp3");
			printf("you are absent\n");
			dotmatrix(10);
		}

	}

	close(camera_dev);
	close(fd);
}
