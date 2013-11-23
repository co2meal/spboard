#include "headers.h"

/* Buzzer을 제어하기 위한 물리주소 */
#define ADDRESSOFBUZZER 0x08808000
int Buzzer_Control(int a,int b) {
         int fd;
         unsigned char *addr_buzzer;
         if ((fd=open("/dev/mem",O_RDWR|O_SYNC)) < 0) {
                    perror("mem open fail\n");
                    exit(1);
          }
          /* mmap()함수를 통해 물리주소를 사용하기위한 주소할당 */
          addr_buzzer= (unsigned char *) mmap(NULL,4096,PROT_WRITE,
    MAP_SHARED,fd,ADDRESSOFBUZZER);
          if(addr_buzzer < 0) {
                    close(fd);
                    printf("mmap error\n");
                    exit(1);
          }
          /* a=>Buzzer 동작방법, b=> Buzzer 울리는 시간(초) */
          *addr_buzzer = 0x00;
          if(a==1)
                    *addr_buzzer = 0x01;
          else if(a==0)
                    *addr_buzzer = 0x00;
          else if(a==2){
                    *addr_buzzer = 0x01;
                    sleep(b);
                    *addr_buzzer = 0x00;
  }
  munmap(addr_buzzer, 4096);
  close(fd);
  return 0;
}

