#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include "headers.h"
#include "keypad.h"
#include "camera.h"
#include "timer.h"
#include "buzzer.h"
#include "led.h"
#include "curltest.h"
#include "dotmatrix.h"
int main() {
	pthread_t camera_thread;
	pthread_t keypad_thread;
//	pthread_t segment_thread;
	pthread_t led_thread;
	
	

	
	int camera_thread_id = pthread_create(&camera_thread, NULL, camera, NULL);
	if (camera_thread_id < 0) {
		perror("thread create error: ");
		exit(0);
	}
	
	int keypad_thread_id = pthread_create(&keypad_thread, NULL, keypad, NULL);
	if (keypad_thread_id < 0) {
		perror("thread create error: ");
		exit(0);
	}
/*
	int segment_thread_id = pthread_create(&segment_thread, NULL, timer, NULL);
	if (segment_thread_id < 0) {
	        perror("thread create error: ");
	        exit(0);
	}
*/
	int led_thread_id = pthread_create(&led_thread, NULL, led, NULL);
	if (led_thread_id < 0) {
	        perror("thread create error: ");
	        exit(0);
	}

	cout << "TQ?" << endl;

	//curltest();

	//cout << attendance("1", "1", "30", "photo.jpg") << endl;
	//cout << attendance("2", "1", "30", "photo.jpg") << endl;

	pthread_join(camera_thread, NULL);
	pthread_join(keypad_thread, NULL);
//	pthread_join(segment_thread, NULL);
	pthread_join(led_thread, NULL);
}
