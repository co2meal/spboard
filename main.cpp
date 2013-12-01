#include "headers.h"
#include "EasyBMP.h"
#include "touch.h"
#include "segment.h"
#include "keypad.h"
#include "camera.h"
#include "led.h"

typedef unsigned char ubyte;

unsigned short makepixel(unsigned char r, unsigned char g, unsigned char b) {
	return (unsigned short) (((r >> 3) << 11)|((g >> 2) << 5) | (b >> 3));
}

int active_mode;
int waiting_mode;


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

void active() {
	active_mode = 1;
	captured = 0;
	pthread_t led_thread;

	int led_thread_id = pthread_create(&led_thread, NULL, led, NULL);
	if (led_thread_id < 0) {
		perror("thread create error: ");
		exit(0);
	}

        keypad_dev = open("/dev/keypad", O_RDWR);
        if(keypad_dev<0) {
                printf("application : keypad driver open fail!.\n");
                exit(1);
        }

        signal(SIGUSR1, pressed);
        int id = getpid();
        write(keypad_dev, &id, 4);


        camera();

        close(keypad_dev);

	active_mode = 0;
	pthread_join(led_thread, NULL);

}

void waiting() {
	waiting_mode = 1;
	touched = 0;
BMP Image;
	Image.ReadFromFile("/root/pressimage.bmp");
	write_rgb(Image);

	pthread_t touch_thread;
	int touch_thread_id = pthread_create(&touch_thread, NULL, touch, NULL);
	if (touch_thread_id < 0) {
		perror("thread create error: ");
		exit(0);
	}
	pthread_t segment_thread;
	int segment_thread_id = pthread_create(&segment_thread, NULL, segment, NULL);
	if (segment_thread_id < 0) {
	        perror("thread create error: ");
	        exit(0);
	}
	pthread_join(touch_thread, NULL);
	pthread_join(segment_thread, NULL);

	waiting_mode = 0;
}
int main() {
	while(1) {
		waiting();
		active();
	}
}
