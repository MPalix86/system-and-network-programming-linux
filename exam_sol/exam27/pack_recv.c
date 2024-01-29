/*
*	author : Mirco Palese
*	program name : pack_recv
*	last edit :	18/01/2020
*	description : receive packets generated by pac_gen
*	note : thi programm is meant to be run as pack_gen child (not standalone)
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../include/apue.h"

#define BUFFSIZE 50
#define MAX_FIFO_SIZE 100


void* fill_fifo(void* val);
void pr_fifo();
void* pop(void *val);
void pr_default();
void pr_red();

unsigned char **fifo;	/* fifo array */
int fifo_size = 0;		/* fifo size */
pthread_mutex_t mu;		/* mutex to synchronize threads */



int main (int argc , char *argv[]){
	int err;
	pthread_t tid1, tid2, tid3;
	
	pthread_mutex_init(&mu, NULL);	/* mutex initializzations */
	
	/* creating thread */
	if ( (err = pthread_create(&tid1, NULL, fill_fifo, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	if ( (err = pthread_create(&tid2, NULL, pop, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	if ( (err = pthread_create(&tid3, NULL, pop, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	
	/* waiting for thread termination */
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);
	
	return 0;
	
}




/* 
* fill_fifo, receive the packet send trough the pipe by pack_gen, allocating the memory  
* of next entry in fifo_size based on the first 4 bytes of received pack, that contains an
* int that specify the packet length
*/
void* fill_fifo(void* val){
	unsigned char buf[1024];	
	int readend = 3; /* read end of the pipe */
	int bytenum, n, err; 
	fifo = malloc(MAX_FIFO_SIZE * sizeof(char*));

	
	close(4);	/* close write end of the pipe */
	for (;;){
		if( (err = pthread_mutex_lock(&mu)) != 0) err_exit(err,"lock mutex error: ");
		if ( (n = read(readend, buf, BUFFSIZE)) < 0 ) printf("error");
		if(n == 0) continue;
		else{
			memcpy(&bytenum,&buf,4);		/* 
									 		* recovering the first 4 byte of the packet that contains
									 		* the bytenum
									 		*/
  	
 			fifo[fifo_size] = realloc(fifo[fifo_size],(bytenum) * sizeof(char));
  			for (int j = 0; j < bytenum; j++){
					fifo[fifo_size][j] = buf[j];
 			}
 			pr_fifo();
			fifo_size ++;
 		}
 		if( (err = pthread_mutex_unlock(&mu)) != 0) err_exit(err,"unlock mutex error: ");
 		sleep(1);
 	}

}




/* pop, remove last packet from fifo */
void* pop(void *val){
	int err;
	for(;;){
		if(fifo_size > 2){
			if( (err = pthread_mutex_lock(&mu)) != 0) err_exit(err,"lock mutex error: ");
			fifo[fifo_size] = NULL;
			fifo_size--;
			printf("POP\n");
			pr_red();
			pr_fifo();
			pr_default();
			if( (err = pthread_mutex_unlock(&mu)) != 0) err_exit(err,"unlock mutex error: ");
		}
		else sleep(3);
	}
	
}




/* print the fifo */
void pr_fifo(){
	int bytenum, err;
	for(int i = 0; i <= fifo_size; i++){
		memcpy(&bytenum,fifo[i],4);		
		for (int j = 4; j < bytenum; j++){
			printf("%d ", fifo[i][j]);
		}
		printf("\n");
	}
	printf("\n\n\n");
}




/* print default terminal color */
void pr_default(){
	printf("\033[0m");
}




/* print red terminal color */
void pr_red(){
	printf("\033[0;31m"); /* red */
}


