/*
*	author : Mirco Palese
*	program name : 
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "../../include/apue.h"

void sigint_handler(int dummy);
	pid_t pid1, pid2, lb_pid;

int main (int argc , char *argv[]){
	

	char *path1="./lightbulb_sem";
	char *path2="./a_sem";
	char buf[20];
	
	signal(SIGINT, sigint_handler); /* installing signal handler */
	
	
	if( (lb_pid = fork()) == -1 )exit(0);
	else if(lb_pid == 0){
 		execl(path1,"lightbulb_sem",(void*)0);
		exit(0);
	}
	
	snprintf(buf, sizeof(buf), "%d", lb_pid);
	
	if( (pid1 = fork()) == -1 )exit(0);
	else if(pid1 == 0){
 		execl(path2,"a",buf,"1",(void*)0);
		exit(0);
	}
	
	if( (pid2 = fork()) == -1 )exit(0);
	else if(pid2 == 0){
 		execl(path2,"a",buf,"2",(void*)0);
		exit(0);
	}
	
	sleep(5);
	if ( kill(getpid(), SIGINT) == -1) err_sys("kill error\n");

	return 0;
	
}

void sigint_handler(int dummy){
	for(int i = 0; i < 4; i++){
		if ( kill(pid2, SIGINT) == -1) err_sys("kill error\n");
		if ( kill(pid1, SIGINT) == -1) err_sys("kill error\n");
		if ( kill(lb_pid, SIGINT) == -1) err_sys("kill error\n");
		printf("child killed\n");
		exit(0);
	}
}
