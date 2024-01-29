/*
*	author : Mirco Palese
*	program name : mutex_test
*	last edit :	24/01/2020
*	description : mutex test 
*	note : 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


typedef struct{
	int a,b;
	pthread_mutex_t mutex;
}foo;

void fill_foo(foo *str);
void print_foo(foo *str);
void* thread1(void *str);
void* thread2(void *str);

int main (int argc , char *argv[]){
	
	foo str;
	int err;
	pthread_t tid;
	fill_foo(&str);
	print_foo(&str);
	
	srand(time(NULL));   	/* Initialization, should only be called once. */
	int r = rand() % 2;     /* Returns a pseudo-random integer between 0 and 1. */
	
	if(r == 0){
		err = pthread_create(&tid, NULL, thread1(&str), NULL);
		if (err != 0){
			printf("error during thread creation\n");
			exit(1);
		}
		err = pthread_create(&tid, NULL, thread2(&str), NULL);
		if(err != 0){
			printf("error during thread creation\n");
			exit(1);
		}
	}
	else{
		err = pthread_create(&tid, NULL, thread2(&str), NULL);
		if (err != 0){
			printf("error during thread creation\n");
			exit(1);
		}
		err = pthread_create(&tid, NULL, thread1(&str), NULL);
		if(err != 0){
			printf("error during thread creation\n");
			exit(1);
		}
	}
	print_foo(&str);
	
	return 0;
	
}



void fill_foo(foo *str){
	int m_err;
	str->a = 1;
	str->b = 2;
	if( (m_err = pthread_mutex_init(&str->mutex,NULL)) != 0) {
		printf("error creating mutex\n");	
		printf("err numb: %d\n", m_err);
		exit(0);
	}
}



void print_foo(foo *str){
	printf("foo\n");
	printf("a = %d\n",str->a);
	printf("b = %d\n",str->b);
	printf("mutex\n");
}



void *thread1(void *val){
	foo *str = val;
	int err;
	int err_m;
	if( (err_m = pthread_mutex_lock(&str->mutex)) !=0){
		printf("mutex error");
		exit(0);
	}
	else {
		printf("i'm thread1 -> mutex taken\n");
// 		str->a++;
// 		str->b++;
		sleep(1); 
		if(pthread_mutex_unlock(&str->mutex) != 0){
			printf("mutex releasing error %d\n", err);
			exit(0);
		}
		printf("i'm thread1 -> mutex released\n");
	}
	return 0;
}


void *thread2(void *val){
	foo *str = val;
	int err;
	int err_m;
	if( (err_m = pthread_mutex_lock(&str->mutex)) !=0){
		printf("mutex error");
		exit(0);
	}
	else {
		printf("i'm thread2 -> mutex taken\n");
// 		str->a++;
// 		str->b++;
		sleep(1); 
		if((err = pthread_mutex_unlock(&str->mutex)) != 0){
			printf("mutex unlock error : %d\n", err);
			exit(0);
		}
		printf("i'm thread2 -> mutex released\n");
	}
	return 0;
}
