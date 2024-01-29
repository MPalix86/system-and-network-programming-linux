/*
*	author : Mirco Palese
*	program name : malloc_test
*	last edit :	11/02/2020
*	description : dynamic allocation test	
*	note : perche funziona ???
*/

#include <stdio.h>
#include <stdlib.h>

int main (int argc , char *argv[]){
	
	int *fd_arr;
	
	
	fd_arr = (int*) malloc(sizeof(int));
	char *pswd = "password";
	
	for (int i = 0; i < 10; i++){
		fd_arr[i] = i;
		printf("fd_arr[%d]: %d\n", i, &fd_arr[i]);
	}
	
	return 0;
	
}