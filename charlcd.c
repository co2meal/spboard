
#include "textlcd.h"
#include "headers.h"

int charlcd(string first, string second)
{
	int i,dev;
	//char buf[13] = "wellcome!!!!!";
	//char wellcom[16];
	//char wellcom2[16];
	struct strcommand_varible strcommand;
	
	strcommand.rows = 0;		
	strcommand.nfonts = 0;		
	strcommand.display_enable = 1;		
	strcommand.cursor_enable = 0;		
	strcommand.nblink = 0;		
	strcommand.set_screen = 0;		
	strcommand.set_rightshit = 1;		
	strcommand.increase = 1;		
	strcommand.nshift = 0;		
	strcommand.pos = 0;		
	strcommand.command = 1;		
	strcommand.strlength = 16;		
	
	dev = open("/dev/textlcd", O_WRONLY|O_NDELAY );
	
	if (dev != -1) {

//		write(dev,buf,13);
#if 1
//		sleep(2);
		ioctl(dev,TEXTLCD_CLEAR,&strcommand,32);
		strcommand.pos = 0;
		ioctl(dev,TEXTLCD_DD_ADDRESS,&strcommand,32);
		for(i=0;i<first.length();i++) {//첫줄 
			memcpy(&strcommand.buf[0],&first.c_str()[i],16);
			ioctl(dev,TEXTLCD_WRITE_BYTE,&strcommand,32);
		}
//		sleep(2);
		strcommand.pos = 40;
		ioctl(dev,TEXTLCD_DD_ADDRESS,&strcommand,32);
		for(i=0;i<second.length();i++) {//둘쨋줄 
			memcpy(&strcommand.buf[0],&second.c_str()[i],16);
			ioctl(dev,TEXTLCD_WRITE_BYTE,&strcommand,32);
		}
#endif
		close(dev);
	} else {
		printf( "LCD driver open error!!\n");
		exit(1);
	}
	return 0;
}


