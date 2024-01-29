/*
*	author : Mirco Palese
*	program name : shmem recv
*	last edit :	29/02/2020
*	description : receive data send by speed_test trough pipe
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PLENGHT 60 	/* number of bytes for each pack */

int main (int argc , char *argv[]){

	
	char buf[PLENGHT];
	int n;
	
	
	close(4);	 /* close write end og the pipe */

	/* receive and do nothing, only for speed purpose */
	while ( (n = read(3, buf, PLENGHT)) > 0){
		continue;
	}
		
	return 0;
	
}