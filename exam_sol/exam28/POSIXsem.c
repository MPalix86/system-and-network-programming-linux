/*
*	author : Mirco Palese
*	program name : POSIXsem
*	last edit :	17/02/2020
*	description : simulate a semaphore with streets
*	note : smaphore is simulated by this and streets by 4 children launched by this programm,
*		   so is not standalone 	
*/

#include <stdio.h>
#include <stdlib.h>
#include "../../include/apue.h"
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/errno.h>
#define SEMVAL 2 /* represent the number of cars that can transitate simultaneously */


sem_t *sem;
const char *semname = "/sem";
void exit_handler();
void sigint_handler(int dummy);

pid_t pid[4];

int main (int argc , char *argv[]){
	
	const char *path = "./streets";  
	

	signal(SIGINT, sigint_handler); /* installing signal handler */
	atexit(exit_handler);			/* installing exit handler */
	
	/* opening semaphore */
	if(( sem = sem_open(semname, O_CREAT, 0666, SEMVAL )) ==  SEM_FAILED ) err_sys("semaphore error: ");
	
	printf("hint ^C to exit\n");
	sleep(1);
	
	for(int i = 0; i < 4; i++){
	
		/* streets initialization, each process represent one street with his own cars queue */
		if( (pid[i] = fork()) == -1 ) err_sys("fork error\n");
	
		else if (pid[i] == 0){
			char proc_num = (char) i+1;	/*
										* proc num is char (and not int) because of exec argument that accepts only chars. 
										* i+1 because i starts from zero.
										*/
			if( execl(path,"streets", &proc_num, (void *)0) == -1) err_sys("exec error ");
			_Exit(0);	/* exit without calling handler to avoid semclose */
		}
	}
	
	wait(NULL); /** wait for children termination */
	
	return 0;
}


/* exit handler */
void exit_handler(){
	if ( sem_close(sem) == -1) err_sys("semclose: ");
	if ( sem_unlink(semname) == -1) err_sys("semunlink: ");
}



/* 
* sigint handler. When ^c is received, all streets was closed (all children was killed)
* wait function return and this process terminate
*/
void sigint_handler(int dummy){
	for(int i = 0; i < 4; i++){
		if ( kill(pid[i], SIGINT) == -1) err_sys("kill error\n");
		printf("child %d killed\n", i+1);
	}
}

 

