/*
*	author : Mirco Palese
*	program name : 
*	last edit :	
*	description : 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct { int a,b,c,d; }foo;
void* thread(void* val);
void print_foo(foo *str);

int main (int argc , char *argv[]){
	foo *str;
	pthread_t tid;
	int err;
	 
	printf("creating new thread\n");
	if ( (err = pthread_create(&tid, NULL, thread, NULL)) != 0){
		printf("error during thread creation\n");
		exit(1);
	}	
	printf("waiting for new thread return\n");
	pthread_join(tid,&str);
	printf("thread returned\n");
	print_foo(&str);
	
	
	return 0;
	
}



void print_foo(foo *str){
	printf("foo\n");
	printf(" a : %d\n", str->a);
	printf(" b : %d\n", str->b);
	printf(" c : %d\n", str->c);
	printf(" d : %d\n", str->d);
}



void* t{
	foo str;
	str.a = 1;
	str.b = 2;
	str.c = 3;
	str.d = 4;
	printf("i'm new thread\n");
	print_foo(&str);
	pthread_exit(&str);
}



