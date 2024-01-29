/*
*	author : 		Mirco Palese
*	program name : 	readWrite
*	last edit :		...
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#define BUFFSIZE 4096

int main (int argc , char *argv[]){
	
	char buf[BUFFSIZE];
	int n;
	
	printf("write something and press enter ( max 4096 characters)\npress ctrl + c to exit \n");
	
	while ( (n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
		if( write(STDOUT_FILENO,buf,n) != n){
			break;
		}
	}
	
	if(n < 0) printf("some error occurred %d\n", errno);
		
	return 0;
	
}