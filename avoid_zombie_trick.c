/*
*	author : 		Mirco Palese
*	program name : 	avoid_zombie_trick	
*	last edit :		14/01/2020
*	note : 			If we want to write a process so that it forks a child but we don’t want 
*					to wait for the child to complete and we don’t want the child to become 
*					a zombie until we terminate, the trick is to call fork twice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <apue.h>
#include <sys/wait.h>

int main (int argc , char *argv[]){

	pid_t pid;
	int *statloc;
	
	if ( (pid = fork()) < 0 ){
		err_sys("fork error\n");
	}
	else if (pid == 0){
		// we are in the first child
		
		pid_t pid1;
		
		if ( (pid1 = fork()) < 0 ){
			err_sys("fork_error\n");
		}
		else if ( pid1 == 0 ){
		
			/*
			* this is the second child.
			* take time to make init became parent of second child.
			*/
			pid_t pid2;
			sleep(2);
			pid2 = getppid();
			printf("i'm second child and my parent is %d", pid2);
		}
		
		/*
		* exit from first child. 
		* This will cause the second child to be inherited by init, and also trigger wait 
		* inside parent (see wait below). 
		* In this way parent and second child can continue his execution independently, 
		* at same time firs child don't became a zombie.
		*/
		exit(0);
	}
	
	if(wait(statloc) == pid){
		printf("im'parent, first child terminated. first terminated because : \n");	
	}
	else{
		printf("something happens\n");
	}
	
	/*
	* for some reason pr_exit(*statloc) below cause segmentation fault error. 
	* I couldn't understand why.
	* pr_exit(*statloc); // *statloc = value of variable pointed by statloc.
	*/
	
	exit(0);
	
}