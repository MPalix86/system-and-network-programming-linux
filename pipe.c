/*
*	author : Mirco Palese
*	program name : pipe
*	last edit :	31/01/2020
*	description : pipe usage
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "include/apue.h"

void sigusr1_handler(int arg);

int fd[2], byte_num;
char buf[1024];
char string[] = "questa frase è bella\n";


int main (int argc , char *argv[]){
	pid_t pid;
	int *statloc = NULL;

	
	if(pipe(fd) < 0 ){
		printf("some pipe error\n");
		exit(0);
	}
	
	if( (pid = fork()) < 0){
		printf("fork error");
		exit(0);
	}
	
	else if(pid == 0){
		/* child */
 		printf("i'm child\n");
		close(fd[1]);
		if ( signal(SIGUSR1,sigusr1_handler) == SIG_ERR){
			printf("error signal2\n");
			exit(0);
		}		
		sleep(5);
		exit(0);
	}
	
	else if (pid > 0){
		/* parent */
		sleep(1);
		close(fd[0]);
 		printf("parent writing in pipe\n");
		if( write(fd[1],string, sizeof(string)) < 0){
			printf("error during writing in parent\n");
			exit(0);
		}
		printf("sending signal to child\n");
		kill(pid,SIGUSR1);
		
		printf("waiting for child termination\n");
		if( (pid = wait(statloc)) < 0 ) {
			printf("wait error\n");
			exit(0);
		}
		else {
			printf("child terminated, pid: %d\n", pid);
			exit(0);
		}
	}	
		
	return 0;
}


void sigusr1_handler(int arg){
	printf("child signal received\n");
	if (arg == SIGUSR1){
		printf("child reading in pipe\n");
		byte_num = read(fd[0], buf, 1024);
		if(write(STDIN_FILENO,buf,byte_num) < 0 ){
			printf("some error occurred during writing in child\n");
			exit(0);
		}
		printf("byte_num: %d\n", byte_num);
		printf("child exit\n");
		exit(0);
	}
}