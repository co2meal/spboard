#include "dotmatrix.h"

void dotmatrix (int data)
{
        int dev;
        
	
	 dev = open("/dev/dotmatrix", O_WRONLY);

	if(dev != -1) {
		// while(value != 0) {
		//	printf("Input counter value: [1-99] (0 : exit program) \n");
                  //      scanf("%d", &value);
                        if(data < 100)
                                write(dev, &data, 4);
			else {
				printf(" Error : Number length 1-99 \n");
				exit(1);
			}
              //  }
                close(dev);
        } else {
		printf("Device Open ERROR!\n");
                exit(1);
        }
        
}
