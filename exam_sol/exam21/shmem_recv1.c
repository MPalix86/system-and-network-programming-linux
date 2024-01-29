/*
*	author : Mirco Palese
*	program name : shmem recv mu
*	last edit :	29/02/2020
*	description : receive data send by speed_test trough shared memory using mutex
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "../../include/apue.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

#define PLENGHT 60 	/* number of bytes for each pack */
#define PNUM 17500	/* number of packages to send */
					/* PNUM * PLENGHT =~ bytenum, in this case 60 byte *17500 packets  =~ 1 MB */

int main (int argc , char *argv[]){
	char packets[PNUM][PLENGHT];
	
	
	/* -----recovering mutex inside shared memory */
	pthread_mutex_t *mu;
	int shmem_mu_id;
	void* shmem_mu_ptr;
	int mu_err;
	
	/* getting shared m. id for data*/
	if ( (shmem_mu_id =  shmget((key_t)1111, 0 , 0666 | IPC_CREAT )) == -1) err_sys("shared memory error ");
	
	/* getting shared m. pointer for data */
	if ((shmem_mu_ptr = shmat(shmem_mu_id, 0, 0)) == (void *)-1) err_sys("shmat error");
	
	/* make mu, that expects a memory area made like pthread_mutex_t *, to point the shared memory. */
	mu = (pthread_mutex_t *)shmem_mu_ptr;
	
	
	/* -----recovering condition variable inside shared memory */
	void *shmem_cond_ptr;			/* pointer to cond shared memory */
	int shmem_cond_id;				/* shared memory cond id */		
	pthread_cond_t *cond;
	
	/* getting shared m. id for cond*/
	if ( (shmem_cond_id =  shmget((key_t)2222, 0 , 0666 | IPC_CREAT )) == -1) err_sys("shared memory error ");
	
	/* getting shared m. pointer for cond */
	if ((shmem_cond_ptr = shmat(shmem_cond_id, 0, 0)) == (void *)-1) err_sys("shmat error");
	
	/* make cond, that expects a memory area made like pthread_cond_t , to point the shared memory. */
	cond = (pthread_cond_t *)shmem_cond_ptr;
	
	/* recovering shared memory aread with data */
	int	shmem;
	char *shmem_ptr;
	
	if ( (shmem =  shmget(3333, 0, 0666 )) == -1) err_sys("shared memory error ");
	if ((shmem_ptr = shmat(shmem, 0, 0)) == (void *)-1) err_sys("shmat error child");
	
	if( (mu_err = pthread_mutex_lock(mu)) != 0 ) err_exit(mu_err,"lock mutex error");
	
	for (int i = 0; i < PNUM; i++){	
		strcpy(packets[i],shmem_ptr);
		if ( (mu_err = pthread_cond_signal(cond)) != 0) err_exit(mu_err,"cond signal error: ");
		if ( (mu_err = pthread_cond_wait(cond, mu)) != 0) err_exit(mu_err,"cond wait error"); 
		printf("data read, iteration number :%d\n",i);
	}
	
	if ( (mu_err = pthread_mutex_unlock(mu)) != 0) err_exit(mu_err,"mutex unlock error");
	return 0;
	
}


