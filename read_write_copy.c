/*
*	author : Mirco Palese
*	program name : read_write_copy
*	last edit :	12/12/2019
*	description : copy file in argv[1] to path specified in argv[2]
*	note :  The program doesn’t close the input ﬁle or output ﬁle. Instead, the program uses 
*		 	the feature of the UNIX kernel that closes all open ﬁle descriptors in a process 
*		 	when that process terminates.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <string.h>
//-------------------
#define BUF_SIZE 4096

void print_err_exit();

int main (int argc , char *argv[]){
	
	ssize_t rw_bytes; 
	int fd1 , fd2;
	char buf[BUF_SIZE];
	size_t buf_size = BUF_SIZE;
	
	// argv[1] contains file to copy
	if(argv[1] == NULL ){
		printf("source path missing\n");
		exit(0);
	}
	
	// argv[2] contains destination path
	if(argv[2] == NULL ){
		printf("Destination path missing\n");
		exit(0);
	}
	
	if(strcmp(argv[1],argv[2]) == 0){
		printf("Specify different paths\n");
		exit(0);
	}
	
	// verify if argv[3] contains buffer size else use the standard one
	if(argv[3] != NULL) buf_size = atoi(argv[3]);

	
	// open file to copy
	if( (fd1 = open(argv[1], O_RDONLY)) == -1 ) print_err_exit();
	
	// create new file
	if( (fd2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT)) == -1 ) print_err_exit();
	
	// reading file to copy
	while ( (rw_bytes = read(fd1, buf, buf_size)) ){
		if(rw_bytes == -1) print_err_exit();
		else if (rw_bytes == 0) exit(0);
		if( write(fd2,buf,rw_bytes) == -1 ) print_err_exit();
	}
	
	return 0;
	
}

void print_err_exit(){
	printf("errno : %d\nerror message : %s\n", errno, strerror(errno));
	exit(0);
}