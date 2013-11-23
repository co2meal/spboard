#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

static int dev;
static unsigned char vkey;

void usrsignal(int sig)
{
	read(dev, &vkey, 1);
	printf("Key input: %2d\n", vkey);
}

int main(void)
{
	pid_t id;
	int quit = 1;
	
	dev = open("/dev/keypad", O_RDWR);
	if(dev<0) {
		printf("application : keypad driver open fail!.\n");
		exit(1);
	}
	
	(void)signal(SIGUSR1, usrsignal);
	id = getpid();
	write(dev, &id, 4);
	printf("press the key button!\n");
	printf("press the key 16 to exit!\n");
	
	while(quit) {
		if(vkey == 16) {
			printf("Exit Program!! (key = %d)\n", vkey);
			quit = 0;
		}
	}
	close(dev);
	return 0;
}
