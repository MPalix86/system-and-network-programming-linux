/*
*	author : 		Mirco Palese
*	program name : 	thread_id
*	last edit : 	22/01/2020		
*	description : 	print thread_id
*	note : 			new thread obtains its thread ID by calling pthread_self instead of reading it 
*					out of shared memory or receiving it as an argument to its thread-start routine. 
*					Recall that pthread_create will return the thread ID of the newly 
*					created thread through the ﬁrst parameter (tidp). In our example, the 
*					main thread stores this ID in tid, but the new thread can’t safely use it. 
*					If the new thread runs before the main thread returns from calling 
*					pthread_create, then the new thread will see the uninitialized contents
*					of ntid instead of the thread ID
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* thread(void *ptr);
void print_ids(const char *s);
	
	pthread_t tid;

int main (int argc , char *argv[]){
	

	int err;
	
	/** main thread id*/
	tid = pthread_self();
	
	if ( (err = pthread_create(&tid, NULL, thread, NULL)) != 0 ){
		printf("error during thread creation");
		exit(0);
	}	
	
	print_ids("main thread\n");
	sleep(1); /** avoid return before starting of new thread (race condition) */
	return 0;
	
}




/** 
* void* thread(void *ptr) 
* 
* E' una funzione che accetta come parametro un puntatore di tipo void e ritorna un puntatore di tipo void.
* nota che void* thread(void *ptr) è totalmente diverso da void* *(thread)(void *ptr) 
* infatti quest'ultimo è un puntatore a funzione. 
* per maggiori info : http://www.federica.unina.it/ingegneria/programmazione-1-ing/puntatori-funzione/
*/
void* thread(void *ptr){
	print_ids("second thread\n");
	return NULL;
}




void print_ids(const char *s){
	
	pthread_t tid1;
	int pid;
	tid = pthread_self();

	printf("%s", s);
	printf ("thread id : %lld\n", tid1);
	printf ("Process id : %d\n", getpid());

}


