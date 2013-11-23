#include "headers.h"

int keypad_dev;
unsigned char vkey;
char tmp;
string tmpsid;

extern int camera_stop;

void usrsignal(int sig)
{
        read(keypad_dev, &vkey, 1);
        printf("Key input: %2d\n", vkey);

	if(1 <= vkey && vkey <= 9) {
                tmpsid+=vkey+'0';
	} else if (vkey == 10) {
		tmpsid += '0';
	} else if (vkey == 16) {
		printf("sid : %s\n", tmpsid.c_str());
		camera_stop = 1;
	}else {
		printf("vkey : %d\n", vkey);
		
	}
        if(vkey != 16){
                if(1<=vkey &&vkey<=9)
                        tmp = vkey;
                else if (vkey ==10)
                        tmp =0;
                else if (vkey>=11&&vkey<=15)
                        tmp = 1 << (vkey-7);

        }
}

void* keypad(void* data) {
        keypad_dev = open("/dev/keypad", O_RDWR);
        if(keypad_dev<0) {
                printf("application : keypad driver open fail!.\n");
                exit(1);
        }

        (void)signal(SIGUSR1, usrsignal);
        int id = getpid();
        write(keypad_dev, &id, 4);

	/*
        while(1) {
        }
	*/
        close(keypad_dev);

}


