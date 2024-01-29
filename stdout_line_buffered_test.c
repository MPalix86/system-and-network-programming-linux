/*
*	author : Mirco Palese
*	program name : stdout line buffered test
*	last edit :	31/01/2020
*	description : test on stdout buffering mode
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main (int argc , char *argv[]){

	/** no output is produced because stdout is line buffered and \n is not present in string */
	while(true){
		printf("ciao");
		sleep(1);
	}
	
	
	/** output is produced because fflush(stdout) force flush of the buffer */
	/*
		while(true){
			printf("ciao");
			fflush(stdout);
			sleep(1);
		}
	*/
	
	
	/** output is produced because \n character is in the string */
	/*
		while(true){
			printf("ciao\n");
			sleep(1);
		}
	*/

	return 0;
	
}