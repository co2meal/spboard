#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main()
{
        
	int fd,tt=121212;
	int value;

        if((fd=open("/dev/7segment",O_RDWR|O_SYNC)) < 0) {
                printf("FND open fail\n");
                exit(1);
        }
	time_t tim;
	struct tm *t;
        while(1) {
       //         printf("Input counter value : (0 : exit program) \n");
         //      scanf("%d", &value);
           //     write(fd,&value,4);
		
		while(1)
		{
	tim=time(NULL);
	t=localtime(&tim);
	value =0;
	value += t->tm_hour * 10000;
        value += t->tm_min * 100;
        value += t->tm_sec;
			write(fd,&value,4);
		}
        }

	close(fd);
        return 0;
           
} 
