#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

#define EVENT_BUF_NUM 64
int event_fd = -1; /* the file descriptor for the device */

int main(int argc, char **argv)
{
	int i;
	size_t read_bytes;
	struct input_event event_buf[EVENT_BUF_NUM];
	char *device;

	if( argc != 2 )
	{
		printf("Usage: touchtest key fpga\n");
		printf("or\n");
		printf(" touchtest touch\n");
		exit(1);
	}
	if( !strncmp("touch", argv[1], 5) )       // TouchScreen
	{
		device = "/dev/input/event0";
	}
  /*
	else if( !strncmp("key", argv[1], 3) )    // FPGA Keypad
		device = "/dev/input/event1";
	}
  */
	else
	{
		printf("device argument error\n");
		exit(1);
	}

	// input device open
	if ((event_fd = open(device, O_RDONLY)) < 0)
	{
		printf("%s: open error", device);
		exit(1);
	}
  printf("open : [%s] \n",device);   // raoudi

	while (1)
	{
		read_bytes = read(event_fd, event_buf, (sizeof(struct input_event)*EVENT_BUF_NUM) );
		if( read_bytes < sizeof(struct input_event) )
		{
			printf("%s: read error", device);
			exit(1);
		}
    printf(" touch Event... \n");

		for( i=0; i<(read_bytes/sizeof(struct input_event)); i++ )
		{
				switch( event_buf[i].type )
				{
						case EV_SYN:
								printf("---------------------------------------\n");
								break;
						case EV_KEY:
								printf("Button code %d", event_buf[i].code);
								switch (event_buf[i].value)
								{
										case 1:
												printf(": pressed\n");
												break;
										case 0:
												printf(": released\n");
												break;
										default:
												printf("Unknown: type %d, code %d, value %d",
																event_buf[i].type,
																event_buf[i].code,
																event_buf[i].value);
												break;
								}
								break;

						case EV_ABS:
								switch (event_buf[i].code)
								{
										case ABS_X:
												printf("X position: %d\n", event_buf[i].value);
												break;
										case ABS_Y:
												printf("Y position: %d\n", event_buf[i].value);
												break;
										case ABS_PRESSURE:
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
