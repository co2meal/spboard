#include "headers.h"
#include "timer.h"
#include <time.h>


void* timer(void *data){

int fd;
int value;

if((fd=open("/dev/7segment",O_RDWR|O_SYNC)) < 0) {
       printf("FND open fail\n");
       exit(1);
       }
/*
while(value!=0){
	
	printf("Input counter value:\n");
	scanf("%d",&value);
	write(fd,&value,4);								        }
*/

 time_t tim;
    struct tm *t;

       tim = time(NULL);

         // time 구조체를 구분하여 주는 함수
	 //   // localtime 을 이용하여 구분해야 원하는 시간만 따로 뽑아낼 수 있다.
	      t = localtime(&tim);
	 //
	 //         // 원하는 시간을 따로 나타내기
	       printf("second: %d \n", t->tm_sec);
	       printf("minute: %d \n", t->tm_min);
	       printf("hour: %d \n", t->tm_hour);
	 //
	 //                     // 더 표현하고 싶은 시간은 아래 tm 구조체 참고.
	 //
	 //                        // 현재 경과된 초. unix time
	       printf("unix time: %d \n", tim);
	 //
	 //                              // 년월일 형식으로 나타내기
	 //                                 // ctime 은 년월일과 요일등을 스트링형으로 반환한다. 
	       printf("current time: %s \n", ctime(&tim) );
	
	while(t!=0){
		        
        write(fd,&t,4);                                                                     }
}
	
	 
	 
	 	 
