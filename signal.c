/*
*	author : 		Mirco Palese
*	program name : 	signal
*	last edit :		20/01/2020
*	note : 			...
*/

#include <stdio.h>
#include <stdlib.h>
#include "include/apue.h"
#include <signal.h>
#include <unistd.h>

void sig_usr(int arg);

int main (int argc , char *argv[]){
	
	pid_t pid1;
	pid_t pid2;
	
	if( (pid1 = fork()) == -1 ) exit(0);
		
	else if(pid1 == 0){
		/* first child */
		if( (pid2 = fork()) == -1 ) exit(0);
		
		else if( pid2 == 0){
			/* second child */
			sleep(1);
			printf("type \"kill %d\" to send me a SIGTERM signal\n", getpid());
			printf("my pid is : %d,\nand my parent is %d \ni'm sleeping for 30 sec\n", getpid(),getppid());
			printf("type enter to continue\n");
			if ( signal(SIGTERM,sig_usr) == SIG_ERR ) exit(1);
			sleep(40);
			printf("too late..");
		}
		
		/* exit from first child so first child was adopted by init */
		exit(0);
	}
	exit(0);
	
}

void sig_usr(int signo){   /* argument is signal number */
	
	if(signo == SIGTERM) printf("great job, you send me a signal\n");
	exit(0);
}





// ecxample from apue

// #include "apue.h"
// 
// static void	sig_usr(int);	/* one handler for both signals */
// 
// int
// main(void)
// {
// 	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
// 		err_sys("can't catch SIGUSR1");
// 	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
// 		err_sys("can't catch SIGUSR2");
// 	for ( ; ; )
// 		pause();
// }
// 
// static void
// sig_usr(int signo)		/* argument is signal number */
// {
// 	if (signo == SIGUSR1)
// 		printf("received SIGUSR1\n");
// 	else if (signo == SIGUSR2)
// 		printf("received SIGUSR2\n");
// 	else
// 		err_dump("received signal %d\n", signo);
// }





