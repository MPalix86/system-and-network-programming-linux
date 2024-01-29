/*
*	author : 		Mirco Palese
*	program name : 	atexit_test
*	last edit :		9/01/2020
*	note :			atexit register handeler that is called after exit() and before _exit
*/


#include <stdio.h>
#include <stdlib.h>


static void func1(void);
static void func2(void);

int main(int argc, char *argv[])
{
	printf("program start\n");
	
	if ( atexit(func1) != 0) printf("ERROR\n");
	
	if ( atexit(func2) != 0) printf("ERROR\n");
	
	return 0;
}

static void func1(void){
	printf("func1 exit handler\n");
}

static void func2(void){
	printf("func2 exit handler\n");
}

/*  Exit handler will be executed in reverse order of registration */

/* static functions are functions that are only visible to other functions in the same file */