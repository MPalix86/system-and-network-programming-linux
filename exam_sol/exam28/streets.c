/*
*	author : Mirco Palese
*	program name : streets
*	last edit :	17/02/2020
*	description : simulate a street
*	note : run POSIXsem to see this program running (is not standalone)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../include/apue.h"
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>  
#include <string.h>     
#define MAX_ARR 5
#define MAX_SLEEP 5
#define MIN_ARR 1


void *rm(void *val);
void *add(void *val);
void sigint_handler(int dummy);
void exit_handler();
void pr_default();
void pr_color();

char *queue;
pthread_mutex_t mu;
sem_t *sem;
char streetnum; 	/* 
					* declared as char because street num come from argv[1], and POSIXsem 
					* (that starts this process as child) send streetnum as char like argument 
					* because of exec syscall that accepts only chars like argument
					*/

int main (int argc , char *argv[]){
	
	
	int err;
	pthread_t tid1 = NULL;
	pthread_t tid2 = NULL;
	srand(time(NULL)); 
	
	streetnum = *argv[1];			/* recovering streetnum */
	signal(SIGINT, sigint_handler);	/* installing signal handler */
	atexit(exit_handler);			/* installing exit handler */
	
	  
	queue = malloc(sizeof(char));  	/* allocating queue for the first time */
	queue[0] = 'a';					/* enter first vehicle in queue */	
	
	pthread_mutex_init(&mu, NULL);	/* mutex initializzations */
	
	/* creating thread for adding and removing vehicles from queue */
	if ( (err = pthread_create(&tid1, NULL, add, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	if ( (err = pthread_create(&tid1, NULL, rm, (void *)0 )) != 0 ) err_exit(err,"thread error: ");
	
	/* waiting for threads to terminates */
	pthread_join(tid1, NULL); 
	pthread_join(tid2, NULL); 

	return 0;
	
}



/* add vehicles in the queue (simulate vehicles arrive in a street)*/
void *add(void *val){
	int err;
		
	for(;;){
		if( (err = pthread_mutex_lock(&mu)) != 0) err_exit(err,"lock mutex error: ");
 		int sleep_time = rand() % MAX_SLEEP;
		int r = (rand() % (MAX_ARR - MIN_ARR + 1)) + MIN_ARR;  ;    
		size_t n = strlen(queue);
		queue = realloc(queue, sizeof(queue) + (sizeof(char) * r) );
		for(int i = 0; i < r; i++){
			queue[n+i] = 'a';
		}
		pr_color();
		printf("arrived %d\n",r);
		printf("queued %lu\n", strlen(queue));
		pr_default();
		if( (err = pthread_mutex_unlock(&mu)) != 0) err_exit(err,"unlock mutex error: ");
		sleep(sleep_time);
 	}
}



/* remove vehicles from queue (simulate vehicles transitation from street) */
void *rm(void *val){
	int err;
	const char *semname = "/sem";
	size_t n = 0;
	
	if(( sem = sem_open(semname, 0 )) ==  SEM_FAILED ) err_sys("semaphore error: ");
	
	for(;;){
		if(strlen(queue) > 0){
			if( sem_wait(sem) == -1) err_sys("semwait: ");
			if( (err = pthread_mutex_lock(&mu)) != 0) err_exit(err,"lock mutex error: ");
			n = strlen(queue);
			queue[n - 1] = (char)NULL;
			if( sem_post(sem) == -1) err_sys("semwait: ");
			pr_color();
			printf("car goes out, now in queue %lu\n", strlen(queue));
			pr_default();
			if( (err = pthread_mutex_unlock(&mu)) != 0) err_exit(err,"unlock mutex error: ");
			sleep(1);
		}
	}
}



/* on sigint (^C) call exit to call exit_handler */
void sigint_handler(int dummy) {
    exit(0);
}



/* exit handler */
void exit_handler(){
	if ( sem_close(sem) == -1) err_sys("semclose: ");
	pr_color();
	printf("street %d sem closed\n", streetnum);
	pr_default();
}



/* print default terminal color */
void pr_default(){
	printf("\033[0m");
}



/* print coloured strings in terminal based on streetnum */
void pr_color(){
	if(streetnum == 1) printf("\033[0;31m"); 			/* red */
	else if(streetnum == 2) printf("\033[0;32m");		/* green */
	else if(streetnum == 3) printf("\033[0;33m");		/* yellow */
	else if(streetnum == 4) printf("\033[0;34m");		/* blue */
}






