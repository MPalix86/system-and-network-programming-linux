/*
*	author : 		Mirco Palese
*	program name : 	ls
*	last edit :		2/12/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main (int argc , char *argv[]){
	
	DIR *dp; 
	struct dirent *_dirp;
	

	if (argc != 2) printf("usage: ls directory_name");

	if ((dp = opendir(argv[1])) == NULL){
		printf("can’t open %s", argv[1]); 
		exit(0);
	}
	while ((_dirp = readdir(dp)) != NULL){
		printf("%s", _dirp->d_name); 		// same as (*_dirp).name
		printf("   |   %llu\n",_dirp->d_ino);	
	}

	exit(0);
	
}