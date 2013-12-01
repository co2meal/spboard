#include "headers.h"

int touched;

#define EVENT_BUF_NUM 64
int event_fd = -1; /* the file descriptor for the device */

void* touch(void *data) 
{
	int i;
	size_t read_bytes;
	struct input_event event_buf[EVENT_BUF_NUM];
	char *device = "/dev/input/event0";

	// input device open
	if ((event_fd = open(device, O_RDONLY)) < 0)
	{
		printf("%s: open error", device);
		exit(1);
	}
	// printf("open : [%s] \n",device);   // raoudi

	while (touched==0)
	{
		read_bytes = read(event_fd, event_buf, (sizeof(struct input_event)*EVENT_BUF_NUM) );
		if( read_bytes < sizeof(struct input_event) )
		{
			printf("%s: read error", device);
			exit(1);
		}
		printf(" touch Event... \n");

		int x,y;

		for( i=0; i<(read_bytes/sizeof(struct input_event)); i++ )
		{
			switch( event_buf[i].type )
			{
				case EV_SYN:
					printf("---------------------------------------\n");
					break;

				case EV_ABS:
					switch (event_buf[i].code)
					{
						case ABS_X:
							x = event_buf[i].value;
							printf("X position: %d\n", event_buf[i].value);

							break;
						case ABS_Y:
							y = event_buf[i].value;
							printf("Y position: %d\n", event_buf[i].value);
							break;
						case ABS_PRESSURE:
							if (87 * 1000  / 320 < x && x < 275 * 1000 / 320
									&&  75 * 1000 / 240 < y && y < 170 * 1000 / 240) {
								touched = 1;
								printf("Touched!!!\n");
							}
							printf("Pressure : %s\n", (event_buf[i].value > 1)? "yes":"no" );
							break;
						default:
							printf("Touch Unknown: type %d, code %d, value %d\n",
									event_buf[i].type,
									event_buf[i].code,
									event_buf[i].value);
							break;
					}
					break;
				default:
					printf("Unknown: type %d, code %d, value %d\n",
							event_buf[i].type,
							event_buf[i].code,
							event_buf[i].value);
					break;
			}

		}
	}

}
