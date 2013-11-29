#ifndef _AUDIO_LIB_
#define _AUDIO_LIB_
/*
Modes and Limits:
Device	Sample	Minimum	Maximum
Channels	Size		Rate		Rate
-------- ---------------- --------
	2	16		8000		32000
*/
	#include <unistd.h>
	#include <fcntl.h>
	#include <sys/types.h>
	#include <sys/ioctl.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <linux/soundcard.h>
	#include <sys/types.h> 
	#include <sys/socket.h>

	char Init_audio(int rate);

	int Record_Audio_Data(char* buf,int size);

	void Play_Audio_Data(char* buf,int size);
	
	void close_audio();
#endif
	
