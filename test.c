#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>

using namespace std;

static int keypad_dev;
static unsigned char vkey;
char tmp;
string tmpsid;

void camera()
{
	int XPOS= 240;
	int YPOS =320;
	int camera_dev;
	int fd;
	int t, tt;
	unsigned short rgb[XPOS*YPOS];
	unsigned int offset;
	unsigned short pixel;

	if((camera_dev = open("/dev/camera", O_RDWR)) == -1 ) {
		printf("Can't open dev_cad(camera_dev, rgb, 153600); camera %s\n", 			camera_dev);
		exit(1);
	}

	fd = open("/dev/fb", O_RDWR);
	while(1) {
	sleep(100);
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
	vkey=0;	
	break;
	}

	close(camera_dev);
	close(fd);
}
void led(unsigned char sig) {
	int dev = open("/dev/led", O_WRONLY);
	//printf("dev = %d\n", dev);

	if (dev == -1) {
		printf("Device Open ERror!!\n");
		exit(1);
	}
	printf("%s\n",tmpsid.c_str());
	tmpsid="";
	write(dev, &sig, 1);
	close(dev);
}

void usrsignal(int sig)
{
	read(keypad_dev, &vkey, 1);
	printf("Key input: %2d\n", vkey);
	
	if(vkey != 16){
		tmpsid+=vkey+'0';
	
		if(1<=vkey &&vkey<=9)
			tmp = vkey;
		else if (vkey ==10)
			tmp =0;
		else if (vkey>=11&&vkey<=15)
			tmp = 1 << (vkey-7);
	}
}

int main(void)
{
	pid_t id;
	int quit = 1;
	
	keypad_dev = open("/dev/keypad", O_RDWR);
	if(keypad_dev<0) {
		printf("application : keypad driver open fail!.\n");
		exit(1);
	}
	
	(void)signal(SIGUSR1, usrsignal);
	id = getpid();
	write(keypad_dev, &id, 4);
	printf("press the key button!\n");
	printf("press the key 16 to exit!\n");

	

	while(quit) {
		if(vkey == 16) {
			led(tmp);
			vkey=0;
			camera();
			printf("asdfasdfasdfasdf");		
			vkey=0;
		}
		vkey=0;
	}
	close(keypad_dev);
	return 0;
}
