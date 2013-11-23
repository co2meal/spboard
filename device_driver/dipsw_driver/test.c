#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <asm/fcntl.h>
#include <signal.h>

int dev;
char vkey[4], old_vkey[4],temp[4];

void usrsignal(int sig){
        read(dev, &vkey, 4);
}	

int main(int argc, char **argv)
{
	pid_t id;
	int quit = 1,i,ret;
	char cmp = 'a';

	if(argc <=1) {
		printf("Usage : please input Stop Number! ex)./test 1234\n");
		exit(1);
	}

	if ((dev=open("/dev/dipsw",O_RDWR|O_SYNC)) < 0) {
		perror("DIPSW open fail\n");
		exit(1);
	}


	(void)signal(SIGUSR1, usrsignal);
	id = getpid();
	write(dev,&id,sizeof(id));

	while(quit) {
		if((strlen(argv[1]) > 4)) {
			printf("Error : Too long value \n");
			exit(1);
		}	

		if(vkey[0] != old_vkey[0] || vkey[1] != old_vkey[1] || vkey[2] != old_vkey[2] 
														|| vkey[3] != old_vkey[3]) {

			printf("%01x %01x %01x %01x \n",vkey[0],vkey[1],vkey[2],vkey[3]);

			old_vkey[0] = vkey[0];
			old_vkey[1] = vkey[1];
			old_vkey[2] = vkey[2];
			old_vkey[3] = vkey[3];

			for(i=0;i<=3;i++) {
				if(argv[1][i] < cmp)	
					temp[i] = vkey[i]+48;
				else
					temp[i] = vkey[i]+87;
			}

			if(temp[0] == argv[1][0] && temp[1] == argv[1][1] && temp[2] == argv[1][2] 
															&& temp[3] == argv[1][3]) {
				printf("Program Terminated!! \n");
				quit =0;
			}		
			ret =0;
		}
	}
        close(dev);
        return 0;
}




