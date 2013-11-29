/*
 * parrot.c
 * Program to illustrate /dev/dsp device
 * Records several seconds of sound, then echoes it back.
 * Runs until Control-C is pressed.
 */


#include <stdio.h>
#include"audio_lib.h"

#define LENGTH 1    /* how many seconds of speech to store */
#define SAMPLING_RATE 8000   /* the sampling rate */
#define SIZE 16     /* sample size: 16 bits */
#define CHANNELS 2  /* stereo */

/* this buffer holds the digitized audio */
char buf[LENGTH*SAMPLING_RATE*SIZE*CHANNELS/8];


int main()
{
 	Init_audio(SAMPLING_RATE);
	init_keyboard();	

	while (1) {
  
    if(kbhit()) break;

		printf("Say something:\n");	
		Record_Audio_Data(buf,sizeof(buf));
		
		printf("You said: \n");	
		Play_Audio_Data(buf,sizeof(buf));

	}
	close_keyboard() ;
//  close(fd);
	close_audio();
}
