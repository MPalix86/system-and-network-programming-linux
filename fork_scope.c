/*
*	author : 		Mirco Palese
*	program name : 	exec
*	last edit : 	...
*	note : 			fork scope test
*/

#include <unistd.h>
#include <stdio.h>

int globvar = 10;

  
int main (int argc , char *argv[]){
	
	int autovar = 10;
	pid_t pid;
	
	printf("parent before fork\n");
	printf("globvar : %d\n", globvar);
	printf("autovar : %d\n\n", autovar);
	
	printf("doing fork\n\n");
	if( (pid = fork()) < 0) printf("fork error\n");
	
	if (pid == 0) {
		printf("child\n");
		globvar++;
		autovar++;
	}
	else {
		sleep(2);
		printf("parent after fork\n");
	}
	
	printf("globvar : %d\n", globvar);
	printf("autovar : %d\n\n", autovar);

	return 0;
}

/* note that child and parent are not synchronized (see race condition) */
/* with fork, child have his copy of memory space */