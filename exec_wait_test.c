/*
*	author : 		Mirco Palese
*	program name : 	exec_wait_test	
*	last edit :		16/01/2020
*	note :			this program simply execute one new program and wait for it to return,
*					executed program in this case is set_user_id_bit_test in exec folder.
*/

#include <stdio.h>
#include <stdlib.h>
#include <apue.h>


int main (int argc , char *argv[]){

	pid_t pid;
	int statloc;
	pid_t wait_s;
	
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
	
	if( (wait_s = wait(&statloc) ) == -1){
		err_sys("wait error\n");
	}
	else if( wait_s == pid ){
		printf("\nexec returned\n ");
	}
	
	return 0;
	
}