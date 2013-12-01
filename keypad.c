#include "headers.h"
#include "charlcd.h"
#include "buzzer.h"
#include "camera.h"

int keypad_dev;
unsigned char vkey;
char tmp;
string tmpsid;
string sid;
string text;

void pressed(int sig)
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

		captured = 1;
		sid = tmpsid;
		tmpsid="";
		
	}else {
		printf("vkey : %d\n", vkey);
	}

	text = "press your ID";

	string second = tmpsid;
	while(second.length() < 9)
		second += '_';
	


		
	
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

