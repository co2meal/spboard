#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
        int dev,temp=0;
        int value;

        dev = open("/dev/dotmatrix", O_WRONLY);

	if(dev != -1) {
		 while(value != 0) {
			printf("Input counter value: [1-99] (0 : exit program) \n");
                        scanf("%d", &value);
                        if(value < 100)
                                write(dev, &value, 4);
			else {
				printf(" Error : Number length 1-99 \n");
				exit(1);
			}
                }
                close(dev);
        } else {
		printf("Device Open ERROR!\n");
                exit(1);
        }
        printf("Program Exit !!\n");
        return 0;
}
