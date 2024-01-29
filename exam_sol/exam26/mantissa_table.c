/*
*	author : Mirco Palese
*	program name : mantissa table
*	last edit :	19/02/2020
*	description : leggere snp 26
*	note :  non è finito, manca la parte di gestione delle thread che leggono dalle pipe
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../../include/apue.h"

#define BUFFSIZE 100

int main (int argc , char *argv[]){

	unsigned char tr_num = *argv[1];
	int pipe_re = 3;
	int fd, n;
	char buf[BUFFSIZE];
	
	if ( (fd = open("./response.txt", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) err_sys("file open error: ");
	
	
	
	while ( (n = read(pipe_re, buf, BUFFSIZE)) > 0){
		if( write(fd,buf,n) != n){
			break;
		}
	}
	
	return 0;
	
}