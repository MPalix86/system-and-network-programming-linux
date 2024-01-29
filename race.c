/*
*	author : 		Mirco Palese
*	program name : 	race
*	last edit :		14/04/2019
*	note:			execute multiple background instance of this program to see race condition
*					example: ./race A & ./race B & ./race C & ./race D &
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char *argv[]){

	char *str;
	if(argc != 2){
		printf("you must insert one argument.\n execute multiple background instance of this program to see race condition example: ./race A & ./race B & ./race C & ./race D &");
		exit(0);
	}
	str = argv[1];
	while (1){
		sleep(2);
		printf("%s\n", str);
	}
	
	return 0;
	
}