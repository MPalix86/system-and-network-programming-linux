/*
*	author : Mirco Palese
*	program name : XSIsemaphore
*	last edit :	16/02/2020
*	description : xsi semaphore demonstration
*	note : this is not part of the exam28 is just an example of how XSI semaphores work
*/


#include <stdio.h>
#include <stdlib.h>
#include "../../include/apue.h"
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
/*-----------------------------*/

int sem_int = 1000;
const char *sem_fpath = "test.txt";


static int set_semvalue(int sem_id, int val);
static void del_sem(int sem_id);
static int sem_minus(int sem_id, int val);
static int sem_plus(int sem_id, int val);


int main (int argc , char *argv[]){
	
	
	union semun semunion;
	int semval;
	key_t key = 1000;
	int sem_id;
	
	
	/* creating sempahore */
	if( ( sem_id = semget(key, 1, 0666 | IPC_CREAT)) == -1) err_sys("semaphore error: ");
	
	set_semvalue(sem_id, 10);
	
	if( (semval = semctl(sem_id, 0, GETVAL, semunion)) == -1) err_sys("semaphore setting value error: ");
	printf("semval = %d\n", semval);
	
	for (int i = 0; i < 9; i++){
		sem_minus(sem_id,1);
	}
	if( (semval = semctl(sem_id, 0, GETVAL, semunion)) == -1) err_sys("semaphore setting value error: ");
	
	printf("semval = %d\n", semval);
	
	for (int i = 0; i < 9; i++){
		sem_plus(sem_id,1);
	}
	if( (semval = semctl(sem_id, 0, GETVAL, semunion)) == -1) err_sys("semaphore setting value error: ");
	
	printf("semval = %d\n", semval);
	
	del_sem(sem_id);
	

	
	return 0;
	
}




/* The function set_semvalue initializes the semaphore using the SETVAL command in a
 semctl call. We need to do this before we can use the semaphore. */
static int set_semvalue(int sem_id, int val)
{
    union semun sem_union;

    sem_union.val = val;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}



/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */
static void del_sem(int sem_id)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}



/* sempahore_minus changes the semaphore by -1 (waiting). */
static int sem_minus(int sem_id, int val)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -val; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}



/* semaphore_plus increment sem value by 1. */
static int sem_plus(int sem_id, int val)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = val; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}
















