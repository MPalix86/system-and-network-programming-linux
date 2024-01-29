 /*
*	author : Mirco Palese
*	program name : my prompt
*	last edit :	31/01/2020
*	description : transform lowercase input into upper case output and vice versa
*	note : this program use the "filter" and popen to perform the actions ;
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXLINE 100

int main (int argc , char *argv[]){
	
	FILE *fin;
	char line[MAXLINE];
	
	if( (fin = popen("./filter", "r")) == NULL){
		printf("popen error\n");
		exit(0);
	}
	
	printf("type something and press enter\n");
	
	while (true){
		printf("prompt > ");
		fflush(stdout); /* We need to call fflush after writing the prompt, because the standard output is normally line buffered, and the prompt does not contain a newline. */		
		if (fgets(line, MAXLINE, fin) == NULL) /* read from pipe */ 
			break;
		printf("%s\n",line);
	}
	
	if (pclose(fin) == -1) exit(0);
	putchar('\n'); 
	exit(0);
		
	return 0;
	
}