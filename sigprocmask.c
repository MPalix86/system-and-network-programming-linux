/*
*	author : 		Mirco Palese
*	program name : 	sigprocmask
*	last edit :		20/01/2020
*	note : 			show an example for sigprocmask function to set a mask of signals
*					all signals in the mask are ignored
*/

#include <stdio.h>
#include <stdlib.h>
#include "include/apue.h"
#include <signal.h>
#define SLEEP 60

int main (int argc , char *argv[]){

	printf("\nif you do not, run this program in background with &\n");
	printf("my pid is : %lld\n", getpid());
	printf("send me a SIGTERM message by typing kill %lld\n",getpid());
	printf("if nothing happens all work fine\n");
	printf("sleeping for %d sec, i will exit in %d\n", SLEEP, SLEEP);
	printf("type enter to continue\n");

	sigset_t sigset ;
	sigset_t o_sigset ;
	
	sigemptyset(&sigset); 	/* emptying set */
	sigfillset(&sigset);	/* filling set with all signals */	
	
	if(sigismember(&sigset,SIGTERM) == 1){
		//printf("SIGTERM  is in the mask\n");
	}	
	
	if ( sigprocmask(SIG_SETMASK, &sigset, &o_sigset) == -1 ){
		err_sys("error");
		exit(0);
	}
	

	sleep(SLEEP);
	printf("\ni'm process n° %lld, bye bye \n", getpid());
	printf("type enter to continue \n");
	
	return 0;
	
}