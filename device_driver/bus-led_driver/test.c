#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int dev;
	char buff;
	
	if(argc <= 1) {
		printf("please input the parameter! ex)./test 0xa1\n");
		exit(1);
	}

	dev = open("/dev/led", O_WRONLY);
	if (dev != -1) {
		if(argv[1][0] == '0' && (argv[1][1] == 'x' || argv[1][1] == 'X'))
			buff  = (unsigned char)strtol(&argv[1][2],NULL,16);
		else 		
			buff = atoi(argv[1]);
		write(dev,&buff,1);
		close(dev);
	} else {
		printf( "Device Open ERROR!\n");
		exit(1);
	}

	return 0;
}
