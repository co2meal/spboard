#include "headers.h"
#include "led.h"
#include "camera.h"

int communicating;
unsigned char buff = 170;
int dev;

void mark_communicating(int a){
	communicating = a;
}
void led_shuffle(){
       buff = ~buff;
}
void led_on(){
	write(dev,&buff,1);
}
void led_off(){
	buff = 0;
	write(dev,&buff,1);
}

void* led(void *data) {
	dev = open("/dev/led", O_WRONLY);

	if (dev != -1) {
		while(active_mode){
			if (communicating == 1){
				led_on();
				usleep(50000);
				led_shuffle();
			} 
			else {
				led_off();
			}
		}
		close(dev);
	} 
		else {
		printf( "Device Open ERROR!\n");
		exit(1);
	}

}
