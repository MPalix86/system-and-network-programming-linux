/*
*	author : 		Mirco Palese
*	program name : 	exec
*	last edit :		... 
*/

#include <stdio.h>

void prchar(char *pointer);
void prvector (char *vec[]);


int main (int argc , char *argv[]){
	
	char c = 'c'; 					/* remembre that char must be declared with single quote */  
	char *pointer = &c;
	char *vector[] = {"astringa1", "bstringa2", "cstringa3"};
	
	
	prchar(&c); 					/* same as : prchar(pointer) */
	prvector(vector);
	
	
}

void prchar(char *pointer){
	printf("%c\n", *pointer);	
}

void prvector(char *vec[]){

	/* print address of first letter of every string */
	for	(int i = 0; i < 3; i++){
		printf("%p\n", &*vec[i]);
	}
	
	printf("\n");
	
	/* print first letter of every string */
	for	(int i = 0; i < 3; i++){
		printf("%c\n", *vec[i]);
	}
	
	printf("\n");
	
	/* print every string */
	for	(int i = 0; i < 3; i++){
		printf("%s\n", vec[i]);
	}
	
	printf("\n");
	
	/* print address of first letter of every string */
	for	(int i = 0; i < 3; i++){
		printf("%p\n", vec[i]);
	}
	
}