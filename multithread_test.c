/*
*	author : Mirco Palese
*	program name : multithread_test
*	last edit :	29/01/2019
*	description : demonstation of how treads work and their synchronization
*	note : as you can se there's no pattern between trhead 1 and 2. execution is asynchronous
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define EXEC_TIME 5
#include "include/apue.h"

void* thread1(void *);
void* thread2(void *);

int main (int argc , char *argv[]){
	pthread_t tid1;
	pthread_t tid2;
	int err;
	int a;
	printf("creating thread 1\n");
	err = pthread_create(&tid1, NULL, thread1, NULL);
	if (err != 0){
		printf("error during thread creation\n");
		exit(1);
	}
	printf("tid1 %lld\n", tid1);
	printf("creating thread 2\n");
	err = pthread_create(&tid2, NULL, thread2, NULL);
	if(err != 0){
		printf("error during thread creation\n");
		exit(1);
	}
	printf("tid1 %lld\n", tid2);
	
	
	if ( (err = pthread_join(&(*tid1), NULL)) != 0){
		printf("some error on pthread_join %d\n", err);
	}
	if ( (err = pthread_join(&(*tid2), NULL)) != 0){
		printf("some error on pthread_join %d\n",err);
	}

	return 0;
}


void* thread1(void *arg){
	for(int i = 0; i < EXEC_TIME; i++) {
		printf("thread 1\n");
		sleep(1);
	}
	pthread_exit((void *)0);
}


void* thread2(void *arg){
	for(int i = 0; i < EXEC_TIME; i++){
		printf("thread 2\n");
		sleep(1);
	}
	pthread_exit((void *)0);
}




