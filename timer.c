#include <time.h>
#include "headers.h"
#include "timer.h"
#include "mutex.h"

void* timer(void *data){

	int fd;
	int value;

	if((fd=open("/dev/7segment",O_RDWR|O_SYNC)) < 0) {
		printf("7segment open fail\n");
		exit(1);
	}

	time_t tim;
	struct tm *t;


         // time 구조체를 구분하여 주는 함수
	 //   // localtime 을 이용하여 구분해야 원하는 시간만 따로 뽑아낼 수 있다.  //
	 //         // 원하는 시간을 따로 나타내기
	 //
	 //                     // 더 표현하고 싶은 시간은 아래 tm 구조체 참고.
	 //
	 //                        // 현재 경과된 초. unix time
	       //printf("unix time: %d \n", tim);
	 //
	 //                              // 년월일 형식으로 나타내기
	 //                                 // ctime 은 년월일과 요일등을 스트링형으로 반환한다. 
	 //printf("current time: %s \n", ctime(&tim) );
	
		tim = time(NULL);
	 	t = localtime(&tim);
		value = 0;
		value += t->tm_hour * 10000;
		value += t->tm_min * 100;
		value += t->tm_sec;
	while(1){        
		//pthread_mutex_lock(&write_mutex);
		//printf("value = %d\n", value);
		//printf("A\n");
        	write(fd,&value,4);
		//printf("B\n");
		//pthread_mutex_unlock(&write_mutex);
		//printf("C\n");
	}

	close(fd);

}
	
	 
	 
	 	 
