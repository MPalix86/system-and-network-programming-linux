/*
*	author : 		Mirco Palese
*	program name : 	no_race_fork_tell_wait	
*	last edit :		16/01/2020
*/
/*
*	author : 		Mirco Palese
*	program name : 	race_fork.c
*	last edit :		14/01/2020
*/

#include <apue.h>
#include <stdio.h>
#include <stdlib.h>
#include <apue.h>
#include <stdbool.h>
#include <ctype.h>

void myprint (char *str);

int main (int argc , char *argv[]){
	
	pid_t pid;
	char a = 'A';
	char b = 'B';
	
	TELL_WAIT(); /* set things up for TELL_xxx & WAIT_xxx */
	
	if(argc != 2){
		printf("you must insert 1 integer argument\n\n");
		exit(0);
	}
	printf("parent process will print B and child process A, race condition should produce string like that: ABABAB, \nwe avoid race condition thanks to TELL_WAIT macros, to see race condition execute \"race_fork\"\nsee also the file tell_wait.c to understand TELL_WAIT.\n\n ");
	
	printf("Press Enter to Continue");
	while( getchar() != '\n' );
	printf("\n\n");
		
	if( (pid = fork()) == -1){
		err_sys("error during fork");
		exit(0);
	}
	else if (pid == 0){
		// we are in child
		
		for (int i = 0; i < atoi(argv[1]); i++){
			myprint(&a);
		}
		TELL_PARENT(getppid()); /* tell parent we’re done */ 	
		exit(0);
	}
	
	WAIT_CHILD(); 		/* and wait for child */
		
	for (int i = 0; i < atoi(argv[1]); i++){
		myprint(&b);
	}
	exit(0);
	
}



/*
* print one character at time. We use write and not putc or printf to force kernel to 
* write on standard output every time that character is passed, and to avoid buffers.
*/
void myprint(char *str){

	if( write (STDOUT_FILENO, str, 1 ) == -1 ){
		err_sys("error during writing\n");
		exit(0);
	}
	
}