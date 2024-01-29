/*
*	author : 		Mirco Palese
*	program name : 	umask_test
*	last edit :		26/12/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)


void print_err_exit();

int main (int argc , char *argv[]){
	
	
	umask(0);
	if(open(argv[1], O_RDWR | O_CREAT | O_TRUNC, RWRWRW) < 0){
		print_err_exit();
	}
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(open(argv[2], O_RDWR | O_CREAT | O_TRUNC, RWRWRW ) < 0){
		print_err_exit();
	}
	return 0;
	
}


void print_err_exit(){
	printf("errno : %d\nerror message : %s\n", errno, strerror(errno));
	exit(0);
}