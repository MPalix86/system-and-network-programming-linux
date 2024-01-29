/*
*	author : 		Mirco Palese
*	program name : 	wait_signal	
*	last edit :		20/01/2020
*	note :			this program simply execute one new program and wait for signal SIGCHLD
*					to wait child and return; executed program in this case is set_user_id_bit_test 
*					in exec folder.
*/

#include <stdio.h>
#include <stdlib.h>
#include "include/apue.h"
#include <signal.h>


pid_t pid;
int statloc;
pid_t wait_s;

void sig_usr(int signum);

int main (int argc , char *argv[]){


	
	signal(SIGCHLD, sig_usr);
	
	if ( (pid = fork()) == -1 ){
		err_sys("some error occurred during fork\n");
		exit(0);
	} 
	else if( pid == 0){
		if ( execl("/Users/mircopalese/MEGAsync/Università/Magistrale/1 anno/Systems and network programming/my_programs/exec/set_user_id_bit_test","set_user_id_bit_test", "1000" ,(char *)0 ) == -1){
			err_sys("some error occurred during execl\n");
			exit(0);
		} 
	}
	
	/** do stuff without waiting for child to return, 
	*	instead doing this below
	*
	*	-----------------------------------------
	*	if( (wait_s = wait(&statloc) ) == -1){
	*		err_sys("wait error\n");
	*	}
	*	else if( wait_s == pid ){
	*		printf("\nexec returned\n ");
	*	}
	*	------------------------------------------
	*
	*	when child will return, sig_usr will handle it.
	*/
	
	sleep(100); 
	return 0;
}

void sig_usr(int signum){
	printf("signal received");
	if(signum == SIGCHLD){
		if( (wait_s = wait(&statloc) ) == -1){
			err_sys("wait error\n");
		}
		else if( wait_s == pid ){
			printf("\nexec child returned\n ");
		}
		exit(0);
	}
}