/*
*	author : 		Mirco Palese
*	program name : 	file_type.c
*	last edit :		...
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/errno.h>



void print_err_exit();

int main (int argc , char *argv[]){
	
	struct stat str;
	
	if(argv[1] == NULL) {
		printf("No argument...");
		exit(0);
	}	
	if(fstatat(AT_FDCWD, argv[1], &str, AT_SYMLINK_NOFOLLOW) == -1) print_err_exit();
	else{
		if(S_ISREG( str.st_mode)) printf("regular ﬁle\n");
		else if(S_ISREG( str.st_mode)) printf("regular ﬁle\n");
		else if(S_ISDIR( str.st_mode)) printf("directory ﬁle\n");
		else if(S_ISCHR( str.st_mode)) printf("character special ﬁle\n");
		else if(S_ISBLK( str.st_mode)) printf("block special ﬁle\n");
		else if(S_ISFIFO( str.st_mode)) printf("pipe or fifo\n");
		// ... ecc ecc
	}
	
	
	
	return 0;
	
}

void print_err_exit(){
	printf("errno : %d\nerror message : %s\n", errno, strerror(errno));
	exit(0);
}