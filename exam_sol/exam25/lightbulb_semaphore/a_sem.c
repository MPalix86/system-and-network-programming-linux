/*
*	author : Mirco Palese
*	program name : 
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/sem.h>
#include "../../include/apue.h"


static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

int main (int argc , char *argv[]){

	pid_t pid;

	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
	
	if(argc != 3){
		printf("usage: a 'pid' 'color (1 = red 2 = green)' \n");
		err_quit("too few argument\n");
	}
		 	
	pid = atoi(argv[1]);
 	int color = atoi(argv[2]);
	
	
	for(;;){
		semaphore_p();
		//printf("\033[0;33m");	/* yellow */
		if(color == 1){
			kill(pid,SIGUSR1);
			//printf("SIGUSR1\n");
		}	
		else if (color == 2){
			kill(pid,SIGUSR2);
			//printf("SIGUSR2\n");
		}
		sleep(1);
		semaphore_v();
		
	}
	
	
	
	return 0;
	
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