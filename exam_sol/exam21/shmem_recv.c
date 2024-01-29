/*
*	author : Mirco Palese
*	program name : shmem recv
*	last edit :	29/02/2020
*	description : receive data send by speed_test trough shared memory
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include "../../include/apue.h"
#include <sys/mman.h>
#include <fcntl.h>

#define PLENGHT 60 	/* number of bytes for each pack */
#define PNUM 17500	/* number of packages to send */
					/* PNUM * PLENGHT =~ bytenum, in this case 60 byte *17500 packets  =~ 1 MB */

int main (int argc , char *argv[]){

	int	shmem;
	char **shmem_ptr;
	key_t mem_key = ftok(".", 'a');
	
	if ( (shmem =  shmget(mem_key, sizeof(PLENGHT * PNUM), 0666 )) == -1) err_sys("shared memory error ");
	if ((shmem_ptr = shmat(shmem, 0, 0)) == (void *)-1) err_sys("shmat error child");
	
	for (int i = 0; i < PNUM; i++){	
		printf("%s\n", shmem_ptr[i]);
	}
	
	return 0;
	
}