/*
*	author : 		Mirco Palese
*	program name : 	file_hole
*	last edit :		10/12/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <string.h>
//---------------------
#define BUF_SIZE 10
#define HOLE_SIZE 16394

int main (int argc , char *argv[]){
	
	char buf1[BUF_SIZE] = "abcdefghil";
	char buf2[BUF_SIZE] = "ABCDEFGHIL";
	int fd;
	
	// create new file
	if( (fd = open("../files/file.hole.txt",O_CREAT | O_TRUNC | O_WRONLY)) == -1){
		printf("some error occurred during file creation\nerrno: %d\nerror message: %s\n", errno, strerror(errno));
		exit(0);
	}
	
	// writing buf1 into file
	if(write(fd, buf1, BUF_SIZE) == -1){
		printf("some error occurred during writing\nerrno: %d\nerror message: %s\n", errno, strerror(errno));
		exit(0);
	}
	
	// crating hole
	if( lseek(fd, HOLE_SIZE, SEEK_SET) == -1 ){
		printf("some error occurred moving cursor\nerrno: %d\nerror message: %s\n", errno, strerror(errno));
        exit(0);
	}
	
	// writing buf2 into file
	if(write(fd, buf2, BUF_SIZE) == -1){
		printf("some error occurred during writing\nerrno: %d\nerror message: %s\n", errno, strerror(errno));
		exit(0);
	}
	
	
	
	return 0;
	
}


IT38C0306967684510341037947s