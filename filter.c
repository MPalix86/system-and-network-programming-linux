/*
*	author : Mirco Palese
*	program name : filter
*	last edit :	31/01/2020
*	description : change upper case letters into lower case and vice versa
*	note : this program is used in my_prompt.cn to show popen's usage
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc , char *argv[]){
	char c;
	
	while ( (c = getc(stdin)) != EOF ){
		if(isupper(c)) c = tolower(c);
		else if(islower(c)) c = toupper(c);
		if ( putc(c,stdout) == EOF){
			printf("putc error\n");
			exit(0);
		}
		if (c == '\n') fflush(stdout);
	}
	
	return 0;
	
}