/*
*	author : 		Mirco Palese
*	program name : 	signal_time
*	last edit :		20/01/2020
*	note : 	One problem with early versions of signals was that the action for a signal was 
*			reset to its default each time the signal occurred. (In the previous example, 
*			when we ran the program in signal.c, we avoided this detail by catching 
*			each signal only once.) The classic example from programming books that described 
*			these earlier systems concerns how to handle the interrupt signal. 
*			The code that was described usually looked like
*
*			int
*
*			. . .
*
*			sig_int();
*
*			// my signal handling function 
*
*			signal(SIGINT, sig_int); // establish handler 
*
*			. . .
*
*			sig_int()
*
*			{
*
*			signal(SIGINT, sig_int); // reestablish handler for next time 
*
*			. . .
*
*			// process the signal ...
*
*			}
*
*			The problem with this code fragment is that there is a window of time—after the 
*			signal has occurred, but before the call to signal in the signal handler—when 
*			the interrupt signal could occur another time.
*/

#include <stdio.h>
#include <stdlib.h>
#include "include/apue.h"
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

void sig_usr(int arg);
bool flag = false;

int main (int argc , char *argv[]){
	
	pid_t pid1;
	pid_t pid2;
	int n;

	
	if( (pid1 = fork()) == -1 ) exit(0);
		
	else if(pid1 == 0){
		/* first child */
		if( (pid2 = fork()) == -1 ) exit(0);
		
		else if( pid2 == 0){
			/* second child */
			sleep(1);
			printf("\n");
			printf("my pid is : %d,\nand my parent is %d \ni'm sleeping for 60 sec\n", getpid(),getppid());
			if ( signal(SIGTERM,sig_usr) == SIG_ERR ) exit(1);
			for (int i = 0; i < 2; i++){
				flag = false;
				printf("entered inside for\n");
				printf("type \"kill %d\" to send me a SIGTERM signal\n", getpid());
				printf("type enter to continue\n");
				n=0;
				while (!flag){
					sleep(1);
					if(n == 60)	break;
					n++;
				}
			}
			printf("if you sent me 2 times signal SIGTERMp with success all signals problems was solved\nand signals are reliable\n");
			exit(0);
		}
		
		/* exit from first child so first child was adopted by init */
		exit(0);
	}
	exit(0);
	
}

void sig_usr(int signo){   /* argument is signal number */
	if(signo == SIGTERM) printf("great job, you send me a signal\n");
	flag = true;
}