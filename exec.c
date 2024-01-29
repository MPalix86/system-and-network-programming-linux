/*
*	author : 		Mirco 	Palese
*	program name : 	exec
*	last edit : 	9/04/2019
*	note :			this program works together with echoall
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <sys/wait.h>
#include "include/apue.h"

int main (int argc , char *argv[]){
	
	int statloc = 0;
	int *ptrstatloc = &statloc;
	struct rusage usage;
	struct rusage *rusage = &usage;
	pid_t pid, pid_w;

	char *envp[] = {"TEST=test" , "PATH=/usr" , NULL };	
	char path[] = "/Users/mircopalese/MEGAsync/Università/Magistrale/1 anno/Systems and network programming/MyPrograms/echoall";
	
	printf("EXEC, calling new program ECHOALL\n\n");
	sleep(1);
	
	if ( (pid = fork()) < 0 ) printf("fork error")	;
	
	/* child */
	if (pid == 0) { 
		if ( (execle(path , "echoall" , "myarg1" , "myarg2", "myarg3", (char *) 0 , envp) ) < 0) {
			printf("execle error\n");
			printf("%d\n", errno);
		}	
	}
	
	if( (pid_w = wait4(pid, ptrstatloc, WUNTRACED, rusage)) < 0){ 
		printf("wait error\n");
		printf("errno: %d", errno);
		return 0;
	}
	
	printf("EXEC wait4 ok ! Some stat about ECHOALL\n\n");
	
	printf("time spent in operating system code on behalf of processes-> %ld\n", rusage->ru_maxrss);
	printf("the maximum number of kilobytes of physical memory that processes used simultaneously-> %ld\n",rusage->ru_ixrss);
	printf("the amount of memory used by text that was shared with other processes-> %ld\n", rusage->ru_idrss);
	printf("the amount of unshared memory used for data-> %ld\n", rusage->ru_isrss);
	printf("the amount of unshared memory used for stack space-> %ld\n", rusage->ru_minflt);
	printf("the number of times the file system had to read from the disk on behalf of processes-> %ld\n", rusage->ru_oublock);	
	printf("the number of signals received-> %ld\n", rusage->ru_nvcsw);
	
	
	return 0;
}


/* The waitpid() call is identical to wait4() with an rusage value of zero */