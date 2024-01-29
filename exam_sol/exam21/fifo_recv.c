/*
*	author : Mirco Palese
*	program name : shmem recv
*	last edit :	29/02/2020
*	description : receive data send by speed_test trough fifo
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../include/apue.h"
#include <fcntl.h>


#define PLENGHT 60 	/* number of bytes for each pack */

int main (int argc , char *argv[]){

	
	char buf[PLENGHT];
	int n, fifo;
	
	if ( (fifo = open("./fifo", O_RDONLY )) == -1) err_sys("fifo error ");

	/* receive and do nothing, only for speed purpose */
	while ( (n = read(fifo, buf, PLENGHT)) > 0){
		continue;
	}
		
	return 0;
	
}