#include"audio_lib.h"

#define STEREO	2
#define SAMPLE_SIZE 16   

int fd;	/* sound device file descriptor */


/* open sound device and init */
char Init_audio(int rate){
	
	int arg;	/* argument for ioctl calls */
	int status;	 /* return status of system calls */	
	char ret = 0;

	fd = open("/dev/dsp", O_RDWR);

	if (fd < 0)
	{
		perror("open of /dev/dsp failed");
		return -1;
	}
	
	/* set sampling parameters */
	arg = SAMPLE_SIZE;	 /* sample size */

	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);

	if (status == -1)
		perror("SOUND_PCM_WRITE_BITS ioctl failed\n");

	if (arg != SAMPLE_SIZE)
		printf("unable to set sample size \n");

	
	arg = STEREO;	/* stereo */
	status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);

	if (status == -1)
		perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
	if (arg != STEREO)
		perror("unable to set number of channels");

	arg = rate;		 /* sampling rate */
	status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);

	if (status == -1)
		perror("SOUND_PCM_WRITE_WRITE ioctl failed");
	
	return 0;
}

int Record_Audio_Data(char* buf,int size)
{	
	int len;	

	len = read(fd, buf, size); /* record some sound */
  printf("### len = %d\n", len);

	if (len != size)
	{
		perror("read wrong number of bytes");
		return 0;
	}
	return len;
}	

void Play_Audio_Data(char* buf,int size)
{
	int status;	
	status = write(fd, buf, size); /* play it back */
	
	if (status != size)
		perror("wrote wrong number of bytes");

	/* wait for playback to complete before recording again */
	 status = ioctl(fd, SOUND_PCM_SYNC, 0); 
	
	if (status == -1)
		perror("SOUND_PCM_SYNC ioctl failed");
}

void close_audio()
{
//	int arg = 100000;
// 	ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);	
	close(fd);
}
