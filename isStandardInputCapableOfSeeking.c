/*
*	author : 		Mirco Palese
*	program name : 	isStandardInputCapableOfSeeking ?
*	last edit :		5/12/2019
*/

#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <sys/errno.h>


int main (int argc , char *argv[]){
	off_t off;
	
	printf("standard input is: \n");
	if(	(off = lseek(STDIN_FILENO, 0, SEEK_CUR)) < 0){
		printf("not capable of seeking.\nError: %d", errno);
		exit(0);
	}
	
	printf("capble of seeking and current offset is: %lld", off);
	
	exit(0);
	
}