/*
*	author : 		Mirco Palese
*	program name : 	vfork_scope
*	last edit :		14/1/2020
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int globvar = 10;

int main(int argc, char *argv[]){
	
	pid_t pid;
	int autovar = 10;
	
	printf("before vfork\n");
	printf("globval : %d\n", globvar);
	printf("autovar : %d\n\n", autovar);
	
	printf("doing vfork\n\n");
	
	if((pid = vfork()) < 0){
		 printf("vfork error");
		 exit(0);
	} 
	else if(pid == 0){
		globvar++;
		autovar++;
		printf("child\n");
	}
	else {
		sleep(2);
		printf("parent after fork\n");
	}

	printf("pid = %ld, glob = %d, autovar = %d\n", (long)getpid(), globvar,autovar);
	printf("globvar : %d\n", globvar);
	printf("autovar : %d\n\n", autovar);

	_exit(0); //no buffers are flushed
	//exit(0) performs the flushing of standard I/O buffers, causing segmentation fault
}

