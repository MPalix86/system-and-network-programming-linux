/*
*	author : 		Mirco Palese
*	program name : 	...
*	last edit :		...
*/

#include <stdio.h>
#include "include/apue.h"


int main (int argc , char *argv[]){
	
	FILE *f1;
	FILE *f2;
	char line[1024], *char_ptr;
	
	
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
	 
	if( (f1 = fopen(argv[1],"r") ) == NULL) err_quit("error occured");
	
	if( (f2 = fopen(argv[2],"w") ) == NULL) err_quit("error occured");
	
	while ( ( char_ptr = fgets(line , sizeof(line), f1)) != NULL ){
		fputs(line, f2);
	}
	
	exit(0);
	
	
	
	return 0;
	
}