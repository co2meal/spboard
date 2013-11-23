#include "headers.h"
#include "charlcd.h"
#include "buzzer.h"
#include "timer.h"
int keypad_dev;
unsigned char vkey;
char tmp;
string tmpsid;
string text;

extern int camera_stop;

void usrsignal(int sig)
{
        read(keypad_dev, &vkey, 1);
        printf("Key input: %2d\n", vkey);

	if(1 <= vkey && vkey <= 9) {
		if(tmpsid.length()>=9){
			printf("exceed the number!!\n");
			Buzzer_Control(2,1);
			tmpsid="";
			return;			
		}
                tmpsid+=vkey+'0';
	} else if (vkey == 10) {
		if(tmpsid.length()>=9){
			printf("exceed the number!!\n");
			Buzzer_Control(2,1);
			tmpsid="";
			return; 
		}
		tmpsid += '0';
	} else if (vkey == 16) {
		printf("sid : %s\n", tmpsid.c_str());
		if(tmpsid.length()<9){
			printf("Need More Number!!!\n");
			Buzzer_Control(2,1);
			tmpsid="";
			return ;
	}
		camera_stop = 1;
		tmpsid="";
		timer(NULL);
		
	}else {
		printf("vkey : %d\n", vkey);
	}

	text = "press your ID";

	string second = tmpsid;
	while(second.length() < 9)
		second += '_';
	

 
		
	
	//	printf("Wrong Student ID!!\n");
		
	
	charlcd(text,second);
	
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

	
        while(1) {
        }
	
        close(keypad_dev);

}


