/*
*	author : Mirco Palese
*	program name : lightbulb
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/sem.h>
#include "../../include/apue.h"


/* 
* SIGUSR1 handle red
* SIGUSR2 handle green
*/

void sig_handler(int, siginfo_t *str, void * val);
static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);


bool red_is_on = false;
bool green_is_on = false;
bool sig_r = false;
bool sig_g = false;
static int sem_id;
bool go_out = false;
int counter = 0;

int main (int argc , char *argv[]){

	pid_t my_pid = getpid();
	
	struct sigaction sa; /* sigaction structure to define the signals handlers behavior */
	
    sa.sa_handler = sig_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART; /* Restart functions if interrupted by handler */
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) err_sys("sigaction error ");
    if (sigaction(SIGUSR2, &sa, NULL) == -1) err_sys("sigaction error ");
    if (sigaction(SIGINT, &sa, NULL) == -1) err_sys("sigaction error ");
    
	
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
	if (!set_semvalue()) err_sys("semaphore error ");
	
	//printf("my pid %d\n", my_pid);
	
	for(;;){
		if(!go_out){
			if(sig_r){
				//printf("\033[0m"); /* print default terminal color */
				if(red_is_on){
					 //printf("red is on\n");
					 counter++;
				}
				else //printf("red is off\n");
				sig_r = false;
			}
			else if(sig_g){
				//printf("\033[0m"); /* print default terminal color */
				if(green_is_on){
					//printf("green is on\n");
					counter++;
				} 
				else //printf("green is off\n");
				sig_g = false;
			}

		}
		else break;
	}
		
	
	printf("counter %d\n\n",counter);
	
	return 0;
	
}



void sig_handler(int sig, siginfo_t *siginfo, void * val){

		if( sig == SIGUSR1){
			red_is_on = !red_is_on;
			sig_r = true;
		}

		if( sig == SIGUSR2){
			green_is_on = !green_is_on;
			sig_g = true;
		} 
		
		
		if(sig == SIGINT){
			go_out = true;
		} 
}


 
 
/* The function set_semvalue initializes the semaphore using the SETVAL command in a
semctl call. We need to do this before we can use the semaphore. */
static int set_semvalue(void)
{
    union semun sem_union;

    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}

/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */
static void del_semvalue(void)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

/* semaphore_p changes the semaphore by -1 (waiting). */
static int semaphore_p(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */
static int semaphore_v(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}